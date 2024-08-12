#pragma once
#include <array>
#include <vector>
class Matrix
{
public:
    enum Algo
    {
        Block,
        Recursive
    };
    Matrix(int m, int n);
    float& operator()(const int i, const int j)
    {
        const int idx = j  + i * _dims[1];
        return _data[idx];
    }

    const float& operator()(const int i, const int j) const
    {
        const int idx = j  + i * _dims[1];
        return _data[idx];
    }

    bool operator==(const Matrix &other) const
    {
        return _dims == other._dims && _data == other._data;
    }

    void print() const;
    [[nodiscard]] Matrix transpose(Algo = Recursive) const;
private:
    std::vector<float> _data;
    std::array<int, 2> _dims;
};


