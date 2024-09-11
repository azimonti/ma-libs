#ifndef _ANN_MLP_V1_H_689A4B173C084B718D8CE25911743572_
#define _ANN_MLP_V1_H_689A4B173C084B718D8CE25911743572_

/************************/
/*    ann_mlp_v1.h      */
/*    Version 1.0       */
/*     2023/02/26       */
/************************/

#include <mutex>
#include <random>
#include <vector>
#include "math/algebra/matrix.h"
#include "activations.h"

namespace nn
{
    template <typename T> class ANN_MLP
    {
      public:
        void PrintNetworkInfo();
        void PrintBiases();
        void PrintWeights();

        void SetName(const std::string& s) { sName = s; };

        void SetEpochs(size_t n) { nEpochs = n; };

        void UpdateEpochs(size_t n = 1) { nEpochs += n; };

        void Serialize(const std::string& fname);
        void Deserialize(const std::string& fname);

        inline size_t GetPopSize() const { return nPopSize; }

        inline size_t GetTopPerformersSize() const { return nTop; }

        inline size_t GetEpochs() const { return nEpochs; }

      protected:
        ANN_MLP();
        ANN_MLP(std::vector<size_t> size, int seed, size_t populationSize = 1, size_t topPerformersSize = 1,
                size_t activationFunction = SIGMOID);
        virtual ~ANN_MLP() = 0;

        inline T GetRandomNormal() { return normal_distribution(generator); }

        inline T GetRandomUniformReal() { return static_cast<T>(uniform_real_distribution(generator)); }

        inline int GetRandomUniformInt() { return uniform_int_distribution(generator); }

        std::vector<size_t> vSize{};
        std::vector<std::vector<la::Matrix<T>>> vBiases{};
        std::vector<std::vector<la::Matrix<T>>> vWeights{};
        size_t nLayers;
        size_t nEpochs;
        size_t nPopSize;
        size_t nTop;
        size_t act;
        int flags;
        std::mutex mtx;

      private:
        void AllocateWeightsBiases();
        std::string sName{};
        std::random_device rd{};
        std::mt19937 generator{rd()};
        std::normal_distribution<T> normal_distribution;
        std::uniform_real_distribution<> uniform_real_distribution;
        std::uniform_int_distribution<> uniform_int_distribution;
    };

} // namespace nn
#endif
