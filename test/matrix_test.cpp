#include <stdexcept>
#include <gtest/gtest.h>
#include <algebra/finite_fields/finite_fields.hpp>
#include <algebra/matrices/matrices.hpp>

using namespace algebra;

using E = PrimeFieldElement<7>;
using Square = Matrix<E, 2, 2>;

template<class A, class B>
concept CanMultiply = requires(const A& a, const B& b) { a * b; };

static_assert(RingElement<Square>);
static_assert(!FieldElement<Square>);
static_assert(!CommutativeRingElement<Square>);
static_assert(!RingElement<Matrix<E, 2, 3>>);
static_assert(!CanMultiply<Matrix<E, 2, 3>, Matrix<E, 2, 2>>);
static_assert(RingElement<Matrix<Polynomial<E>, 2, 2>>);
static_assert(RingElement<Polynomial<Square>>);

TEST(Matrices, DistinguishesIdentityFromAllOnes) {
    EXPECT_TRUE(Square::zero().is_zero());
    EXPECT_TRUE(Square::one().is_one());
    const Square all_ones{{E::one(), E::one()}, {E::one(), E::one()}};
    EXPECT_FALSE(all_ones.is_one());
}

TEST(Matrices, MultipliesRectangularShapes) {
    const Matrix<E, 2, 3> a{{E::from_integer(1), E::from_integer(2), E::from_integer(3)},
                           {E::from_integer(4), E::from_integer(5), E::from_integer(6)}};
    const Matrix<E, 3, 2> b{{E::one(), E::zero()}, {E::zero(), E::one()}, {E::one(), E::one()}};
    const Square expected{{E::from_integer(4), E::from_integer(5)},
                          {E::from_integer(3), E::from_integer(4)}};
    EXPECT_EQ(a * b, expected);
    EXPECT_EQ(a.transpose().transpose(), a);
}

TEST(Matrices, HandlesInPlaceAliasingAndRowSwaps) {
    Square a{{E::from_integer(1), E::from_integer(2)}, {E::from_integer(3), E::from_integer(4)}};
    const Square expected{{E::zero(), E::from_integer(3)}, {E::one(), E::one()}};
    a *= a;
    EXPECT_EQ(a, expected);
    a.swap_rows(0, 1);
    EXPECT_EQ(a(0, 0), E::one());
    EXPECT_THROW(a.swap_rows(0, 2), std::out_of_range);
    EXPECT_THROW(a.at(2, 0), std::out_of_range);
}

TEST(Matrices, ChecksInitializerShapes) {
    EXPECT_THROW((Square{{E::one(), E::zero()}}), std::invalid_argument);
    EXPECT_THROW((Square{{E::one()}, {E::one()}}), std::invalid_argument);
}

TEST(Matrices, PreservesNoncommutativeCoefficientOrder) {
    const Square upper{{E::one(), E::one()}, {E::zero(), E::one()}};
    const Square lower{{E::one(), E::zero()}, {E::one(), E::one()}};
    EXPECT_NE(upper * lower, lower * upper);
    using Nested = Matrix<Square, 1, 1>;
    EXPECT_EQ((Nested{{upper}} * Nested{{lower}})(0, 0), upper * lower);
    using Poly = Polynomial<Square>;
    const Poly a{upper};
    const Poly b{lower};
    EXPECT_NE(a * b, b * a);
    EXPECT_EQ(Poly::variable() * a, a * Poly::variable());
}

TEST(Matrices, SupportsUnitsWithOrderReversingReciprocals) {
    const Square upper{{E::one(), E::one()}, {E::zero(), E::one()}};
    const Square upper_inverse{{E::one(), E::from_integer(6)}, {E::zero(), E::one()}};
    const Square lower{{E::one(), E::zero()}, {E::one(), E::one()}};
    const Square lower_inverse{{E::one(), E::zero()}, {E::from_integer(6), E::one()}};
    const Unit<Square> a(upper, upper_inverse);
    const Unit<Square> b(lower, lower_inverse);
    const auto product = a * b;
    EXPECT_TRUE((product.value() * product.reciprocal().value()).is_one());
}

TEST(Matrices, SupportsPolynomialEntriesAndVirtualParents) {
    using P = Polynomial<E>;
    using M = Matrix<P, 2, 2>;
    M matrix = M::one();
    matrix(0, 1) = P::variable();
    EXPECT_EQ(matrix * M::one(), matrix);
    const interfaces::Ring<M>& view = matrix.parent();
    EXPECT_EQ(view.multiply(matrix, M::one()), matrix);
}

TEST(Matrices, ConstructsFiftyByFiftyExtensionFieldMatrices) {
    using F = FiniteField<11, 3>;
    using M = Matrix<F::Element, 50, 50>;
    auto matrix = M::one();
    matrix(0, 49) = F::alpha();
    EXPECT_FALSE(matrix.is_one());
    EXPECT_EQ(matrix(0, 49), F::alpha());
    EXPECT_EQ(matrix(49, 49), F::Element::one());
}
