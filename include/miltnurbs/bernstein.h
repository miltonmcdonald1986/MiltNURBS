#ifndef MILTNURBS_BERNSTEIN_H
#define MILTNURBS_BERNSTEIN_H

#include <concepts>
#include <vector>
#include <cstddef>

/**
 * @file bernstein.h
 * @brief Bernstein polynomial basis functions for Bézier curves
 * 
 * Implements computation of Bernstein basis polynomials using a stable
 * dynamic programming algorithm based on de Casteljau's method.
 */

namespace miltnurbs::bernstein
{

	/**
	 * @brief Compute a single Bernstein basis polynomial
	 * 
	 * Evaluates the Bernstein basis function B_{i,n}(u) using a numerically
	 * stable recurrence relation instead of directly computing binomial
	 * coefficients and powers.
	 * 
	 * The Bernstein polynomial is mathematically defined as:
	 * B_{i,n}(u) = C(n,i) · u^i · (1-u)^(n-i)
	 * 
	 * where C(n,i) is the binomial coefficient "n choose i".
	 * 
	 * This implementation uses the recursive property:
	 * B_{j,k}(u) = (1-u) · B_{j,k-1}(u) + u · B_{j-1,k-1}(u)
	 * 
	 * @tparam T Floating point type (float, double, or long double)
	 * 
	 * @param i Index of the basis function (0 ≤ i ≤ n)
	 * @param n Degree of the polynomial
	 * @param u Parameter value at which to evaluate (typically in [0,1])
	 * 
	 * @return The value of B_{i,n}(u)
	 * 
	 * @note This function is constexpr and can be evaluated at compile time
	 * @note Time complexity: O(n²)
	 * @note Space complexity: O(n)
	 * @note More numerically stable than computing powers and factorials directly
	 * 
	 * Properties of Bernstein polynomials:
	 * - Non-negative: B_{i,n}(u) ≥ 0 for u ∈ [0,1]
	 * - Partition of unity: Σ B_{i,n}(u) = 1 for i=0 to n
	 * - Symmetry: B_{i,n}(u) = B_{n-i,n}(1-u)
	 * - Endpoint values: B_{i,n}(0) = δ_{i,0}, B_{i,n}(1) = δ_{i,n}
	 * 
	 * Example:
	 * @code
	 * // Evaluate B_{2,3}(0.5) - the basis function for the 3rd control point
	 * // of a cubic Bézier curve at parameter 0.5
	 * double b = bernstein(2, 3, 0.5);  // Returns 0.375
	 * 
	 * // All basis functions at u=0.5 sum to 1
	 * double sum = 0.0;
	 * for (size_t i = 0; i <= 3; ++i) {
	 *     sum += bernstein(i, 3, 0.5);
	 * }
	 * // sum == 1.0
	 * @endcode
	 */
	template<std::floating_point T>
	constexpr T bernstein(size_t i, size_t n, T u)
	{
		// Initialize temporary array to store intermediate values
		std::vector<T> temp(n + 1, T{0});
		
		// Seed the initial value at position n-i
		temp[n - i] = T{1};
		
		// Precompute (1-u) for efficiency
		T u1 = T{1} - u;
		
		// Build up the Bernstein polynomial using dynamic programming
		// Each iteration increases the degree by 1
		for (size_t k = 1; k <= n; ++k) {
			// Process from right to left to avoid overwriting values we need
			for (size_t j = n; j >= k; --j) {
				temp[j] = u1 * temp[j] + u * temp[j - 1];
			}
		}
		
		// The final result is at temp[n]
		return temp[n];
	}

	/**
	 * @brief Compute all Bernstein basis polynomials for a given degree
	 *
	 * Efficiently computes all n+1 basis functions B_{0,n}(u) through B_{n,n}(u)
	 * at parameter value u. This is much more efficient than calling bernstein()
	 * n+1 times separately.
	 *
	 * The algorithm builds up from degree 0 to degree n using dynamic programming,
	 * computing all basis functions simultaneously. This approach reuses intermediate
	 * computations and requires only O(n²) operations total, versus O(n³) for calling
	 * bernstein() n+1 times.
	 *
	 * @tparam T Floating point type (float, double, or long double)
	 *
	 * @param n Degree of the polynomials
	 * @param u Parameter value at which to evaluate (typically in [0,1])
	 *
	 * @return Vector of size n+1 containing [B_{0,n}(u), B_{1,n}(u), ..., B_{n,n}(u)]
	 *
	 * @note Time complexity: O(n²)
	 * @note Space complexity: O(n)
	 * @note The returned values are guaranteed to sum to 1.0 (within numerical precision)
	 *
	 * Example:
	 * @code
	 * // Compute all cubic Bernstein polynomials at u=0.5
	 * auto basis = all_bernstein(3, 0.5);
	 * // basis[0] = 0.125  (B_{0,3}(0.5))
	 * // basis[1] = 0.375  (B_{1,3}(0.5))
	 * // basis[2] = 0.375  (B_{2,3}(0.5))
	 * // basis[3] = 0.125  (B_{3,3}(0.5))
	 *
	 * // Use for Bézier curve evaluation
	 * Vector<double, 3> control_points[4] = {...};
	 * auto b = all_bernstein(3, 0.5);
	 * Vector<double, 3> point(0.0, 0.0, 0.0);
	 * for (size_t i = 0; i <= 3; ++i) {
	 *     point = point + control_points[i] * b[i];
	 * }
	 * @endcode
	 */
	template<std::floating_point T>
	std::vector<T> all_bernstein(size_t n, T u)
	{
		// Initialize result vector with n+1 elements
		std::vector<T> basis(n + 1);

		// Start with degree 0: B_{0,0}(u) = 1
		basis[0] = T{ 1 };

		// Precompute (1-u) for efficiency
		T u1 = T{ 1 } - u;

		// Build up from degree 1 to degree n
		for (size_t j = 1; j <= n; ++j) {
			// Initialize the new highest-degree term
			basis[j] = T{ 0 };

			// Update existing terms from right to left
			// This computes the degree-j basis functions from degree-(j-1)
			for (size_t k = j; k >= 1; --k) {
				basis[k] = u1 * basis[k] + u * basis[k - 1];
			}

			// Update the lowest-degree term
			basis[0] = u1 * basis[0];
		}

		return basis;
	}

} // namespace miltnurbs

#endif // MILTNURBS_BERNSTEIN_H


