# CppTest with pybind11 Python bindings

This project demonstrates a C++ class `CppTest` with a derived Python class `PyTest`, using **PyBind11** to create Python bindings for C++ code. The project provides access to an integer array and a vector of doubles, allowing Python users to manipulate the data efficiently.

## Features

- **CppTest** class: Contains an integer array `n[3]` and a `std::vector<double>` with 4 randomly generated values.
- Functions to print the integer array and vector content from C++.
- Python bindings via **PyBind11** allow users to access C++ functions in Python, including:
  - Accessing a memory view of the integer array and double vector.
  - Viewing array/vector contents using `pybind11::memoryview`.
  
## Installation

1. **Create a Virtual Environment**:

   ```
   ./create_virtualenv
   ```

2. **Build the Project**:

   ```
   ../../cbuild -t Release (or Debug) 
   ```

## Usage

After compiling the project, you can run the Python bindings.

1. **Activate the virtual environment** (if not already activated):

   ```bash
   source ../../build/python_env/activate 
   ```

2. **Run Python** and test the bindings:

   ```
   python pybind11_add_p.py
   ./build/Release/main_static (or ./build/Debug/main_static)
   ```

- The project uses **PyBind11** to expose C++ classes and functions to Python.
- The `CppTest` class contains an array `n[3]` and a vector `random_doubles` initialized with 4 random `double` values.
- A derived `PyTest` class provides two main functions:
  - `array_int()`: Exposes the integer array as a `pybind11::memoryview`, which allows manipulation from Python.
  - `vector_double()`: Exposes the double vector as a `pybind11::memoryview`.
  
These bindings allow seamless access to C++ data structures in Python for high performance and efficient memory management.

## Example output

When running the Python code to interact with the bindings, you should see output similar to:

```
int from cpp: 1
string from cpp: This is a string
default call: 2
call one function: 3
Test String
unpack n[0] 22
unpack n[0] 56
unpack n[0] 87
getter n[0] 22
getter n[0] v2 22
getter constexpr 4
unpack vector[0]: 0.796542984287846
unpack vector[1]: 0.18343478789336848
unpack vector[2]: 0.7796909976126613
unpack vector[3]: 0.5968501615800565
```

When running the c++ code, you should see output similar to:

```
Hello World!
7
Test String
22
Integer array n: 22 56 87 
Double vector: 0.796543 0.183435 0.779691 0.59685 
```
