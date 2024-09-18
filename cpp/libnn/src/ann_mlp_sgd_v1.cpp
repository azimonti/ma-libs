/************************/
/* ann_mlp_sgd_v1.cpp   */
/*    Version 2.0       */
/*     2023/04/12       */
/************************/

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <assert.h>
#include "math/algebra/matrix_operations.h"
#include "ann_mlp_sgd_v1.h"
#include "la_blas_mult.h"

template <typename T> nn::ANN_MLP_SGD<T>::ANN_MLP_SGD() {}

template <typename T>
nn::ANN_MLP_SGD<T>::ANN_MLP_SGD(std::vector<size_t> size, int seed, size_t activationFunction)
    : ANN_MLP<T>(size, seed, 1, 1, activationFunction)
{
}

template <typename T>
void nn::ANN_MLP_SGD<T>::TrainSGD(const std::vector<std::vector<T>>& data, const std::vector<std::vector<T>>& reference,
                                  size_t epochs, size_t miniBatchSize, double eta)
{

    std::random_device rd;
    std::vector<size_t> s_;
    std::vector<size_t>::iterator it_;
    s_.resize(data.size());
    std::mt19937 g_{rd()};
    void (*pAct)(T*, const T*, size_t) = nullptr;
    void (*pDAct)(T*, size_t)          = nullptr;

    switch (act)
    {
    case SIGMOID:
        pAct  = &sigmoid<T>;
        pDAct = &dsigmoid<T>;
        break;
    case TANH:
        pAct  = &tanh<T>;
        pDAct = &dtanh<T>;
        break;
    default: throw std::invalid_argument("Unknown activation function");
    }

    (void)pAct;
    (void)pDAct;

    // allocate temporary memory for Gradient Descent
    // na_ store the activation
    // nb_, nw_ store the nabla of the gradient
    // dnb_, dnw_ store the gradient of the nabla of the gradient
    // dno_ store the delta for backpropagation (same size as the biases)
    std::vector<la::Matrix<T>> na_, nzv_, dno_, dno2_, nb_, nw_, dnb_, dnw_, wt_;

    na_.push_back(la::Matrix<T>(vSize[0], 1));

    for (size_t i = 1; i < nLayers; ++i)
    {
        const size_t nRows = vSize[i], nCols = vSize[i - 1];
        na_.push_back(la::Matrix<T>{nRows, 1});
        nzv_.push_back(la::Matrix<T>{nRows, 1});
        dno_.push_back(la::Matrix<T>{nRows, 1});
        dno2_.push_back(la::Matrix<T>{nRows, 1});
        nb_.push_back(la::Matrix<T>{nRows, 1});
        dnb_.push_back(la::Matrix<T>{nRows, 1});
        nw_.push_back(la::Matrix<T>{nRows, nCols});
        dnw_.push_back(la::Matrix<T>{nRows, nCols});
        wt_.push_back(la::Matrix<T>{nCols, nRows});
    }

    for (size_t i = 0; i < epochs; ++i)
    {
        // populate and shuffle the interator of integers for training
        std::iota(s_.begin(), s_.end(), 0);
        std::shuffle(s_.begin(), s_.end(), g_);
        for (size_t j = 0; j < s_.size() - miniBatchSize; j += miniBatchSize)
        {
            // process a mini-batch of data
            it_ = s_.begin() + (long)j;
            // Zeros the Gradient
            for (size_t l = 0; l < nLayers - 1; ++l)
            {
                nb_[l].Zeros();
                nw_[l].Zeros();
            }

            for (size_t k = 0; k < miniBatchSize; ++k)
            {
                for (size_t l = 0; l < nLayers - 1; ++l)
                {
                    nzv_[l].Zeros();
                    dnb_[l].Zeros();
                    dnw_[l].Zeros();
                    // as the weights are not updated, the transpose can be computed here
                    la::MatTranspose(wt_[l], vWeights[0][l]);
                }

                // first activation layer - input data
                na_[0].assign(data[*it_]);
                for (size_t j = 1; j < nLayers; ++j)
                {
                    // with MSVC and O2 / Ox the following MatMultVec with BLAS DGEMV is returning nan in nzv_[j - 1].
                    // O1 is necessary to avoid wrong results
                    la::MatMultVec(nzv_[j - 1], vWeights[0][j - 1], na_[j - 1]);
                    nzv_[j - 1] += vBiases[0][j - 1];
                    nn::ActFunc(na_[j], nzv_[j - 1], pAct);
                }
                {
                    // compute delta from the output layer
                    const size_t osize = dno_.size() - 1;
                    dno2_[osize]       = na_[osize + 1];
                    dno2_[osize] -= reference[*it_];
                    nn::ActFunc(nzv_[osize], pDAct);
                    MatHadamard(dno_[osize], dno2_[osize], nzv_[osize]);
                    dnb_[osize] = dno_[osize];
                    MatOuter(dnw_[osize], dno_[osize], na_[osize]);
                }

                for (size_t l = nLayers - 2; l > 0; --l)
                {
                    nn::ActFunc(nzv_[l - 1], pDAct);
                    // the transpose is stored at the beginning of the minibatch
                    la::MatMultVec(dno2_[l - 1], wt_[l], dno_[l]);
                    MatHadamard(dno_[l - 1], dno2_[l - 1], nzv_[l - 1]);
                    dnb_[l - 1] = dno_[l - 1];
                    MatOuter(dnw_[l - 1], dno_[l - 1], na_[l - 1]);
                }

                // update nabla
                for (size_t l = 0; l < (nLayers - 1); ++l)
                {
                    nw_[l] += dnw_[l];
                    nb_[l] += dnb_[l];
                }

                // update to the next sample
                it_++;
            }

            // update bias and weights
            for (size_t l = 0; l < (nLayers - 1); ++l)
            {
                nw_[l] *= (eta / static_cast<double>(miniBatchSize));
                vWeights[0][l] -= nw_[l];
                nb_[l] *= (eta / static_cast<double>(miniBatchSize));
                vBiases[0][l] -= nb_[l];
            }
        }

        // global training epochs is updated
        nEpochs++;
        std::cout << "Training Epoch " << i + 1 << " / " << epochs << " completed (Total " << nEpochs << ")\n";
    }
}

template <typename T>
int nn::ANN_MLP_SGD<T>::TestSGD(const std::vector<std::vector<T>>& data, const std::vector<std::vector<T>>& reference)
{

    void (*pAct)(T*, size_t) = nullptr;

    switch (act)
    {
    case SIGMOID: pAct = &sigmoid<T>; break;
    case TANH: pAct = &tanh<T>; break;
    default: throw std::invalid_argument("Unknown activation function");
    }

    (void)pAct;
    int iCorrect = 0;
    std::vector<la::Matrix<T>> na_;
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

// Explicit template instantiation
template class nn::ANN_MLP_SGD<float>;
template class nn::ANN_MLP_SGD<double>;
