// Resenje zadatka 4

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include <omp.h>

#define MAX_NITI 5

using namespace std;

int main() {

    vector<int> brojevi = { 1, 2, 3, 4, 5, 6 };
    vector<int> suma(MAX_NITI, 0);

    #pragma omp parallel num_threads(MAX_NITI)
    {
        int id = omp_get_thread_num();
        int broj_niti = omp_get_num_threads();
        int n  = ceil((float) brojevi.size() / broj_niti);

        vector<int>::const_iterator pocetak = brojevi.begin() + id * n;
        vector<int>::const_iterator kraj = brojevi.begin() + min((id + 1) * n, (int) brojevi.size());

        for (auto it = pocetak; it < kraj; it++) {
            suma[id] += *it;
            // ako isprobavate na kratkom vektoru brojeva, mozete otkomentarisati da vidite koja
            // sabira koji broj iz vektora
            // cout << "Nit " + to_string(id) + " dodala " + to_string(*it) + "\n";
        }
    }

    int ukupno = 0;
    for (int i = 0; i < MAX_NITI; i++)
        ukupno += suma[i];

    cout << "Suma brojeva u nizu je " << ukupno << "." << endl;

    return 0;
}
