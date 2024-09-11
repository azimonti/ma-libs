/************************/
/*    ann_mlp_v1.cpp    */
/*    Version 1.0       */
/*     2023/02/26       */
/************************/

#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "hdf5/hdf5_ext.h"
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
    int v_ = nnflags::NNVersion::V1;
    h5.write("NN/" + sName + "/version", v_);
}

template <typename T> void nn::ANN_MLP<T>::Deserialize(const std::string& fname)
{
    if (sName.empty()) throw std::runtime_error("Network name not defined nn::ANN_MLP:::Deserialize.");
    std::lock_guard<std::mutex> lock(mtx);
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
}

// Explicit template instantiation
template class nn::ANN_MLP<float>;
template class nn::ANN_MLP<double>;
