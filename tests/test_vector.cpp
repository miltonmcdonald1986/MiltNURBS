#include <gtest/gtest.h>
#include <miltnurbs/vector.h>

using miltnurbs::vector::Vector;

// Test default constructor
TEST(VectorTest, DefaultConstructor) {
    Vector<double, 3> v;
    EXPECT_EQ(v[0], 0.0);
    EXPECT_EQ(v[1], 0.0);
    EXPECT_EQ(v[2], 0.0);
}

// Test variadic constructor
TEST(VectorTest, VariadicConstructor) {
    Vector<double, 3> v(1.0, 2.0, 3.0);
    EXPECT_EQ(v[0], 1.0);
    EXPECT_EQ(v[1], 2.0);
    EXPECT_EQ(v[2], 3.0);
}

// Test variadic constructor with integers
TEST(VectorTest, VariadicConstructorInt) {
    Vector<int, 4> v(1, 2, 3, 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

// Test element access
TEST(VectorTest, ElementAccess) {
    Vector<double, 3> v(1.0, 2.0, 3.0);
    v[1] = 5.0;
    EXPECT_EQ(v[0], 1.0);
    EXPECT_EQ(v[1], 5.0);
    EXPECT_EQ(v[2], 3.0);
}

// Test vector addition
TEST(VectorTest, VectorAddition) {
    Vector<double, 3> v1(1.0, 2.0, 3.0);
    Vector<double, 3> v2(4.0, 5.0, 6.0);
    auto v3 = v1 + v2;

    EXPECT_EQ(v3[0], 5.0);
    EXPECT_EQ(v3[1], 7.0);
    EXPECT_EQ(v3[2], 9.0);
}

// Test vector addition with integers
TEST(VectorTest, VectorAdditionInt) {
    Vector<int, 3> v1(1, 2, 3);
    Vector<int, 3> v2(10, 20, 30);
    auto v3 = v1 + v2;

    EXPECT_EQ(v3[0], 11);
    EXPECT_EQ(v3[1], 22);
    EXPECT_EQ(v3[2], 33);
}

// Test scalar multiplication (vector * scalar)
TEST(VectorTest, ScalarMultiplicationRight) {
    Vector<double, 3> v1(1.0, 2.0, 3.0);
    auto v2 = v1 * 2.0;

    EXPECT_EQ(v2[0], 2.0);
    EXPECT_EQ(v2[1], 4.0);
    EXPECT_EQ(v2[2], 6.0);
}

// Test scalar multiplication (scalar * vector)
TEST(VectorTest, ScalarMultiplicationLeft) {
    Vector<double, 3> v1(1.0, 2.0, 3.0);
    auto v2 = 3.0 * v1;

    EXPECT_EQ(v2[0], 3.0);
    EXPECT_EQ(v2[1], 6.0);
    EXPECT_EQ(v2[2], 9.0);
}

// Test commutativity of scalar multiplication
TEST(VectorTest, ScalarMultiplicationCommutative) {
    Vector<double, 3> v(1.0, 2.0, 3.0);
    auto v1 = v * 2.5;
    auto v2 = 2.5 * v;

    EXPECT_EQ(v1[0], v2[0]);
    EXPECT_EQ(v1[1], v2[1]);
    EXPECT_EQ(v1[2], v2[2]);
}

// Test scalar multiplication with integers
TEST(VectorTest, ScalarMultiplicationInt) {
    Vector<int, 3> v1(1, 2, 3);
    auto v2 = v1 * 5;

    EXPECT_EQ(v2[0], 5);
    EXPECT_EQ(v2[1], 10);
    EXPECT_EQ(v2[2], 15);
}

// Test 2D vector
TEST(VectorTest, TwoDimensionalVector) {
    Vector<double, 2> v1(3.0, 4.0);
    Vector<double, 2> v2(1.0, 2.0);
    auto v3 = v1 + v2;

    EXPECT_EQ(v3[0], 4.0);
    EXPECT_EQ(v3[1], 6.0);
}

// Test chaining operations
TEST(VectorTest, ChainingOperations) {
    Vector<double, 3> v1(1.0, 2.0, 3.0);
    Vector<double, 3> v2(1.0, 1.0, 1.0);
    auto v3 = (v1 + v2) * 2.0;

    EXPECT_EQ(v3[0], 4.0);
    EXPECT_EQ(v3[1], 6.0);
    EXPECT_EQ(v3[2], 8.0);
}

// Test constexpr evaluation
TEST(VectorTest, ConstexprEvaluation) {
    constexpr Vector<int, 3> v1(1, 2, 3);
    constexpr Vector<int, 3> v2(4, 5, 6);
    constexpr auto v3 = v1 + v2;
    constexpr auto v4 = v1 * 2;

    EXPECT_EQ(v3[0], 5);
    EXPECT_EQ(v3[1], 7);
    EXPECT_EQ(v3[2], 9);
    EXPECT_EQ(v4[0], 2);
    EXPECT_EQ(v4[1], 4);
    EXPECT_EQ(v4[2], 6);
}

// Test floating point operations
TEST(VectorTest, FloatType) {
    Vector<float, 3> v1(1.5f, 2.5f, 3.5f);
    Vector<float, 3> v2(0.5f, 0.5f, 0.5f);
    auto v3 = v1 + v2;

    EXPECT_FLOAT_EQ(v3[0], 2.0f);
    EXPECT_FLOAT_EQ(v3[1], 3.0f);
    EXPECT_FLOAT_EQ(v3[2], 4.0f);
}

// Test zero vector
TEST(VectorTest, ZeroVector) {
    Vector<double, 3> zero;
    Vector<double, 3> v(1.0, 2.0, 3.0);
    auto result = v + zero;

    EXPECT_EQ(result[0], 1.0);
    EXPECT_EQ(result[1], 2.0);
    EXPECT_EQ(result[2], 3.0);
}

// Test multiplication by zero
TEST(VectorTest, MultiplicationByZero) {
    Vector<double, 3> v(1.0, 2.0, 3.0);
    auto result = v * 0.0;

    EXPECT_EQ(result[0], 0.0);
    EXPECT_EQ(result[1], 0.0);
    EXPECT_EQ(result[2], 0.0);
}

// Test multiplication by one
TEST(VectorTest, MultiplicationByOne) {
    Vector<double, 3> v(1.0, 2.0, 3.0);
    auto result = v * 1.0;

    EXPECT_EQ(result[0], 1.0);
    EXPECT_EQ(result[1], 2.0);
    EXPECT_EQ(result[2], 3.0);
}

// Test negative scalar
TEST(VectorTest, NegativeScalar) {
    Vector<double, 3> v(1.0, 2.0, 3.0);
    auto result = v * -1.0;

    EXPECT_EQ(result[0], -1.0);
    EXPECT_EQ(result[1], -2.0);
    EXPECT_EQ(result[2], -3.0);
}
