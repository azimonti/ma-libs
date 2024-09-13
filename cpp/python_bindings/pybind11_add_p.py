#!/usr/bin/env python3
'''
/************************/
/*   pybind11_add_p.py  */
/*     Version 1.0      */
/*      2021/04/04      */
/************************/
'''
import os
import sys
import struct

cwd = os.getcwd()
build_dir = os.path.join(cwd, "./build")

if "DEBUG" in os.environ:
    build_path = os.path.join(build_dir, "Debug")
else:
    build_path = os.path.join(build_dir, "Release")

# Add the appropriate build path to sys.path
sys.path.append(os.path.realpath(build_path))

# Try importing the pybind11_add module
try:
    import pybind11_add as pybind11_add
    print(f"Module pybind11_add imported successfully from {build_path}.")
except ModuleNotFoundError as e:
    print(f"Error: {e}")


def main():
    print('int from cpp:', pybind11_add.test_integer)
    print('string from cpp:', pybind11_add.test_string)
    print('default call:', pybind11_add.add())
    print('call one function:', pybind11_add.add(1, 2))

    t = pybind11_add.PyTest()
    t.print_test()

    nn = t.array_int().tobytes()
    n = struct.unpack('i', nn[0:4])
    print("unpack n[0]", n[0])
    n = struct.unpack('i', nn[4:8])
    print("unpack n[0]", n[0])
    n = struct.unpack('i', nn[8:12])
    print("unpack n[0]", n[0])
    print("getter n[0]", t.n0())
    print("getter n[0] v2", t.n0_2())
    print("getter constexpr", t.n2())
    dd = t.vector_double().tobytes()

    # Unpacking double (each double is 8 bytes)
    _d = struct.unpack('d', dd[0:8])
    print("unpack vector[0]:", _d[0])
    _d = struct.unpack('d', dd[8:16])
    print("unpack vector[1]:", _d[0])
    _d = struct.unpack('d', dd[16:24])
    print("unpack vector[2]:", _d[0])
    _d = struct.unpack('d', dd[24:32])
    print("unpack vector[3]:", _d[0])


if __name__ == '__main__':
    if sys.version_info[0] < 3:
        raise 'Must be using Python 3'
    main()
