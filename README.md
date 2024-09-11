# Useful C++ / Fortran libraries 

This repository is containing some libraries that I have developed through the years and that I will reuse for other project.

Since the installation is not easy (in particular on Windows it requires both installation of Fortran and compilation of HDF5) I decided to create a dedicated repository, so that I can just link the relevant pre-compiled binaries whenever I need them.

Two Libraries are currently present:

- C++ BLAS/LAPACK bindings made with templates supporting different types (`float` / `double` / `std::complex<float>` / `std::complex<double>` 

- a C++ neural network library with BLAS/LAPACK backend and HDF5 for storing the weights so that can be later reused.

## Required Tools

- Git
- CMake
- clang
- Fortran

## Quick Start

### 1) Get the external dependencies

```bash
./manage_dependency_libraries.sh --download
./manage_dependency_libraries.sh --build
```

### 2) Do a full build

You should do a full-build at least once via command line so that libraries (static and shared) are created, both for Release and Debug:

```bash
./cbuild.sh -t Debug
./cbuild.sh -t Release
```
Executables can now be found under `build/`.
