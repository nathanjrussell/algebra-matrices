#include <iostream>
#include <algebra/finite_fields/finite_fields.hpp>
#include <algebra/matrices/matrices.hpp>

using namespace algebra;

int main() {
    using F = FiniteField<11, 3>;
    using E = F::Element;
    const auto alpha = F::alpha();
    const Matrix<E, 2, 2> matrix{{E::one(), alpha}, {alpha, E::zero()}};
    const auto square = matrix.multiply(matrix);
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            std::cout << square(i, j) << '\n';
        }
    }
}
