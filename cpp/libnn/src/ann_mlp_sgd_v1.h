#ifndef _ANN_MLP_SGD_V1_H_77B00AA95A964AE289411164A9035EC9_
#define _ANN_MLP_SGD_V1_H_77B00AA95A964AE289411164A9035EC9_

/************************/
/*  ann_mlp_sgd_v1.h    */
/*    Version 2.0       */
/*     2023/04/12       */
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
        using ANN_MLP<T>::flags;
        using ANN_MLP<T>::mtx;

      public:
        ANN_MLP_SGD();
        ANN_MLP_SGD(std::vector<size_t> size, int seed = 7419, size_t activationFunction = SIGMOID);
        void TrainSGD(const std::vector<std::vector<T>>& data, const std::vector<std::vector<T>>& reference,
                      size_t epochs, size_t miniBatchSize, double eta);

        int TestSGD(const std::vector<std::vector<T>>& data, const std::vector<std::vector<T>>& reference);

      private:
    };

} // namespace nn
#endif
