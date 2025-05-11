/************************/
/* ann_mlp_sgd_v1.cpp   */
/*    Version 2.1       */
/*     2025/05/10       */
/************************/

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <assert.h>
#include "math/algebra/matrix_operations.h"
#include "ann_mlp_sgd_v1.h"

template <typename T> nn::ANN_MLP_SGD<T>::ANN_MLP_SGD() : ANN_MLP<T>() {}

template <typename T>
nn::ANN_MLP_SGD<T>::ANN_MLP_SGD(std::vector<size_t> size, int seed, size_t activationFunction)
    : ANN_MLP<T>(size, seed, 1, 1, activationFunction)
{
    initialize_sgd_buffers();
}

template <typename T> void nn::ANN_MLP_SGD<T>::initialize_sgd_buffers()
{

    current_na_.clear();
    current_nzv_.clear();

    current_na_.push_back(la::Matrix<T>(this->vSize[0], 1));

    for (size_t i = 1; i < this->nLayers; ++i)
    {
        const size_t nRows = this->vSize[i];
        current_na_.push_back(la::Matrix<T>{nRows, 1});
        current_nzv_.push_back(la::Matrix<T>{nRows, 1});
    }
}

template <typename T>
const std::vector<la::Matrix<T>>& nn::ANN_MLP_SGD<T>::feedforward_store_activations(const std::vector<T>& input)
{
    if (this->vSize.empty()) { throw std::runtime_error("Network size not initialized for feedforward."); }
    if (input.size() != this->vSize[0])
    {
        throw std::invalid_argument("Input size mismatch with network's input layer size.");
    }

    // Ensure buffers are ready, especially if network size changed (e.g. after Deserialize)
    if (current_na_.empty() || current_na_[0].GetRowsNb() != this->vSize[0]) { initialize_sgd_buffers(); }

    void (*pAct)(T*, const T*, size_t) = nullptr;
    switch (this->act)
    {
    case SIGMOID: pAct = &sigmoid<T>; break;
    case TANH: pAct = &tanh<T>; break;
    default: throw std::invalid_argument("Unknown activation function in feedforward_store_activations");
    }

    current_na_[0].assign(input.data(), input.size());

    for (size_t l = 1; l < this->nLayers; ++l)
    {
        // current_nzv_ stores z for layer l (index l-1 in vector)
        // current_na_ stores a for layer l (index l in vector)
        la::MatMultVec(current_nzv_[l - 1], this->vWeights[0][l - 1], current_na_[l - 1]);
        current_nzv_[l - 1] += this->vBiases[0][l - 1];
        nn::ActFunc(current_na_[l], current_nzv_[l - 1], pAct);
    }
    return current_na_;
}

template <typename T>
void nn::ANN_MLP_SGD<T>::backpropagate_calculate_gradients(
    const std::vector<T>& target_output,
    std::vector<la::Matrix<T>>& out_nabla_b, // Gradient for biases for this sample
    std::vector<la::Matrix<T>>& out_nabla_w) // Gradient for weights for this sample
{
    if (this->vSize.empty()) { throw std::runtime_error("Network size not initialized for backpropagation."); }
    if (target_output.size() != this->vSize.back())
    {
        throw std::invalid_argument("Target output size mismatch with network's output layer size.");
    }
    if (current_na_.empty() || current_nzv_.empty())
    {
        throw std::runtime_error("Feedforward must be called before backpropagation to populate activations.");
    }

    // Ensure out_nabla_b and out_nabla_w are correctly sized (like nb_ and nw_ in original TrainSGD)
    // These are gradients for ONE sample, to be accumulated by the caller.
    out_nabla_b.resize(this->nLayers - 1);
    out_nabla_w.resize(this->nLayers - 1);
    for (size_t l = 0; l < this->nLayers - 1; ++l)
    {
        out_nabla_b[l].Resize(this->vSize[l + 1], 1);
        out_nabla_b[l].Zeros();
        out_nabla_w[l].Resize(this->vSize[l + 1], this->vSize[l]);
        out_nabla_w[l].Zeros();
    }

    void (*pDAct)(T*, size_t) = nullptr; // Derivative of activation function applied to z
    switch (this->act)
    {
    case SIGMOID: pDAct = &dsigmoid<T>; break;
    case TANH: pDAct = &dtanh<T>; break;
    default: throw std::invalid_argument("Unknown activation function for derivative in backprop");
    }

    // Temporary matrices for deltas and calculations
    // delta (dno_) is error at each layer: (L-1), (L-2) ... 1
    std::vector<la::Matrix<T>> delta(this->nLayers - 1);
    for (size_t l = 0; l < this->nLayers - 1; ++l) delta[l].Resize(this->vSize[l + 1], 1);

    // Calculate delta for the output layer (L-1)
    // delta_L = (a_L - y) * sigma_prime(z_L)
    // delta.back() refers to delta for the output layer, which is delta[this->nLayers-2]
    la::Matrix<T>& actual_delta_output_layer_ref = delta.back(); // Get a reference

    la::Matrix<T> output_error                   = current_na_.back(); // a_L (activation of output layer)
    output_error -= target_output;                                     // (a_L - y)

    la::Matrix<T> sp_L = current_nzv_.back(); // z_L (z-value of output layer)
    nn::ActFunc(sp_L, pDAct);                 // sigma_prime(z_L) in-place on sp_L copy

    // Calculate delta_L and store it directly into the delta vector
    MatHadamard(actual_delta_output_layer_ref, output_error, sp_L);

    // Nabla for biases at output layer is just delta_L
    out_nabla_b.back() = actual_delta_output_layer_ref;
    // Nabla for weights at output layer is delta_L * (a_{L-1})^T
    // current_na_[this->nLayers - 2] is a_{L-1} (activation of the layer before output)
    MatOuter(out_nabla_w.back(), actual_delta_output_layer_ref, current_na_[this->nLayers - 2]);

    // Backpropagate delta to hidden layers
    // Loop for conceptual layers l from L-2 down to 1.
    // l_one_based is the conceptual layer number (1 to L-2).
    // Array indices will be l_one_based-1 for 0-indexed arrays starting at conceptual layer 1 (like current_nzv_,
    // delta, out_nabla_b/w) or l_one_based for 0-indexed arrays starting at conceptual layer 0 (like vWeights[0], where
    // vWeights[0][k] connects layer k to k+1).
    if (this->nLayers >= 3)
    { // Only loop if there's at least one hidden layer to backprop to.
        for (size_t l_one_based = this->nLayers - 2; l_one_based >= 1; --l_one_based)
        {
            // Current layer being processed is conceptual layer 'l_one_based'.
            // Its z-value is current_nzv_[l_one_based-1].
            // Its delta will be delta[l_one_based-1].
            // Weights connecting it to next layer (l_one_based+1) are vWeights[0][l_one_based].
            // Delta from next layer (l_one_based+1) is delta[l_one_based].
            // Activation from previous layer (l_one_based-1) is current_na_[l_one_based-1].

            la::Matrix<T> z_current_layer = current_nzv_[l_one_based - 1];
            la::Matrix<T> sp_current_layer(z_current_layer.GetRowsNb(), z_current_layer.GetColsNb());
            sp_current_layer.assign(z_current_layer.data());
            nn::ActFunc(sp_current_layer, pDAct); // sigma_prime(z_l_one_based)

            // W_next_T is (W_{l_one_based+1})^T
            la::Matrix<T>& W_next_layer = this->vWeights[0][l_one_based]; // Weights from l_one_based to l_one_based+1
            la::Matrix<T> W_next_T(W_next_layer.GetColsNb(), W_next_layer.GetRowsNb());
            la::MatTranspose(W_next_T, W_next_layer);

            // propagated_error = (W_{l_one_based+1})^T * delta_{l_one_based+1}
            // delta[l_one_based] is delta for layer l_one_based+1
            la::Matrix<T> propagated_error(W_next_T.GetRowsNb(), delta[l_one_based].GetColsNb());
            la::MatMultVec(propagated_error, W_next_T, delta[l_one_based]);

            // delta_current_layer = propagated_error * sp_current_layer
            MatHadamard(delta[l_one_based - 1], propagated_error, sp_current_layer);

            out_nabla_b[l_one_based - 1] = delta[l_one_based - 1];
            // current_na_[l_one_based-1] is activation of layer l_one_based-1 (previous to current)
            MatOuter(out_nabla_w[l_one_based - 1], delta[l_one_based - 1], current_na_[l_one_based - 1]);
        }
    }
}

template <typename T>
void nn::ANN_MLP_SGD<T>::apply_gradients(
    const std::vector<la::Matrix<T>>& sum_nabla_b, // Sum of bias gradients over mini-batch
    const std::vector<la::Matrix<T>>& sum_nabla_w, // Sum of weight gradients over mini-batch
    double eta_over_minibatch_size)
{
    if (sum_nabla_b.size() != this->nLayers - 1 || sum_nabla_w.size() != this->nLayers - 1)
    {
        throw std::invalid_argument("Gradient vectors size mismatch with network layers.");
    }

    for (size_t l = 0; l < (this->nLayers - 1); ++l)
    {
        // vWeights[0][l] -= (eta/miniBatchSize) * sum_nabla_w[l]
        la::Matrix<T> scaled_nw = sum_nabla_w[l];
        scaled_nw *= static_cast<T>(eta_over_minibatch_size);
        this->vWeights[0][l] -= scaled_nw;

        // vBiases[0][l] -= (eta/miniBatchSize) * sum_nabla_b[l]
        la::Matrix<T> scaled_nb = sum_nabla_b[l];
        scaled_nb *= static_cast<T>(eta_over_minibatch_size);
        this->vBiases[0][l] -= scaled_nb;
    }
}

template <typename T> size_t nn::ANN_MLP_SGD<T>::predict(const std::vector<T>& input)
{
    // Perform feedforward (this will update internal current_na_)
    feedforward_store_activations(input);

    // Get the output layer activations (last element in current_na_)
    const std::vector<T>& output_activations = current_na_.back().data();

    // Find the index of the maximum element
    auto max_it                              = std::max_element(output_activations.begin(), output_activations.end());
    return static_cast<size_t>(std::distance(output_activations.begin(), max_it));
}

// Explicit template instantiation
template class nn::ANN_MLP_SGD<float>;
template class nn::ANN_MLP_SGD<double>;
