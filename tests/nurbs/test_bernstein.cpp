#include <gtest/gtest.h>
#include "miltnurbs/bernstein.h"

#include <cmath>
#include <numbers>
#include <numeric>

using miltnurbs::bernstein::all_bernstein;
using miltnurbs::bernstein::bernstein;

// ============================================================================
// Endpoint Value Tests
// ============================================================================

TEST(BernsteinTest, EndpointAtZero) {
    // B_{i,n}(0) should be 1 only when i=0, otherwise 0

    // Degree 2
    EXPECT_DOUBLE_EQ(bernstein(0, 2, 0.0), 1.0);
    EXPECT_DOUBLE_EQ(bernstein(1, 2, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(bernstein(2, 2, 0.0), 0.0);

    // Degree 3
    EXPECT_DOUBLE_EQ(bernstein(0, 3, 0.0), 1.0);
    EXPECT_DOUBLE_EQ(bernstein(1, 3, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(bernstein(2, 3, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(bernstein(3, 3, 0.0), 0.0);
}

TEST(BernsteinTest, EndpointAtOne) {
    // B_{i,n}(1) should be 1 only when i=n, otherwise 0

    // Degree 2
    EXPECT_DOUBLE_EQ(bernstein(0, 2, 1.0), 0.0);
    EXPECT_DOUBLE_EQ(bernstein(1, 2, 1.0), 0.0);
    EXPECT_DOUBLE_EQ(bernstein(2, 2, 1.0), 1.0);

    // Degree 3
    EXPECT_DOUBLE_EQ(bernstein(0, 3, 1.0), 0.0);
    EXPECT_DOUBLE_EQ(bernstein(1, 3, 1.0), 0.0);
    EXPECT_DOUBLE_EQ(bernstein(2, 3, 1.0), 0.0);
    EXPECT_DOUBLE_EQ(bernstein(3, 3, 1.0), 1.0);
}

// ============================================================================
// Partition of Unity Tests
// ============================================================================

TEST(BernsteinTest, PartitionOfUnity_Degree1) {
    // Sum of all basis functions should equal 1
    double u = 0.3;
    double sum = bernstein(0, 1, u) + bernstein(1, 1, u);
    EXPECT_NEAR(sum, 1.0, 1e-15);
}

TEST(BernsteinTest, PartitionOfUnity_Degree2) {
    double u = 0.7;
    double sum = 0.0;
    for (size_t i = 0; i <= 2; ++i) {
        sum += bernstein(i, 2, u);
    }
    EXPECT_NEAR(sum, 1.0, 1e-15);
}

TEST(BernsteinTest, PartitionOfUnity_Degree3) {
    double u = 0.5;
    double sum = 0.0;
    for (size_t i = 0; i <= 3; ++i) {
        sum += bernstein(i, 3, u);
    }
    EXPECT_NEAR(sum, 1.0, 1e-15);
}

TEST(BernsteinTest, PartitionOfUnity_Degree5) {
    double u = 0.25;
    double sum = 0.0;
    for (size_t i = 0; i <= 5; ++i) {
        sum += bernstein(i, 5, u);
    }
    EXPECT_NEAR(sum, 1.0, 1e-15);
}

TEST(BernsteinTest, PartitionOfUnity_MultipleValues) {
    // Test at multiple u values
    for (double u = 0.0; u <= 1.0; u += 0.1) {
        double sum = 0.0;
        for (size_t i = 0; i <= 4; ++i) {
            sum += bernstein(i, 4, u);
        }
        EXPECT_NEAR(sum, 1.0, 1e-14);
    }
}

// ============================================================================
// Symmetry Property Tests
// ============================================================================

TEST(BernsteinTest, SymmetryProperty_Degree2) {
    // B_{i,n}(u) = B_{n-i,n}(1-u)
    double u = 0.3;

    EXPECT_NEAR(bernstein(0, 2, u), bernstein(2, 2, 1.0 - u), 1e-15);
    EXPECT_NEAR(bernstein(1, 2, u), bernstein(1, 2, 1.0 - u), 1e-15);
    EXPECT_NEAR(bernstein(2, 2, u), bernstein(0, 2, 1.0 - u), 1e-15);
}

TEST(BernsteinTest, SymmetryProperty_Degree3) {
    double u = 0.6;

    EXPECT_NEAR(bernstein(0, 3, u), bernstein(3, 3, 1.0 - u), 1e-15);
    EXPECT_NEAR(bernstein(1, 3, u), bernstein(2, 3, 1.0 - u), 1e-15);
    EXPECT_NEAR(bernstein(2, 3, u), bernstein(1, 3, 1.0 - u), 1e-15);
    EXPECT_NEAR(bernstein(3, 3, u), bernstein(0, 3, 1.0 - u), 1e-15);
}

// ============================================================================
// Non-Negativity Tests
// ============================================================================

TEST(BernsteinTest, NonNegativity) {
    // All Bernstein polynomials should be non-negative for u in [0,1]
    for (double u = 0.0; u <= 1.0; u += 0.05) {
        for (size_t n = 1; n <= 5; ++n) {
            for (size_t i = 0; i <= n; ++i) {
                double b = bernstein(i, n, u);
                EXPECT_GE(b, 0.0) << "B_" << i << "," << n << "(" << u << ") < 0";
            }
        }
    }
}

// ============================================================================
// Known Value Tests - Linear (Degree 1)
// ============================================================================

TEST(BernsteinTest, Linear_Degree1) {
    // B_{0,1}(u) = 1-u
    // B_{1,1}(u) = u

    EXPECT_DOUBLE_EQ(bernstein(0, 1, 0.0), 1.0);
    EXPECT_DOUBLE_EQ(bernstein(1, 1, 0.0), 0.0);

    EXPECT_DOUBLE_EQ(bernstein(0, 1, 0.5), 0.5);
    EXPECT_DOUBLE_EQ(bernstein(1, 1, 0.5), 0.5);

    EXPECT_DOUBLE_EQ(bernstein(0, 1, 0.25), 0.75);
    EXPECT_DOUBLE_EQ(bernstein(1, 1, 0.25), 0.25);

    EXPECT_DOUBLE_EQ(bernstein(0, 1, 1.0), 0.0);
    EXPECT_DOUBLE_EQ(bernstein(1, 1, 1.0), 1.0);
}

// ============================================================================
// Known Value Tests - Quadratic (Degree 2)
// ============================================================================

TEST(BernsteinTest, Quadratic_Degree2_AtHalf) {
    // At u = 0.5:
    // B_{0,2}(0.5) = (1-0.5)^2 = 0.25
    // B_{1,2}(0.5) = 2 * 0.5 * 0.5 = 0.5
    // B_{2,2}(0.5) = 0.5^2 = 0.25

    EXPECT_DOUBLE_EQ(bernstein(0, 2, 0.5), 0.25);
    EXPECT_DOUBLE_EQ(bernstein(1, 2, 0.5), 0.5);
    EXPECT_DOUBLE_EQ(bernstein(2, 2, 0.5), 0.25);
}

TEST(BernsteinTest, Quadratic_Degree2_General) {
    double u = 0.3;

    // B_{0,2}(0.3) = (1-0.3)^2 = 0.49
    EXPECT_NEAR(bernstein(0, 2, u), 0.49, 1e-15);

    // B_{1,2}(0.3) = 2 * 0.3 * 0.7 = 0.42
    EXPECT_NEAR(bernstein(1, 2, u), 0.42, 1e-15);

    // B_{2,2}(0.3) = 0.3^2 = 0.09
    EXPECT_NEAR(bernstein(2, 2, u), 0.09, 1e-15);
}

// ============================================================================
// Known Value Tests - Cubic (Degree 3)
// ============================================================================

TEST(BernsteinTest, Cubic_Degree3_AtHalf) {
    // At u = 0.5:
    // B_{0,3}(0.5) = (0.5)^3 = 0.125
    // B_{1,3}(0.5) = 3 * 0.5 * (0.5)^2 = 0.375
    // B_{2,3}(0.5) = 3 * (0.5)^2 * 0.5 = 0.375
    // B_{3,3}(0.5) = (0.5)^3 = 0.125

    EXPECT_DOUBLE_EQ(bernstein(0, 3, 0.5), 0.125);
    EXPECT_DOUBLE_EQ(bernstein(1, 3, 0.5), 0.375);
    EXPECT_DOUBLE_EQ(bernstein(2, 3, 0.5), 0.375);
    EXPECT_DOUBLE_EQ(bernstein(3, 3, 0.5), 0.125);
}

TEST(BernsteinTest, Cubic_Degree3_AtQuarter) {
    double u = 0.25;

    // B_{0,3}(0.25) = (0.75)^3 = 0.421875
    EXPECT_NEAR(bernstein(0, 3, u), 0.421875, 1e-15);

    // B_{1,3}(0.25) = 3 * 0.25 * (0.75)^2 = 0.421875
    EXPECT_NEAR(bernstein(1, 3, u), 0.421875, 1e-15);

    // B_{2,3}(0.25) = 3 * (0.25)^2 * 0.75 = 0.140625
    EXPECT_NEAR(bernstein(2, 3, u), 0.140625, 1e-15);

    // B_{3,3}(0.25) = (0.25)^3 = 0.015625
    EXPECT_NEAR(bernstein(3, 3, u), 0.015625, 1e-15);
}

// ============================================================================
// Degree 0 Test
// ============================================================================

TEST(BernsteinTest, Degree0) {
    // B_{0,0}(u) = 1 for all u
    EXPECT_DOUBLE_EQ(bernstein(0, 0, 0.0), 1.0);
    EXPECT_DOUBLE_EQ(bernstein(0, 0, 0.5), 1.0);
    EXPECT_DOUBLE_EQ(bernstein(0, 0, 1.0), 1.0);
    EXPECT_DOUBLE_EQ(bernstein(0, 0, 0.7), 1.0);
}

// ============================================================================
// Higher Degree Tests
// ============================================================================

TEST(BernsteinTest, Degree5_Endpoints) {
    // Test degree 5 at key points
    for (size_t i = 0; i <= 5; ++i) {
        if (i == 0) {
            EXPECT_DOUBLE_EQ(bernstein(i, 5, 0.0), 1.0);
        }
        else {
            EXPECT_DOUBLE_EQ(bernstein(i, 5, 0.0), 0.0);
        }

        if (i == 5) {
            EXPECT_DOUBLE_EQ(bernstein(i, 5, 1.0), 1.0);
        }
        else {
            EXPECT_DOUBLE_EQ(bernstein(i, 5, 1.0), 0.0);
        }
    }
}

TEST(BernsteinTest, Degree10_PartitionOfUnity) {
    // Test higher degree still maintains partition of unity
    double sum = 0.0;
    for (size_t i = 0; i <= 10; ++i) {
        sum += bernstein(i, 10, 0.3);
    }
    EXPECT_NEAR(sum, 1.0, 1e-14);
}

// ============================================================================
// Float Type Tests
// ============================================================================

TEST(BernsteinTest, FloatType) {
    // Test with float instead of double
    float b0 = bernstein(0, 2, 0.5f);
    float b1 = bernstein(1, 2, 0.5f);
    float b2 = bernstein(2, 2, 0.5f);

    EXPECT_FLOAT_EQ(b0, 0.25f);
    EXPECT_FLOAT_EQ(b1, 0.5f);
    EXPECT_FLOAT_EQ(b2, 0.25f);

    EXPECT_NEAR(b0 + b1 + b2, 1.0f, 1e-6f);
}

TEST(BernsteinTest, LongDoubleType) {
    // Test with long double
    long double b = bernstein(1, 3, 0.5L);
    EXPECT_NEAR(static_cast<double>(b), 0.375, 1e-15);
}

// ============================================================================
// Constexpr Tests
// ============================================================================

TEST(BernsteinTest, ConstexprEvaluation) {
    // Verify compile-time evaluation works
    constexpr double b0 = bernstein(0, 2, 0.5);
    constexpr double b1 = bernstein(1, 2, 0.5);
    constexpr double b2 = bernstein(2, 2, 0.5);

    EXPECT_DOUBLE_EQ(b0, 0.25);
    EXPECT_DOUBLE_EQ(b1, 0.5);
    EXPECT_DOUBLE_EQ(b2, 0.25);

    static_assert(b0 == 0.25, "Constexpr evaluation failed");
    static_assert(b1 == 0.5, "Constexpr evaluation failed");
    static_assert(b2 == 0.25, "Constexpr evaluation failed");
}

// ============================================================================
// Maximum Value Tests
// ============================================================================

TEST(BernsteinTest, MaximumValue_Quadratic) {
    // B_{1,2}(u) = 2u(1-u) has maximum at u=0.5 with value 0.5
    double max_val = bernstein(1, 2, 0.5);
    EXPECT_DOUBLE_EQ(max_val, 0.5);

    // Check it's less at other points
    EXPECT_LT(bernstein(1, 2, 0.3), max_val);
    EXPECT_LT(bernstein(1, 2, 0.7), max_val);
}

TEST(BernsteinTest, MaximumValue_Cubic) {
    // B_{1,3}(u) = 3u(1-u)^2 has maximum at u=1/3
    double u_max = 1.0 / 3.0;
    double max_val = bernstein(1, 3, u_max);

    // Should be approximately 0.444...
    EXPECT_NEAR(max_val, 4.0 / 9.0, 1e-15);

    // Check it's less at nearby points
    EXPECT_LT(bernstein(1, 3, u_max - 0.1), max_val);
    EXPECT_LT(bernstein(1, 3, u_max + 0.1), max_val);
}

// ============================================================================
// Special Mathematical Values
// ============================================================================

TEST(BernsteinTest, GoldenRatio) {
    // Test at golden ratio for fun
    double phi = (1.0 + std::sqrt(5.0)) / 2.0 - 1.0;  // ~0.618

    double sum = 0.0;
    for (size_t i = 0; i <= 3; ++i) {
        sum += bernstein(i, 3, phi);
    }
    EXPECT_NEAR(sum, 1.0, 1e-14);
}

// ============================================================================
// Size and Consistency Tests
// ============================================================================

TEST(AllBernsteinTest, CorrectSize) {
    auto basis0 = all_bernstein(0, 0.5);
    EXPECT_EQ(basis0.size(), 1);

    auto basis1 = all_bernstein(1, 0.5);
    EXPECT_EQ(basis1.size(), 2);

    auto basis3 = all_bernstein(3, 0.5);
    EXPECT_EQ(basis3.size(), 4);

    auto basis10 = all_bernstein(10, 0.5);
    EXPECT_EQ(basis10.size(), 11);
}

TEST(AllBernsteinTest, MatchesIndividualCalls_Degree2) {
    double u = 0.3;
    auto basis = all_bernstein(2, u);

    EXPECT_DOUBLE_EQ(basis[0], bernstein(0, 2, u));
    EXPECT_DOUBLE_EQ(basis[1], bernstein(1, 2, u));
    EXPECT_DOUBLE_EQ(basis[2], bernstein(2, 2, u));
}

TEST(AllBernsteinTest, MatchesIndividualCalls_Degree3) {
    double u = 0.7;
    auto basis = all_bernstein(3, u);

    EXPECT_DOUBLE_EQ(basis[0], bernstein(0, 3, u));
    EXPECT_DOUBLE_EQ(basis[1], bernstein(1, 3, u));
    EXPECT_DOUBLE_EQ(basis[2], bernstein(2, 3, u));
    EXPECT_DOUBLE_EQ(basis[3], bernstein(3, 3, u));
}

TEST(AllBernsteinTest, MatchesIndividualCalls_Degree5) {
    double u = 0.25;
    auto basis = all_bernstein(5, u);

    for (size_t i = 0; i <= 5; ++i) {
        EXPECT_DOUBLE_EQ(basis[i], bernstein(i, 5, u));
    }
}

TEST(AllBernsteinTest, MatchesIndividualCalls_MultipleU) {
    // Test at multiple u values
    for (double u = 0.0; u <= 1.0; u += 0.1) {
        auto basis = all_bernstein(4, u);
        for (size_t i = 0; i <= 4; ++i) {
            EXPECT_NEAR(basis[i], bernstein(i, 4, u), 1e-15);
        }
    }
}

// ============================================================================
// Partition of Unity Tests
// ============================================================================

TEST(AllBernsteinTest, PartitionOfUnity_Degree0) {
    auto basis = all_bernstein(0, 0.5);
    double sum = std::accumulate(basis.begin(), basis.end(), 0.0);
    EXPECT_NEAR(sum, 1.0, 1e-15);
}

TEST(AllBernsteinTest, PartitionOfUnity_Degree1) {
    auto basis = all_bernstein(1, 0.3);
    double sum = std::accumulate(basis.begin(), basis.end(), 0.0);
    EXPECT_NEAR(sum, 1.0, 1e-15);
}

TEST(AllBernsteinTest, PartitionOfUnity_Degree3) {
    auto basis = all_bernstein(3, 0.5);
    double sum = std::accumulate(basis.begin(), basis.end(), 0.0);
    EXPECT_NEAR(sum, 1.0, 1e-15);
}

TEST(AllBernsteinTest, PartitionOfUnity_Degree10) {
    auto basis = all_bernstein(10, 0.6);
    double sum = std::accumulate(basis.begin(), basis.end(), 0.0);
    EXPECT_NEAR(sum, 1.0, 1e-14);
}

TEST(AllBernsteinTest, PartitionOfUnity_MultipleValues) {
    // Test partition of unity at many u values
    for (double u = 0.0; u <= 1.0; u += 0.05) {
        auto basis = all_bernstein(5, u);
        double sum = std::accumulate(basis.begin(), basis.end(), 0.0);
        EXPECT_NEAR(sum, 1.0, 1e-14) << "Failed at u = " << u;
    }
}

// ============================================================================
// Endpoint Tests
// ============================================================================

TEST(AllBernsteinTest, EndpointAtZero_Degree2) {
    auto basis = all_bernstein(2, 0.0);

    EXPECT_DOUBLE_EQ(basis[0], 1.0);
    EXPECT_DOUBLE_EQ(basis[1], 0.0);
    EXPECT_DOUBLE_EQ(basis[2], 0.0);
}

TEST(AllBernsteinTest, EndpointAtZero_Degree3) {
    auto basis = all_bernstein(3, 0.0);

    EXPECT_DOUBLE_EQ(basis[0], 1.0);
    EXPECT_DOUBLE_EQ(basis[1], 0.0);
    EXPECT_DOUBLE_EQ(basis[2], 0.0);
    EXPECT_DOUBLE_EQ(basis[3], 0.0);
}

TEST(AllBernsteinTest, EndpointAtOne_Degree2) {
    auto basis = all_bernstein(2, 1.0);

    EXPECT_DOUBLE_EQ(basis[0], 0.0);
    EXPECT_DOUBLE_EQ(basis[1], 0.0);
    EXPECT_DOUBLE_EQ(basis[2], 1.0);
}

TEST(AllBernsteinTest, EndpointAtOne_Degree3) {
    auto basis = all_bernstein(3, 1.0);

    EXPECT_DOUBLE_EQ(basis[0], 0.0);
    EXPECT_DOUBLE_EQ(basis[1], 0.0);
    EXPECT_DOUBLE_EQ(basis[2], 0.0);
    EXPECT_DOUBLE_EQ(basis[3], 1.0);
}

TEST(AllBernsteinTest, EndpointAtZero_Degree5) {
    auto basis = all_bernstein(5, 0.0);

    for (size_t i = 0; i <= 5; ++i) {
        if (i == 0) {
            EXPECT_DOUBLE_EQ(basis[i], 1.0);
        }
        else {
            EXPECT_DOUBLE_EQ(basis[i], 0.0);
        }
    }
}

TEST(AllBernsteinTest, EndpointAtOne_Degree5) {
    auto basis = all_bernstein(5, 1.0);

    for (size_t i = 0; i <= 5; ++i) {
        if (i == 5) {
            EXPECT_DOUBLE_EQ(basis[i], 1.0);
        }
        else {
            EXPECT_DOUBLE_EQ(basis[i], 0.0);
        }
    }
}

// ============================================================================
// Known Exact Values
// ============================================================================

TEST(AllBernsteinTest, Linear_AtHalf) {
    auto basis = all_bernstein(1, 0.5);

    EXPECT_DOUBLE_EQ(basis[0], 0.5);
    EXPECT_DOUBLE_EQ(basis[1], 0.5);
}

TEST(AllBernsteinTest, Linear_AtQuarter) {
    auto basis = all_bernstein(1, 0.25);

    EXPECT_DOUBLE_EQ(basis[0], 0.75);
    EXPECT_DOUBLE_EQ(basis[1], 0.25);
}

TEST(AllBernsteinTest, Quadratic_AtHalf) {
    auto basis = all_bernstein(2, 0.5);

    EXPECT_DOUBLE_EQ(basis[0], 0.25);
    EXPECT_DOUBLE_EQ(basis[1], 0.5);
    EXPECT_DOUBLE_EQ(basis[2], 0.25);
}

TEST(AllBernsteinTest, Quadratic_General) {
    auto basis = all_bernstein(2, 0.3);

    // B_{0,2}(0.3) = (0.7)^2 = 0.49
    EXPECT_NEAR(basis[0], 0.49, 1e-15);

    // B_{1,2}(0.3) = 2 * 0.3 * 0.7 = 0.42
    EXPECT_NEAR(basis[1], 0.42, 1e-15);

    // B_{2,2}(0.3) = (0.3)^2 = 0.09
    EXPECT_NEAR(basis[2], 0.09, 1e-15);
}

TEST(AllBernsteinTest, Cubic_AtHalf) {
    auto basis = all_bernstein(3, 0.5);

    EXPECT_DOUBLE_EQ(basis[0], 0.125);
    EXPECT_DOUBLE_EQ(basis[1], 0.375);
    EXPECT_DOUBLE_EQ(basis[2], 0.375);
    EXPECT_DOUBLE_EQ(basis[3], 0.125);
}

TEST(AllBernsteinTest, Cubic_AtQuarter) {
    auto basis = all_bernstein(3, 0.25);

    EXPECT_NEAR(basis[0], 0.421875, 1e-15);
    EXPECT_NEAR(basis[1], 0.421875, 1e-15);
    EXPECT_NEAR(basis[2], 0.140625, 1e-15);
    EXPECT_NEAR(basis[3], 0.015625, 1e-15);
}

// ============================================================================
// Symmetry Tests
// ============================================================================

TEST(AllBernsteinTest, Symmetry_Degree2) {
    double u = 0.3;
    auto basis_u = all_bernstein(2, u);
    auto basis_1u = all_bernstein(2, 1.0 - u);

    // B_{i,n}(u) = B_{n-i,n}(1-u)
    EXPECT_NEAR(basis_u[0], basis_1u[2], 1e-15);
    EXPECT_NEAR(basis_u[1], basis_1u[1], 1e-15);
    EXPECT_NEAR(basis_u[2], basis_1u[0], 1e-15);
}

TEST(AllBernsteinTest, Symmetry_Degree3) {
    double u = 0.6;
    auto basis_u = all_bernstein(3, u);
    auto basis_1u = all_bernstein(3, 1.0 - u);

    EXPECT_NEAR(basis_u[0], basis_1u[3], 1e-15);
    EXPECT_NEAR(basis_u[1], basis_1u[2], 1e-15);
    EXPECT_NEAR(basis_u[2], basis_1u[1], 1e-15);
    EXPECT_NEAR(basis_u[3], basis_1u[0], 1e-15);
}

TEST(AllBernsteinTest, Symmetry_Degree5) {
    double u = 0.4;
    auto basis_u = all_bernstein(5, u);
    auto basis_1u = all_bernstein(5, 1.0 - u);

    for (size_t i = 0; i <= 5; ++i) {
        EXPECT_NEAR(basis_u[i], basis_1u[5 - i], 1e-15);
    }
}

// ============================================================================
// Non-Negativity Tests
// ============================================================================

TEST(AllBernsteinTest, NonNegativity_Degree3) {
    for (double u = 0.0; u <= 1.0; u += 0.1) {
        auto basis = all_bernstein(3, u);
        for (size_t i = 0; i <= 3; ++i) {
            EXPECT_GE(basis[i], 0.0) << "Negative value at i=" << i << ", u=" << u;
        }
    }
}

TEST(AllBernsteinTest, NonNegativity_Degree10) {
    for (double u = 0.0; u <= 1.0; u += 0.1) {
        auto basis = all_bernstein(10, u);
        for (size_t i = 0; i <= 10; ++i) {
            EXPECT_GE(basis[i], 0.0) << "Negative value at i=" << i << ", u=" << u;
        }
    }
}

// ============================================================================
// Degree 0 Test
// ============================================================================

TEST(AllBernsteinTest, Degree0) {
    auto basis = all_bernstein(0, 0.5);

    EXPECT_EQ(basis.size(), 1);
    EXPECT_DOUBLE_EQ(basis[0], 1.0);
}

TEST(AllBernsteinTest, Degree0_MultipleU) {
    for (double u = 0.0; u <= 1.0; u += 0.1) {
        auto basis = all_bernstein(0, u);
        EXPECT_DOUBLE_EQ(basis[0], 1.0);
    }
}

// ============================================================================
// Float Type Tests
// ============================================================================

TEST(AllBernsteinTest, FloatType) {
    auto basis = all_bernstein(2, 0.5f);

    EXPECT_FLOAT_EQ(basis[0], 0.25f);
    EXPECT_FLOAT_EQ(basis[1], 0.5f);
    EXPECT_FLOAT_EQ(basis[2], 0.25f);

    float sum = std::accumulate(basis.begin(), basis.end(), 0.0f);
    EXPECT_NEAR(sum, 1.0f, 1e-6f);
}

TEST(AllBernsteinTest, LongDoubleType) {
    auto basis = all_bernstein(3, 0.5L);

    EXPECT_EQ(basis.size(), 4);

    long double sum = std::accumulate(basis.begin(), basis.end(), 0.0L);
    EXPECT_NEAR(static_cast<double>(sum), 1.0, 1e-15);
}

// ============================================================================
// Higher Degree Tests
// ============================================================================

TEST(AllBernsteinTest, HighDegree_Degree15) {
    auto basis = all_bernstein(15, 0.5);

    EXPECT_EQ(basis.size(), 16);

    // Check partition of unity
    double sum = std::accumulate(basis.begin(), basis.end(), 0.0);
    EXPECT_NEAR(sum, 1.0, 1e-13);

    // Check all non-negative
    for (const auto& b : basis) {
        EXPECT_GE(b, 0.0);
    }
}

TEST(AllBernsteinTest, HighDegree_Degree20) {
    auto basis = all_bernstein(20, 0.3);

    EXPECT_EQ(basis.size(), 21);

    double sum = std::accumulate(basis.begin(), basis.end(), 0.0);
    EXPECT_NEAR(sum, 1.0, 1e-13);
}

// ============================================================================
// Performance Comparison Test
// ============================================================================

TEST(AllBernsteinTest, ConsistencyWithIndividualCalls_HighDegree) {
    // Verify that all_bernstein matches individual calls for higher degree
    size_t n = 12;
    double u = 0.37;

    auto basis_all = all_bernstein(n, u);

    for (size_t i = 0; i <= n; ++i) {
        double basis_individual = bernstein(i, n, u);
        EXPECT_NEAR(basis_all[i], basis_individual, 1e-14)
            << "Mismatch at i=" << i;
    }
}

// ============================================================================
// Edge Case Tests
// ============================================================================

TEST(AllBernsteinTest, VerySmallU) {
    auto basis = all_bernstein(3, 1e-10);

    // Should be approximately [1, 0, 0, 0]
    EXPECT_NEAR(basis[0], 1.0, 1e-9);
    EXPECT_NEAR(basis[1], 0.0, 1e-9);
    EXPECT_NEAR(basis[2], 0.0, 1e-9);
    EXPECT_NEAR(basis[3], 0.0, 1e-9);
}

TEST(AllBernsteinTest, VeryLargeU) {
    auto basis = all_bernstein(3, 1.0 - 1e-10);

    // Should be approximately [0, 0, 0, 1]
    EXPECT_NEAR(basis[0], 0.0, 1e-9);
    EXPECT_NEAR(basis[1], 0.0, 1e-9);
    EXPECT_NEAR(basis[2], 0.0, 1e-9);
    EXPECT_NEAR(basis[3], 1.0, 1e-9);
}

// ============================================================================
// Practical Use Case Test
// ============================================================================

TEST(AllBernsteinTest, BezierCurveEvaluation) {
    // Simulate evaluating a Bézier curve using all_bernstein
    // Control points: [0, 1, 2, 3]
    std::vector<double> control_points = { 0.0, 1.0, 2.0, 3.0 };

    double u = 0.5;
    auto basis = all_bernstein(3, u);

    // Compute curve point: sum of (control_point[i] * basis[i])
    double point = 0.0;
    for (size_t i = 0; i <= 3; ++i) {
        point += control_points[i] * basis[i];
    }

    // At u=0.5 for linear control points, should be 1.5
    EXPECT_DOUBLE_EQ(point, 1.5);
}
