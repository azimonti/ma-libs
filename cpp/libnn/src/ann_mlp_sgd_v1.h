#ifndef _ANN_MLP_SGD_V1_H_77B00AA95A964AE289411164A9035EC9_
#define _ANN_MLP_SGD_V1_H_77B00AA95A964AE289411164A9035EC9_

/************************/
/*  ann_mlp_sgd_v1.h    */
/*    Version 2.0       */
/*     2025/05/11       */
/************************/

#include <iterator>
#include <vector>
#include "ann_mlp_v1.h"

namespace nn
{
    template <typename T> class ANN_MLP_SGD : public ANN_MLP<T>
    {
        using ANN_MLP<T>::vSize;
        using ANN_MLP<T>::vBiases;
        using ANN_MLP<T>::vWeights;
        using ANN_MLP<T>::nLayers;
        using ANN_MLP<T>::nEpochs;
        using ANN_MLP<T>::nPopSize;
        using ANN_MLP<T>::nTop;
        using ANN_MLP<T>::act;
        using ANN_MLP<T>::mtx;

      public:
        ANN_MLP_SGD();
        ANN_MLP_SGD(std::vector<size_t> size, int seed = 7419, size_t activationFunction = SIGMOID);

        // Performs feedforward and returns all layer activations.
        // Also stores z-vectors internally for backpropagation.
        const std::vector<la::Matrix<T>>& feedforward_store_activations(const std::vector<T>& input);

        // Computes gradients for a single sample based on target output and activations from feedforward.
        // nabla_b and nabla_w are output parameters for bias and weight gradients.
        void backpropagate_calculate_gradients(const std::vector<T>& target_output,
                                               std::vector<la::Matrix<T>>& out_nabla_b,
                                               std::vector<la::Matrix<T>>& out_nabla_w);

        // Updates the network's weights and biases using the provided accumulated gradients.
        void apply_gradients(const std::vector<la::Matrix<T>>& sum_nabla_b,
                             const std::vector<la::Matrix<T>>& sum_nabla_w, double eta_over_minibatch_size);

        // Simplified test function (just feedforward and get max index)
        size_t predict(const std::vector<T>& input);

      private:
        // Internal state for feedforward and backpropagation
        std::vector<la::Matrix<T>> current_na_{};  // Activations (a) per layer for current sample
        std::vector<la::Matrix<T>> current_nzv_{}; // Z-vectors (z) per layer for current sample

        // Helper to initialize/resize internal buffers if needed
        void initialize_sgd_buffers();
    };

} // namespace nn
#endif
