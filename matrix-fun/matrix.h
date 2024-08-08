#pragma once
#include <array>
#include <memory>
class Matrix
{
public:
    enum Algo
    {
        Block,
        Recursive
    };
    Matrix(int m, int n);
    float& operator[](const int i, const int j) const
    {
        const int idx = j  + i * _dims[1];
        return _data.get()[idx];
    }

    void print() const;
    [[nodiscard]] Matrix transpose(Algo = Recursive) const;
private:
    using raw_mem = std::unique_ptr<float, decltype(&free)>;
    raw_mem _data;
    std::array<int, 2> _dims;
};


