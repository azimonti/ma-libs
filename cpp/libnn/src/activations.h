#ifndef _ACTIVATIONS_H_1238E51D438B45AEA583461737C65341_
#define _ACTIVATIONS_H_1238E51D438B45AEA583461737C65341_

/************************/
/*    activations.h     */
/*    Version 2.0       */
/*     2023/03/27       */
/************************/

#include "math/algebra/matrix.h"

#define T_C(x) static_cast<T>(x)

namespace nn
{
    enum ACT : size_t { SIGMOID = 0, TANH };

    template <typename S> inline la::Matrix<S>& ActFunc(la::Matrix<S>& res, void (*funcptr)(S*, size_t))
    {
        funcptr(res.data().data(), res.size());
        return res;
    }

    template <typename S> inline la::Matrix<S>& ActFunc(la::Matrix<S>& res, S (*funcptr)(S))
    {
        std::vector<S>& v_ = res.data();
        for (size_t i = 0; i < res.size(); ++i) v_[i] = funcptr(v_[i]);
        return res;
    }

    template <typename S>
    inline la::Matrix<S>& ActFunc(la::Matrix<S>& res, const la::Matrix<S>& a, void (*funcptr)(S*, const S*, size_t))
    {
        assert(res.size() == a.size());
        funcptr(res.data().data(), a.data().data(), a.size());
        return res;
    }

    template <typename S> inline la::Matrix<S>& ActFunc(la::Matrix<S>& res, const la::Matrix<S>& a, S (*funcptr)(S))
    {
        assert(res.size() == a.size());
        for (size_t i = 0; i < res.size(); ++i) res.data()[i] = funcptr(a.data()[i]);
        return res;
    }

    //**********************
    //      sigmoid
    //**********************
    template <typename T> inline T sigmoid(T x)
    {
        return T_C(1) / (T_C(1) + exp(-x));
    }

    template <typename T> inline void sigmoid(T* arr, const size_t size)
    {
        for (size_t i = 0; i < size; ++i) arr[i] = T_C(1) / (T_C(1) + exp(-arr[i]));
    }

    template <typename T> inline void sigmoid(T* res, const T* arr, const size_t size)
    {
        for (size_t i = 0; i < size; ++i) res[i] = T_C(1) / (T_C(1) + exp(-arr[i]));
    }

    template <typename T> inline T dsigmoid(T x)
    {
        return sigmoid(x) * (T_C(1) - sigmoid(x));
    }

    template <typename T> inline void dsigmoid(T* arr, const size_t size)
    {
        for (size_t i = 0; i < size; ++i) arr[i] = dsigmoid(arr[i]);
    }

    template <typename T> inline void dsigmoid(T* res, const T* arr, const size_t size)
    {
        for (size_t i = 0; i < size; ++i) res[i] = dsigmoid(arr[i]);
    }

    //**********************
    //     tanh
    //**********************
    template <typename T> inline T tanh(T x)
    {
        return std::tanh(x);
    }

    template <typename T> inline void tanh(T* arr, const size_t size)
    {
        for (size_t i = 0; i < size; ++i) arr[i] = tanh(arr[i]);
    }

    template <typename T> inline void tanh(T* res, const T* arr, const size_t size)
    {
        for (size_t i = 0; i < size; ++i) res[i] = tanh(arr[i]);
    }

    template <typename T> inline T dtanh(T x)
    {
        T th = tanh(x);
        return T_C(1) - th * th;
    }

    template <typename T> inline void dtanh(T* arr, const size_t size)
    {
        for (size_t i = 0; i < size; ++i) arr[i] = dtanh(arr[i]);
    }

    template <typename T> inline void dtanh(T* res, const T* arr, const size_t size)
    {
        for (size_t i = 0; i < size; ++i) res[i] = dtanh(arr[i]);
    }

#undef T_C

} // namespace nn
#endif
