/************************/
/*   ann_mlp_ga_v1.cpp  */
/*    Version 1.1       */
/*     2023/05/04       */
/************************/

#include <algorithm>
#include <numeric>
#include "log/log.h"
#include "math/algebra/matrix_operations.h"
#include "ann_mlp_ga_v1.h"
#include "la_blas_mult.h"

namespace nnflags
{
    enum NNFlags : int {
        MIXED_POPULATION = 1 << 0,
    };
}

template <typename T> nn::ANN_MLP_GA<T>::ANN_MLP_GA() {}

template <typename T>
nn::ANN_MLP_GA<T>::ANN_MLP_GA(std::vector<size_t> size, int seed, size_t populationSize, size_t topPerformersSize,
                              size_t activationFunction, bool bGenerateMixed)
    : ANN_MLP<T>(size, seed, populationSize, topPerformersSize, activationFunction)
{
    flags = bGenerateMixed ? flags | nnflags::NNFlags::MIXED_POPULATION : flags & ~nnflags::NNFlags::MIXED_POPULATION;
    AllocatePopulation();
}

template <typename T>
void nn::ANN_MLP_GA<T>::feedforward(const T* pInputs, size_t inputsSize, T* pOutputs, size_t outputsSize,
                                    size_t memberid, bool singleReturn)
{
    void (*pAct)(T*, size_t) = nullptr;
    switch (act)
    {
    case SIGMOID: pAct = &sigmoid<T>; break;
    case TANH: pAct = &tanh<T>; break;
    default: throw std::invalid_argument("Unknown activation function");
    }

    // set the input layer
    std::vector<la::Matrix<T>> na_;
    for (size_t i = 0; i < nLayers; ++i) { na_.push_back(la::Matrix<T>(vSize[i])); }
    na_[0].assign(pInputs, inputsSize);

    // feedforward the network
    for (size_t l = 1; l < nLayers; ++l)
    {
        la::MatMultVec(na_[l], vWeightsPop[memberid][l - 1], na_[l - 1]);
        na_[l] += vBiasesPop[memberid][l - 1];
        nn::ActFunc(na_[l], pAct);
    }
    if (singleReturn)
    {
        // set all contribution to zero except the maximum value which is set to 1
        std::vector<T>& res_                         = na_[nLayers - 1].data();
        const typename std::vector<T>::iterator it2_ = std::max_element(res_.begin(), res_.end());
        const ptrdiff_t maxPos                       = std::distance(res_.begin(), it2_);

        std::fill(res_.begin(), res_.end(), static_cast<T>(0));
        res_[static_cast<size_t>(maxPos)] = static_cast<T>(1);
    }

    const auto& outLayer = na_[nLayers - 1];
    assert(outLayer.size() == outputsSize);
    for (size_t i = 0; i < outputsSize; ++i) { pOutputs[i] = outLayer[i][0]; }
}

template <typename T>
void nn::ANN_MLP_GA<T>::feedforward(const std::vector<T>& vInputs, std::vector<T>& vOutputs, size_t memberid,
                                    bool singleReturn)
{
    feedforward(vInputs.data(), vInputs.size(), vOutputs.data(), vOutputs.size(), memberid, singleReturn);
}

template <typename T>
void nn::ANN_MLP_GA<T>::TrainGA(const std::vector<std::vector<T>>& data, const std::vector<std::vector<T>>& reference,
                                size_t nGenerations, size_t BatchSize, bool shuffleTrainingData)
{
    std::random_device rd;
    std::vector<size_t> s_(data.size()), f_(nPopSize), v_(nPopSize);
    std::vector<size_t>::iterator it_;
    std::mt19937 g_{rd()};
    void (*pAct)(T*, size_t) = nullptr;
    switch (act)
    {
    case SIGMOID: pAct = &sigmoid<T>; break;
    case TANH: pAct = &tanh<T>; break;
    default: throw std::invalid_argument("Unknown activation function");
    }

    // Allocate the memory for the computed activations
    std::vector<la::Matrix<T>> na_;
    for (size_t i = 0; i < nLayers; ++i) { na_.push_back(la::Matrix<T>(vSize[i], 1)); }

    for (size_t i = 0; i < nGenerations; ++i)
    {
        // populate and shuffle the interator of integers for training
        std::iota(s_.begin(), s_.end(), 0);
        if (shuffleTrainingData && !(i % 10))
        {
            std::cout << "Reshuffling training data\n";
            std::shuffle(s_.begin(), s_.end(), g_);
        }
        // create the population
        CreatePopulation();
        // reset the fitness vector
        std::fill(f_.begin(), f_.end(), 0);
        // all population is tested vs the reference
        for (size_t j = 0; j < std::min(s_.size(), BatchSize); ++j)
        {
            it_ = s_.begin() + (ptrdiff_t)j;
            // first activation layer - input data. Is never overwritten and therefore can be allocated only once
            na_[0].assign(data[*it_]);
            for (size_t k = 0; k < nPopSize; ++k)
            {
                // first activation layer - input data.
                vNaPop[k][0].assign(data[*it_]);
                // feedforward the network
                for (size_t l = 1; l < nLayers; ++l)
                {
                    la::MatMultVec(vNaPop[k][l], vWeightsPop[k][l - 1], vNaPop[k][l - 1]);
                    vNaPop[k][l] += vBiasesPop[k][l - 1];
                    nn::ActFunc(vNaPop[k][l], pAct);
                }
                // compute the fitness (+1 if the result is correct)
                typename std::vector<T>& res                 = vNaPop[k][nLayers - 1].data();
                const typename std::vector<T>::iterator it2_ = std::max_element(res.begin(), res.end());
                const ptrdiff_t maxPos                       = std::distance(res.begin(), it2_);
                if (reference[j][static_cast<size_t>(maxPos)] == 1) f_[k]++;
            }

            // update to the next sample
            it_++;
        }
        // sort in descending order and store the index
        std::iota(v_.begin(), v_.end(), 0);
        std::sort(v_.begin(), v_.end(), [&](size_t i, size_t j) { return f_[i] > f_[j]; });
        // update the weights
        {
            // vector length is fixed
            const size_t length_ = vBiases[0].size();
            for (size_t j = 0; j < nTop; ++j)
            {
                for (size_t k = 0; k < length_; ++k)
                {
                    vBiases[j][k].assign(vBiasesPop[v_[j]][k].data());
                    vWeights[j][k].assign(vWeightsPop[v_[j]][k].data());
                }
            }
        }
        // global training epochs is updated with the number of generations
        nEpochs++;
        std::cout << "Current correct ratio: " << int(100. * static_cast<T>(f_[v_[0]]) / static_cast<T>(BatchSize))
                  << "% (" << f_[v_[0]] << " / " << BatchSize << " training data [shuffle "
                  << (shuffleTrainingData ? "ON" : " OFF") << "])\n";
        std::cout << "Generation " << i + 1 << " / " << nGenerations << " completed (Total " << nEpochs << ")\n";
    }
}

template <typename T>
int nn::ANN_MLP_GA<T>::TestGA(const std::vector<std::vector<T>>& data, const std::vector<std::vector<T>>& reference)
{
    int iCorrect = 0;
    std::vector<la::Matrix<T>> na_;
    void (*pAct)(T*, size_t) = nullptr;
    switch (act)
    {
    case SIGMOID: pAct = &sigmoid<T>; break;
    case TANH: pAct = &tanh<T>; break;
    default: throw std::invalid_argument("Unknown activation function");
    }

    for (size_t i = 0; i < nLayers; ++i) { na_.push_back(la::Matrix<T>(vSize[i], 1)); }

    for (size_t i = 1; i < data.size(); ++i)
    {
        for (size_t l = 0; l < na_[0].GetRowsNb(); ++l) na_[0][l][0] = data[i][l];

        for (size_t j = 1; j < nLayers; ++j)
        {
            la::MatMultVec(na_[j], vWeights[0][j - 1], na_[j - 1]);
            na_[j] += vBiases[0][j - 1];
            nn::ActFunc(na_[j], pAct);
        }
        // find the max element
        typename std::vector<T>& res                = na_[nLayers - 1].data();
        const typename std::vector<T>::iterator it_ = std::max_element(res.begin(), res.end());
        const ptrdiff_t maxPos                      = std::distance(res.begin(), it_);
        const ptrdiff_t refPos =
            std::distance(reference[i].begin(), std::max_element(reference[i].begin(), reference[i].end()));
        if (maxPos == refPos) iCorrect++;
    }
    return iCorrect;
}

template <typename T> void nn::ANN_MLP_GA<T>::AllocatePopulation()
{
    std::lock_guard<std::mutex> lock(mtx);
    // allocate the memory
    for (size_t i = 0; i < nPopSize; ++i)
    {
        vBiasesPop.push_back(std::vector<la::Matrix<T>>{});
        vWeightsPop.push_back(std::vector<la::Matrix<T>>{});
        vNaPop.push_back(std::vector<la::Matrix<T>>{});

        vNaPop[i].push_back(la::Matrix<T>(vSize[0]));

        for (size_t j = 1; j < nLayers; ++j)
        {
            const size_t nRows = vSize[j], nCols = vSize[j - 1];
            vBiasesPop[i].push_back(la::Matrix<T>{nRows});
            vWeightsPop[i].push_back(la::Matrix<T>{nRows, nCols});
            vNaPop[i].push_back(la::Matrix<T>(nRows));
        }
    }
}

template <typename T> void nn::ANN_MLP_GA<T>::CreatePopulation(bool bKeepPrevious)
{
    if (flags & nnflags::NNFlags::MIXED_POPULATION) CreatePopulationMixed(bKeepPrevious);
    else CreatePopulationFixed(bKeepPrevious);
}

template <typename T> void nn::ANN_MLP_GA<T>::Serialize(const std::string& fname)
{
    // called the super class
    nn::ANN_MLP<T>::Serialize(fname);
}

template <typename T> void nn::ANN_MLP_GA<T>::Deserialize(const std::string& fname)
{
    // called the super class
    nn::ANN_MLP<T>::Deserialize(fname);
    // allocate the memory
    AllocatePopulation();
    // create the population
    CreatePopulation(true);
}

template <typename T> void nn::ANN_MLP_GA<T>::CreatePopulationFixed(bool bKeepPrevious)
{
    std::lock_guard<std::mutex> lock(mtx);
    size_t a_ = 0, b_ = 0;
    // linearly combine the population
    [&] {
        // keep the top of the previous generation
        if (bKeepPrevious)
            for (size_t i = 0; i < nTop; ++i)
            {
                for (size_t k = 0; k < nLayers - 1; ++k)
                {
                    vBiasesPop[i][k].assign(vBiases[i][k].data());
                    vWeightsPop[i][k].assign(vWeights[i][k].data());
                }
                ++a_;
                if (a_ == nPopSize) return;
            }
        for (size_t i = 0; i < nTop; ++i)
            for (size_t j = i + 1; j < nTop; ++j)
            {
                for (size_t k = 0; k < nLayers - 1; ++k)
                {
                    vBiasesPop[a_][k].Zeros();
                    vBiasesPop[a_][k] += vBiases[i][k];
                    vBiasesPop[a_][k] += vBiases[j][k];
                    vBiasesPop[a_][k] *= static_cast<T>(0.5);
                    vWeightsPop[a_][k].Zeros();
                    vWeightsPop[a_][k] += vWeights[i][k];
                    vWeightsPop[a_][k] += vWeights[j][k];
                    vWeightsPop[a_][k] *= static_cast<T>(0.5);
                }
                ++a_;
                if (a_ == nPopSize) return;
            }
    }();
    while (a_ < nPopSize)
    {
        // create random population
        for (size_t i = 0; i < nLayers - 1; ++i)
        {
            vBiasesPop[a_][i].Zeros();
            vBiasesPop[a_][i] += vBiases[b_][i];
            vBiasesPop[a_][i] *= static_cast<T>(0.9);
            vWeightsPop[a_][i].Zeros();
            vWeightsPop[a_][i] += vWeights[b_][i];
            vWeightsPop[a_][i] *= static_cast<T>(0.9);
            const size_t nRows = vSize[i + 1], nCols = vSize[i];
            for (size_t n = 0; n < nRows; ++n)
            {
                vBiasesPop[a_][i][n][0] += static_cast<T>(0.1) * GetRandomNormal();
                for (size_t m = 0; m < nCols; ++m) vWeightsPop[a_][i][n][m] += static_cast<T>(0.1) * GetRandomNormal();
            }
        }
        // chose the next top performer for the following mutation
        b_ = (b_ + 1) % nTop;
        ++a_;
    }
}

template <typename T> void nn::ANN_MLP_GA<T>::CreatePopulationMixed(bool bKeepPrevious)
{
    // ramdomly combine the population
    std::lock_guard<std::mutex> lock(mtx);
    size_t a_ = 0, b_ = 0, count_ = 0;
    if (bKeepPrevious)
        for (size_t i = 0; i < nTop; ++i)
        {
            for (size_t k = 0; k < nLayers - 1; ++k)
            {
                vBiasesPop[i][k].assign(vBiases[i][k].data());
                vWeightsPop[i][k].assign(vWeights[i][k].data());
            }
            count_++;
        }
    for (size_t i = count_; i < nPopSize; ++i)
    {
        // select two random individuals
        a_ = static_cast<size_t>(GetRandomUniformInt() % static_cast<int>(nTop));
        b_ = static_cast<size_t>(GetRandomUniformInt() % static_cast<int>(nTop));
        // ensure that they are different
        while (a_ == b_) b_ = static_cast<size_t>(GetRandomUniformInt() % static_cast<int>(nTop));
        // create a new individual randomly combining the two selected
        for (size_t j = 0; j < nLayers - 1; ++j)
        {
            // get a random number between 0 and 1 to decide which parent to chose
            const size_t p = (GetRandomUniformReal() < 0.5) ? a_ : b_;
            // copy the parent's weights
            vBiasesPop[i][j].assign(vBiases[p][j].data());
            vBiasesPop[i][j] *= static_cast<T>(0.9);
            vWeightsPop[i][j].assign(vWeights[p][j].data());
            vWeightsPop[i][j] *= static_cast<T>(0.9);
            // mutate the weights
            const size_t nRows = vSize[j + 1], nCols = vSize[j];
            for (size_t n = 0; n < nRows; ++n)
            {
                vBiasesPop[i][j][n][0] += static_cast<T>(0.1) * GetRandomNormal();
                for (size_t m = 0; m < nCols; ++m) vWeightsPop[i][j][n][m] += static_cast<T>(0.1) * GetRandomNormal();
            }
        }
    }
}

template <typename T> void nn::ANN_MLP_GA<T>::UpdateWeightsAndBiases(const std::vector<size_t>& v_)
{
    std::lock_guard<std::mutex> lock(mtx);
    // vector length is fixed
    const size_t length_ = vBiases[0].size();
    for (size_t j = 0; j < nTop; ++j)
    {
        for (size_t k = 0; k < length_; ++k)
        {
            vBiases[j][k].assign(vBiasesPop[v_[j]][k].data());
            vWeights[j][k].assign(vWeightsPop[v_[j]][k].data());
        }
    }
}

template <typename T> void nn::ANN_MLP_GA<T>::SetMixed(bool bMixed)
{
    if (bMixed) flags |= nnflags::NNFlags::MIXED_POPULATION;
    else flags &= ~nnflags::NNFlags::MIXED_POPULATION;
}

template <typename T> bool nn::ANN_MLP_GA<T>::GetMixed()
{
    return flags & nnflags::NNFlags::MIXED_POPULATION;
}

// Explicit template instantiation
template class nn::ANN_MLP_GA<float>;
template class nn::ANN_MLP_GA<double>;
