#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

#include "hpc_helpers.hpp"

#define NELEM 1000000000

using namespace std;

vector<int> histogram_sekvencijalni(vector<int> &vrednosti)
{
    cout << "Izvrsavanje zapocela sekvencijalna implementacija." << endl;

    // 1. inicijalizacija histograma - svakoj razlicitoj vrednosti se dodeljuje 0 kao broj pojavljivanja
    vector<int> histogram(10, 0);

    // 2. formiranje histograma brojanjem vrednosti iz ulaznog vektora
    for (int v : vrednosti) {
        histogram[v]++;
    }

    return histogram;
}

vector<int> histogram_openmp(vector<int> &vrednosti)
{
    cout << "Izvrsavanje zapocela prva openmp implementacija." << endl;

    vector<int> histogram(10, 0);

    #pragma omp parallel for
    for(int i = 0; i < vrednosti.size(); i++)
    {
        int indeks = vrednosti[i];
        #pragma oomp critical
        histogram[indeks]++;
    }

    return histogram;
}

vector<int> histogram_openmp2(vector<int> &vrednosti)
{
    cout << "Izvrsavanje zapocela druga openmp implementacija." << endl;

    vector<int> histogram(10, 0);

    #pragma omp parallel
    {
        vector<int> privatni_histogram(10, 0);
        #pragma omp for
        for(int i = 0; i < vrednosti.size(); i++)
        {
            int indeks = vrednosti[i];
            privatni_histogram[indeks]++;
        }

        for(int i = 0; i < histogram.size(); i++)
        {
            #pragma omp critical
            histogram[i] += privatni_histogram[i];
        }
    }

    return histogram;
}

int main()
{
    vector<int> vrednosti(NELEM);

    srand(time(NULL));
    generate(vrednosti.begin(), vrednosti.end(), [](){ return rand() % 10; });

    TIMERSTART(seq)
    vector<int> histogram1 = histogram_sekvencijalni(vrednosti);
    TIMERSTOP(seq)

    TIMERSTART(openmp1)
    vector<int> histogram2 = histogram_openmp(vrednosti);
    TIMERSTOP(openmp1)

    TIMERSTART(openmp2)
    vector<int> histogram3 = histogram_openmp2(vrednosti);
    TIMERSTOP(openmp2)

    bool rez12 = equal(histogram1.begin(), histogram1.end(), histogram2.begin());
    bool rez23 = equal(histogram2.begin(), histogram2.end(), histogram3.begin());

    if (rez12 && rez23)
        cout << "Svi rezultujuci vektori su isti" << endl;
    else
        cout << "Rezultujuci vektori se razlikuju" << endl;

    return 0;
}
