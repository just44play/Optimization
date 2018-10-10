#include "Schedule.hpp"

void Schedule::mix_hops()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, hops.size());

    shuffle(hops.begin(), hops.end(), gen);

    for (int h = 0; h < hops.size(); h++) { // ustawia w gorym wierszu macierzy skoki losowo
        matrix[0][h] = hops[h];
    }
}

void Schedule::replace_hops(int pos1, int pos2)
{
    int i1 = pos1/matrix[0].size(), j1 = pos1 % matrix[0].size(); // na podst. pozycji ustala indeksy macierzy
    int i2 = pos2/matrix[0].size(), j2 = pos2 % matrix[0].size();

    /* jesli "nowe" wezly moga nadawac jednoczesnie z wezlami,
       ktore do tej pory nadawaly ze "starymi" wezlami */
    if (can_parallel(matrix[i1][j1], matrix[(i2+1)%3][j2]) and
            can_parallel(matrix[i1][j1], matrix[(i2+2)%3][j2]) and
            can_parallel(matrix[i2][j2], matrix[(i1+1)%3][j1]) and
            can_parallel(matrix[i2][j2], matrix[(i1+2)%3][j1])) {

        swap(matrix[i1][j1], matrix[i2][j2]); // mozna je zamienic
    }
    else {  /* w przeciwnym wypadku zamieniaja sie cale "kolumny", czyli wylosowane wezly
            i te, ktore z nimi nadaja jednoczesnie */
        swap(matrix[i1][j1], matrix[i1][j2]);
        swap(matrix[(i1+1)%3][j1], matrix[(i1+1)%3][j2]);
        swap(matrix[(i1+2)%3][j1], matrix[(i1+2)%3][j2]);
    }
}

int Schedule::count_delay(int path_id)
{
    vector<Hop>::iterator itpos = find_if(hops.begin(), hops.end(), // szuka skoku o trasie path_id
                                      [&](Hop& h){ return h.get_path_id() == path_id; });

    int position = itpos - hops.begin();    // pozycja (indeks) tej trasy w wektorze hops


    Path path = hops[position].get_path();  // tworzy obiekt "kopie" trasy o path_id (arg. funkcji)

    vector<vector<Hop>::iterator> it(path.get_nodes().size());  // wektor iteratorow do wektora typu int do szukania
                                                                // pozycji wszystkich skokow na trasie path_id
    vector<int> pos(path.get_nodes().size());   // przechowuje pozycje (indeksy) skokow na trasie

    for (int n = 0; n < path.get_nodes().size(); n++) {
            it[n] = find_if(matrix[0].begin(), matrix[0].end(), // szuka skoku w "0" rzedzie macierzy
                             [&](Hop& h){ return (h.get_path_id() == path_id and
                                           h.get_node_id() == path.get_node(n).get_id()); });

            pos[n] = it[n] - matrix[0].begin(); // zachowuje pozycje (indeks)

            if (it[n] == matrix[0].end()) { // jesli nie znalazlo, to szuka w "1" rzedzie
                it[n] = find_if(matrix[1].begin(), matrix[1].end(),
                                 [&](Hop& h){ return (h.get_path_id() == path_id and
                                               h.get_node_id() == path.get_node(n).get_id()); });

                pos[n] = it[n] - matrix[1].begin();
            }
            if (it[n] == matrix[1].end()) {
                it[n] = find_if(matrix[2].begin(), matrix[2].end(),
                                 [&](Hop& h){ return (h.get_path_id() == path_id and
                                               h.get_node_id() == path.get_node(n).get_id()); });

                pos[n] = it[n] - matrix[2].begin();
            }
    }

    int delay = 1;  // opoznienie dla skoku, 1 jest gwarantowane
    for (int n = 1; n < pos.size(); n++) { // uzyskuje pozycje w wektorze hops
            if (pos[n] > pos[n-1]) {    // jesli kolejny wezel na trasie jest "po drodze"
                delay += pos[n] - pos[n-1] + 1;
            }
            else {  // jesli nie, liczy do konca, a potem od poczatku
                delay += (matrix[0].size() - pos[n-1]) + pos[n] + 1;
            }
    }

    delay -= path.get_nodes().size() - 1;   // poprawka

    return delay;
}

int Schedule::get_max_delay()
{
    int paths_number = 0;
        for (int p = 0; p < hops.size(); p++) {
            if (hops[p].get_path_id() > paths_number)
                paths_number = hops[p].get_path_id();
        }

        vector<int> delays;  // opoznienia dla kazdej sciezki (path)
        delays.resize(paths_number);
        int max = 0;
        for (int i = 0; i < delays.size(); i++) {
            delays[i] = count_delay(i+1);
            if (delays[i] > max)
                max = delays[i];
        }
        return max;
}

int Schedule::get_energy()
{
    int bonus = hops.size() - matrix[0].size();

    return get_max_delay() - bonus;
}

void Schedule::remove_empty_slots()
{
    int it = matrix[0].size();
    int pos = 0;
    while (it > 0) {
        if (matrix[0][pos].get_id() == 0 and matrix[1][pos].get_id() == 0 and matrix[2][pos].get_id() == 0) {
            matrix[0].erase(matrix[0].begin()+pos);
            matrix[1].erase(matrix[1].begin()+pos);
            matrix[2].erase(matrix[2].begin()+pos);
        }
        else {
            pos++;
        }
        it--;
    }
}

bool Schedule::can_parallel(Hop h1, Hop h2)
{
    if ( different_parents(h1.get_node(), h2.get_node()) and
        half_duplex_transceiver(h1.get_node(), h2.get_node()) and
        different_path(h1.get_path(), h2.get_path()) ) {

            return true;
    }
    return false;
}

bool Schedule::different_parents(Node n1, Node n2)
{
    if (n1.get_id() == 0 or n2.get_id() == 0) return true;
    return n1.get_parent() != n2.get_parent();
}

bool Schedule::half_duplex_transceiver(Node n1, Node n2)
{
    if (n1.get_id() == 0 or n2.get_id() == 0) return true;
        return ( (n1.get_parent() != n2.get_id()) and (n2.get_parent() != n1.get_id()) );
}

bool Schedule::different_path(Path p1, Path p2)
{
    if (p1.get_id() == 0 and p2.get_id() == 0) return true;
        return p1.get_id() != p2.get_id();
}

void Schedule::to_string_matrix()
{
    shift_up();

    for (int i = 0; i < matrix[0].size(); i++) {
        if (matrix[0][i].get_id() == 0)
            cout << "      ";
        else cout << matrix[0][i].to_string_hop() << " ";
    }
    cout << endl;
    for (int i = 0; i < matrix[1].size(); i++) {
        if (matrix[1][i].get_id() == 0)
            cout << "      ";
        else cout << matrix[1][i].to_string_hop() << " ";
    }
    cout << endl;
    for (int i = 0; i < matrix[2].size(); i++) {
        if (matrix[2][i].get_id() == 0)
            cout << "      ";
        else cout << matrix[2][i].to_string_hop() << " ";
    }
    cout << endl;
}

void Schedule::save_to_file()
{
    fstream file;

    file.open("/home/dzastina/SimulatedAnnealing/Results.txt", ios::out);
    if (file.good() == false) {
        cout << "no file!";
        exit(0);
    }

    shift_up();

    for (int i = 0; i < matrix[0].size(); i++) {
        file << "Time Slot:" << i+1 << endl;
        file << "1 : " << matrix[0][i].to_string_hop() << endl;
        file << "2 : ";
        if (matrix[1][i].get_id() != 0) file << matrix[1][i].to_string_hop();
        file << endl;
        file << "3 : ";
        if (matrix[2][i].get_id() != 0) file << matrix[2][i].to_string_hop();
        file << endl << endl;
    }

    file.close();
}

void Schedule::shift_up()   // jesli jednoczenie nadaje jeden wezel i wyswietla sie w dolnym
{                              //  wierszu macierzy, to umieszcza je na gorze
    for (int m = 0; m < matrix[0].size(); m++) {
        if (matrix[0][m].get_id() == 0 and matrix[1][m].get_id() != 0)
            swap(matrix[0][m], matrix[1][m]);
        if (matrix[1][m].get_id() == 0 and matrix[2][m].get_id() != 0)
            swap(matrix[1][m], matrix[2][m]);
        if (matrix[0][m].get_id() == 0 and matrix[1][m].get_id() != 0)
            swap(matrix[0][m], matrix[1][m]);
    }
}

int Schedule::size_of_matrix() { return matrix.size() * matrix[0].size(); }
