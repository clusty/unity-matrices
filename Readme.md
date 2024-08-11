
Explanations:

*Question 1*:

Please find the matrix class
- Two implementations: blocked and recursive Divide and conquer. 
- The recursive is a bit faster, stats in the main test function (I cannot fully explain the difference)
- They were attempts at creating cache oblivious transposes.

*Question 2:*
- Please find the Tensor class (shuffle)
- This algorithm is only a "tiled" version (1d block). Simplicity was the reason
- Tested mostly that the permutations are symmetric: permuting the same array twice gives the initial input.

*Question 3*
- Permutations are composable, so first compose permutations and then call Question 2

*Question 4*
- If a permutation swaps only 2 axis, the operation can be expressed as a "transpose", if the transpose routine can accept 2 extra strides. Memory is not contiguous

*Question 5*
- I need to refactor the question 1 utility function to accept the 2 extra strides.
Did not finish !!!
