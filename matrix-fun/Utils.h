#pragma once
#include <array>
namespace utils
{
    inline void RecursiveTranspose(const int rowBegin, const int rowEnd, const int columnBegin, const int columnEnd, const float* src, float *dst, const int rows, const int columns)
    {
        constexpr int BLOCK_SIZE = 64;
        const int r = rowEnd - rowBegin;
        const int c = columnEnd - columnBegin;
        if ( r <= BLOCK_SIZE && c <= BLOCK_SIZE)
        {
            for (int i = rowBegin; i < rowEnd; i++)
            {
                for (int j = columnBegin; j < columnEnd; j++)
                {
                    dst[j * rows + i] = src[i * columns + j];
                }
            }
        }
        else if (r >= c)
        {
            RecursiveTranspose(rowBegin, rowBegin + r / 2, columnBegin, columnEnd, src, dst, rows,columns);
            RecursiveTranspose(rowBegin + r / 2, rowEnd, columnBegin, columnEnd, src, dst, rows,columns);
        }
        else
        {
            RecursiveTranspose(rowBegin, rowEnd, columnBegin, columnBegin + c / 2, src, dst, rows,columns);
            RecursiveTranspose(rowBegin, rowEnd, columnBegin + c / 2, columnEnd, src, dst, rows,columns);
        }
    }

    inline void TransposeBlock(const float *src, float *dst, const int M, const int N)
    {
        constexpr int BLOCK_SIZE = 32;
        for (int i = 0; i < M; i += BLOCK_SIZE)
        {
            for (int j = 0; j < N; j += BLOCK_SIZE)
            {
                for (int k = j; k < j + BLOCK_SIZE && k < M; ++k)
                {
                    for (int l = i; l < i + BLOCK_SIZE && l < N; ++l)
                    {
                        const auto r = l + k * N;
                        const auto w = k + l * M;
                        dst[w] = src[r];
                    }
                }
            }
        }
    }


    inline int computeIndices(const std::array<int, 2> idx, const std::array<int, 2> dims,
                       const std::array<int, 2> permutation)
    {
        return idx[permutation[1]] + idx[permutation[0]] * dims[permutation[1]];
    }


    inline void TransposeBlockGeneric(const float *src, float *dst, const std::array<int, 2> dims)
    {
        constexpr std::array permutation = {1, 0};
        constexpr std::array noopPermutation = {0, 1};

        constexpr int BLOCK_SIZE = 32;
        for (int ii = 0; ii < dims[0]; ii += BLOCK_SIZE)
        {
            for (int jj = 0; jj < dims[1]; jj += BLOCK_SIZE)
            {
                for (int i = ii; i < ii + BLOCK_SIZE && i < dims[0]; ++i)
                {
                    for (int j = jj; j < jj + BLOCK_SIZE && j < dims[1]; ++j)
                    {
                        const auto r = computeIndices({i, j}, dims, noopPermutation); // l + k * dims[1];
                        const auto w = computeIndices({i, j}, dims, permutation); // k + l * dims[0];
                        dst[w] = src[r];
                    }
                }
            }
        }
    }
}