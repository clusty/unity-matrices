#pragma once
#include <array>
namespace utils
{
    inline void TransposeRecursive(const int x, const int delx, const int y, const int dely, const int M, const int N,
                            float *src, float *dst)
    {
        // doing it in blocks allows for better cache locality (cache oblivious algo)
        constexpr int BLOCK_SIZE = 32;
        if (delx <= BLOCK_SIZE && dely <= BLOCK_SIZE) // unroll
        {
            for (int j = y; j < y + dely; j++)
                for (int i = x; i < x + delx; i++)
                    dst[i * N + j] = src[j * M + i];
        }
        else if (delx >= dely)
        {
            const int midX = delx / 2;
            TransposeRecursive(x, midX, y, dely, M, N, src, dst);
            TransposeRecursive(x + midX, delx - midX, y, dely, M, N, src, dst);
        }
        else
        {
            const int midY = dely / 2;
            TransposeRecursive(x, delx, y, midY, M, N, src, dst);
            TransposeRecursive(x, delx, y + midY, dely - midY, M, N, src, dst);
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

    /*public int to1D( int x, int y, int z ) {
    return (z * xMax * yMax) + (y * xMax) + x;
}*/
    /*inline int computeIndices(const std::array<int, 4> idx, const std::array<int, 4> dims,
                   const std::array<int, 4> permutation)
    {
        const auto index = _dims[0] * _dims[1] * _dims[2] * k +
                                         _dims[0] * _dims[1] * j +
                                                    _dims[0] * i + l;
        return idx[permutation[1]] + idx[permutation[0]] * dims[permutation[1]];
    }*/
    /*
    void TransposeBlockGeneric(const float * src, float*  dst, std::array<int, 4> dims, std::array<int, 4> permutation)
    {
        constexpr std::array noopPermutation = { 0, 1 };

        constexpr int BLOCK_SIZE = 32;
        for (int ii = 0; ii < dims[0]; ii += BLOCK_SIZE)
        {
            for (int jj = 0; jj < dims[1]; jj += BLOCK_SIZE)
            {
                for (int i = ii; i < ii + BLOCK_SIZE && i < dims[0]; ++i)
                {
                    for (int j = jj; j < jj + BLOCK_SIZE && j < dims[1]; ++j)
                    {
                        const auto r = computeIndices({i,j}, dims, noopPermutation);//l + k * dims[1];
                        const auto w = computeIndices({i,j}, dims, permutation); //k + l * dims[0];
                        dst[w] = src[r];
                    }
                }
            }
        }
    }*/
}