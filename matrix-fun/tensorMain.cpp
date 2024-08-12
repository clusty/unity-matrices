#include <iostream>
#include <vector>
#include <array>

// Function to calculate the 1D index for a given 4D index
size_t getIndex(const std::array<int, 4>& indices, const std::array<int, 4>& dims) {
    return ((indices[0] * dims[1] + indices[1]) * dims[2] + indices[2]) * dims[3] + indices[3];
}

// Function to shuffle two axes of a 4D tensor
void shuffleAxes(const std::vector<float>& inputTensor,
                 std::vector<float>& outputTensor,
                 const std::array<int, 4>& dims,
                 int axis1, int axis2) {
    // Iterate over all elements in the original tensor
    for (int n = 0; n < dims[0]; ++n) {
        for (int c = 0; c < dims[1]; ++c) {
            for (int h = 0; h < dims[2]; ++h) {
                for (int w = 0; w < dims[3]; ++w) {
                    // Store the original indices
                    std::array<int, 4> originalIndices = {n, c, h, w};
                    // Swap the two specified axes
                    std::swap(originalIndices[axis1], originalIndices[axis2]);
                    // Calculate the input and output indices in the flat arrays
                    size_t inputIndex = getIndex({n, c, h, w}, dims);
                    size_t outputIndex = getIndex(originalIndices, dims);
                    // Place the element in the new position
                    outputTensor[outputIndex] = inputTensor[inputIndex];
                }
            }
        }
    }
}

int main() {
    // Example dimensions
    std::array<int, 4> dims = {2, 3, 4, 5}; // N, C, H, W

    // Create a flat 4D tensor
    size_t totalElements = dims[0] * dims[1] * dims[2] * dims[3];
    std::vector<float> inputTensor(totalElements);

    // Fill the tensor with some values for demonstration
    for (size_t i = 0; i < totalElements; ++i) {
        inputTensor[i] = static_cast<float>(i);
    }

    // Create an output tensor to hold the shuffled data
    std::vector<float> outputTensor(totalElements);

    // Define the axes to shuffle, e.g., swap axis 1 (C) and axis 2 (H)
    int axis1 = 1; // C
    int axis2 = 2; // H

    // Perform the shuffle
    shuffleAxes(inputTensor, outputTensor, dims, axis1, axis2);

    // Output the shuffled tensor
    std::cout << "Shuffled Tensor:" << std::endl;
    for (size_t i = 0; i < totalElements; ++i) {
        std::cout << outputTensor[i] << " ";
        if ((i + 1) % dims[3] == 0) std::cout << std::endl;
        if ((i + 1) % (dims[2] * dims[3]) == 0) std::cout << std::endl;
    }

    return 0;
}
