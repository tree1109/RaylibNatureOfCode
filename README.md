# RaylibNatureOfCode

A Raylib-based implementation of Nature of Code concepts, featuring physics simulations, particle systems, and interactive demonstrations.

## Project Structure

```
RaylibNatureOfCode/
├── src/                    # Main application source code
│   └── RaylibNatureOfCode.cpp
├── examples/              # Playground examples and demonstrations
│   ├── Playground1.cpp
│   ├── Playground2.cpp
│   └── ... (Playground1-12)
├── utility/               # Utility classes and helper functions
│   ├── BasicGameRunner.h/cpp    # Base game runner framework
│   ├── Math.h/cpp              # Mathematical utilities
│   ├── PathFinder.h/cpp        # Pathfinding algorithms
│   ├── SafeImage.h/cpp         # Safe image handling
│   ├── TextureManager.h/cpp    # Texture management
│   ├── Tool.h/cpp             # General tools and utilities
│   └── CommonType.h           # Common type definitions
├── game_object/          # Game objects and entities
│   ├── Particle.h/cpp         # Particle system implementation
│   ├── Attractor.h/cpp        # Attraction force field
│   ├── Repeller.h/cpp         # Repulsion force field
│   ├── RandomField.h/cpp      # Random force field
│   ├── Emitter.h/cpp          # Particle emitter
│   └── IForceField.h          # Force field interface
├── CMakeLists.txt        # Build configuration
├── CMakePresets.json     # CMake presets for different platforms
└── README.md            # Project documentation
```

## Building the Project

### Prerequisites
- CMake 3.10 or higher
- C++20 compatible compiler
- Raylib 5.5 (automatically downloaded via FetchContent)

### Linux/macOS
```bash
cmake --preset=linux-debug  # or macos-debug
cmake --build out/build/linux-debug -j 4
```

### Windows
```bash
cmake --preset=x64-debug   # or x64-release, x86-debug, x86-release
cmake --build out/build/x64-debug
```

## Running Examples

After building, you can run:
- `./RaylibNatureOfCode` - Main application
- `./Playground1` to `./Playground12` - Individual examples and demonstrations

Each playground demonstrates different Nature of Code concepts like:
- Physics simulations
- Particle systems
- Force fields (attraction/repulsion)
- Random movements
- Mathematical patterns in nature