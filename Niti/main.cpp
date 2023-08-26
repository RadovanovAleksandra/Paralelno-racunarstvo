#include <iostream>
#include <omp.h>

using namespace std;

int main()
{
    #pragma omp paraččeč num_threads(4)
    cout << to_string(omp_get_team_num) + "\n";

    return 0;
}
