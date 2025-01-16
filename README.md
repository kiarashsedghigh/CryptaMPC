# Quantum Safe Threshold (QST)

## Prerequisites

Before building, ensure that you have the following installed on your system:

- [CMake](https://cmake.org/install/) (version 3.26 or later)
- A C++ compiler (C++ 23)
- Doxygen compiler (version 1.9.8 or later)


## Available Build Targets

This project supports the following targets:
- **tests**: Builds the test suite.
The following test executables are available in this project:

| **Test Target** | **Description** |
|-----------------|-----------------|
| `test_netio`    | Network I/O tests |
| `test_npot`     | Non-Transfer OT tests |
| `test_coot`     | COOT (Compressed Oblivious Transfer) tests |
| `test_cot`      | COT (Correlation Oblivious Transfer) tests |
| `test_gc`       | Garbled Circuits tests |
| `test_2pc`      | Two-party Computation tests |
| `test_abit`     | ABIT (Arithmetic Bit) tests |
| `test_prng`     | Pseudo-Random Number Generator tests |
- **docs**: Generates the documentation if Doxygen is available.

## How to Build the Project

To build the project, follow these steps:

1. Create a build directory:

   ```bash
   mkdir build
   cd build
   cmake ..
   ```

## How to Build Tests

To build the tests, use the following command:

```bash
cmake --build . --target <test_target>
```

# Generate Documentation

If the project includes documentation and Doxygen is available, you can generate the documentation by running:

```bash
cmake --build . --target docs
```