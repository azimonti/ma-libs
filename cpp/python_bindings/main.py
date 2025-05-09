'''
/***************/
/*   main.py   */
/* Version 1.0 */
/*  2025/05/07 */
/***************/
'''
import numpy as np
import os
import sys
import struct

cwd = os.getcwd()
print(cwd)
# if call from the root directory
build_dir = os.path.join(cwd, "./build/Release")
sys.path.append(os.path.realpath(build_dir))
# if call from the cpp/python_bindings/ directory
build_dir = os.path.join(cwd, "../../build/Release")
sys.path.append(os.path.realpath(build_dir))

# Import the compiled module.
# The name 'example_module' must match the name given in PYBIND11_MODULE.
try:
    import example_module
except ImportError as e:
    print(f"Error: {e}")
    sys.exit(1)


def run_example_tests():
    print("--- Testing Example class ---")
    ex = example_module.Example("World")
    ex.greet()  # Should print "Hello, World!"
    print(f"Initial name: {ex.getName()}")
    ex.setName("Python User")
    print(f"Updated name: {ex.name}")  # Using property
    ex.greet()  # Should print "Hello, Python User!"

    print("\n--- Testing VectorWriterFloat ---")
    vw_float = example_module.VectorWriterFloat()
    print(f"Float vector: {vw_float.getVector()}")
    vw_float.printVector()
    print(f"Float array: {vw_float.getArray()}")
    vw_float.printArray()

    print("\n--- Testing VectorWriterDouble ---")
    vw_double = example_module.VectorWriterDouble()
    print(f"Double vector: {vw_double.getVector()}")
    vw_double.printVector()
    print(f"Double array: {vw_double.getArray()}")
    vw_double.printArray()

    print("\n--- Testing writeArrayToMemory (Float) ---")
    float_array_output = np.zeros(3, dtype=np.float32)
    vw_float.writeArrayToMemory(float_array_output)
    print("Content of float_array_output after writeArrayToMemory: "
          f"{float_array_output}")

    print("\n--- Testing writeArrayToMemory (Double) ---")
    double_array_output = np.zeros(3, dtype=np.float64)
    vw_double.writeArrayToMemory(double_array_output)
    print("Content of double_array_output after writeArrayToMemory: "
          f"{double_array_output}")

    print("\n--- Testing writeVectorToMemory (Float) ---")
    float_vector_output = np.zeros(4, dtype=np.float32)
    vw_float.writeVectorToMemory(float_vector_output)
    print("Content of float_vector_output after writeVectorToMemory: "
          f"{float_vector_output}")

    print("\n--- Testing writeVectorToMemory (Double) ---")
    double_vector_output = np.zeros(4, dtype=np.float64)
    vw_double.writeVectorToMemory(double_vector_output)
    print("Content of double_vector_output after writeVectorToMemory: "
          f"{double_vector_output}")

    print("\n--- Testing Memory View (array_t - Float) ---")
    memview_array_f = vw_float.array_t()
    print(f"  Memory view object: {memview_array_f}")
    print(f"  Length (bytes): {len(memview_array_f)}")
    print(f"  Readonly: {memview_array_f.readonly}")
    float_array_bytes = memview_array_f.tobytes()
    print("  Unpacked float array (data_):")
    for i in range(3):  # data_ has 3 elements
        # 4 bytes for a float
        val = struct.unpack_from('f', float_array_bytes, i * 4)
        print(f"    Element {i}: {val[0]}")

    print("\n--- Testing Memory View (vector_t - Float) ---")
    memview_vector_f = vw_float.vector_t()
    print(f"  Memory view object: {memview_vector_f}")
    print(f"  Length (bytes): {len(memview_vector_f)}")
    float_vector_bytes = memview_vector_f.tobytes()
    print("  Unpacked float vector (data2_):")
    for i in range(4):  # data2_ has 4 elements
        val = struct.unpack_from('f', float_vector_bytes, i * 4)
        print(f"    Element {i}: {val[0]}")

    print("\n--- Testing Memory View (array_t - Double) ---")
    memview_array_d = vw_double.array_t()
    print(f"  Memory view object: {memview_array_d}")
    print(f"  Length (bytes): {len(memview_array_d)}")
    double_array_bytes = memview_array_d.tobytes()
    print("  Unpacked double array (data_):")
    for i in range(3):  # data_ has 3 elements
        # 8 bytes for a double
        val = struct.unpack_from('d', double_array_bytes, i * 8)
        print(f"    Element {i}: {val[0]}")

    print("\n--- Testing Memory View (vector_t - Double) ---")
    memview_vector_d = vw_double.vector_t()
    print(f"  Memory view object: {memview_vector_d}")
    print(f"  Length (bytes): {len(memview_vector_d)}")
    double_vector_bytes = memview_vector_d.tobytes()
    print("  Unpacked double vector (data2_):")
    for i in range(4):  # data2_ has 4 elements
        val = struct.unpack_from('d', double_vector_bytes, i * 8)
        print(f"    Element {i}: {val[0]}")


if __name__ == "__main__":
    run_example_tests()
