/*******************************/
/* ann_mlp_ga_py_interface.cpp */
/*         Version 2.0         */
/*          2025/05/11         */
/*******************************/

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "ann_mlp_ga_v1.h"

namespace py = pybind11;

// Helper function to bind the templated class
template <typename T> void bind_ann_mlp_ga_class(py::module& m, const std::string& class_name_suffix)
{
    using Class            = nn::ANN_MLP_GA<T>; // Alias for the specific instantiation
    std::string class_name = "ANN_MLP_GA_" + class_name_suffix;

    py::class_<Class>(m, class_name.c_str())
        .def(py::init<>())
        .def(py::init<std::vector<size_t>, int, size_t, size_t, size_t>(), py::arg("layers_size"),
             py::arg("activation_fcn_type"), py::arg("pop_size"), py::arg("top_performers_size"), py::arg("epochs"))

        .def("PrintNetworkInfo", &Class::PrintNetworkInfo)
        .def("PrintBiases", &Class::PrintBiases)
        .def("PrintWeights", &Class::PrintWeights)
        .def("SetName", &Class::SetName, py::arg("name"))
        .def("SetEpochs", &Class::SetEpochs, py::arg("epochs"))
        .def("UpdateEpochs", &Class::UpdateEpochs, py::arg("epochs"))
        .def("Serialize", &Class::Serialize, py::arg("filename"))
        .def("Deserialize", &Class::Deserialize, py::arg("filename"))
        .def("GetPopSize", &Class::GetPopSize)
        .def("GetTopPerformersSize", &Class::GetTopPerformersSize)
        .def("GetEpochs", &Class::GetEpochs)
        .def("UpdateWeightsAndBiases", &Class::UpdateWeightsAndBiases)
        .def("feedforward",
             [](Class& self, py::array_t<T> inputs, py::array_t<T> outputs, size_t memberid, bool singleReturn) {
        py::buffer_info inputs_buf  = inputs.request();
        py::buffer_info outputs_buf = outputs.request();

        if (inputs_buf.ndim != 1) { throw std::runtime_error("Input array must be 1-dimensional."); }
        if (outputs_buf.ndim != 1) { throw std::runtime_error("Output array must be 1-dimensional."); }

        const T* pInputs   = static_cast<const T*>(inputs_buf.ptr);
        T* pOutputs        = static_cast<T*>(outputs_buf.ptr);
        size_t inputsSize  = static_cast<size_t>(inputs_buf.shape[0]);
        size_t outputsSize = static_cast<size_t>(outputs_buf.shape[0]);

        self.feedforward(pInputs, inputsSize, pOutputs, outputsSize, memberid, singleReturn);
    }, py::arg("inputs"), py::arg("outputs"), py::arg("memberid"), py::arg("singleReturn"))

        .def("feedforwardIndex",
             [](Class& self, py::array_t<T> inputs, size_t memberid) -> size_t {
        py::buffer_info inputs_buf = inputs.request();
        if (inputs_buf.ndim != 1) { throw std::runtime_error("Input array must be 1-dimensional."); }

        const T* pInputs  = static_cast<const T*>(inputs_buf.ptr);
        size_t inputsSize = static_cast<size_t>(inputs_buf.shape[0]);
        return self.feedforward(pInputs, inputsSize, memberid);
    }, py::arg("inputs"), py::arg("memberid"))

        .def("CreatePopulation", &Class::CreatePopulation)
        .def("GetNetworkSizeDim", &Class::GetNetworkSizeDim)
        .def("GetNetworkSize", &Class::GetNetworkSize)
        .def("SetPopulationStrategy", &Class::SetPopulationStrategy, py::arg("strategy"),
             py::arg("injection_ratio") = 0.15, "Sets the population creation strategy and the random injection ratio.")
        .def("GetPopulationStrategy", &Class::GetPopulationStrategy, "Gets the current population strategy.")
        .def("GetRandomInjectionRatio", &Class::GetRandomInjectionRatio, "Gets the current random injection ratio.");
}

#if defined(CPP_NN_PY_BIND)
PYBIND11_MODULE(cpp_nn_py, m)
#elif defined(CPP_NN_PY_BLAS_BIND)
PYBIND11_MODULE(cpp_nn_blas_py, m)
#elif defined(CPP_NN_PY_BLAS_HDF5_BIND)
PYBIND11_MODULE(cpp_nn_blas_hdf5_py, m)
#else
#error "Define one of: CPP_NN_PY_BIND, CPP_NN_PY_BLAS_BIND, CPP_NN_PY_BLAS_HDF5_BIND"
#endif
{
    m.doc() = "pybind11 plugin for ANN_MLP_GA";

    py::enum_<nn::PopulationStrategy>(m, "PopulationStrategy")
        .value("MIXED", nn::PopulationStrategy::MIXED)
        .value("FIXED", nn::PopulationStrategy::FIXED)
        .value("MIXED_WITH_RANDOM_INJECTION", nn::PopulationStrategy::MIXED_WITH_RANDOM_INJECTION)
        .value("FIXED_WITH_RANDOM_INJECTION", nn::PopulationStrategy::FIXED_WITH_RANDOM_INJECTION)
        .export_values();

    // Bind the float version
    bind_ann_mlp_ga_class<float>(m, "float");

    // Bind the double version
    bind_ann_mlp_ga_class<double>(m, "double");
}
