#include <miltnurbs/horner.h>
#include <miltnurbs/vector.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

using Vec2 = miltnurbs::vector::Vector<double, 2>;

struct AppState {
    std::vector<std::vector<Vec2>> curves;
    int currentCurve = 0;
    float t = 0.0f;
    bool animate = true;
    Vec2 cachedPoint{};
    double cachedT = -1.0;

    // Bounding box for current curve
    double minX, maxX, minY, maxY;

    // Vector scale factor (must match JavaScript)
    const double vectorScale = 0.3;
};

AppState state;

// Compute bounding box for current curve
void computeBoundingBox() {
    state.minX = std::numeric_limits<double>::max();
    state.maxX = std::numeric_limits<double>::lowest();
    state.minY = std::numeric_limits<double>::max();
    state.maxY = std::numeric_limits<double>::lowest();

    auto& coeffs = state.curves[state.currentCurve];

    // Sample the curve to find bounds
    const int samples = 100;
    for (int i = 0; i <= samples; i++) {
        double t = static_cast<double>(i) / samples;
        Vec2 point = miltnurbs::horner::horner1(coeffs, t);

        state.minX = std::min(state.minX, point[0]);
        state.maxX = std::max(state.maxX, point[0]);
        state.minY = std::min(state.minY, point[1]);
        state.maxY = std::max(state.maxY, point[1]);
    }

    // Include c₀ (the base point)
    state.minX = std::min(state.minX, coeffs[0][0]);
    state.maxX = std::max(state.maxX, coeffs[0][0]);
    state.minY = std::min(state.minY, coeffs[0][1]);
    state.maxY = std::max(state.maxY, coeffs[0][1]);

    // Include vector endpoints (c₀ + cᵢ * vectorScale for i > 0)
    for (size_t i = 1; i < coeffs.size(); i++) {
        double endX = coeffs[0][0] + coeffs[i][0] * state.vectorScale;
        double endY = coeffs[0][1] + coeffs[i][1] * state.vectorScale;

        state.minX = std::min(state.minX, endX);
        state.maxX = std::max(state.maxX, endX);
        state.minY = std::min(state.minY, endY);
        state.maxY = std::max(state.maxY, endY);
    }

    // Add padding (10% on each side)
    double width = state.maxX - state.minX;
    double height = state.maxY - state.minY;
    double padding = 0.1;

    state.minX -= width * padding;
    state.maxX += width * padding;
    state.minY -= height * padding;
    state.maxY += height * padding;
}

extern "C" {
    EMSCRIPTEN_KEEPALIVE
        void init() {
        state.curves = {
            // Cubic spiral
            {
                Vec2(100.0, 300.0),
                Vec2(400.0, -200.0),
                Vec2(-300.0, 200.0),
                Vec2(300.0, 100.0)
            },
            // Quadratic arc
            {
                Vec2(100.0, 400.0),
                Vec2(300.0, -300.0),
                Vec2(400.0, 250.0)
            },
            // Quartic wave
            {
                Vec2(50.0, 300.0),
                Vec2(200.0, 100.0),
                Vec2(-100.0, -200.0),
                Vec2(150.0, 200.0),
                Vec2(100.0, -100.0)
            }
        };
        computeBoundingBox();
    }

    EMSCRIPTEN_KEEPALIVE
        void evaluateCurve(double t) {
        auto& coeffs = state.curves[state.currentCurve];
        state.cachedPoint = miltnurbs::horner::horner1(coeffs, t);
        state.cachedT = t;
    }

    EMSCRIPTEN_KEEPALIVE
        double getLastX() {
        return state.cachedPoint[0];
    }

    EMSCRIPTEN_KEEPALIVE
        double getLastY() {
        return state.cachedPoint[1];
    }

    EMSCRIPTEN_KEEPALIVE
        int getCurveDegree() {
        return state.curves[state.currentCurve].size() - 1;
    }

    EMSCRIPTEN_KEEPALIVE
        int getNumCoeffs() {
        return state.curves[state.currentCurve].size();
    }

    EMSCRIPTEN_KEEPALIVE
        double getCoeffX(int index) {
        return state.curves[state.currentCurve][index][0];
    }

    EMSCRIPTEN_KEEPALIVE
        double getCoeffY(int index) {
        return state.curves[state.currentCurve][index][1];
    }

    EMSCRIPTEN_KEEPALIVE
        double getBoundsMinX() {
        return state.minX;
    }

    EMSCRIPTEN_KEEPALIVE
        double getBoundsMaxX() {
        return state.maxX;
    }

    EMSCRIPTEN_KEEPALIVE
        double getBoundsMinY() {
        return state.minY;
    }

    EMSCRIPTEN_KEEPALIVE
        double getBoundsMaxY() {
        return state.maxY;
    }

    EMSCRIPTEN_KEEPALIVE
        void nextCurve() {
        state.currentCurve = (state.currentCurve + 1) % state.curves.size();
        state.t = 0.0f;
        state.cachedT = -1.0;
        computeBoundingBox();
    }

    EMSCRIPTEN_KEEPALIVE
        void prevCurve() {
        state.currentCurve = (state.currentCurve - 1 + state.curves.size()) % state.curves.size();
        state.t = 0.0f;
        state.cachedT = -1.0;
        computeBoundingBox();
    }

    EMSCRIPTEN_KEEPALIVE
        void toggleAnimation() {
        state.animate = !state.animate;
    }

    EMSCRIPTEN_KEEPALIVE
        float updateAnimation() {
        if (state.animate) {
            state.t += 0.01f;
            if (state.t > 1.0f) state.t = 0.0f;
        }
        return state.t;
    }
}


