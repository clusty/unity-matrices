//
// Created by Vlad-Andrei Lazar on 2024-08-08.
//

#include "Tensor4.h"
#include "Utils.h"
#include <iostream>
#include <ostream>
namespace
{
    using Coords = Tensor4::Coords;


    // Function to compute the index in a flat array from multi-dimensional indices
    int getIndex(const Coords d, const Coords i)
    {
        return ((i[0] * d[1] + i[1]) * d[2] + i[2]) * d[3] + i[3];
    }

    // Function to get multi-dimensional indices from a flat array index
    Coords getMultiDimIndices(const Coords d, int index)
    {
        const int l = index % d[3];
        index /= d[3];
        const int k = index % d[2];
        index /= d[2];
        const int j = index % d[1];
        index /= d[1];
        const int i = index;

        return Coords{i, j, k, l};
    }

    void printTensor(const float* tensor, const Coords d) {
        for (int i = 0; i < d[0]; ++i) {
            std::cout << "Matrix (d0=" << i << "):\n";
            for (int j = 0; j < d[1]; ++j) {
                std::cout << " Row (d1=" << j << "): ";
                for (int k = 0; k < d[2]; ++k) {
                    std::cout << "[ ";
                    for (int l = 0; l < d[3]; ++l) {
                        std::cout << tensor[getIndex(d, {i, j, k, l})] << ' ';
                    }
                    std::cout << "] ";
                }
                std::cout << '\n';
            }
            std::cout << "=========\n";
        }
    }

    Coords ApplyPerm(const Coords in, const Coords &perm)
    {
        std::array<int, 4> out{};
        for (int i=0;i<4;++i)
        {
            out[i] = in[perm[i]];
        }

        return out;
    }

    // Probably a lousy algorithm, but easier to write compared to the D&C recursive transpose
    void shuffleTensor(const float* input, float* output, const std::array<int, 4> dim, const std::array<int,4>& perm)
    {
        const auto newDim = ApplyPerm(dim, perm);

        const int tensor_size = dim[0] * dim[1] * dim[2] * dim[3];
        constexpr int block_size = 32;
        const int num_blocks = (tensor_size + block_size - 1) / block_size;

        for (int block = 0; block < num_blocks; ++block)
        {
            const int block_start = block * block_size;
            const int block_end = std::min(block_start + block_size, tensor_size);

            for (int index = block_start; index < block_end; ++index) {
                const auto idxs = ApplyPerm(getMultiDimIndices(dim, index), perm);

                const int old_index = index;
                const int new_index = getIndex(newDim, idxs);
                output[new_index] = input[old_index];
            }
        }
    }

    // Function to permute the dimensions of a 4D tensor using a permutation array
    float* shuffleIterative(const float* input, float* newTensor, const std::array<int, 4>& dims, const std::array<int, 4>& perm) {
        std::array newDims = {dims[perm[0]], dims[perm[1]], dims[perm[2]], dims[perm[3]]};

        // Strides for each dimension in the original tensor
        std::array newStride = {newDims[1] * newDims[2] * newDims[3], newDims[2] * newDims[3], newDims[3], 1};

        for (int i = 0; i < dims[0]; ++i) {
            for (int j = 0; j < dims[1]; ++j) {
                for (int k = 0; k < dims[2]; ++k) {
                    for (int l = 0; l < dims[3]; ++l) {
                        std::array<int, 4> oldIndices = {i, j, k, l};
                        std::array<int, 4> newIndices = {oldIndices[perm[0]], oldIndices[perm[1]], oldIndices[perm[2]], oldIndices[perm[3]]};

                        int oldIndex = getIndex(oldIndices, dims);
                        int newIndex = getIndex(newIndices, newDims);

                        utils::TransposeWithStrides(&input[oldIndex], &newTensor[newIndex], newDims[perm[0]], newDims[perm[1]], newStride[0], newStride[1]);
                    }
                }
            }
        }

        return newTensor;
    }

}
Tensor4::Tensor4(const Coords dims) :
    _data(static_cast<float *>(malloc(dims[0] * dims[1] * dims[2] * dims[3] * sizeof(float))), &free), _dims(dims)
{
}

float &Tensor4::operator[](const Coords c) const { return _data.get()[getIndex(_dims, c)]; }
Tensor4 Tensor4::shuffle(const std::array<int, 4> perm) const
{
    Tensor4 ret(ApplyPerm(_dims, perm));
    shuffleTensor(_data.get(), ret._data.get(), _dims, perm);

    return std::move(ret);
}

Tensor4 Tensor4::shuffleIterative(Coords perm) const
{
    Tensor4 ret(ApplyPerm(_dims, perm));
    shuffleTensor(_data.get(), ret._data.get(), _dims, perm);

    return std::move(ret);
}

Tensor4 Tensor4::shuffle2(const Coords perm1, const Coords perm2) const
{
    const auto newPerm = ApplyPerm(perm1, perm2);
    return shuffle(newPerm);
}

void Tensor4::print() const { printTensor(_data.get(), _dims); }
bool Tensor4::operator==(const Tensor4 &other) const
{
    if (_dims != other._dims)
        return false;

    for (int i=0;i< _dims[0] * _dims[1] * _dims[2] * _dims[3];++i)
    {
        if (_data.get()[i] != other._data.get()[i])
            return false;
    }

    return true;
}

