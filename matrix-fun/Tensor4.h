#pragma once
#include <array>
#include <memory>

class Tensor4
{
public:
    using Coords = std::array<int, 4>;
    explicit Tensor4(Coords dims);
    float& operator[](Coords c) const;

    [[nodiscard]] Tensor4 shuffle(Coords perm) const;
    [[nodiscard]] Tensor4 shuffle2(Coords perm1,Coords perm2 ) const;
    void print() const;
    bool operator==(const Tensor4 &) const;
   // [[nodiscard]] Tensor4 transpose() const;

private:
    using raw_mem = std::unique_ptr<float, decltype(&free)>;
    raw_mem _data;
    Coords _dims;
};




