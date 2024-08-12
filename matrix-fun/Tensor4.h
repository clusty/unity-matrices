#pragma once
#include <array>
#include <vector>

class Tensor4
{
public:
    using Coords = std::array<int, 4>;
    explicit Tensor4(Coords dims);
    float& operator[](Coords c) ;

    [[nodiscard]] Tensor4 shuffle(Coords perm) const;
    [[nodiscard]] Tensor4 shuffle2(Coords perm1,Coords perm2 ) const;
    [[nodiscard]] Tensor4 shuffleIterative(Coords perm1) const;
    void print() const;
    bool operator==(const Tensor4 &) const;
   // [[nodiscard]] Tensor4 transpose() const;

private:
    std::vector<float> _data;
    Coords _dims;
};




