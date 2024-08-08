//
// Created by Vlad-Andrei Lazar on 2024-08-08.
//

#include "matrix.h"
#include "Utils.h"
#include <cstdlib>
#include <iostream>



Matrix::Matrix(const int m, const int n) :
    _data(static_cast<float *>(malloc( m * n * sizeof(float))), &free), _dims({m,n})
{
}
void Matrix::print() const
{
    for (int i = 0; i < _dims[0]; i++)
    {
        std::cout << "\n";
        for (int j = 0; j < _dims[1]; j++)
        {
            std::cout << operator[](i, j) << " ";
        }
    }

    std::cout << std::endl;
}
Matrix Matrix::transpose(const Algo a) const
{
    Matrix result(_dims[1], _dims[0]);
    if (a == Block)
    {
        std::cout<<"Block"<<std::endl;
        utils::TransposeBlockGeneric(_data.get(), result._data.get(), _dims);
    }
    else
    {
        std::cout<<"Recursive"<<std::endl;
        utils::TransposeRecursive(0, _dims[1], 0, _dims[0], _dims[1],_dims[0],_data.get(), result._data.get());
    }
    return std::move(result);
}
