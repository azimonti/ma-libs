/******************/
/*    main.cpp    */
/*  Version 1.0   */
/*   2023/05/05   */
/******************/

#include <iomanip>
#include <iostream>
#include <vector>
#include "example.h"

int main()
{
    std::cout << "--- C++ Main Demonstration ---" << std::endl;

    // Test Example class
    std::cout << "\nTesting Example class:" << std::endl;
    Example ex("C++ User");
    ex.greet();
    std::cout << "Current name: " << ex.getName() << std::endl;
    ex.setName("Direct C++ User");
    ex.greet();

    // Test VectorWriter<float>
    std::cout << "\nTesting VectorWriter<float>:" << std::endl;
    VectorWriter<float> vw_float;
    std::cout << "Float vector elements from getVector(): ";
    std::vector<float> float_vec = vw_float.getVector();
    for (size_t i = 0; i < float_vec.size(); ++i)
    {
        std::cout << float_vec[i] << (i == float_vec.size() - 1 ? "" : ", ");
    }
    std::cout << std::endl;
    vw_float.printVector();

    std::cout << "Float array elements from getArray(): ";
    std::vector<float> float_arr_from_get = vw_float.getArray();
    for (size_t i = 0; i < float_arr_from_get.size(); ++i)
    {
        std::cout << float_arr_from_get[i] << (i == float_arr_from_get.size() - 1 ? "" : ", ");
    }
    std::cout << std::endl;
    vw_float.printArray();

    float float_output_buffer[3];
    vw_float.writeArrayToMemory(float_output_buffer, 3);
    std::cout << "Content of float_output_buffer after writeArrayToMemory: ";
    for (size_t i = 0; i < 3; ++i) { std::cout << float_output_buffer[i] << (i == 2 ? "" : ", "); }
    std::cout << std::endl;

    float float_vec_output_buffer[4]; // For writeVectorToMemory (vector has 4 elements)
    vw_float.writeVectorToMemory(float_vec_output_buffer, 4);
    std::cout << "Content of float_vec_output_buffer after writeVectorToMemory: ";
    for (size_t i = 0; i < 4; ++i) { std::cout << float_vec_output_buffer[i] << (i == 3 ? "" : ", "); }
    std::cout << std::endl;

    // Test VectorWriter<double>
    std::cout << "\nTesting VectorWriter<double>:" << std::endl;
    VectorWriter<double> vw_double;
    std::cout << "Double vector elements from getVector(): ";
    std::vector<double> double_vec = vw_double.getVector();
    for (size_t i = 0; i < double_vec.size(); ++i)
    {
        std::cout << double_vec[i] << (i == double_vec.size() - 1 ? "" : ", ");
    }
    std::cout << std::endl;
    vw_double.printVector();

    std::cout << "Double array elements from getArray(): ";
    std::vector<double> double_arr_from_get = vw_double.getArray();
    for (size_t i = 0; i < double_arr_from_get.size(); ++i)
    {
        std::cout << double_arr_from_get[i] << (i == double_arr_from_get.size() - 1 ? "" : ", ");
    }
    std::cout << std::endl;
    vw_double.printArray();

    double double_output_buffer[3];
    vw_double.writeArrayToMemory(double_output_buffer, 3);
    std::cout << "Content of double_output_buffer after writeArrayToMemory: ";
    for (size_t i = 0; i < 3; ++i) { std::cout << double_output_buffer[i] << (i == 2 ? "" : ", "); }
    std::cout << std::endl;

    double double_vec_output_buffer[4]; // For writeVectorToMemory (vector has 4 elements)
    vw_double.writeVectorToMemory(double_vec_output_buffer, 4);
    std::cout << "Content of double_vec_output_buffer after writeVectorToMemory: ";
    for (size_t i = 0; i < 4; ++i) { std::cout << double_vec_output_buffer[i] << (i == 3 ? "" : ", "); }
    std::cout << std::endl;

    std::cout << "\n--- C++ Main Demonstration End ---" << std::endl;
    return 0;
}
