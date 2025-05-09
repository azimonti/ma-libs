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
./create_virtualenv.sh
```

2. **Build the Project**:

```
../../cbuild -t Release (or Debug)
```

## Usage

After compiling the project, you can run the Python bindings.

1. **Activate the virtual environment** (if not already activated):

```bash
source ./venv/bin/activate #./venv/Scripts/activate on Windows
```

2. **Run Python** and test the bindings:

```
python main.py
../../build/Release/bind_example_main
```

The Python bindings, created using **PyBind11** and available under the `example_module`, expose the following C++ classes and functionalities:

1.  **`Example` Class**:
    *   A simple class demonstrating basic C++ to Python binding.
    *   `__init__(self, name: str)`: Constructor.
    *   `greet(self) -> str`: Returns a greeting string.
    *   `getName(self) -> str`: Gets the current name.
    *   `setName(self, name: str)`: Sets a new name.
    *   `name`: A property for getting and setting the name.

2.  **`VectorWriterFloat` Class**:
    *   Manages a C-style array and an `std::vector` of `float` values.
    *   `__init__(self)`: Constructor.
    *   `array_t(self)`: Returns a `pybind11::memoryview` of the internal C-style array, allowing direct, efficient access from Python.
    *   `vector_t(self)`: Returns a `pybind11::memoryview` of the internal `std::vector<float>`, enabling direct manipulation.
    *   `getVector(self)`: Returns the `std::vector<float>`.
    *   `printVector(self)`: Prints the content of the vector.
    *   `getArray(self)`: Returns the C-style array.
    *   `printArray(self)`: Prints the content of the array.
    *   `writeArrayToMemory(self, output_array: numpy.ndarray[float])`: Copies the content of the internal C-style array to a pre-allocated NumPy array.
    *   `writeVectorToMemory(self, output_array: numpy.ndarray[float])`: Copies the content of the internal `std::vector<float>` to a pre-allocated NumPy array.

3.  **`VectorWriterDouble` Class**:
    *   Similar to `VectorWriterFloat`, but manages `double` values.
    *   `__init__(self)`: Constructor.
    *   `array_t(self)`: Returns a `pybind11::memoryview` of the internal C-style array of doubles.
    *   `vector_t(self)`: Returns a `pybind11::memoryview` of the internal `std::vector<double>`.
    *   `getVector(self)`: Returns the `std::vector<double>`.
    *   `printVector(self)`: Prints the content of the vector.
    *   `getArray(self)`: Returns the C-style array.
    *   `printArray(self)`: Prints the content of the array.
    *   `writeArrayToMemory(self, output_array: numpy.ndarray[double])`: Copies the internal C-style array to a NumPy array.
    *   `writeVectorToMemory(self, output_array: numpy.ndarray[double])`: Copies the internal `std::vector<double>` to a NumPy array.

These bindings facilitate efficient data exchange between C++ and Python, using `pybind11::memoryview` for direct memory access and providing utilities to work with NumPy arrays for numerical computations. The underlying C++ structure involves a base `VectorWriter<T>` class and a derived `PyVectorWriter<T>` class, which are specialized for `float` and `double` types in the Python module.

## Example output

When running the Python code to interact with the bindings, you should see output similar to:

```
(venv) python main.py
--- Testing Example class ---
Hello, World!
Initial name: World
Updated name: Python User
Hello, Python User!

--- Testing VectorWriterFloat ---
Float vector: [0.3745401203632355, 0.7965430021286011, 0.9507142901420593, 0.18343478441238403]
Vector elements: 0.37454, 0.796543, 0.950714, 0.183435
Float array: [22.0, 56.0, 87.0]
Array elements: 22, 56, 87

--- Testing VectorWriterDouble ---
Double vector: [0.796542984287846, 0.18343478789336848, 0.7796909976126613, 0.5968501615800565]
Vector elements: 0.796543, 0.183435, 0.779691, 0.59685
Double array: [22.0, 56.0, 87.0]
Array elements: 22, 56, 87

--- Testing writeArrayToMemory (Float) ---
Content of float_array_output after writeArrayToMemory: [22. 56. 87.]

--- Testing writeArrayToMemory (Double) ---
Content of double_array_output after writeArrayToMemory: [22. 56. 87.]

--- Testing writeVectorToMemory (Float) ---
Content of float_vector_output after writeVectorToMemory: [0.37454012 0.796543   0.9507143  0.18343478]

--- Testing writeVectorToMemory (Double) ---
Content of double_vector_output after writeVectorToMemory: [0.79654298 0.18343479 0.779691   0.59685016]

--- Testing Memory View (array_t - Float) ---
  Memory view object: <memory at 0x103a22a40>
  Length (bytes): 12
  Readonly: True
  Unpacked float array (data_):
    Element 0: 22.0
    Element 1: 56.0
    Element 2: 87.0

--- Testing Memory View (vector_t - Float) ---
  Memory view object: <memory at 0x103a22980>
  Length (bytes): 4
  Unpacked float vector (data2_):
    Element 0: 0.3745401203632355
    Element 1: 0.7965430021286011
    Element 2: 0.9507142901420593
    Element 3: 0.18343478441238403

--- Testing Memory View (array_t - Double) ---
  Memory view object: <memory at 0x103a22b00>
  Length (bytes): 24
  Unpacked double array (data_):
    Element 0: 22.0
    Element 1: 56.0
    Element 2: 87.0

--- Testing Memory View (vector_t - Double) ---
  Memory view object: <memory at 0x103a22bc0>
  Length (bytes): 4
  Unpacked double vector (data2_):
    Element 0: 0.796542984287846
    Element 1: 0.18343478789336848
    Element 2: 0.7796909976126613
    Element 3: 0.5968501615800565
```

When running the c++ code, you should see output similar to:

```
../../build/Release/bind_example_main
--- C++ Main Demonstration ---

Testing Example class:
Hello, C++ User!
Current name: C++ User
Hello, Direct C++ User!

Testing VectorWriter<float>:
Float vector elements from getVector(): 0.37454, 0.796543, 0.950714, 0.183435
Vector elements: 0.37454, 0.796543, 0.950714, 0.183435
Float array elements from getArray(): 22, 56, 87
Array elements: 22, 56, 87
Content of float_output_buffer after writeArrayToMemory: 22, 56, 87
Content of float_vec_output_buffer after writeVectorToMemory: 0.37454, 0.796543, 0.950714, 0.183435

Testing VectorWriter<double>:
Double vector elements from getVector(): 0.796543, 0.183435, 0.779691, 0.59685
Vector elements: 0.796543, 0.183435, 0.779691, 0.59685
Double array elements from getArray(): 22, 56, 87
Array elements: 22, 56, 87
Content of double_output_buffer after writeArrayToMemory: 22, 56, 87
Content of double_vec_output_buffer after writeVectorToMemory: 0.796543, 0.183435, 0.779691, 0.59685

--- C++ Main Demonstration End ---
```
