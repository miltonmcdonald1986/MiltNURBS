#ifndef MILTNURBS_BEZIER_H
#define MILTNURBS_BEZIER_H

#include "miltnurbs/vector.h"
#include "miltnurbs/bernstein.h"

#include <array>
#include <concepts>
#include <cstddef>

using miltnurbs::bernstein::all_bernstein;

/**
 * @file bezier.h
 * @brief Bézier curve representation and evaluation
 *
 * Implements parametric Bézier curves defined by control points.
 */

namespace miltnurbs::bezier
{

	/**
	 * @brief A Bézier curve of fixed degree in fixed-dimensional space
	 *
	 * Represents a parametric Bézier curve defined by N+1 control points.
	 * The curve is a weighted sum of control points using Bernstein basis functions:
	 *
	 * C(u) = Σ B_{i,N}(u) * P_i  for i = 0 to N
	 *
	 * where B_{i,N}(u) are the Bernstein basis polynomials and P_i are control points.
	 *
	 * @tparam T Floating point type for coordinates and parameters (float, double, long double)
	 * @tparam D Dimension of the space (2 for 2D, 3 for 3D, etc.)
	 * @tparam N Degree of the curve (number of control points = N+1)
	 *
	 * Properties:
	 * - The curve passes through the first and last control points
	 * - The curve lies within the convex hull of its control points
	 * - For u ∈ [0,1], the curve interpolates between endpoints
	 * - Evaluation outside [0,1] extrapolates the curve
	 *
	 * Common degrees:
	 * - N=1: Linear (2 control points)
	 * - N=2: Quadratic (3 control points)
	 * - N=3: Cubic (4 control points)
	 *
	 * Example:
	 * @code
	 * // Create a cubic Bézier curve in 3D
	 * BezierCurve<double, 3, 3> curve;
	 * curve.control_points[0] = Vector<double, 3>(0.0, 0.0, 0.0);
	 * curve.control_points[1] = Vector<double, 3>(1.0, 2.0, 0.0);
	 * curve.control_points[2] = Vector<double, 3>(2.0, 2.0, 0.0);
	 * curve.control_points[3] = Vector<double, 3>(3.0, 0.0, 0.0);
	 *
	 * // Evaluate at midpoint
	 * auto point = curve.evaluate(0.5);
	 * @endcode
	 */
	template<std::floating_point T, size_t D, size_t N>
	struct BezierCurve {
		/**
		 * @brief Control points defining the curve
		 *
		 * Array of N+1 control points in D-dimensional space.
		 * The curve shape is completely determined by these points.
		 */
		std::array<vector::Vector<T, D>, N + 1> control_points;

		/**
		 * @brief Evaluate the curve at parameter value u using Bernstein basis
		 *
		 * Computes the point on the Bézier curve at the given parameter value
		 * using Bernstein polynomial evaluation.
		 *
		 * The evaluation uses:
		 * C(u) = Σ B_{i,N}(u) * P_i
		 *
		 * where B_{i,N}(u) are Bernstein basis functions computed efficiently
		 * by all_bernstein().
		 *
		 * @param u Parameter value at which to evaluate the curve
		 *          - u ∈ [0,1] gives points along the curve segment
		 *          - u = 0 returns the first control point
		 *          - u = 1 returns the last control point
		 *          - u < 0 or u > 1 extrapolates the curve
		 *
		 * @return The point on the curve at parameter u
		 *
		 * @note Time complexity: O(N²)
		 * @note This is the default evaluation method
		 * @note For an alternative algorithm, see evaluate_decasteljau()
		 *
		 * Example:
		 * @code
		 * BezierCurve<double, 2, 2> curve;
		 * curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
		 * curve.control_points[1] = Vector<double, 2>(1.0, 2.0);
		 * curve.control_points[2] = Vector<double, 2>(2.0, 0.0);
		 *
		 * auto start = curve.evaluate(0.0);    // (0, 0)
		 * auto mid = curve.evaluate(0.5);      // (1, 1)
		 * auto end = curve.evaluate(1.0);      // (2, 0)
		 * @endcode
		 */
		vector::Vector<T, D> evaluate(T u) const
		{
			// Compute all Bernstein basis functions at u
			auto basis = all_bernstein(N, u);

			// Initialize result as zero vector
			vector::Vector<T, D> point;
			for (size_t d = 0; d < D; ++d) {
				point[d] = T{ 0 };
			}

			// Compute weighted sum: C(u) = Σ B_i(u) * P_i
			for (size_t i = 0; i <= N; ++i) {
				for (size_t d = 0; d < D; ++d) {
					point[d] += basis[i] * control_points[i][d];
				}
			}

			return point;
		}

		/**
		 * @brief Evaluate the curve at parameter value u using de Casteljau's algorithm
		 *
		 * Computes the point on the Bézier curve using de Casteljau's algorithm,
		 * which is a recursive subdivision method that is numerically stable and
		 * geometrically intuitive.
		 *
		 * The algorithm works by repeatedly performing linear interpolation between
		 * consecutive control points:
		 *
		 * P_i^0 = P_i  (original control points)
		 * P_i^r = (1-u) * P_i^(r-1) + u * P_(i+1)^(r-1)
		 *
		 * The final result is P_0^N, computed after N subdivision levels.
		 *
		 * @param u Parameter value at which to evaluate the curve
		 *          - u ∈ [0,1] gives points along the curve segment
		 *          - u = 0 returns the first control point
		 *          - u = 1 returns the last control point
		 *          - u < 0 or u > 1 extrapolates the curve
		 *
		 * @return The point on the curve at parameter u
		 *
		 * @note Time complexity: O(N²)
		 * @note Very numerically stable
		 * @note Useful for curve subdivision and geometric operations
		 * @note Produces identical results to evaluate() (within numerical precision)
		 *
		 * Example:
		 * @code
		 * BezierCurve<double, 2, 3> curve;
		 * // ... initialize control points ...
		 *
		 * auto point1 = curve.evaluate(0.5);              // Bernstein method
		 * auto point2 = curve.evaluate_decasteljau(0.5);  // de Casteljau method
		 * // point1 ≈ point2 (should be equal within numerical precision)
		 * @endcode
		 */
		vector::Vector<T, D> evaluate_decasteljau(T u) const
		{
			// Copy control points to working array
			std::array<vector::Vector<T, D>, N + 1> points = control_points;

			// Precompute (1-u) for efficiency
			T u1 = T{ 1 } - u;

			// Perform N levels of subdivision
			for (size_t level = 1; level <= N; ++level) {
				// At each level, compute N+1-level points by linear interpolation
				for (size_t i = 0; i <= N - level; ++i) {
					for (size_t d = 0; d < D; ++d) {
						points[i][d] = u1 * points[i][d] + u * points[i + 1][d];
					}
				}
			}

			// The final result is at points[0]
			return points[0];
		}
	};

} // namespace miltnurbs

#endif // MILTNURBS_BEZIER_H
