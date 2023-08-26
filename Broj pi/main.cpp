#include <algorithm>
#include <iostream>
#include <vector>
#include <climits>

#include "hpc_helpers.hpp"

#define NELEM 10000000

using namespace std;

int linearna_pretraga_sekvencijalno(vector<int> &brojevi, int trazeni_broj)
{
    for(int i = 0; i < brojevi.size(); i++)
        if(brojevi[i] == trazeni_broj)
            return i;

    return -1;
}

int linearna_pretraga_openmp(vector<int> &brojevi, int trazeni_broj)
{
    int indeks = INT_MAX;
    bool nadjen = false;

    #pragma omp parallel for firstprivate(nadjen) reduction(min:indeks)
    for(int i = 0; i < brojevi.size(); i++)
    {
        if(brojevi[i] == trazeni_broj && !nadjen)
        {
            indeks = i;
            nadjen = true;
        }
    }

    if(indeks == INT_MAX)
        return -1;

    return indeks;
}

int main()
{
    int trazeni_broj = 1;
     vector<int> brojevi(NELEM);

    // inicijalizacija vektora
    srand(time(NULL));

    // 1. inicijalizacija vektora nasumicnim brojevima razlicitim od trazenog broja zbog pouzdanijeg testiranja
    generate(brojevi.begin(), brojevi.end(), [&](){ return trazeni_broj + 1 + rand() % 100; });

    // 2. postavljanje n trazenih brojeva na nasumicne pozicije uz sispis indeksa vektora na koji je postavljen
    // broj koji ce se kasnije traziti. Istestirati resenje tako da je n = 0, n = 1 i n > 1.
    int n = 3;
    for (int i = 0; i < n; i++) {
        int poz = rand() % brojevi.size();
        brojevi[poz] = trazeni_broj;
        cout << "[init] Broj " << trazeni_broj << " postavljen na poziciju " << poz << " u nizu." << endl;
    }
    // KRAJ inicijalizacija vektora

    TIMERSTART(seq)
    int indeks1 = linearna_pretraga_sekvencijalno(brojevi, trazeni_broj);
    TIMERSTOP(seq)

    TIMERSTART(openmp)
    int indeks2 = linearna_pretraga_openmp(brojevi, trazeni_broj);
    TIMERSTOP(openmp)

    cout << "[sekvencijalno] Indeks prvog pojavljivanja broja: " << indeks1 << endl;
    cout << "[openmp] Indeks prvog pojavljivanja broja: " << indeks2 << endl;

    return 0;
}
