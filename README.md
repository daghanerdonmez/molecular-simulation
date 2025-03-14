# Molecular Simulation

A C++ application for molecular simulations.

## Prerequisites

- CMake (version 3.12 or higher)
- C++ compiler with C++17 support
- OpenGL
- Git (for automatic dependency downloading)

## Building the Project

The project is set up to automatically download and build all required dependencies (GLM, GLFW, and yaml-cpp) during the CMake configuration process. You don't need to install these libraries separately.

1. Clone the repository:
```bash
git clone https://github.com/yourusername/Molecular-Simulation.git
cd Molecular-Simulation
```

2. Create a build directory and navigate into it:
```bash
mkdir build
cd build
```

3. Generate the build files with CMake:
```bash
cmake ..
```

4. Build the project:
```bash
cmake --build .
```

5. Run the application:
```bash
./Molecular_Simulation
```

## Configuration

The simulation parameters can be configured in the Config directory:
- `config.h`: Contains main simulation parameters
- `network_config.yaml`: Contains network configuration for simulation networks

## Project Structure

- `Molecular Simulation/`: Main source code directory
  - `Config/`: Configuration files
  - `Boundaries/`: Boundary condition implementations
  - `GLHeaders/`: OpenGL header files
  - `Math/`: Mathematical utilities
  - `Receivers/`: Receiver implementations
  - `Shaders/`: GLSL shader files
  - `Output/`: Directory for simulation output

## Features

- Single simulation mode
- Simulation network mode
- Various boundary conditions (box, cylinder, etc.)
- Different receiver types
- 3D visualization with OpenGL

## License

[MIT License](LICENSE)
