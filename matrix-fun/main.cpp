#include <chrono>
#include <iostream>
#include "Tensor4.h"
#include "Utils.h"
#include "matrix.h"

using Coords = Tensor4::Coords;

void TestTranspose()
{
   /* constexpr int k = 3;
    constexpr int l = 5;*/
    constexpr int k = 11000;
    constexpr int l = 11001;
    Matrix m(k,l);
    float idx = 0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < l; j++)
        {
            m(i,j) = idx++;
        }
    }

     // m.print();

        std::cout<<std::endl;
    {
        const auto start = std::chrono::high_resolution_clock::now();
        auto mT = m.transpose(Matrix::Recursive);
        //mT.print();
        const auto stop = std::chrono::high_resolution_clock::now();

        const auto duration = duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
        // Simple block
        // Blocksize 32 -> 138872 us
        // blocksize 1 ->  301906 us
        // Recursive        86731 us
        bool correct = true;
        for (int i = 0; i < k && correct; i++)
        {
            for (int j = 0; j < l ; j++)
            {
                if (m(i,j) != mT(j,i))
                {
                    correct = false;
                    break;
                }
            }
        }

        if (correct)
        {
            std::cout << "Transpose Is Correct" << std::endl;
        }
        else
        {
            std::cout << "Transpose Is Not Correct!!!!" << std::endl;
        }
    }

}

void TensorTest()
{
    constexpr Coords dim{2, 3, 4, 5};

    Tensor4 t(dim);
    float val = 0;
    for (int i = 0; i < dim[0]; ++i) {
        for (int j = 0; j < dim[1]; ++j) {
            for (int k = 0; k < dim[2]; ++k) {
                for (int l = 0; l < dim[3]; ++l) {
                    t[{i, j, k, l}] = val++;
                }
            }
        }
    }

    t.print();

    constexpr std::array permutation = {0,1,3,2};
    const auto permT = t.shuffle(permutation);

    std::cout<<"Permutation: "<<std::endl;
    permT.print();

    std::cout<<"Permutation Iterative: "<<std::endl;
    t.shuffleIterative(permutation).print();

    if (t.shuffle(permutation) == t.shuffleIterative(permutation))
    {
        std::cout <<"Shuffle Iterative Works"<<std::endl;
    }
    else
    {
        std::cout <<"Shuffle Iterative Broken !!"<<std::endl;
    }

    std::array<int,4> invPerm;
    utils::InversePermutation(permutation, invPerm);

    // permutting twice should give the original T
    if (t.shuffle(permutation).shuffle(invPerm) == t)
    {
        std::cout <<"Shuffle Works"<<std::endl;
    }
    else
    {
        std::cout <<"Shuffle Symmetric Broken!!!!"<<std::endl;
    }

    if (t.shuffle2(permutation,invPerm) == t)
    {
        std::cout <<"Shuffle2 Works"<<std::endl;
    }
    else
    {
        std::cout <<"Shuffle2 Broken!!!!"<<std::endl;
    }
}


int main()
{
    TestTranspose();
    TensorTest();
    return 0;
}
