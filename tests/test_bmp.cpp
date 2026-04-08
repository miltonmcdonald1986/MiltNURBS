#include <gtest/gtest.h>
#include "miltnurbs/bezier.h"
#include "miltnurbs/bmp.h"
#include <filesystem>

using miltnurbs::bezier::BezierCurve;
using miltnurbs::vector::Vector;
using miltnurbs::bmp::Color;
using miltnurbs::bmp::render_to_bmp;
using miltnurbs::bmp::RenderConfig;

// Helper to check if file was created
bool file_exists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

// ============================================================================
// Basic Rendering Tests
// ============================================================================

TEST(BezierBMPTest, RenderLinearCurve) {
    BezierCurve<double, 2, 1> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(10.0, 10.0);

    bool success = render_to_bmp(curve, "test_linear.bmp");

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_linear.bmp"));
}

TEST(BezierBMPTest, RenderQuadraticCurve) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(5.0, 10.0);
    curve.control_points[2] = Vector<double, 2>(10.0, 0.0);

    bool success = render_to_bmp(curve, "test_quadratic.bmp");

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_quadratic.bmp"));
}

TEST(BezierBMPTest, RenderCubicCurve) {
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 3.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 3.0);
    curve.control_points[3] = Vector<double, 2>(3.0, 0.0);

    bool success = render_to_bmp(curve, "test_cubic.bmp");

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_cubic.bmp"));
}

// ============================================================================
// Custom Configuration Tests
// ============================================================================

TEST(BezierBMPTest, CustomConfiguration) {
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 2.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 2.0);
    curve.control_points[3] = Vector<double, 2>(3.0, 0.0);

    RenderConfig config;
    config.image_width = 1024;
    config.image_height = 768;
    config.samples = 200;
    config.curve_color = Color::Red();
    config.control_point_color = Color::Blue();
    config.background_color = Color{ 240, 240, 255 };

    bool success = render_to_bmp(curve, "test_custom_config.bmp", config);

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_custom_config.bmp"));
}

TEST(BezierBMPTest, NoControlPoints) {
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 2.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 2.0);
    curve.control_points[3] = Vector<double, 2>(3.0, 0.0);

    RenderConfig config;
    config.draw_control_points = false;
    config.draw_control_polygon = false;
    config.curve_color = Color::Black();

    bool success = render_to_bmp(curve, "test_no_control_points.bmp", config);

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_no_control_points.bmp"));
}

// ============================================================================
// Visual Test Gallery (Usage Examples)
// ============================================================================

TEST(BezierBMPTest, Gallery_SimpleCubic) {
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 3.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 3.0);
    curve.control_points[3] = Vector<double, 2>(3.0, 0.0);

    RenderConfig config;
    config.samples = 200;
    config.curve_thickness = 3;

    bool success = render_to_bmp(curve, "gallery_simple_cubic.bmp", config);
    EXPECT_TRUE(success);
}

TEST(BezierBMPTest, Gallery_SShape) {
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(0.0, 2.0);
    curve.control_points[2] = Vector<double, 2>(2.0, -1.0);
    curve.control_points[3] = Vector<double, 2>(2.0, 1.0);

    RenderConfig config;
    config.curve_color = Color::Green();
    config.samples = 300;

    bool success = render_to_bmp(curve, "gallery_s_shape.bmp", config);
    EXPECT_TRUE(success);
}

TEST(BezierBMPTest, Gallery_Loop) {
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(0.0, 3.0);
    curve.control_points[2] = Vector<double, 2>(3.0, -3.0);
    curve.control_points[3] = Vector<double, 2>(3.0, 0.0);

    RenderConfig config;
    config.curve_color = Color{ 255, 0, 255 };  // Magenta
    config.samples = 400;

    bool success = render_to_bmp(curve, "gallery_loop.bmp", config);
    EXPECT_TRUE(success);
}

TEST(BezierBMPTest, Gallery_HighDegree) {
    BezierCurve<double, 2, 5> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 2.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 2.5);
    curve.control_points[3] = Vector<double, 2>(3.0, 1.5);
    curve.control_points[4] = Vector<double, 2>(4.0, 2.0);
    curve.control_points[5] = Vector<double, 2>(5.0, 0.0);

    RenderConfig config;
    config.curve_color = Color{ 255, 128, 0 };  // Orange
    config.samples = 500;
    config.image_width = 1200;

    bool success = render_to_bmp(curve, "gallery_high_degree.bmp", config);
    EXPECT_TRUE(success);
}

TEST(BezierBMPTest, Gallery_Circle_Approximation) {
    // Approximate a circle arc with cubic Bézier
    const double k = 0.551915024494;  // Magic constant for circle approximation

    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(1.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, k);
    curve.control_points[2] = Vector<double, 2>(k, 1.0);
    curve.control_points[3] = Vector<double, 2>(0.0, 1.0);

    RenderConfig config;
    config.curve_color = Color::Blue();
    config.samples = 200;
    config.background_color = Color{ 250, 250, 250 };

    bool success = render_to_bmp(curve, "gallery_circle_arc.bmp", config);
    EXPECT_TRUE(success);
}

TEST(BezierBMPTest, Gallery_Heart) {
    // One half of a heart shape
    BezierCurve<double, 2, 3> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(-2.0, 2.0);
    curve.control_points[2] = Vector<double, 2>(-1.5, 3.5);
    curve.control_points[3] = Vector<double, 2>(0.0, 3.0);

    RenderConfig config;
    config.curve_color = Color::Red();
    config.control_polygon_color = Color{ 200, 200, 200 };
    config.samples = 200;

    bool success = render_to_bmp(curve, "gallery_heart.bmp", config);
    EXPECT_TRUE(success);
}

TEST(BezierBMPTest, Gallery_Wiggle) {
    BezierCurve<double, 2, 5> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, -1.0);
    curve.control_points[3] = Vector<double, 2>(3.0, 1.0);
    curve.control_points[4] = Vector<double, 2>(4.0, -1.0);
    curve.control_points[5] = Vector<double, 2>(5.0, 0.0);

    RenderConfig config;
    config.curve_color = Color{ 0, 128, 255 };  // Light blue
    config.samples = 500;
    config.image_width = 1000;
    config.image_height = 600;

    bool success = render_to_bmp(curve, "gallery_wiggle.bmp", config);
    EXPECT_TRUE(success);
}

// ============================================================================
// Edge Cases
// ============================================================================

TEST(BezierBMPTest, SinglePoint) {
    BezierCurve<double, 2, 0> curve;
    curve.control_points[0] = Vector<double, 2>(5.0, 5.0);

    bool success = render_to_bmp(curve, "test_single_point.bmp");

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_single_point.bmp"));
}

TEST(BezierBMPTest, VerticalLine) {
    BezierCurve<double, 2, 1> curve;
    curve.control_points[0] = Vector<double, 2>(5.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(5.0, 10.0);

    bool success = render_to_bmp(curve, "test_vertical.bmp");

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_vertical.bmp"));
}

TEST(BezierBMPTest, HorizontalLine) {
    BezierCurve<double, 2, 1> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 5.0);
    curve.control_points[1] = Vector<double, 2>(10.0, 5.0);

    bool success = render_to_bmp(curve, "test_horizontal.bmp");

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_horizontal.bmp"));
}

TEST(BezierBMPTest, VerySmallCurve) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(0.01, 0.01);
    curve.control_points[2] = Vector<double, 2>(0.02, 0.0);

    bool success = render_to_bmp(curve, "test_small_curve.bmp");

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_small_curve.bmp"));
}

// ============================================================================
// Different Image Sizes
// ============================================================================

TEST(BezierBMPTest, SmallImage) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 0.0);

    RenderConfig config;
    config.image_width = 200;
    config.image_height = 200;

    bool success = render_to_bmp(curve, "test_small_image.bmp", config);

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_small_image.bmp"));
}

TEST(BezierBMPTest, LargeImage) {
    BezierCurve<double, 2, 2> curve;
    curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
    curve.control_points[1] = Vector<double, 2>(1.0, 1.0);
    curve.control_points[2] = Vector<double, 2>(2.0, 0.0);

    RenderConfig config;
    config.image_width = 1920;
    config.image_height = 1080;
    config.samples = 500;

    bool success = render_to_bmp(curve, "test_large_image.bmp", config);

    EXPECT_TRUE(success);
    EXPECT_TRUE(file_exists("test_large_image.bmp"));
}
