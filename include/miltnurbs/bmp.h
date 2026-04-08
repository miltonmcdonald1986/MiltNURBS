#ifndef MILTNURBS_BEZIER_BMP_H
#define MILTNURBS_BEZIER_BMP_H

#include "miltnurbs/bezier.h"
#include "miltnurbs/vector.h"
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <algorithm>
#include <cmath>

/**
 * @file bezier_bmp.h
 * @brief Utility functions for rendering Bézier curves to BMP images
 *
 * Provides visualization tools for 2D Bézier curves.
 */

namespace miltnurbs::bmp
{

	/**
	 * @brief RGB color structure
	 */
	struct Color {
		uint8_t r, g, b;

		static constexpr Color White() { return { 255, 255, 255 }; }
		static constexpr Color Black() { return { 0, 0, 0 }; }
		static constexpr Color Red() { return { 255, 0, 0 }; }
		static constexpr Color Green() { return { 0, 255, 0 }; }
		static constexpr Color Blue() { return { 0, 0, 255 }; }
		static constexpr Color Gray() { return { 128, 128, 128 }; }
	};

	/**
	 * @brief Simple BMP image class for rendering
	 */
	class BMPImage {
	private:
		uint32_t width_;
		uint32_t height_;
		std::vector<Color> pixels_;

		void write_bmp_header(std::ofstream& file) const {
			// BMP file header (14 bytes)
			uint32_t file_size = 54 + width_ * height_ * 3;
			uint32_t pixel_data_offset = 54;

			file.put('B').put('M');  // Signature
			file.write(reinterpret_cast<const char*>(&file_size), 4);
			file.write("\0\0\0\0", 4);  // Reserved
			file.write(reinterpret_cast<const char*>(&pixel_data_offset), 4);

			// DIB header (40 bytes)
			uint32_t dib_header_size = 40;
			uint32_t w = static_cast<uint32_t>(width_);
			uint32_t h = static_cast<uint32_t>(height_);
			uint16_t planes = 1;
			uint16_t bpp = 24;
			uint32_t compression = 0;
			uint32_t image_size = static_cast<uint32_t>(width_ * height_ * 3);

			file.write(reinterpret_cast<const char*>(&dib_header_size), 4);
			file.write(reinterpret_cast<const char*>(&w), 4);
			file.write(reinterpret_cast<const char*>(&h), 4);
			file.write(reinterpret_cast<const char*>(&planes), 2);
			file.write(reinterpret_cast<const char*>(&bpp), 2);
			file.write(reinterpret_cast<const char*>(&compression), 4);
			file.write(reinterpret_cast<const char*>(&image_size), 4);
			file.write("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16);  // Rest of header
		}

	public:
		BMPImage(uint32_t width, uint32_t height, Color background = Color::White())
			: width_(width), height_(height), pixels_(width* height, background) {}

		void set_pixel(int x, int y, Color color) {
			if (x >= 0 && x < static_cast<int>(width_) &&
				y >= 0 && y < static_cast<int>(height_)) {
				pixels_[y * width_ + x] = color;
			}
		}

		Color get_pixel(int x, int y) const {
			if (x >= 0 && x < static_cast<int>(width_) &&
				y >= 0 && y < static_cast<int>(height_)) {
				return pixels_[y * width_ + x];
			}
			return Color::Black();
		}

		void draw_line(int x0, int y0, int x1, int y1, Color color) {
			// Bresenham's line algorithm
			int dx = std::abs(x1 - x0);
			int dy = std::abs(y1 - y0);
			int sx = x0 < x1 ? 1 : -1;
			int sy = y0 < y1 ? 1 : -1;
			int err = dx - dy;

			while (true) {
				set_pixel(x0, y0, color);

				if (x0 == x1 && y0 == y1) break;

				int e2 = 2 * err;
				if (e2 > -dy) {
					err -= dy;
					x0 += sx;
				}
				if (e2 < dx) {
					err += dx;
					y0 += sy;
				}
			}
		}

		void draw_circle(int cx, int cy, int radius, Color color, bool fill = true) {
			for (int y = -radius; y <= radius; ++y) {
				for (int x = -radius; x <= radius; ++x) {
					if (fill) {
						if (x * x + y * y <= radius * radius) {
							set_pixel(cx + x, cy + y, color);
						}
					}
					else {
						if (std::abs(x * x + y * y - radius * radius) < radius) {
							set_pixel(cx + x, cy + y, color);
						}
					}
				}
			}
		}

		bool save(const std::string& filename) const {
			std::ofstream file(filename, std::ios::binary);
			if (!file) return false;

			write_bmp_header(file);

			// Write pixel data (BMP is stored bottom-up, BGR format)
			for (int y = static_cast<int>(height_) - 1; y >= 0; --y) {
				for (size_t x = 0; x < width_; ++x) {
					const Color& pixel = pixels_[y * width_ + x];
					file.put(pixel.b).put(pixel.g).put(pixel.r);
				}
			}

			return true;
		}
	};

	/**
	 * @brief Configuration for rendering Bézier curves to BMP
	 */
	struct RenderConfig {
		uint32_t image_width = 800;
		uint32_t image_height = 600;
		size_t samples = 100;  // Number of curve samples
		double margin = 50.0;  // Margin around curve in pixels

		Color background_color = Color::White();
		Color curve_color = Color::Blue();
		Color control_point_color = Color::Red();
		Color control_polygon_color = Color::Gray();

		bool draw_control_points = true;
		bool draw_control_polygon = true;
		int control_point_radius = 5;
		int curve_thickness = 2;  // Line thickness (simple implementation)
	};

	/**
	 * @brief Render a 2D Bézier curve to a BMP file
	 *
	 * Creates a BMP image showing the curve, optionally with control points
	 * and control polygon. Automatically scales and centers the curve in the image.
	 *
	 * @tparam T Floating point type
	 * @tparam N Degree of the curve
	 *
	 * @param curve The 2D Bézier curve to render (must have D=2)
	 * @param filename Output BMP filename
	 * @param config Rendering configuration
	 *
	 * @return true if file was written successfully, false otherwise
	 *
	 * Example:
	 * @code
	 * BezierCurve<double, 2, 3> curve;
	 * curve.control_points[0] = Vector<double, 2>(0.0, 0.0);
	 * curve.control_points[1] = Vector<double, 2>(1.0, 2.0);
	 * curve.control_points[2] = Vector<double, 2>(2.0, 2.0);
	 * curve.control_points[3] = Vector<double, 2>(3.0, 0.0);
	 *
	 * RenderConfig config;
	 * config.samples = 200;
	 * render_to_bmp(curve, "my_curve.bmp", config);
	 * @endcode
	 */
	template<std::floating_point T, size_t N>
	bool render_to_bmp(
		const bezier::BezierCurve<T, 2, N>& curve,
		const std::string& filename,
		const RenderConfig& config = RenderConfig{})
	{
		BMPImage image(config.image_width, config.image_height, config.background_color);

		// Find bounding box of control points
		T min_x = curve.control_points[0][0];
		T max_x = curve.control_points[0][0];
		T min_y = curve.control_points[0][1];
		T max_y = curve.control_points[0][1];

		for (size_t i = 1; i <= N; ++i) {
			min_x = std::min(min_x, curve.control_points[i][0]);
			max_x = std::max(max_x, curve.control_points[i][0]);
			min_y = std::min(min_y, curve.control_points[i][1]);
			max_y = std::max(max_y, curve.control_points[i][1]);
		}

		// Add some padding
		T range_x = max_x - min_x;
		T range_y = max_y - min_y;
		if (range_x < 1e-6) range_x = 1.0;
		if (range_y < 1e-6) range_y = 1.0;

		// Calculate scaling to fit in image with margin
		double drawable_width = config.image_width - 2 * config.margin;
		double drawable_height = config.image_height - 2 * config.margin;
		double scale = std::min(drawable_width / range_x, drawable_height / range_y);

		// Lambda to convert curve coordinates to pixel coordinates
		auto to_pixel = [&](T x, T y) -> std::pair<int, int> {
			int px = static_cast<int>((x - min_x) * scale + config.margin);
			int py = static_cast<int>(config.image_height - ((y - min_y) * scale + config.margin));
			return { px, py };
			};

		// Draw control polygon
		if (config.draw_control_polygon) {
			for (size_t i = 0; i < N; ++i) {
				auto [x0, y0] = to_pixel(curve.control_points[i][0], curve.control_points[i][1]);
				auto [x1, y1] = to_pixel(curve.control_points[i + 1][0], curve.control_points[i + 1][1]);
				image.draw_line(x0, y0, x1, y1, config.control_polygon_color);
			}
		}

		// Draw the curve
		for (size_t i = 0; i < config.samples; ++i) {
			T u0 = static_cast<T>(i) / static_cast<T>(config.samples);
			T u1 = static_cast<T>(i + 1) / static_cast<T>(config.samples);

			auto p0 = curve.evaluate(u0);
			auto p1 = curve.evaluate(u1);

			auto [x0, y0] = to_pixel(p0[0], p0[1]);
			auto [x1, y1] = to_pixel(p1[0], p1[1]);

			// Draw with thickness
			for (int t = -config.curve_thickness / 2; t <= config.curve_thickness / 2; ++t) {
				image.draw_line(x0 + t, y0, x1 + t, y1, config.curve_color);
				image.draw_line(x0, y0 + t, x1, y1 + t, config.curve_color);
			}
		}

		// Draw control points
		if (config.draw_control_points) {
			for (size_t i = 0; i <= N; ++i) {
				auto [px, py] = to_pixel(curve.control_points[i][0], curve.control_points[i][1]);
				image.draw_circle(px, py, config.control_point_radius, config.control_point_color);
			}
		}

		return image.save(filename);
	}

} // namespace miltnurbs::bmp

#endif // MILTNURBS_BEZIER_BMP_H
