/************************/
/*   ann_mlp_ga_v1.cpp  */
/*    Version 2.0       */
/*     2025/05/11       */
/************************/

#include <algorithm>
#include <numeric>
#include "log/log.h"
#include "math/algebra/matrix_operations.h"
#include "ann_mlp_ga_v1.h"
#if defined(USE_BLAS)
#include "la_blas_mult.h"
#endif

namespace nnflags
{
    enum NNFlags : int {
        MIXED_POPULATION = 1 << 0,
    };
}

template <typename T> nn::ANN_MLP_GA<T>::ANN_MLP_GA() {}

template <typename T>
nn::ANN_MLP_GA<T>::ANN_MLP_GA(std::vector<size_t> size, int seed, size_t populationSize, size_t topPerformersSize,
                              size_t activationFunction)
    : ANN_MLP<T>(size, seed, populationSize, topPerformersSize, activationFunction)
{
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

template <typename T> size_t nn::ANN_MLP_GA<T>::feedforward(const T* pInputs, size_t inputsSize, size_t memberid)
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

    const std::vector<T>& res_                           = na_[nLayers - 1].data();

    const typename std::vector<T>::const_iterator maxPos = std::max_element(res_.begin(), res_.end());

    return static_cast<size_t>(std::distance(res_.begin(), maxPos));
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

template <typename T> void nn::ANN_MLP_GA<T>::SetPopulationStrategy(PopulationStrategy strategy, double injection_ratio)
{
    population_strategy_ = strategy;
    if (injection_ratio >= 0.0 && injection_ratio <= 1.0) { random_injection_ratio_ = injection_ratio; }
    else
    {
        // LOG_WARN("ANN_MLP_GA: Invalid random_injection_ratio provided: %f. Using current value: %f", injection_ratio,
        // random_injection_ratio_);
        std::cerr << "Warning: ANN_MLP_GA: Invalid random_injection_ratio provided: " << injection_ratio
                  << ". Using current value: " << random_injection_ratio_ << std::endl;
    }
}

template <typename T> void nn::ANN_MLP_GA<T>::CreatePopulation(bool bKeepPrevious)
{
    // Dispatch to the appropriate creation method based on strategy
    switch (population_strategy_)
    {
    case PopulationStrategy::MIXED: CreatePopulationMixed(bKeepPrevious); break;
    case PopulationStrategy::FIXED: CreatePopulationFixed(bKeepPrevious); break;
    case PopulationStrategy::MIXED_WITH_RANDOM_INJECTION:
        CreatePopulationMixedWithRandomInjection(bKeepPrevious);
        break;
    case PopulationStrategy::FIXED_WITH_RANDOM_INJECTION:
        CreatePopulationFixedWithRandomInjection(bKeepPrevious);
        break;
    default: // Fallback to MIXED
        // LOG_WARN("ANN_MLP_GA: Unknown population strategy. Defaulting to MIXED.");
        std::cerr << "Warning: ANN_MLP_GA: Unknown population strategy. Defaulting to MIXED." << std::endl;
        CreatePopulationMixed(bKeepPrevious);
        break;
    }
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

template <typename T> void nn::ANN_MLP_GA<T>::CreatePopulationMixedWithRandomInjection(bool bKeepPrevious)
{
    std::lock_guard<std::mutex> lock(mtx);
    size_t current_pop_idx = 0;

    // 1. Elitism: Keep top performers if bKeepPrevious is true
    if (bKeepPrevious)
    {
        for (size_t i = 0; i < nTop && current_pop_idx < nPopSize; ++i)
        {
            for (size_t k = 0; k < nLayers - 1; ++k)
            {
                vBiasesPop[current_pop_idx][k].assign(vBiases[i][k].data());
                vWeightsPop[current_pop_idx][k].assign(vWeights[i][k].data());
            }
            current_pop_idx++;
        }
    }

    // 2. Introduce a percentage of purely random individuals
    size_t num_to_fill_after_elitism = nPopSize - current_pop_idx;
    size_t num_random_individuals =
        static_cast<size_t>(static_cast<double>(num_to_fill_after_elitism) * random_injection_ratio_);

    for (size_t i = 0; i < num_random_individuals && current_pop_idx < nPopSize; ++i)
    {
        for (size_t j = 0; j < nLayers - 1; ++j)
        {
            const size_t nRows = vSize[j + 1];
            const size_t nCols = vSize[j];
            for (size_t r = 0; r < nRows; ++r)
            {
                vBiasesPop[current_pop_idx][j][r][0] = GetRandomNormal(); // Pure random
                for (size_t c = 0; c < nCols; ++c)
                {
                    vWeightsPop[current_pop_idx][j][r][c] = GetRandomNormal(); // Pure random
                }
            }
        }
        current_pop_idx++;
    }

    // 3. Fill the rest with the mixed strategy (crossover + mutation from top performers)
    for (size_t i = current_pop_idx; i < nPopSize; ++i)
    {
        size_t parent_a_idx = static_cast<size_t>(GetRandomUniformInt() % static_cast<int>(nTop));
        size_t parent_b_idx = static_cast<size_t>(GetRandomUniformInt() % static_cast<int>(nTop));
        if (nTop > 1)
        { // Ensure different parents if possible
            while (parent_a_idx == parent_b_idx)
                parent_b_idx = static_cast<size_t>(GetRandomUniformInt() % static_cast<int>(nTop));
        }

        for (size_t j = 0; j < nLayers - 1; ++j)
        {
            const size_t chosen_parent_idx = (GetRandomUniformReal() < 0.5 || nTop == 1) ? parent_a_idx : parent_b_idx;

            vBiasesPop[i][j].assign(vBiases[chosen_parent_idx][j].data());
            vBiasesPop[i][j] *= static_cast<T>(0.9); // Scale down
            vWeightsPop[i][j].assign(vWeights[chosen_parent_idx][j].data());
            vWeightsPop[i][j] *= static_cast<T>(0.9); // Scale down

            const size_t nRows = vSize[j + 1];
            const size_t nCols = vSize[j];
            for (size_t r = 0; r < nRows; ++r)
            {
                vBiasesPop[i][j][r][0] += static_cast<T>(0.1) * GetRandomNormal(); // Mutate
                for (size_t c = 0; c < nCols; ++c)
                {
                    vWeightsPop[i][j][r][c] += static_cast<T>(0.1) * GetRandomNormal(); // Mutate
                }
            }
        }
    }
}

template <typename T> void nn::ANN_MLP_GA<T>::CreatePopulationFixedWithRandomInjection(bool bKeepPrevious)
{
    std::lock_guard<std::mutex> lock(mtx);
    size_t current_pop_idx = 0;

    // 1. Elitism: Keep top performers
    if (bKeepPrevious)
    {
        for (size_t i = 0; i < nTop && current_pop_idx < nPopSize; ++i)
        {
            for (size_t k = 0; k < nLayers - 1; ++k)
            {
                vBiasesPop[current_pop_idx][k].assign(vBiases[i][k].data());
                vWeightsPop[current_pop_idx][k].assign(vWeights[i][k].data());
            }
            current_pop_idx++;
        }
    }

    // 2. Crossover part from CreatePopulationFixed
    for (size_t i = 0; i < nTop && current_pop_idx < nPopSize; ++i)
    {
        for (size_t j = i + 1; j < nTop && current_pop_idx < nPopSize; ++j)
        {
            for (size_t k = 0; k < nLayers - 1; ++k)
            {
                vBiasesPop[current_pop_idx][k].Zeros();
                vBiasesPop[current_pop_idx][k] += vBiases[i][k];
                vBiasesPop[current_pop_idx][k] += vBiases[j][k];
                vBiasesPop[current_pop_idx][k] *= static_cast<T>(0.5);
                vWeightsPop[current_pop_idx][k].Zeros();
                vWeightsPop[current_pop_idx][k] += vWeights[i][k];
                vWeightsPop[current_pop_idx][k] += vWeights[j][k];
                vWeightsPop[current_pop_idx][k] *= static_cast<T>(0.5);
            }
            current_pop_idx++;
        }
    }

    // 3. Introduce a percentage of purely random individuals
    size_t num_to_fill_after_elitism_crossover = nPopSize - current_pop_idx;
    size_t num_random_individuals =
        static_cast<size_t>(static_cast<double>(num_to_fill_after_elitism_crossover) * random_injection_ratio_);
    // Adjust if random_injection_ratio_ is high, ensure it doesn't try to fill more than available slots
    num_random_individuals = std::min(num_random_individuals, num_to_fill_after_elitism_crossover);

    for (size_t i = 0; i < num_random_individuals && current_pop_idx < nPopSize; ++i)
    {
        for (size_t j = 0; j < nLayers - 1; ++j)
        {
            const size_t nRows = vSize[j + 1];
            const size_t nCols = vSize[j];
            for (size_t r = 0; r < nRows; ++r)
            {
                vBiasesPop[current_pop_idx][j][r][0] = GetRandomNormal(); // Pure random
                for (size_t c = 0; c < nCols; ++c)
                {
                    vWeightsPop[current_pop_idx][j][r][c] = GetRandomNormal(); // Pure random
                }
            }
        }
        current_pop_idx++;
    }

    // 4. Fill remaining with mutation part from CreatePopulationFixed
    size_t top_performer_idx_for_mutation = 0;
    while (current_pop_idx < nPopSize)
    {
        for (size_t j = 0; j < nLayers - 1; ++j)
        {
            vBiasesPop[current_pop_idx][j].Zeros();
            vBiasesPop[current_pop_idx][j] += vBiases[top_performer_idx_for_mutation][j];
            vBiasesPop[current_pop_idx][j] *= static_cast<T>(0.9); // Scale down
            vWeightsPop[current_pop_idx][j].Zeros();
            vWeightsPop[current_pop_idx][j] += vWeights[top_performer_idx_for_mutation][j];
            vWeightsPop[current_pop_idx][j] *= static_cast<T>(0.9); // Scale down

            const size_t nRows = vSize[j + 1];
            const size_t nCols = vSize[j];
            for (size_t r = 0; r < nRows; ++r)
            {
                vBiasesPop[current_pop_idx][j][r][0] += static_cast<T>(0.1) * GetRandomNormal(); // Mutate
                for (size_t c = 0; c < nCols; ++c)
                {
                    vWeightsPop[current_pop_idx][j][r][c] += static_cast<T>(0.1) * GetRandomNormal(); // Mutate
                }
            }
        }
        top_performer_idx_for_mutation = (top_performer_idx_for_mutation + 1) % nTop;
        current_pop_idx++;
    }
}

// Explicit template instantiation
template class nn::ANN_MLP_GA<float>;
template class nn::ANN_MLP_GA<double>;
