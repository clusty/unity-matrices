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


    inline void TransposeWithStrides(const float* src, float* dst, const int rows, const int cols, const int rowStride,
                                     const int colStride)
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
}
