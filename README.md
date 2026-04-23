# MiltLibs

A modern C++23 library collection featuring graphics and NURBS (Non-Uniform Rational B-Splines) support.

## Overview

MiltLibs is a comprehensive C++ library suite designed to provide tools for graphics rendering and advanced mathematical modeling with NURBS. The project demonstrates best practices in modern C++ with clean modular architecture.

## Features

- **NURBS Library**: Mathematical tools for Non-Uniform Rational B-Splines (NURBS) for accurate shape modeling
- **Graphics Library**: Graphics utilities built on OpenGL, ImGui, and GLFW
- **Modern C++23**: Leverages the latest C++ standard features
- **CMake Build System**: Easy to configure and integrate into other projects
- **Comprehensive Testing**: Google Test framework for unit testing
- **Interactive Demos**: Example applications showcasing library capabilities (e.g., Horner's algorithm demo)

## Project Structure

```
MiltLibs/
├── libs/
│   ├── graphics/        # Graphics utilities and rendering
│   └── nurbs/          # NURBS mathematical library
├── demos/              # Example applications
│   └── horner1/        # Horner's algorithm demonstration
├── tests/              # Unit tests
├── CMakeLists.txt      # Build configuration
└── README.md
```

## Requirements

- **C++23 compatible compiler** (GCC 13+, Clang 16+, or MSVC 2022+)
- **CMake 3.25+**
- **OpenGL** (system-dependent)

### Dependencies (automatically fetched via CMake)

- [EnTT v3.16.0](https://github.com/skypjack/entt) - Entity Component System
- [GLFW 3.4](https://github.com/glfw/glfw) - Window and input handling
- [ImGui v1.90.4](https://github.com/ocornut/imgui) - UI framework
- [gl3w](https://github.com/skaslev/gl3w) - OpenGL loader
- [stb](https://github.com/nothings/stb) - Image loading utilities
- [Google Test v1.14.0](https://github.com/google/googletest) - Testing framework

## Getting Started

### Building

1. **Clone the repository:**
   ```bash
   git clone https://github.com/miltonmcdonald1986/MiltLibs.git
   cd MiltLibs
   ```

2. **Create a build directory:**
   ```bash
   mkdir build
   cd build
   ```

3. **Configure and build:**
   ```bash
   cmake ..
   cmake --build . --config Release
   ```

### Build Options

Configure the build with these CMake options:

```bash
cmake -DMiltLibs_BUILD_TESTS=ON -DMiltLibs_BUILD_DEMOS=ON ..
```

- `MiltLibs_BUILD_TESTS` (default: ON) - Build unit tests
- `MiltLibs_BUILD_DEMOS` (default: ON) - Build demo applications

### Running Tests

After building:
```bash
ctest
```

## Usage

### Using the NURBS Library

```cpp
#include <milt/nurbs/nurbs.h>

// NURBS functionality available through the nurbs library
```

### Running Demos

After building, run the Horner's algorithm demo:
```bash
./demos/horner1/horner1
```

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is open source. See the LICENSE file for details.

## Author

**Milton McDonald** - [GitHub Profile](https://github.com/miltonmcdonald1986)

## Support

For issues, questions, or suggestions, please open an issue on the [GitHub repository](https://github.com/miltonmcdonald1986/MiltLibs/issues).
