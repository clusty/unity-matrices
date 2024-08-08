#pragma once
#include </opt/homebrew/include/eigen3/unsupported/Eigen/CXX11/Tensor>
#include <iostream>
auto transpose(const Eigen::Tensor<float, 4> &tensor) {
    Eigen::array<int, 4> dims({1, 0, 2,3});
    return tensor.shuffle(dims);
}

void eigenTensorTest()
{
    Eigen::Tensor<float, 4> a_tensor(2, 2,2,2);
    int val=0;
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++)
            for (int k=0; k<2; k++)
                for (int l=0; l<2; l++)
                a_tensor(i,j,k, l) = val++;

    Eigen::Tensor<float, 4> transpo = transpose(a_tensor);

    std::cout << "a_tensor is\n\n"<< a_tensor << "\n\n";
   // std::cout << "a_tensor transpose is\n\n"<< transpo << "\n\n";
}