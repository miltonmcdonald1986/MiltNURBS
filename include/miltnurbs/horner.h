#ifndef MILTNURBS_HORNER_H
#define MILTNURBS_HORNER_H

#include <span>
#include <concepts>
#include <cstddef>
#include <ranges>

/**
 * @file horner.h
 * @brief Horner's method for efficient polynomial evaluation
 *
 * Implements Horner's rule for evaluating polynomials and polynomial curves.
 * This is a fundamental algorithm used in NURBS curve evaluation.
 */

namespace miltnurbs::horner
{

	/**
	 * @brief Concept for types that support scalar multiplication and addition
	 *
	 * This concept ensures a type T can be used in Horner's algorithm by
	 * requiring support for multiplication by a scalar and addition.
	 */
	template<typename T, typename Scalar>
	concept HornerEvaluable = requires(T a, T b, Scalar s) {
		{ a* s } -> std::convertible_to<T>;
		{ a + b } -> std::convertible_to<T>;
	};

	/**
	 * @brief Evaluate a polynomial using Horner's method
	 *
	 * Efficiently evaluates a polynomial of degree n at parameter u0 using
	 * Horner's rule. This method requires only n multiplications and n additions,
	 * which is optimal for polynomial evaluation.
	 *
	 * The polynomial is defined as:
	 * P(u) = a[0] + a[1]*u + a[2]*u² + ... + a[n]*uⁿ
	 *
	 * Horner's rule rewrites this as:
	 * P(u) = a[0] + u*(a[1] + u*(a[2] + ... + u*a[n]))
	 *
	 * @tparam Range A contiguous range type (std::array, std::vector, etc.)
	 * @tparam Scalar The parameter type (typically double or float)
	 *
	 * @param coefficients Contiguous range of polynomial coefficients [a[0], a[1], ..., a[n]]
	 *                     where a[0] is the constant term and a[n] is the leading coefficient
	 * @param u0 The parameter value at which to evaluate the polynomial
	 *
	 * @return The evaluated polynomial value at u0
	 *
	 * @note This function is constexpr and can be evaluated at compile time
	 * @note The coefficients container must contain at least one element
	 * @note Works with both scalar coefficients and vector-valued coefficients (for curves)
	 *
	 * Example with scalars:
	 * @code
	 * // Evaluate p(u) = 1 + 2u + 3u² at u = 2.0
	 * std::array<double, 3> coeffs{1.0, 2.0, 3.0};
	 * double result = horner1(coeffs, 2.0);  // result = 1 + 2*2 + 3*4 = 17
	 * @endcode
	 *
	 * Example with vectors:
	 * @code
	 * // Evaluate a Bézier curve
	 * using Vec3 = miltnurbs::vector::Vector<double, 3>;
	 * std::array<Vec3, 4> controlPoints{
	 *     Vec3(0.0, 0.0, 0.0),
	 *     Vec3(1.0, 2.0, 0.0),
	 *     Vec3(2.0, 2.0, 0.0),
	 *     Vec3(3.0, 0.0, 0.0)
	 * };
	 * Vec3 point = horner1(controlPoints, 0.5);
	 * @endcode
	 */
	template<std::ranges::contiguous_range Range, typename Scalar>
		requires HornerEvaluable<std::ranges::range_value_t<Range>, Scalar>
	constexpr auto horner1(const Range& coefficients, Scalar u0)
	{
		using T = std::ranges::range_value_t<Range>;

		std::span<const T> coeffs_span(coefficients);

		// Start with the leading coefficient (highest degree term)
		const size_t n = coeffs_span.size() - 1;
		T result = coeffs_span[n];

		// Horner's method: evaluate from highest to lowest degree
		for (size_t i = 0; i < n; ++i) {
			result = result * u0 + coeffs_span[n - 1 - i];
		}

		return result;
	}

} // namespace miltnurbs

#endif // MILTNURBS_HORNER_H
