//
// Created by Vlad-Andrei Lazar on 2024-08-08.
//

#include "Tensor4.h"
#include <iostream>
#include <ostream>
#include <queue>
#include "Utils.h"
namespace
{
    using Coords = Tensor4::Coords;


    // Function to compute the index in a flat array from multi-dimensional indices
    int getIndex(const Coords d, const Coords i)
    {
        return ((i[0] * d[1] + i[1]) * d[2] + i[2]) * d[3] + i[3];
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

        for (int i=0;i<dim[0];++i)
            for(int j = 0;j<dim[1];++j)
                for (int k = 0;k<dim[2];++k)
                    for (int l = 0;l<dim[3];++l)
                    {
                        const auto idxs = ApplyPerm({i, j, k, l}, perm);
                        const auto newIndex = getIndex(newDim, idxs);
                        const auto oldIndex = getIndex(dim, {i,j,k,l});
                        output[newIndex] = input[oldIndex];
                    }
    }


    // Function to permute the dimensions of a 4D tensor using a permutation array as a chain of repeated permutations.
    // shuffleTensor still needs a refactor to look like a matrix transpose with extra strides.
    float* shuffleIterative(const float* input, float* newTensor, const std::array<int, 4>& dims, const std::array<int, 4>& perm)
    {
        auto newPerm = perm;
        auto newDims = dims;
        for (int i=0;i<4;++i)
            for (int j=i+1;j<4;++j)
            {
                if (newPerm[j] < newPerm[i])
                {
                    std::array tempPerm = {0,1,2,3};
                    std::swap(tempPerm[i], tempPerm[j]);
                    shuffleTensor(input, newTensor, newDims, tempPerm);
                    std::swap(newDims[i], newDims[j]);
                    std::swap(newPerm[i], newPerm[j]);
                }
            }

        return newTensor;
    }

}
Tensor4::Tensor4(const Coords dims) :
    _data(dims[0] * dims[1] * dims[2] * dims[3] ), _dims(dims)
{
}

float &Tensor4::operator[](const Coords c)
{
    auto * ptr = _data.data();
    return ptr[getIndex(_dims, c)];
}
Tensor4 Tensor4::shuffle(const std::array<int, 4> perm) const
{
    Tensor4 ret(ApplyPerm(_dims, perm));
    shuffleTensor(_data.data(), ret._data.data(), _dims, perm);

    return std::move(ret);
}

Tensor4 Tensor4::shuffleIterative(Coords perm) const
{
    Tensor4 ret(ApplyPerm(_dims, perm));
    ::shuffleIterative(_data.data(), ret._data.data(), _dims, perm);

    return std::move(ret);
}

Tensor4 Tensor4::shuffle2(const Coords perm1, const Coords perm2) const
{
    const auto newPerm = ApplyPerm(perm1, perm2);
    return shuffle(newPerm);
}

void Tensor4::print() const { printTensor(_data.data(), _dims); }
bool Tensor4::operator==(const Tensor4 &other) const
{
    if (_dims != other._dims || _data != other._data)
        return false;

    return true;
}

