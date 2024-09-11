#ifndef _ANN_MLP_GA_V1_H_3DBF200858094AEC952A6DB4FD199ED9_
#define _ANN_MLP_GA_V1_H_3DBF200858094AEC952A6DB4FD199ED9_

/************************/
/*    ann_mlp_ga_v1.h   */
/*    Version 1.1       */
/*     2023/05/04       */
/************************/

#include "ann_mlp_v1.h"

namespace nn
{
    template <typename T> class ANN_MLP_GA : public ANN_MLP<T>
    {
        using ANN_MLP<T>::GetRandomNormal;
        using ANN_MLP<T>::GetRandomUniformReal;
        using ANN_MLP<T>::GetRandomUniformInt;
        using ANN_MLP<T>::Serialize;
        using ANN_MLP<T>::Deserialize;
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
        ANN_MLP_GA();
        ANN_MLP_GA(std::vector<size_t> size, int seed = 4041, size_t populationSize = 130,
                   size_t topPerformersSize = 10, size_t activationFunction = SIGMOID, bool bGenerateMixed = false);

        void UpdateWeightsAndBiases(const std::vector<size_t>& v_);

        void feedforward(const T* pInputs, size_t inputsSize, T* pOutputs, size_t outputsSize, size_t memberid,
                         bool singleReturn);
        void feedforward(const std::vector<T>& vInputs, std::vector<T>& vOutputs, size_t memberid, bool singleReturn);
        void TrainGA(const std::vector<std::vector<T>>& data, const std::vector<std::vector<T>>& reference,
                     size_t nGenerations, size_t BatchSize, bool shuffleTrainingData = true);
        int TestGA(const std::vector<std::vector<T>>& data, const std::vector<std::vector<T>>& reference);

        void SetMixed(bool b);
        bool GetMixed();

        void CreatePopulation(bool bKeepPrevious = true);

        void Serialize(const std::string& fname);
        void Deserialize(const std::string& fname);

      private:
        void AllocatePopulation();
        void CreatePopulationFixed(bool bKeepPrevious);
        void CreatePopulationMixed(bool bKeepPrevious);
        std::vector<std::vector<la::Matrix<T>>> vBiasesPop{};
        std::vector<std::vector<la::Matrix<T>>> vWeightsPop{};
        std::vector<std::vector<la::Matrix<T>>> vNaPop{};
    };

} // namespace nn
#endif
