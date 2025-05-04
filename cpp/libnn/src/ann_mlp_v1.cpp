/************************/
/*    ann_mlp_v1.cpp    */
/*    Version 2.0       */
/*     2025/05/04       */
/************************/

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#if defined(USE_HDF5)
#include "hdf5/hdf5_ext.h"
#endif
#include "ann_mlp_v1.h"

namespace nnflags
{
    enum NNVersion : int { V1 = 0 };
}

template <typename T>
nn::ANN_MLP<T>::ANN_MLP()
    : nEpochs(0), flags(0), normal_distribution(0, 1), uniform_real_distribution(0, 1),
      uniform_int_distribution(0, RAND_MAX)
{
}

template <typename T> nn::ANN_MLP<T>::ANN_MLP::~ANN_MLP() = default;

template <typename T>
nn::ANN_MLP<T>::ANN_MLP(std::vector<size_t> size, int seed, size_t populationSize, size_t topPerformersSize,
                        size_t activationFunction)
    : vSize(size), nEpochs(0), nPopSize(populationSize), nTop(topPerformersSize), act(activationFunction), flags(0),
      normal_distribution(0, 1), uniform_real_distribution(0, 1), uniform_int_distribution(0, RAND_MAX)
{
    assert(nPopSize >= nTop);
    nLayers = size.size();
    generator.seed(static_cast<unsigned int>(seed));
    AllocateWeightsBiases();
}

template <typename T> int nn::ANN_MLP<T>::GetVersion() const
{
    return nnflags::NNVersion::V1;
}

template <typename T> void nn::ANN_MLP<T>::AllocateWeightsBiases()
{
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < nTop; ++i)
    {
        vBiases.push_back(std::vector<la::Matrix<T>>{});
        vWeights.push_back(std::vector<la::Matrix<T>>{});
        // no need to allocate for the first layer as it is an input layer
        for (size_t j = 1; j < nLayers; ++j)
        {
            const size_t nRows = vSize[j], nCols = vSize[j - 1];
            vBiases[i].push_back(la::Matrix<T>{nRows});
            vWeights[i].push_back(la::Matrix<T>{nRows, nCols});
            for (size_t n = 0; n < nRows; ++n)
            {
                vBiases[i][j - 1][n][0] = GetRandomNormal();
                for (size_t m = 0; m < nCols; ++m) vWeights[i][j - 1][n][m] = GetRandomNormal();
            }
        }
    }
}

template <typename T> void nn::ANN_MLP<T>::PrintNetworkInfo()
{
    std::lock_guard<std::mutex> lock(mtx);
    std::string ast(20 + sName.size(), '*');
    ast.append("\n");
    std::cout << ast << "Network " << sName << " Information\n" << ast;
    std::cout << "Number of layers: " << nLayers << "\n\n";
    std::cout << "Input layer: " << vSize[0] << " inputs\n\n";
    std::cout << "Output layer: " << vSize[vSize.size() - 1] << " outputs\n\n";

    for (size_t i = 1; i < nLayers; ++i)
    {
        std::cout << "\nLayer: " << i << "\nWeights: [" << vSize[i] << " x " << vSize[i - 1] << "]\n"
                  << "Biases: [" << vSize[i] << " x 1]\n";
    }
    std::cout << ast;
}

template <typename T> void nn::ANN_MLP<T>::PrintBiases()
{
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < nTop; ++i)
        for (size_t j = 1; j < nLayers; ++j)
        {
            std::cout << std::setprecision(2) << std::fixed;
            const size_t nRows = vSize[j];
            std::cout << "Population " << i + 1 << " layer " << j << " biases. Layer size: [" << nRows << " x 1]\n";
            for (size_t n = 0; n < nRows; ++n) std::cout << vBiases[i][j - 1][n][0] << '\t';
            std::cout << std::endl;
        }
}

template <typename T> void nn::ANN_MLP<T>::PrintWeights()
{
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < nTop; ++i)
        for (size_t j = 1; j < nLayers; ++j)
        {
            std::cout << std::setprecision(2) << std::fixed;
            const size_t nRows = vSize[j], nCols = vSize[j - 1];
            std::cout << "Population " << i + 1 << " layer " << j << " weights. Layer size: [" << nRows << " x "
                      << nCols << "]\n";

            for (size_t n = 0; n < nRows; ++n)
                for (size_t m = 0; m < nCols; ++m) std::cout << vWeights[i][j - 1][n][m] << '\t';
            std::cout << std::endl;
        }
}

template <typename T> void nn::ANN_MLP<T>::Serialize(const std::string& fname)
{
    if (sName.empty()) throw std::runtime_error("Network name not defined nn::ANN_MLP::Serialize.");
    std::lock_guard<std::mutex> lock(mtx);
#if defined(USE_HDF5)
    h5::H5ppWriter h5(fname);
    h5.write("NN/" + sName + "/vSize", vSize);
    for (size_t i = 0; i < nTop; ++i)
    {
        for (size_t j = 0; j < nLayers - 1; ++j)
        {
            h5.write("NN/" + sName + "/vBiases[" + std::to_string(i) + "][" + std::to_string(j) + "]",
                     vBiases[i][j].data());
        }
        for (size_t j = 0; j < nLayers - 1; ++j)
        {
            h5.write("NN/" + sName + "/vWeights[" + std::to_string(i) + "][" + std::to_string(j) + "]",
                     vWeights[i][j].data());
        }
    }
    h5.write("NN/" + sName + "/nLayers", nLayers);
    h5.write("NN/" + sName + "/nEpochs", nEpochs);
    h5.write("NN/" + sName + "/nTop", nTop);
    h5.write("NN/" + sName + "/nPopSize", nPopSize);
    h5.write("NN/" + sName + "/activationFunction", act);
    h5.write("NN/" + sName + "/flags", flags);
    // Serialize Mersenne Twister generator status
    std::stringstream ss;
    ss << generator;
    h5.write("NN/" + sName + "/generator", ss.str());
    int v_ = GetVersion();
    h5.write("NN/" + sName + "/version", v_);
#else
    // Text-based serialization
    std::ofstream outFile(fname, std::ios::binary);
    if (!outFile.is_open()) { throw std::runtime_error("Could not open file for writing: " + fname); }

    outFile << "sName " << sName << "\n";
    outFile << "nEpochs " << nEpochs << "\n";
    outFile << "nPopSize " << nPopSize << "\n";
    outFile << "nTop " << nTop << "\n";
    outFile << "act " << act << "\n";
    outFile << "flags " << flags << "\n";
    outFile << "nLayers " << nLayers << "\n";

    outFile << "vSize ";
    for (size_t size : vSize) { outFile << size << " "; }
    outFile << "\n";

    outFile << "vBiases_start\n";
    for (size_t i = 0; i < nTop; ++i)
    {
        for (size_t j = 0; j < nLayers - 1; ++j)
        {
            outFile << "pop " << i << " layer " << j << "\n";
            outFile << "dims " << vBiases[i][j].GetRowsNb() << " " << vBiases[i][j].GetColsNb() << "\n";
            for (size_t r = 0; r < vBiases[i][j].GetRowsNb(); ++r)
            {
                for (size_t c = 0; c < vBiases[i][j].GetColsNb(); ++c) { outFile << vBiases[i][j](r, c) << " "; }
                outFile << "\n";
            }
        }
    }
    outFile << "vBiases_end\n";

    outFile << "vWeights_start\n";
    for (size_t i = 0; i < nTop; ++i)
    {
        for (size_t j = 0; j < nLayers - 1; ++j)
        {
            outFile << "pop " << i << " layer " << j << "\n";
            outFile << "dims " << vWeights[i][j].GetRowsNb() << " " << vWeights[i][j].GetColsNb() << "\n";
            for (size_t r = 0; r < vWeights[i][j].GetRowsNb(); ++r)
            {
                for (size_t c = 0; c < vWeights[i][j].GetColsNb(); ++c) { outFile << vWeights[i][j](r, c) << " "; }
                outFile << "\n";
            }
        }
    }
    outFile << "vWeights_end\n";

    // Serialize Mersenne Twister generator status
    std::stringstream ss_gen;
    ss_gen << generator;
    outFile << "generator " << ss_gen.str() << "\n";

    int v_text = GetVersion();
    outFile << "version " << v_text << "\n";

    outFile.close();
#endif
}

template <typename T> void nn::ANN_MLP<T>::Deserialize(const std::string& fname)
{
    if (sName.empty()) throw std::runtime_error("Network name not defined nn::ANN_MLP:::Deserialize.");
    std::lock_guard<std::mutex> lock(mtx);
    int version = -1;
#if defined(USE_HDF5)
    h5::H5ppReader h5(fname);
    h5.read("NN/" + sName + "/nLayers", nLayers);
    h5.read("NN/" + sName + "/nEpochs", nEpochs);
    h5.read("NN/" + sName + "/nPopSize", nPopSize);
    h5.read("NN/" + sName + "/nTop", nTop);
    h5.read("NN/" + sName + "/activationFunction", act);
    h5.read("NN/" + sName + "/vSize", vSize);
    h5.read("NN/" + sName + "/flags", flags);

    vBiases  = {};
    vWeights = {};
    for (size_t i = 0; i < nTop; ++i)
    {
        vBiases.push_back(std::vector<la::Matrix<T>>{});
        vWeights.push_back(std::vector<la::Matrix<T>>{});
        for (size_t j = 0; j < nLayers - 1; ++j)
        {

            const size_t nRows = vSize[j + 1], nCols = vSize[j];

            vBiases[i].push_back(la::Matrix<T>{nRows});
            h5.read("NN/" + sName + "/vBiases[" + std::to_string(i) + "][" + std::to_string(j) + "]", 1, [&](size_t n) {
                (void)n;
                return vBiases[i][j].data().data();
            });
            vWeights[i].push_back(la::Matrix<T>{nRows, nCols});
            h5.read("NN/" + sName + "/vWeights[" + std::to_string(i) + "][" + std::to_string(j) + "]", 1,
                    [&](size_t n) {
                (void)n;
                return vWeights[i][j].data().data();
            });
        }
    }
    std::string g;
    h5.read("NN/" + sName + "/generator", g);
    std::stringstream ss(g);
    ss >> generator;
    h5.read("NN/" + sName + "/version", version);
#else
    // Text-based deserialization
    std::ifstream inFile(fname, std::ios::binary);
    if (!inFile.is_open()) { throw std::runtime_error("Could not open file for reading: " + fname); }

    std::string line, key;

    while (std::getline(inFile, line))
    {
        std::stringstream ss_line(line); // Use ss_line for parsing each line
        ss_line >> key;

        if (key == "sName") ss_line >> sName;
        else if (key == "nEpochs") ss_line >> nEpochs;
        else if (key == "nPopSize") ss_line >> nPopSize;
        else if (key == "nTop") ss_line >> nTop;
        else if (key == "act") ss_line >> act;
        else if (key == "flags") ss_line >> flags;
        else if (key == "nLayers") ss_line >> nLayers;
        else if (key == "vSize")
        {
            vSize.clear();
            size_t val;
            while (ss_line >> val) { vSize.push_back(val); }
        }
        else if (key == "vBiases_start")
        {
            vBiases.clear();
            vBiases.resize(nTop);
            size_t current_pop = 0, current_layer = 0;
            size_t rows = 0, cols = 0;
            while (std::getline(inFile, line) && line != "vBiases_end")
            {
                std::stringstream ss_bias(line);
                std::string bias_key;
                ss_bias >> bias_key;
                if (bias_key == "pop")
                {
                    ss_bias >> current_pop >> bias_key >> current_layer; // reads "layer" keyword
                    if (vBiases[current_pop].empty()) { vBiases[current_pop].resize(nLayers - 1); }
                }
                else if (bias_key == "dims")
                {
                    ss_bias >> rows >> cols;
                    vBiases[current_pop][current_layer].Resize(rows, cols); // Use Resize
                }
                else
                {                                    // Assume it's matrix data line
                    std::stringstream ss_data(line); // Use the original line for data
                    for (size_t r = 0; r < rows; ++r)
                    {
                        if (r > 0)
                        { // Read subsequent lines for the same matrix
                            if (!std::getline(inFile, line))
                                throw std::runtime_error("Unexpected EOF in bias matrix data");
                            ss_data.clear();
                            ss_data.str(line);
                        }
                        for (size_t c = 0; c < cols; ++c)
                        {
                            if (!(ss_data >> vBiases[current_pop][current_layer](r, c)))
                            {
                                throw std::runtime_error("Error parsing bias matrix data");
                            }
                        }
                    }
                }
            }
        }
        else if (key == "vWeights_start")
        {
            vWeights.clear();
            vWeights.resize(nTop);
            size_t current_pop = 0, current_layer = 0;
            size_t rows = 0, cols = 0;
            while (std::getline(inFile, line) && line != "vWeights_end")
            {
                std::stringstream ss_weight(line);
                std::string weight_key;
                ss_weight >> weight_key;
                if (weight_key == "pop")
                {
                    ss_weight >> current_pop >> weight_key >> current_layer; // reads "layer" keyword
                    if (vWeights[current_pop].empty()) { vWeights[current_pop].resize(nLayers - 1); }
                }
                else if (weight_key == "dims")
                {
                    ss_weight >> rows >> cols;
                    vWeights[current_pop][current_layer].Resize(rows, cols); // Use Resize
                }
                else
                {                                    // Assume it's matrix data line
                    std::stringstream ss_data(line); // Use the original line for data
                    for (size_t r = 0; r < rows; ++r)
                    {
                        if (r > 0)
                        { // Read subsequent lines for the same matrix
                            if (!std::getline(inFile, line))
                                throw std::runtime_error("Unexpected EOF in weight matrix data");
                            ss_data.clear();
                            ss_data.str(line);
                        }
                        for (size_t c = 0; c < cols; ++c)
                        {
                            if (!(ss_data >> vWeights[current_pop][current_layer](r, c)))
                            {
                                throw std::runtime_error("Error parsing weight matrix data");
                            }
                        }
                    }
                }
            }
        }
        else if (key == "generator")
        {
            std::string gen_state = line.substr(key.length() + 1); // +1 for space
            std::stringstream ss_gen(gen_state);
            ss_gen >> generator;
        }
        else if (key == "version") { ss_line >> version; }
    }
    inFile.close();

    // Validation after loading
    if (vSize.empty() || vBiases.empty() || vWeights.empty() || vBiases.size() != nTop || vWeights.size() != nTop)
    {
        throw std::runtime_error("Deserialization failed: Incomplete or inconsistent data loaded.");
    }
    for (size_t i = 0; i < nTop; ++i)
    {
        if (vBiases[i].size() != nLayers - 1 || vWeights[i].size() != nLayers - 1)
        {
            throw std::runtime_error("Deserialization failed: Incorrect number of layers loaded for biases/weights.");
        }
    }
#endif
    int expectedVersion = GetVersion();
    if (version != expectedVersion)
    {
        throw std::runtime_error("Incorrect Version: expected " + std::to_string(expectedVersion) + " found " +
                                 std::to_string(version));
    }
}

// Explicit template instantiation
template class nn::ANN_MLP<float>;
template class nn::ANN_MLP<double>;
