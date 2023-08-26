#include <iostream>
#include <vector>

#include <omp.h>

using namespace std;

int main()
{
    vector<int> brojevi = { 1, 2, 3, 4, 5, 6 };
    vector<int> suma(2, 0); // suma[0] za nit 0, suma[1] za nit 1

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int broj_niti = omp_get_num_threads();

        vector<int>::const_iterator pocetak, kraj;
        if(id == 0)
        {
            pocetak = brojevi.begin();
            kraj = brojevi.begin() + brojevi.size() / broj_niti;
        }
        else
        {
            pocetak = brojevi.begin() + brojevi.size() / broj_niti;
            kraj = brojevi.end();
        }

        for(auto it = pocetak; it != kraj; it++)
            suma[id] += *it;
    }

    cout << "Suma brojeva u nizu -> " << suma[0] + suma[1] << endl;

    return 0;
}
