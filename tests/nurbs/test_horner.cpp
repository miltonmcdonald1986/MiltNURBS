#include <gtest/gtest.h>
#include "miltnurbs/horner.h"
#include "miltnurbs/vector.h"
#include <array>
#include <cmath>

using miltnurbs::horner::horner1;
using miltnurbs::vector::Vector;

// ============================================================================
// Linear Polynomial Tests (Line Segments)
// ============================================================================

TEST(Horner1Test, LinearScalar_Endpoints) {
    // p(u) = 5 + 10u (line from 5 to 15)
    std::array<double, 2> coeffs{5.0, 10.0};
    
    // At u=0, should be a0
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.0), 5.0);
    
    // At u=1, should be a0 + a1
    EXPECT_DOUBLE_EQ(horner1(coeffs, 1.0), 15.0);
}

TEST(Horner1Test, LinearScalar_Midpoint) {
    // p(u) = 5 + 10u
    std::array<double, 2> coeffs{5.0, 10.0};
    
    // At u=0.5, should be 5 + 10*0.5 = 10 (midpoint)
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.5), 10.0);
}

TEST(Horner1Test, LinearScalar_QuarterPoints) {
    // p(u) = 0 + 100u (line from 0 to 100)
    std::array<double, 2> coeffs{0.0, 100.0};
    
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.25), 25.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.75), 75.0);
}

TEST(Horner1Test, LinearVector_LineSegment) {
    // Line segment from (0,0,0) to (10,5,2)
    // P(u) = (0,0,0) + u*(10,5,2)
    std::array<Vector<double, 3>, 2> coeffs{
        Vector<double, 3>(0.0, 0.0, 0.0),
        Vector<double, 3>(10.0, 5.0, 2.0)
    };
    
    // At u=0
    auto p0 = horner1(coeffs, 0.0);
    EXPECT_DOUBLE_EQ(p0[0], 0.0);
    EXPECT_DOUBLE_EQ(p0[1], 0.0);
    EXPECT_DOUBLE_EQ(p0[2], 0.0);
    
    // At u=1
    auto p1 = horner1(coeffs, 1.0);
    EXPECT_DOUBLE_EQ(p1[0], 10.0);
    EXPECT_DOUBLE_EQ(p1[1], 5.0);
    EXPECT_DOUBLE_EQ(p1[2], 2.0);
    
    // At u=0.5 (midpoint)
    auto p_mid = horner1(coeffs, 0.5);
    EXPECT_DOUBLE_EQ(p_mid[0], 5.0);
    EXPECT_DOUBLE_EQ(p_mid[1], 2.5);
    EXPECT_DOUBLE_EQ(p_mid[2], 1.0);
}

TEST(Horner1Test, LinearVector_2D_LineSegment) {
    // 2D line from (1,2) to (5,10)
    // P(u) = (1,2) + u*(4,8)
    std::array<Vector<double, 2>, 2> coeffs{
        Vector<double, 2>(1.0, 2.0),
        Vector<double, 2>(4.0, 8.0)
    };
    
    auto p_third = horner1(coeffs, 1.0/3.0);
    EXPECT_NEAR(p_third[0], 1.0 + 4.0/3.0, 1e-10);
    EXPECT_NEAR(p_third[1], 2.0 + 8.0/3.0, 1e-10);
}

// ============================================================================
// Quadratic Polynomial Tests (Parabolic Arcs)
// ============================================================================

TEST(Horner1Test, QuadraticScalar_StandardParabola) {
    // p(u) = 0 + 0u + 1u² (standard parabola)
    std::array<double, 3> coeffs{0.0, 0.0, 1.0};
    
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 1.0), 1.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 2.0), 4.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.5), 0.25);
}

TEST(Horner1Test, QuadraticScalar_GeneralParabola) {
    // p(u) = 1 + 2u + 3u²
    std::array<double, 3> coeffs{1.0, 2.0, 3.0};
    
    // At u=0: 1
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.0), 1.0);
    
    // At u=1: 1 + 2 + 3 = 6
    EXPECT_DOUBLE_EQ(horner1(coeffs, 1.0), 6.0);
    
    // At u=2: 1 + 4 + 12 = 17
    EXPECT_DOUBLE_EQ(horner1(coeffs, 2.0), 17.0);
    
    // At u=0.5: 1 + 1 + 0.75 = 2.75
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.5), 2.75);
}

TEST(Horner1Test, QuadraticVector_ParabolicArc) {
    // Parabolic arc in 2D: y = x²
    // P(u) = (0,0) + (0,0)u + (1,1)u²
    // This gives point (u, u²) which is a parabola
    std::array<Vector<double, 2>, 3> coeffs{
        Vector<double, 2>(0.0, 0.0),
        Vector<double, 2>(0.0, 0.0),
        Vector<double, 2>(1.0, 1.0)
    };
    
    // At u=0.5: point should be at (0.25, 0.25)
    auto p = horner1(coeffs, 0.5);
    EXPECT_DOUBLE_EQ(p[0], 0.25);
    EXPECT_DOUBLE_EQ(p[1], 0.25);
    
    // At u=1: point should be at (1, 1)
    auto p1 = horner1(coeffs, 1.0);
    EXPECT_DOUBLE_EQ(p1[0], 1.0);
    EXPECT_DOUBLE_EQ(p1[1], 1.0);
}

TEST(Horner1Test, QuadraticVector_BezierCurve) {
    // Quadratic Bézier curve with control points
    // This is actually the Bernstein form, but we can verify the shape
    std::array<Vector<double, 2>, 3> coeffs{
        Vector<double, 2>(0.0, 0.0),   // Start point
        Vector<double, 2>(2.0, 4.0),   // Influences shape
        Vector<double, 2>(4.0, 0.0)    // End curvature
    };
    
    // Verify endpoints
    auto p0 = horner1(coeffs, 0.0);
    EXPECT_DOUBLE_EQ(p0[0], 0.0);
    EXPECT_DOUBLE_EQ(p0[1], 0.0);
    
    auto p1 = horner1(coeffs, 1.0);
    EXPECT_DOUBLE_EQ(p1[0], 6.0);  // 0 + 2 + 4
    EXPECT_DOUBLE_EQ(p1[1], 4.0);  // 0 + 4 + 0
    
    // Midpoint
    auto p_mid = horner1(coeffs, 0.5);
    EXPECT_DOUBLE_EQ(p_mid[0], 2.0);  // 0 + 1 + 1
    EXPECT_DOUBLE_EQ(p_mid[1], 2.0);  // 0 + 2 + 0
}

// ============================================================================
// Constant Polynomial Tests
// ============================================================================

TEST(Horner1Test, ConstantScalar) {
    // p(u) = 42 (constant)
    std::array<double, 1> coeffs{42.0};
    
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.0), 42.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.5), 42.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 1.0), 42.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 10.0), 42.0);
}

TEST(Horner1Test, ConstantVector) {
    // Constant point
    std::array<Vector<double, 3>, 1> coeffs{
        Vector<double, 3>(1.0, 2.0, 3.0)
    };
    
    auto p = horner1(coeffs, 0.7);
    EXPECT_DOUBLE_EQ(p[0], 1.0);
    EXPECT_DOUBLE_EQ(p[1], 2.0);
    EXPECT_DOUBLE_EQ(p[2], 3.0);
}

// ============================================================================
// Higher Degree Polynomial Tests
// ============================================================================

TEST(Horner1Test, CubicScalar) {
    // p(u) = 1 + 2u + 3u² + 4u³
    std::array<double, 4> coeffs{1.0, 2.0, 3.0, 4.0};
    
    // At u=0: 1
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.0), 1.0);
    
    // At u=1: 1 + 2 + 3 + 4 = 10
    EXPECT_DOUBLE_EQ(horner1(coeffs, 1.0), 10.0);
    
    // At u=2: 1 + 4 + 12 + 32 = 49
    EXPECT_DOUBLE_EQ(horner1(coeffs, 2.0), 49.0);
}

TEST(Horner1Test, CubicVector_3D) {
    // Cubic curve in 3D
    std::array<Vector<double, 3>, 4> coeffs{
        Vector<double, 3>(0.0, 0.0, 0.0),
        Vector<double, 3>(1.0, 0.0, 0.0),
        Vector<double, 3>(0.0, 1.0, 0.0),
        Vector<double, 3>(0.0, 0.0, 1.0)
    };
    
    auto p = horner1(coeffs, 1.0);
    EXPECT_DOUBLE_EQ(p[0], 1.0);
    EXPECT_DOUBLE_EQ(p[1], 1.0);
    EXPECT_DOUBLE_EQ(p[2], 1.0);
}

TEST(Horner1Test, QuarticScalar) {
    // p(u) = 1 + u + u² + u³ + u⁴
    std::array<double, 5> coeffs{1.0, 1.0, 1.0, 1.0, 1.0};
    
    // At u=1: 5
    EXPECT_DOUBLE_EQ(horner1(coeffs, 1.0), 5.0);
    
    // At u=2: 1 + 2 + 4 + 8 + 16 = 31
    EXPECT_DOUBLE_EQ(horner1(coeffs, 2.0), 31.0);
}

// ============================================================================
// Special Cases and Edge Cases
// ============================================================================

TEST(Horner1Test, ZeroCoefficients) {
    // p(u) = 0 + 0u + 0u²
    std::array<double, 3> coeffs{0.0, 0.0, 0.0};
    
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 0.5), 0.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 1.0), 0.0);
}

TEST(Horner1Test, NegativeCoefficients) {
    // p(u) = -1 - 2u - 3u²
    std::array<double, 3> coeffs{-1.0, -2.0, -3.0};
    
    // At u=1: -6
    EXPECT_DOUBLE_EQ(horner1(coeffs, 1.0), -6.0);
    
    // At u=-1: -1 + 2 - 3 = -2
    EXPECT_DOUBLE_EQ(horner1(coeffs, -1.0), -2.0);
}

TEST(Horner1Test, NegativeParameterValue) {
    // p(u) = 1 + 2u + 3u²
    std::array<double, 3> coeffs{1.0, 2.0, 3.0};
    
    // At u=-1: 1 - 2 + 3 = 2
    EXPECT_DOUBLE_EQ(horner1(coeffs, -1.0), 2.0);
}

TEST(Horner1Test, LargeParameterValue) {
    // p(u) = 1 + u
    std::array<double, 2> coeffs{1.0, 1.0};
    
    EXPECT_DOUBLE_EQ(horner1(coeffs, 100.0), 101.0);
    EXPECT_DOUBLE_EQ(horner1(coeffs, 1000.0), 1001.0);
}

// ============================================================================
// Integer Types
// ============================================================================

TEST(Horner1Test, IntegerCoefficients) {
    // p(u) = 1 + 2u + 3u²
    std::array<int, 3> coeffs{1, 2, 3};
    
    EXPECT_EQ(horner1(coeffs, 2), 17);
    EXPECT_EQ(horner1(coeffs, 0), 1);
    EXPECT_EQ(horner1(coeffs, 1), 6);
}

TEST(Horner1Test, IntegerVectors) {
    std::array<Vector<int, 2>, 2> coeffs{
        Vector<int, 2>(0, 0),
        Vector<int, 2>(10, 5)
    };
    
    auto p = horner1(coeffs, 3);
    EXPECT_EQ(p[0], 30);
    EXPECT_EQ(p[1], 15);
}

// ============================================================================
// Constexpr Evaluation
// ============================================================================

TEST(Horner1Test, ConstexprScalar) {
    constexpr std::array<double, 3> coeffs{1.0, 2.0, 3.0};
    constexpr double result = horner1(coeffs, 2.0);
    
    EXPECT_DOUBLE_EQ(result, 17.0);
    
    // This being constexpr means it's computed at compile time
    static_assert(result == 17.0, "Compile-time evaluation failed");
}

TEST(Horner1Test, ConstexprVector) {
    constexpr std::array<Vector<int, 2>, 2> coeffs{
        Vector<int, 2>(1, 2),
        Vector<int, 2>(3, 4)
    };
    constexpr auto result = horner1(coeffs, 2);
    
    EXPECT_EQ(result[0], 7);   // 1 + 3*2
    EXPECT_EQ(result[1], 10);  // 2 + 4*2
}

// ============================================================================
// Floating Point Precision
// ============================================================================

TEST(Horner1Test, FloatType) {
    std::array<float, 3> coeffs{1.0f, 2.0f, 3.0f};
    
    float result = horner1(coeffs, 0.5f);
    EXPECT_FLOAT_EQ(result, 2.75f);
}

TEST(Horner1Test, MixedPrecision) {
    // Double coefficients with float parameter
    std::array<double, 2> coeffs{1.0, 2.0};
    
    double result = horner1(coeffs, 0.5f);
    EXPECT_DOUBLE_EQ(result, 2.0);
}

// ============================================================================
// Known Polynomial Identities
// ============================================================================

TEST(Horner1Test, GeometricSeries) {
    // 1 + u + u² + u³ = (1 - u⁴)/(1 - u) for u ≠ 1
    std::array<double, 4> coeffs{1.0, 1.0, 1.0, 1.0};
    
    double u = 0.5;
    double horner_result = horner1(coeffs, u);
    double formula_result = (1.0 - std::pow(u, 4)) / (1.0 - u);
    
    EXPECT_NEAR(horner_result, formula_result, 1e-10);
}

TEST(Horner1Test, Factorization) {
    // (u - 1)(u - 2) = u² - 3u + 2
    std::array<double, 3> coeffs{2.0, -3.0, 1.0};
    
    // Should be zero at roots u=1 and u=2
    EXPECT_NEAR(horner1(coeffs, 1.0), 0.0, 1e-10);
    EXPECT_NEAR(horner1(coeffs, 2.0), 0.0, 1e-10);
}


