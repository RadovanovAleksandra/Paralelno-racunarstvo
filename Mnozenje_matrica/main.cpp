#include <iostream>
#include <vector>

#include <omp.h>

#include "hpc_helpers.hpp"

template<typename value_t, typename index_t>
void init(
    std::vector<value_t> &A,
    std::vector<value_t> &B,
    index_t m,
    index_t n,
    index_t r
) {
    for (index_t i = 0; i < m; i++) {
        for (index_t j = 0; j < n; j++) {
            A[i * m + j] = i >= j ? 1 : 0;
        }
    }

    for (index_t j = 0; j < r; j++) {
        for (index_t i = 0; i < n; i++) {
            B[i * r + j] = i;
        }
    }
}

template<typename value_t, typename index_t>
void mnozenje_sekvencijalno(
    std::vector<value_t> &A,
    std::vector<value_t> &B,
    std::vector<value_t> &R,
    index_t m,
    index_t n,
    index_t r
) {
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            value_t rez = value_t(0);
            for(int k = 0; k < n; k++)
            {
                rez += A[i * n + k] * B[k * r + j];
            }
            R[i * r + j] = rez;
        }
    }
}

template<typename value_t, typename index_t>
void mnozenje_openmp(
    std::vector<value_t> &A,
    std::vector<value_t> &B,
    std::vector<value_t> &R,
    index_t m,
    index_t n,
    index_t r,
    const index_t broj_niti
) {
    #pragma omp parallel for num_threads(broj_niti)
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            value_t rez = value_t(0);
            for(int k = 0; k < n; k++)
            {
                rez += A[i * n + k] * B[k * r + j];
            }
            R[i * r + j] = rez;
        }
    }
}

template<typename value_t, typename index_t>
void mnozenje_openmp_trans(
    std::vector<value_t> &A,
    std::vector<value_t> &B,
    std::vector<value_t> &R,
    index_t m,
    index_t n,
    index_t r,
    const index_t broj_niti
) {
    std::vector<value_t> Bprim(B.size());

    #pragma omp parallel for num_threads(broj_niti)
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < r; j++)
        {
            Bprim[j * n + 1] = B[i * r + j];
        }
    }

    #pragma omp parallel for num_threads(broj_niti)
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            value_t rez = value_t(0);
            for(int k = 0; k < n; k++)
            {
                rez += A[i * n + k] * B[k * r + j];
            }
            R[i * r + j] = rez;
        }
    }
}

int main()
{
    const uint8_t shift = 11;

    const unsigned int m = 1UL << shift;
    const unsigned int n = 1UL << shift;
    const unsigned int r = 1UL << shift;
    const unsigned int nt = 16UL;

    TIMERSTART(overall)

    TIMERSTART(alloc)
    std::vector<int> A(m*n);
    std::vector<int> B(n*r);
    std::vector<int> R(m*r);
    TIMERSTOP(alloc)

    init(A, B, m, n, r);

    // zakomentarisati nakon sto implementirante mnozenje_openmp funkciju
    // TIMERSTART(seq)
    // mnozenje_sekvencijalno(A, B, R, m, n, r);
    // TIMERSTOP(seq)

    // otkomentarisati nakon sto implementirate mnozenje_openmp funkciju
    // TIMERSTART(openmp)
    // mnozenje_openmp(A, B, R, m, n, r, nt);
    // TIMERSTOP(openmp)

    TIMERSTART(openmp2)
    mnozenje_openmp_trans(A, B, R, m, n, r, nt);
    TIMERSTOP(openmp2)

    TIMERSTOP(overall)

    // ukoliko je rezultat u rezultujucoj matrici tacan, ova petlja nece nista ispisati
    for (int j = 0; j < r; j++)
        for (int i = 0; i < m; i++)
            if (R[i*m+j] != i*(i+1)/2)
                std::cout << "error at position " << i << " "
                            << R[i] << std::endl;

    return 0;
}
