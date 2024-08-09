#include <iostream>
#include <vector>

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

// Perform the 4D tensor shuffle
void tensor_transpose(vector<double>& tensor, size_t a, size_t b, size_t c, size_t d) {
    // Transpose axes (0, 3): (a, b, c, d) -> (d, b, c, a)
    transpose_2d(tensor, a, d, b * c * d, 1, b * c);

    // Transpose axes (1, 3): (d, b, c, a) -> (d, a, c, b)
    transpose_2d(tensor, b, a, c * d, d, c);

    // Transpose axes (2, 3): (d, a, c, b) -> (d, a, b, c)
    transpose_2d(tensor, c, b, d, d * a, 1);
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

    // Perform the transpositions
    tensor_transpose(tensor, a, b, c, d);

    cout << "Transposed Tensor:" << endl;
    print_tensor(tensor, d, a, b, c);  // The tensor is now in (d, a, b, c) order

    return 0;
}
