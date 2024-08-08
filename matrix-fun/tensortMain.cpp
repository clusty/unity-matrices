#include <iostream>
#include <vector>
#include <algorithm> // For std::min

///

// Helper function to map 4D indices to a flat 1D index
inline size_t index4D(size_t i, size_t j, size_t k, size_t l,
                      size_t D1, size_t D2, size_t D3, size_t D4) {
    return i * D2 * D3 * D4 + j * D3 * D4 + k * D4 + l;
}

// Helper function to map a flat 1D index to 4D indices
inline void indexTo4D(size_t idx, size_t &i, size_t &j, size_t &k, size_t &l,
                      size_t D1, size_t D2, size_t D3, size_t D4) {
    i = idx / (D2 * D3 * D4);
    idx %= (D2 * D3 * D4);
    j = idx / (D3 * D4);
    idx %= (D3 * D4);
    k = idx / D4;
    l = idx % D4;
}

// Helper function to transpose a 2D matrix in a flat array
void transpose2D(std::vector<float>& A, std::vector<float>& B,
                 size_t rowStart, size_t colStart,
                 size_t rows, size_t cols,
                 size_t D1, size_t D2, size_t D3, size_t D4) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            B[index4D(rowStart + j, colStart + i, 0, 0, D1, D2, D3, D4)] =
                A[index4D(rowStart + i, colStart + j, 0, 0, D1, D2, D3, D4)];
        }
    }
}

// Function to perform the 4D tensor shuffle using matrix transposes
void shuffle4D(std::vector<float>& A, std::vector<float>& B,
                const int* perm, size_t D1, size_t D2, size_t D3, size_t D4) {
    // Create a permutation matrix for 4D indices
    size_t permutedDims[4];
    for (size_t i = 0; i < 4; ++i) {
        permutedDims[perm[i]] = i;
    }

    // Apply transposes according to the permutation
    if (permutedDims[0] != 0 || permutedDims[1] != 1) {
        transpose2D(A, B, 0, 0, D2, D1, D1, D2, D3, D4); // Transpose dimensions 0 and 1
    }
    if (permutedDims[1] != 2 || permutedDims[2] != 1) {
        transpose2D(A, B, 0, 0, D3, D2, D1, D2, D3, D4); // Transpose dimensions 1 and 2
    }
    if (permutedDims[2] != 3 || permutedDims[3] != 2) {
        transpose2D(A, B, 0, 0, D4, D3, D1, D2, D3, D4); // Transpose dimensions 2 and 3
    }
}

///






std::array<int, 4> ApplyPerm(const std::array<int, 4> in, const std::array<int,4> &perm)
{
    std::array<int, 4> out{};
    for (int i=0;i<4;++i)
    {
        out[i] = in[perm[i]];
    }

    return out;
}

// Function to shuffle a 4D tensor using 32-element blocks
void shuffleTensor(const float* input, float* output, const std::array<int, 4> dim, const std::array<int,4>& perm) {
    auto newDim = ApplyPerm(dim, perm);

    int tensor_size = dim[0] * dim[1] * dim[2] * dim[3];
    int block_size = 32;
    int num_blocks = (tensor_size + block_size - 1) / block_size;

    for (int block = 0; block < num_blocks; ++block)
    {
        int block_start = block * block_size;
        int block_end = std::min(block_start + block_size, tensor_size);

        for (int index = block_start; index < block_end; ++index) {
            auto idxs = ApplyPerm(getMultiDimIndices(dim, index),perm);

            int old_index = index;
            int new_index = getIndex(newDim, idxs);
            output[new_index] = input[old_index];
        }
    }
}





void shuffleTwiceTensor(const float* input, float* output, const std::array<int, 4> dim, const std::array<int,4>& perm1, const std::array<int,4>& perm2)
{
    const auto newPerm = ApplyPerm(perm1, perm2);
    shuffleTensor(input, output, dim, newPerm);

}

// Function to print a 4D tensor (from a flat array)
void printTensor(const float* tensor, int d1, int d2, int d3, int d4) {
    for (int i = 0; i < d1; ++i) {
        std::cout << "Matrix (d1=" << i << "):\n";
        for (int j = 0; j < d2; ++j) {
            std::cout << " Row (d2=" << j << "): ";
            for (int k = 0; k < d3; ++k) {
                std::cout << "[ ";
                for (int l = 0; l < d4; ++l) {
                    std::cout << tensor[getIndex({d1, d2, d3, d4}, {i, j, k, l})] << ' ';
                }
                std::cout << "] ";
            }
            std::cout << '\n';
        }
        std::cout << "=========\n";
    }
}

int main() {
    // Dimensions of the tensor
    int D1 = 2, D2 = 3, D3 = 4, D4 = 5;

    // Create and initialize a 4D tensor in a flat array
    int tensor_size = D1 * D2 * D3 * D4;
    std::vector<float> tensor(tensor_size);
    int val = 1;
    for (int i = 0; i < D1; ++i) {
        for (int j = 0; j < D2; ++j) {
            for (int k = 0; k < D3; ++k) {
                for (int l = 0; l < D4; ++l) {
                    tensor[getIndex({D1, D2, D3, D4}, {i, j, k, l})] = val++;
                }
            }
        }
    }

    std::cout << "Original Tensor:\n";
    printTensor(tensor.data(), D1, D2, D3, D4);

    // Define the permutation for the shuffle
    // This permutation represents (D2, D1, D4, D3)
    std::array<int,4> permutation = {1, 0, 3, 2};

    // Create output tensor with permuted dimensions
    std::vector<float> shuffled_tensor(D1 * D2 * D3 * D4);

    // Perform the shuffle
    shuffleTensor(tensor.data(), shuffled_tensor.data(), {D1, D2, D3, D4}, permutation);
    std::vector<float> iterShuffled_tensor(D1 * D2 * D3 * D4);
    shuffle4D(tensor, iterShuffled_tensor, permutation.data(), D1, D2, D3, D4);

    for (int i = 0; i < D1 * D2 * D3 * D4; ++i)
    {
        if (shuffled_tensor[i] != iterShuffled_tensor[i])
        {
            std::cout << "problem"<< std::endl;
        }
    }
    /*std::cout << "Shuffled Tensor:\n";
    printTensor(shuffled_tensor.data(), D2, D1, D4, D3);
*/

    {
        auto newD = ApplyPerm({D1, D2, D3, D4}, permutation);
        // Create output tensor with permuted dimensions
        std::vector<float> initial_tensor(D1 * D2 * D3 * D4);
        shuffleTensor(shuffled_tensor.data(), initial_tensor.data(), newD, permutation);

        std::cout << "Original Tensor:\n";
        printTensor(initial_tensor.data(), D1, D2, D3, D4);
        for (int i = 0; i < D1 * D2 * D3 * D4; ++i)
        {
            if (initial_tensor[i] != tensor[i])
            {
                std::cout << "problem"<< std::endl;
            }
        }
    }
    {
        std::vector<float> initial_tensor(D1 * D2 * D3 * D4);
        shuffleTwiceTensor(shuffled_tensor.data(), initial_tensor.data(), {D1, D2, D3, D4}, permutation, permutation);
        for (int i = 0; i < D1 * D2 * D3 * D4; ++i)
        {
            if (initial_tensor[i] != shuffled_tensor[i])
            {
                std::cout << "problem"<< std::endl;
            }
        }
    }

    //The shuffle reorders dimensions in such a way that it only affects two dimensions (i.e., it swaps two dimensions and leaves the others unchanged).
    // [D1, D2, 1, 1]

    std::cout << "All good"<<std::endl;

    return 0;
}
