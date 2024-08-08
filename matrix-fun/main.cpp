#include <iostream>
#include <chrono>
#include "matrix.h"
#include "Tensor4.h"
#include "eigenTest.h"

using Coords = Tensor4::Coords;

void TestTranspose()
{
    /*constexpr int k = 4;
    constexpr int l = 3;*/
    constexpr int k = 11000;
    constexpr int l = 11000;
    const Matrix m(k,l);
    float idx = 0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < l; j++)
        {
            m[i,j] = idx++;
        }
    }

      //m.print();

        std::cout<<std::endl;
    {
        const auto start = std::chrono::high_resolution_clock::now();
        const auto mT = m.transpose(Matrix::Recursive);
        //mT.print();
        const auto stop = std::chrono::high_resolution_clock::now();

        const auto duration = duration_cast<std::chrono::microseconds>(stop - start);

        std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
        // Simple block
        // Blocksize 32 -> 138872 us
        // blocksize 1 ->  301906 us
        // Recursive        86731 us
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < l; j++)
            {
                if (m[i,j] != mT[j,i])
                {
                    std::cout << "problem" << std::endl;
                }
            }
        }
    }

}

void TensorTest()
{
    constexpr Coords dim{2, 3, 4, 5};

    const Tensor4 t(dim);
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

    constexpr std::array permutation = {1,0,2,3};
    const auto permT = t.shuffle(permutation);

    std::cout<<"Permutation: "<<std::endl;
    permT.print();

    // permutting twice should give the original T
    if (t.shuffle(permutation).shuffle(permutation) == t)
    {
        std::cout <<"Shuffle Works"<<std::endl;
    }
    if (t.shuffle2(permutation,permutation) == t)
    {
        std::cout <<"Shuffle2 Works"<<std::endl;
    }
    eigenTensorTest();

}


int main()
{
   // eigenTensorTest();
    //TestTranspose();
    TensorTest();
    return 0;
}
