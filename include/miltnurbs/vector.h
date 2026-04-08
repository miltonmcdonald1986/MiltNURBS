#ifndef MILTNURBS_VECTOR_H
#define MILTNURBS_VECTOR_H

#include <array>
#include <cstddef>
#include <concepts>
#include <algorithm>

/**
 * @file vector.h
 * @brief Mathematical vector class for NURBS operations
 *
 * This file contains a templated, fixed-size vector class for mathematical
 * operations. The class supports compile-time evaluation and enforces type
 * safety through C++20 concepts.
 */

namespace miltnurbs::vector
{

	/**
	 * @brief A fixed-size mathematical vector with compile-time dimensions
	 *
	 * This class represents a mathematical vector with a fixed number of components.
	 * All operations are constexpr-enabled for compile-time evaluation when possible.
	 *
	 * @tparam T The arithmetic type of the vector components (int, float, double, etc.)
	 * @tparam N The number of dimensions (must be known at compile time)
	 *
	 * @note Template parameter T is constrained to arithmetic types via std::is_arithmetic_v
	 *
	 * Example usage:
	 * @code
	 * Vector<double, 3> v1(1.0, 2.0, 3.0);
	 * Vector<double, 3> v2(4.0, 5.0, 6.0);
	 * auto v3 = v1 + v2;           // Vector addition
	 * auto v4 = v1 * 2.0;          // Scalar multiplication
	 * auto v5 = 3.0 * v2;          // Commutative scalar multiplication
	 * constexpr auto cv = v1 + v2; // Compile-time evaluation
	 * @endcode
	 */
	template<typename T, size_t N>
		requires std::is_arithmetic_v<T>
	class Vector {
	public:
		/**
		 * @brief Default constructor
		 *
		 * Creates a zero-initialized vector with all components set to zero.
		 *
		 * @note This constructor is constexpr and can be used at compile time
		 */
		constexpr Vector() = default;

		/**
		 * @brief Variadic constructor for initializing vector components
		 *
		 * Constructs a vector from a list of values. The number of arguments
		 * must exactly match the vector dimension N (enforced at compile time).
		 *
		 * @tparam Args Parameter pack of argument types (must be convertible to T)
		 * @param args Component values to initialize the vector
		 *
		 * @note Compile-time checks ensure:
		 *       - Exactly N arguments are provided
		 *       - All arguments are convertible to type T
		 *
		 * Example:
		 * @code
		 * Vector<double, 3> v(1.0, 2.0, 3.0); // OK
		 * Vector<double, 3> v(1.0, 2.0);      // Compile error: wrong number of args
		 * @endcode
		 */
		template<typename... Args>
			requires (sizeof...(Args) == N) && (std::convertible_to<Args, T> && ...)
		constexpr Vector(Args... args) : components{ static_cast<T>(args)... } {}

		/**
		 * @brief Non-const subscript operator for element access
		 *
		 * Provides direct read-write access to vector components by index.
		 *
		 * @param index Zero-based index of the component (0 to N-1)
		 * @return Reference to the component at the specified index
		 *
		 * @warning No bounds checking is performed. Accessing out-of-bounds
		 *          indices results in undefined behavior.
		 */
		constexpr T& operator[](size_t index) {
			return components[index];
		}

		/**
		 * @brief Const subscript operator for element access
		 *
		 * Provides direct read-only access to vector components by index.
		 *
		 * @param index Zero-based index of the component (0 to N-1)
		 * @return Const reference to the component at the specified index
		 *
		 * @warning No bounds checking is performed. Accessing out-of-bounds
		 *          indices results in undefined behavior.
		 */
		constexpr const T& operator[](size_t index) const {
			return components[index];
		}

		/**
		 * @brief Vector addition operator
		 *
		 * Performs component-wise addition of two vectors.
		 *
		 * @param other The vector to add to this vector
		 * @return A new vector containing the sum (this + other)
		 *
		 * @note Vectors must have the same dimension N (enforced by type system)
		 * @note This operation is constexpr and can be evaluated at compile time
		 *
		 * Mathematical operation: result[i] = this[i] + other[i] for all i
		 */
		constexpr Vector operator+(const Vector& other) const {
			Vector result;
			for (size_t i = 0; i < N; ++i) {
				result[i] = components[i] + other[i];
			}
			return result;
		}

		/**
		 * @brief Scalar multiplication operator (vector * scalar)
		 *
		 * Multiplies each component of the vector by a scalar value.
		 *
		 * @param scalar The scalar value to multiply by
		 * @return A new vector with each component multiplied by the scalar
		 *
		 * @note This operation is constexpr and can be evaluated at compile time
		 *
		 * Mathematical operation: result[i] = this[i] * scalar for all i
		 */
		constexpr Vector operator*(T scalar) const {
			Vector result;
			for (size_t i = 0; i < N; ++i) {
				result[i] = components[i] * scalar;
			}
			return result;
		}

		/**
		 * @brief Scalar multiplication operator (scalar * vector)
		 *
		 * Enables commutative scalar multiplication, allowing scalar * vector syntax.
		 * This reflects the mathematical property that k·v = v·k.
		 *
		 * @param scalar The scalar value to multiply by
		 * @param vec The vector to multiply
		 * @return A new vector with each component multiplied by the scalar
		 *
		 * @note Declared as friend to allow scalar on the left-hand side
		 * @note This operation is constexpr and can be evaluated at compile time
		 *
		 * Example:
		 * @code
		 * Vector<double, 3> v(1.0, 2.0, 3.0);
		 * auto v1 = v * 2.0;    // Uses member operator*
		 * auto v2 = 2.0 * v;    // Uses this friend operator*
		 * // v1 == v2 (both are {2.0, 4.0, 6.0})
		 * @endcode
		 */
		friend constexpr Vector operator*(T scalar, const Vector& vec) {
			return vec * scalar;
		}

	private:
		std::array<T, N> components{}; ///< Internal storage for vector components
	};

} // namespace miltnurbs::vector

#endif // MILTNURBS_VECTOR_H
