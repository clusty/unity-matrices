#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Helper function to print a 4D tensor
void print_tensor(const vector<double>& tensor, size_t a, size_t b, size_t c, size_t d) {
    for (size_t i = 0; i < a; ++i) {
        for (size_t j = 0; j < b; ++j) {
            for (size_t k = 0; k < c; ++k) {
                for (size_t l = 0; l < d; ++l) {
                    cout << tensor[i * (b * c * d) + j * (c * d) + k * d + l] << " ";
                }
                cout << " | ";  // Separate each slice
            }
            cout << endl;
        }
        cout << "----" << endl;  // Separate each slice in the first dimension
    }
    cout << endl;
}

// Helper function to calculate strides for each dimension
vector<size_t> calculate_strides(const vector<size_t>& dims) {
    vector<size_t> strides(dims.size());
    strides.back() = 1;
    for (int i = dims.size() - 2; i >= 0; --i) {
        strides[i] = strides[i + 1] * dims[i + 1];
    }
    return strides;
}

// Transpose two dimensions in a 4D tensor
void transpose_2d(vector<double>& tensor, size_t dim1, size_t dim2, size_t stride1, size_t stride2, size_t block_size) {
    for (size_t i = 0; i < dim1; ++i) {
        for (size_t j = i + 1; j < dim2; ++j) {
            for (size_t k = 0; k < block_size; ++k) {
                size_t idx1 = i * stride1 + j * stride2 + k;
                size_t idx2 = j * stride1 + i * stride2 + k;
                swap(tensor[idx1], tensor[idx2]);
            }
        }
    }
}

// Perform the 4D tensor shuffle using a permutation array
void tensor_transpose(vector<double>& tensor, vector<size_t>& dims, const vector<size_t>& perm) {
    vector<size_t> strides = calculate_strides(dims);
    vector<size_t> new_dims(dims.size());

    // Perform the necessary transpositions
    for (size_t i = 0; i < perm.size(); ++i) {
        for (size_t j = i + 1; j < perm.size(); ++j) {
            if (perm[i] > perm[j]) {
                transpose_2d(tensor, dims[i], dims[j], strides[i], strides[j], strides.back() / strides[i]);
                swap(dims[i], dims[j]);
                swap(strides[i], strides[j]);
            }
        }
    }
}

int main() {
    // Define the dimensions of the tensor
    size_t a = 2, b = 2, c = 2, d = 2;  // For simplicity, use a small tensor for easy visualization

    // Initialize the tensor with some values
    vector<double> tensor(a * b * c * d);
    for (size_t i = 0; i < a * b * c * d; ++i) {
        tensor[i] = static_cast<double>(i + 1);  // Start from 1 for better readability
    }

    cout << "Original Tensor:" << endl;
    print_tensor(tensor, a, b, c, d);

    // Specify the desired permutation of dimensions (d, a, b, c)
    vector<size_t> dims = {a, b, c, d};
    vector<size_t> perm = {3, 0, 1, 2};

    // Perform the transpositions based on the permutation
    tensor_transpose(tensor, dims, perm);

    cout << "Transposed Tensor:" << endl;
    print_tensor(tensor, dims[0], dims[1], dims[2], dims[3]);  // The tensor is now in the new permuted order

    return 0;
}
