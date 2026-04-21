#include <gtest/gtest.h>
#include "miltnurbs/bezier.h"
#include "miltnurbs/vector.h"

#include <cmath>

using miltnurbs::bezier::BezierCurve;
using miltnurbs::vector::Vector;

// ============================================================================
// Construction and Basic Properties
// ============================================================================

TEST(BezierCurveTest, ControlPointsAccessible) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 0.0);

    EXPECT_DOUBLE_EQ(curve.control_points[0][0], 0.0);
    EXPECT_DOUBLE_EQ(curve.control_points[1][1], 1.0);
    EXPECT_DOUBLE_EQ(curve.control_points[2][0], 2.0);
}

TEST(BezierCurveTest, CorrectNumberOfControlPoints) {
    BezierCurve<double, 3, 0> degree0;  // 1 control point
    EXPECT_EQ(degree0.control_points.size(), 1);

    BezierCurve<double, 3, 1> degree1;  // 2 control points
    EXPECT_EQ(degree1.control_points.size(), 2);

    BezierCurve<double, 3, 3> degree3;  // 4 control points
    EXPECT_EQ(degree3.control_points.size(), 4);
}

// ============================================================================
// Endpoint Tests
// ============================================================================

TEST(BezierCurveTest, EndpointAtZero_Linear) {
    BezierCurve<double, 2, 1> curve;
    curve.control_points[0] = Vector<double, 2>(1.0, 2.0);
    curve.control_points[1] = Vector<double, 2>(3.0, 4.0);

    auto p = curve.evaluate(0.0);
    EXPECT_DOUBLE_EQ(p[0], 1.0);
    EXPECT_DOUBLE_EQ(p[1], 2.0);
}

TEST(BezierCurveTest, EndpointAtOne_Linear) {
    BezierCurve<double, 2, 1> curve;
    curve.control_points[0] = Vector<double, 2>(1.0, 2.0);
    curve.control_points[1] = Vector<double, 2>(3.0, 4.0);

    auto p = curve.evaluate(1.0);
    EXPECT_DOUBLE_EQ(p[0], 3.0);
    EXPECT_DOUBLE_EQ(p[1], 4.0);
}

TEST(BezierCurveTest, EndpointAtZero_Cubic) {
    BezierCurve<double, 3, 3> curve;
    curve.control_points[0] = Vector<double, 3>(0.0, 0.0, 0.0);
    curve.control_points[1] = Vector<double, 3>(1.0, 2.0, 0.0);
    curve.control_points[2] = Vector<double, 3>(2.0, 2.0, 1.0);
    curve.control_points[3] = Vector<double, 3>(3.0, 0.0, 0.0);

    auto p = curve.evaluate(0.0);
    EXPECT_DOUBLE_EQ(p[0], 0.0);
    EXPECT_DOUBLE_EQ(p[1], 0.0);
    EXPECT_DOUBLE_EQ(p[2], 0.0);
}

TEST(BezierCurveTest, EndpointAtOne_Cubic) {
    BezierCurve<double, 3, 3> curve;
    curve.control_points[0] = Vector<double, 3>(0.0, 0.0, 0.0);
    curve.control_points[1] = Vector<double, 3>(1.0, 2.0, 0.0);
    curve.control_points[2] = Vector<double, 3>(2.0, 2.0, 1.0);
    curve.control_points[3] = Vector<double, 3>(3.0, 0.0, 0.0);

    auto p = curve.evaluate(1.0);
    EXPECT_DOUBLE_EQ(p[0], 3.0);
    EXPECT_DOUBLE_EQ(p[1], 0.0);
    EXPECT_DOUBLE_EQ(p[2], 0.0);
}

// ============================================================================
// Linear Bézier Curves (Degree 1)
// ============================================================================

TEST(BezierCurveTest, Linear_2D_Midpoint) {
    BezierCurve<double, 2, 1> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(10.0, 20.0);

    auto p = curve.evaluate(0.5);
    EXPECT_DOUBLE_EQ(p[0], 5.0);
    EXPECT_DOUBLE_EQ(p[1], 10.0);
}

TEST(BezierCurveTest, Linear_3D_QuarterPoint) {
    BezierCurve<double, 3, 1> curve;
    curve.control_points[0] = Vector<double, 3>(0.0, 0.0, 0.0);
    curve.control_points[1] = Vector<double, 3>(4.0, 8.0, 12.0);

    auto p = curve.evaluate(0.25);
    EXPECT_DOUBLE_EQ(p[0], 1.0);
    EXPECT_DOUBLE_EQ(p[1], 2.0);
    EXPECT_DOUBLE_EQ(p[2], 3.0);
}

// ============================================================================
// Quadratic Bézier Curves (Degree 2)
// ============================================================================

TEST(BezierCurveTest, Quadratic_2D_Midpoint) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 2.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 0.0);

    // At u=0.5: B_0=0.25, B_1=0.5, B_2=0.25
    // x = 0.25*0 + 0.5*1 + 0.25*2 = 1.0
    // y = 0.25*0 + 0.5*2 + 0.25*0 = 1.0
    auto p = curve.evaluate(0.5);
    EXPECT_DOUBLE_EQ(p[0], 1.0);
    EXPECT_DOUBLE_EQ(p[1], 1.0);
}

TEST(BezierCurveTest, Quadratic_2D_QuarterPoint) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 0.0);

    // At u=0.25: B_0=0.5625, B_1=0.375, B_2=0.0625
    auto p = curve.evaluate(0.25);
    EXPECT_NEAR(p[0], 0.5, 1e-15);
    EXPECT_NEAR(p[1], 0.375, 1e-15);
}

// ============================================================================
// Cubic Bézier Curves (Degree 3)
// ============================================================================

TEST(BezierCurveTest, Cubic_2D_Midpoint) {
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(0.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[3] = Vector<double, 2>(1.0, 0.0);

    auto p = curve.evaluate(0.5);
    // At u=0.5: all basis functions are 0.125, 0.375, 0.375, 0.125
    // x = 0.125*0 + 0.375*0 + 0.375*1 + 0.125*1 = 0.5
    // y = 0.125*0 + 0.375*1 + 0.375*1 + 0.125*0 = 0.75
    EXPECT_DOUBLE_EQ(p[0], 0.5);
    EXPECT_DOUBLE_EQ(p[1], 0.75);
}

TEST(BezierCurveTest, Cubic_3D_General) {
    BezierCurve<double, 3, 3> curve;
    curve.control_points[0] = Vector<double, 3>(0.0, 0.0, 0.0);
    curve.control_points[1] = Vector<double, 3>(1.0, 0.0, 0.0);
    curve.control_points[2] = Vector<double, 3>(1.0, 1.0, 0.0);
    curve.control_points[3] = Vector<double, 3>(1.0, 1.0, 1.0);

    auto p = curve.evaluate(0.5);
    EXPECT_DOUBLE_EQ(p[0], 0.875);
    EXPECT_DOUBLE_EQ(p[1], 0.5);
    EXPECT_DOUBLE_EQ(p[2], 0.125);
}

// ============================================================================
// Degree 0 (Single Point)
// ============================================================================

TEST(BezierCurveTest, Degree0_ConstantCurve) {
    BezierCurve<double, 2, 0> curve;
    curve.control_points[0] = Vector<double, 2>(5.0, 7.0);

    // Should always return the single control point
    auto p1 = curve.evaluate(0.0);
    auto p2 = curve.evaluate(0.5);
    auto p3 = curve.evaluate(1.0);

    EXPECT_DOUBLE_EQ(p1[0], 5.0);
    EXPECT_DOUBLE_EQ(p1[1], 7.0);
    EXPECT_DOUBLE_EQ(p2[0], 5.0);
    EXPECT_DOUBLE_EQ(p2[1], 7.0);
    EXPECT_DOUBLE_EQ(p3[0], 5.0);
    EXPECT_DOUBLE_EQ(p3[1], 7.0);
}

// ============================================================================
// Extrapolation Tests (u outside [0,1])
// ============================================================================

TEST(BezierCurveTest, Extrapolation_Linear_Beyond1) {
    BezierCurve<double, 2, 1> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(10.0, 0.0);

    auto p = curve.evaluate(2.0);  // Extrapolate
    EXPECT_DOUBLE_EQ(p[0], 20.0);
    EXPECT_DOUBLE_EQ(p[1], 0.0);
}

TEST(BezierCurveTest, Extrapolation_Linear_BeforeZero) {
    BezierCurve<double, 2, 1> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(10.0, 0.0);

    auto p = curve.evaluate(-1.0);  // Extrapolate backwards
    EXPECT_DOUBLE_EQ(p[0], -10.0);
    EXPECT_DOUBLE_EQ(p[1], 0.0);
}

TEST(BezierCurveTest, Extrapolation_Quadratic) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 0.0);

    auto p = curve.evaluate(1.5);
    // Should extrapolate smoothly
    EXPECT_GT(p[0], 2.0);
    EXPECT_LT(p[1], 0.0);
}

// ============================================================================
// Comparison: evaluate vs evaluate_decasteljau
// ============================================================================

TEST(BezierCurveTest, BothMethodsMatch_Linear) {
    BezierCurve<double, 2, 1> curve;
    curve.control_points[0] = Vector<double, 2>(1.0, 2.0);
    curve.control_points[1] = Vector<double, 2>(3.0, 4.0);

    for (double u = 0.0; u <= 1.0; u += 0.1) {
        auto p1 = curve.evaluate(u);
        auto p2 = curve.evaluate_decasteljau(u);

        EXPECT_NEAR(p1[0], p2[0], 1e-14);
        EXPECT_NEAR(p1[1], p2[1], 1e-14);
    }
}

TEST(BezierCurveTest, BothMethodsMatch_Quadratic) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 2.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 0.0);

    for (double u = 0.0; u <= 1.0; u += 0.1) {
        auto p1 = curve.evaluate(u);
        auto p2 = curve.evaluate_decasteljau(u);

        EXPECT_NEAR(p1[0], p2[0], 1e-14);
        EXPECT_NEAR(p1[1], p2[1], 1e-14);
    }
}

TEST(BezierCurveTest, BothMethodsMatch_Cubic) {
    BezierCurve<double, 3, 3> curve;
    curve.control_points[0] = Vector<double, 3>(0.0, 0.0, 0.0);
    curve.control_points[1] = Vector<double, 3>(1.0, 2.0, 0.0);
    curve.control_points[2] = Vector<double, 3>(2.0, 2.0, 1.0);
    curve.control_points[3] = Vector<double, 3>(3.0, 0.0, 0.0);

    for (double u = 0.0; u <= 1.0; u += 0.1) {
        auto p1 = curve.evaluate(u);
        auto p2 = curve.evaluate_decasteljau(u);

        EXPECT_NEAR(p1[0], p2[0], 1e-14);
        EXPECT_NEAR(p1[1], p2[1], 1e-14);
        EXPECT_NEAR(p1[2], p2[2], 1e-14);
    }
}

TEST(BezierCurveTest, BothMethodsMatch_HighDegree) {
    BezierCurve<double, 2, 5> curve;
    for (size_t i = 0; i <= 5; ++i) {
        curve.control_points[i] = Vector<double, 2>(
            static_cast<double>(i),
            static_cast<double>(i % 2)
        );
    }

    for (double u = 0.0; u <= 1.0; u += 0.2) {
        auto p1 = curve.evaluate(u);
        auto p2 = curve.evaluate_decasteljau(u);

        EXPECT_NEAR(p1[0], p2[0], 1e-13);
        EXPECT_NEAR(p1[1], p2[1], 1e-13);
    }
}

TEST(BezierCurveTest, BothMethodsMatch_Extrapolation) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 0.0);

    // Test extrapolation
    auto p1 = curve.evaluate(1.5);
    auto p2 = curve.evaluate_decasteljau(1.5);

    EXPECT_NEAR(p1[0], p2[0], 1e-14);
    EXPECT_NEAR(p1[1], p2[1], 1e-14);

    auto p3 = curve.evaluate(-0.5);
    auto p4 = curve.evaluate_decasteljau(-0.5);

    EXPECT_NEAR(p3[0], p4[0], 1e-14);
    EXPECT_NEAR(p3[1], p4[1], 1e-14);
}

// ============================================================================
// de Casteljau Specific Tests
// ============================================================================

TEST(BezierCurveTest, DeCasteljau_EndpointsCorrect) {
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 1.0);
    curve.control_points[3] = Vector<double, 2>(3.0, 0.0);

    auto p0 = curve.evaluate_decasteljau(0.0);
    auto p1 = curve.evaluate_decasteljau(1.0);

    EXPECT_DOUBLE_EQ(p0[0], 0.0);
    EXPECT_DOUBLE_EQ(p0[1], 0.0);
    EXPECT_DOUBLE_EQ(p1[0], 3.0);
    EXPECT_DOUBLE_EQ(p1[1], 0.0);
}

// ============================================================================
// Float Type Tests
// ============================================================================

TEST(BezierCurveTest, FloatType) {
    BezierCurve<float, 2, 2> curve;
    curve.control_points[0] = Vector<float, 2>(0.0f, 0.0f);
    curve.control_points[1] = Vector<float, 2>(1.0f, 2.0f);
    curve.control_points[2] = Vector<float, 2>(2.0f, 0.0f);

    auto p = curve.evaluate(0.5f);

    EXPECT_FLOAT_EQ(p[0], 1.0f);
    EXPECT_FLOAT_EQ(p[1], 1.0f);
}

TEST(BezierCurveTest, FloatType_DeCasteljau) {
    BezierCurve<float, 2, 2> curve;
    curve.control_points[0] = Vector<float, 2>(0.0f, 0.0f);
    curve.control_points[1] = Vector<float, 2>(1.0f, 2.0f);
    curve.control_points[2] = Vector<float, 2>(2.0f, 0.0f);

    auto p = curve.evaluate_decasteljau(0.5f);

    EXPECT_FLOAT_EQ(p[0], 1.0f);
    EXPECT_FLOAT_EQ(p[1], 1.0f);
}

// ============================================================================
// Special Geometric Cases
// ============================================================================

TEST(BezierCurveTest, StraightLine_AllPointsCollinear) {
    // All control points on a line should produce a straight line
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 2.0);
    curve.control_points[3] = Vector<double, 2>(3.0, 3.0);

    // Should lie on the line y=x
    for (double u = 0.0; u <= 1.0; u += 0.1) {
        auto p = curve.evaluate(u);
        EXPECT_NEAR(p[0], p[1], 1e-14);  // x should equal y
    }
}

TEST(BezierCurveTest, DuplicateControlPoints) {
    // Duplicate control points create a lower-degree curve
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(0.0, 0.0);  // Duplicate
    curve.control_points[2] = Vector<double, 2>(1.0, 0.0);
    curve.control_points[3] = Vector<double, 2>(1.0, 0.0);  // Duplicate

    auto p0 = curve.evaluate(0.0);
    auto p1 = curve.evaluate(1.0);

    EXPECT_DOUBLE_EQ(p0[0], 0.0);
    EXPECT_DOUBLE_EQ(p1[0], 1.0);
}

// ============================================================================
// Higher Dimensions
// ============================================================================

TEST(BezierCurveTest, FourDimensional) {
    BezierCurve<double, 4, 1> curve;
    curve.control_points[0] = Vector<double, 4>(0.0, 0.0, 0.0, 1.0);
    curve.control_points[1] = Vector<double, 4>(1.0, 1.0, 1.0, 1.0);

    auto p = curve.evaluate(0.5);

    EXPECT_DOUBLE_EQ(p[0], 0.5);
    EXPECT_DOUBLE_EQ(p[1], 0.5);
    EXPECT_DOUBLE_EQ(p[2], 0.5);
    EXPECT_DOUBLE_EQ(p[3], 1.0);
}

// ============================================================================
// Symmetry Tests
// ============================================================================

TEST(BezierCurveTest, SymmetricCurve) {
    // Symmetric control points should produce symmetric curve
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 1.0);
    curve.control_points[3] = Vector<double, 2>(3.0, 0.0);

    auto p1 = curve.evaluate(0.25);
    auto p2 = curve.evaluate(0.75);

    // Should be symmetric about x=1.5
    EXPECT_NEAR(p1[0] + p2[0], 3.0, 1e-14);
    EXPECT_NEAR(p1[1], p2[1], 1e-14);
}
