/*******************************/
/* ann_mlp_ga_py_interface.cpp */
/*         Version 1.0         */
/*          2024/09/20         */
/*******************************/

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "ann_mlp_ga_v1.h"

namespace py = pybind11;
#if defined USE_BLAS
PYBIND11_MODULE(cpp_nn_py, m)
#else
// simpler interface without BLAS nor HDF5 dependencies
PYBIND11_MODULE(cpp_nn_py2, m)
#endif
{
    // Expose ANN_MLP_GA<float> to Python
    py::class_<nn::ANN_MLP_GA<float>>(m, "ANN_MLP_GA_float")
        .def(py::init<>())
        .def(py::init<std::vector<size_t>, int, size_t, size_t, size_t, bool>())

        .def("PrintNetworkInfo", &nn::ANN_MLP_GA<float>::PrintNetworkInfo)
        .def("PrintBiases", &nn::ANN_MLP_GA<float>::PrintBiases)
        .def("PrintWeights", &nn::ANN_MLP_GA<float>::PrintWeights)
        .def("SetName", &nn::ANN_MLP_GA<float>::SetName)
        .def("SetEpochs", &nn::ANN_MLP_GA<float>::SetEpochs)
        .def("UpdateEpochs", &nn::ANN_MLP_GA<float>::UpdateEpochs)
        .def("Serialize", &nn::ANN_MLP_GA<float>::Serialize)
        .def("Deserialize", &nn::ANN_MLP_GA<float>::Deserialize)
        .def("GetPopSize", &nn::ANN_MLP_GA<float>::GetPopSize)
        .def("GetTopPerformersSize", &nn::ANN_MLP_GA<float>::GetTopPerformersSize)
        .def("GetEpochs", &nn::ANN_MLP_GA<float>::GetEpochs)
        .def("UpdateWeightsAndBiases", &nn::ANN_MLP_GA<float>::UpdateWeightsAndBiases)
        .def("feedforward",
             [](nn::ANN_MLP_GA<float>& self, py::array_t<float> inputs, py::array_t<float> outputs, size_t memberid,
                bool singleReturn) {
        // directly get pointers to the underlying data
        const float* pInputs = inputs.data();
        float* pOutputs      = outputs.mutable_data();
        // get sizes
        size_t inputsSize    = static_cast<size_t>(inputs.size());
        size_t outputsSize   = static_cast<size_t>(outputs.size());
        // call the C++ function with raw pointers
        self.feedforward(pInputs, inputsSize, pOutputs, outputsSize, memberid, singleReturn);
    },
             py::arg("inputs"), py::arg("outputs"), py::arg("memberid"), py::arg("singleReturn"))

        .def("feedforwardIndex",
             [](nn::ANN_MLP_GA<float>& self, py::array_t<float> inputs, size_t memberid) -> size_t {
        // directly get pointer to the underlying data
        const float* pInputs = inputs.data();
        // get size
        size_t inputsSize    = static_cast<size_t>(inputs.size());
        // call the C++ function overload that returns the index
        return self.feedforward(pInputs, inputsSize, memberid);
    }, py::arg("inputs"), py::arg("memberid"))

        .def("TrainGA", &nn::ANN_MLP_GA<float>::TrainGA)
        .def("TestGA", &nn::ANN_MLP_GA<float>::TestGA)
        .def("SetMixed", &nn::ANN_MLP_GA<float>::SetMixed)
        .def("GetMixed", &nn::ANN_MLP_GA<float>::GetMixed)
        .def("CreatePopulation", &nn::ANN_MLP_GA<float>::CreatePopulation)
        .def("GetNetworkSizeDim", &nn::ANN_MLP_GA<float>::GetNetworkSizeDim)
        .def("GetNetworkSize", &nn::ANN_MLP_GA<float>::GetNetworkSize);

    // expose ANN_MLP_GA<double> to Python
    py::class_<nn::ANN_MLP_GA<double>>(m, "ANN_MLP_GA_double")
        .def(py::init<>())
        .def(py::init<std::vector<size_t>, int, size_t, size_t, size_t, bool>())

        .def("PrintNetworkInfo", &nn::ANN_MLP_GA<double>::PrintNetworkInfo)
        .def("PrintBiases", &nn::ANN_MLP_GA<double>::PrintBiases)
        .def("PrintWeights", &nn::ANN_MLP_GA<double>::PrintWeights)
        .def("SetName", &nn::ANN_MLP_GA<double>::SetName)
        .def("SetEpochs", &nn::ANN_MLP_GA<double>::SetEpochs)
        .def("UpdateEpochs", &nn::ANN_MLP_GA<double>::UpdateEpochs)
        .def("Serialize", &nn::ANN_MLP_GA<double>::Serialize)
        .def("Deserialize", &nn::ANN_MLP_GA<double>::Deserialize)
        .def("GetPopSize", &nn::ANN_MLP_GA<double>::GetPopSize)
        .def("GetTopPerformersSize", &nn::ANN_MLP_GA<double>::GetTopPerformersSize)
        .def("GetEpochs", &nn::ANN_MLP_GA<double>::GetEpochs)
        .def("UpdateWeightsAndBiases", &nn::ANN_MLP_GA<double>::UpdateWeightsAndBiases)
        .def("feedforward",
             [](nn::ANN_MLP_GA<double>& self, py::array_t<double> inputs, py::array_t<double> outputs, size_t memberid,
                bool singleReturn) {
        // directly get pointers to the underlying data
        const double* pInputs = inputs.data();
        double* pOutputs      = outputs.mutable_data();
        // get sizes
        size_t inputsSize     = static_cast<size_t>(inputs.size());
        size_t outputsSize    = static_cast<size_t>(outputs.size());
        // call the C++ function with raw pointers
        self.feedforward(pInputs, inputsSize, pOutputs, outputsSize, memberid, singleReturn);
    },
             py::arg("inputs"), py::arg("outputs"), py::arg("memberid"), py::arg("singleReturn"))

        .def("feedforwardIndex",
             [](nn::ANN_MLP_GA<double>& self, py::array_t<double> inputs, size_t memberid) -> size_t {
        // directly get pointer to the underlying data
        const double* pInputs = inputs.data();
        // get size
        size_t inputsSize     = static_cast<size_t>(inputs.size());
        // call the C++ function overload that returns the index
        return self.feedforward(pInputs, inputsSize, memberid);
    }, py::arg("inputs"), py::arg("memberid"))

        .def("TrainGA", &nn::ANN_MLP_GA<double>::TrainGA)
        .def("TestGA", &nn::ANN_MLP_GA<double>::TestGA)
        .def("SetMixed", &nn::ANN_MLP_GA<double>::SetMixed)
        .def("GetMixed", &nn::ANN_MLP_GA<double>::GetMixed)
        .def("CreatePopulation", &nn::ANN_MLP_GA<double>::CreatePopulation)
        .def("GetNetworkSizeDim", &nn::ANN_MLP_GA<double>::GetNetworkSizeDim)
        .def("GetNetworkSize", &nn::ANN_MLP_GA<double>::GetNetworkSize);
}
