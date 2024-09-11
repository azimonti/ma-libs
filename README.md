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

A full-build at least once via command line is necessary, so that libraries (static and shared) are created, both for Release and Debug:

```bash
./cbuild.sh -t Debug
./cbuild.sh -t Release
```
Executables can now be found under `build/`.

## IMPORTANT NOTICE for WINDOWS

To properly link Intel Fortran libraries, ensure that the path to the Intel Fortran library directory is set in your environment. This path is necessary to locate `ifconsol.lib`, which is required by Intel Fortran for linking when building DLLs. Set the environment variable `INTEL_FORTRAN_LIB_PATH` to the directory containing `ifconsol.lib`, typically located at `INTEL_INSTALL_PATH/Intel_fortran/compiler/latest/windows/compiler/lib/intel64_win`. Failure to set this path may result in unresolved symbols during the build process.

