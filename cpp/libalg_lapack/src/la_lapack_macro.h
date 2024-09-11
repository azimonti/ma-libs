#ifndef _LA_LAPACK_MACRO_H_88647D251BCC4411B3F3FEFC7A359C6B_
#define _LA_LAPACK_MACRO_H_88647D251BCC4411B3F3FEFC7A359C6B_

/************************/
/*  la_lapack_macro.h   */
/*    Version 1.0       */
/*     2023/06/13       */
/************************/

#define REALTYPE_DEFINE                                                                                                \
    using RealType = typename std::conditional<                                                                        \
        std::is_same<T, std::complex<double>>::value, double,                                                          \
        typename std::conditional<std::is_same<T, std::complex<float>>::value, float, T>::type>::type;

#endif
