#pragma once
#include <array>
namespace utils
{
    inline void TransposeRecursive(const int x, const int dx, const int y, const int dy, const int M, const int N,
                            float *src, float *dst)
    {
        // doing it in blocks allows for better cache locality (cache oblivious algo)
        constexpr int BLOCK_SIZE = 32;
        if (dx <= BLOCK_SIZE && dy <= BLOCK_SIZE) // unroll
        {
            for (int j = y; j < y + dy; j++)
                for (int i = x; i < x + dx; i++)
                    dst[i * N + j] = src[j * M + i];
        }
        else if (dx >= dy)
        {
            const int midX = dx / 2;
            TransposeRecursive(x, midX, y, dy, M, N, src, dst);
            TransposeRecursive(x + midX, dx - midX, y, dy, M, N, src, dst);
        }
        else
        {
            const int midY = dy / 2;
            TransposeRecursive(x, dx, y, midY, M, N, src, dst);
            TransposeRecursive(x, dx, y + midY, dy - midY, M, N, src, dst);
        }
    }


    inline void TransposeWithStrides(const float* src, float* dst,
                             int rows, int cols,
                             int rowStride, int colStride)
    {

        if (rows <= 1 && cols <= 1) {
            dst[0] = src[0];
            return;
        }

        if (rows >= cols) {
            int mid = rows / 2;
            TransposeWithStrides(src, dst, mid, cols, rowStride, colStride);
            TransposeWithStrides(src + mid * rowStride, dst + mid, rows - mid, cols, rowStride, colStride);
        } else {
            int mid = cols / 2;
            TransposeWithStrides(src, dst, rows, mid, rowStride, colStride);
            TransposeWithStrides(src + mid * colStride, dst + mid * rowStride, rows, cols - mid, rowStride, colStride);
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