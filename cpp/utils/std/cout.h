#ifndef _COUT_H_48E3527EFEBA48A3928278973B5F3C1B_
#define _COUT_H_48E3527EFEBA48A3928278973B5F3C1B_

/************************/
/*       cout.h         */
/*    Version 1.0       */
/*     2022/07/19       */
/************************/

#if defined(COUTEXT)
#include <iostream>

// clang-format off
#define COUNT_N(_1, _2, _3, _4, _5, _6, _7, _8, \
         _9, _10, _11, _12, _13, _14, _15, _16, \
        _17, _18, _19, _20, _21, _22, _23, _24, \
        _25, _26, _27, _28, _29, _30, _31, _32, \
        _33, N, ...) N
// clang-format on
#define COUNT_N_1(...) MSVC_BUG(COUNT_N, (__VA_ARGS__))
#define COUNT(...)                                                                                                     \
    COUNT_N_1(__VA_ARGS__, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
              10, 9, 8, 7, 6, 5, 4, 3, 2, 1, ...)

#define MSVC_BUG(MACRO, ARGS)   MACRO ARGS
#define MSVC_BUG1(MACRO, ARGS)  MACRO ARGS
#define MSVC_BUG2(MACRO, ARGS)  MACRO ARGS
#define MSVC_BUG3(MACRO, ARGS)  MACRO ARGS
#define MSVC_BUG4(MACRO, ARGS)  MACRO ARGS
#define MSVC_BUG5(MACRO, ARGS)  MACRO ARGS
#define MSVC_BUG6(MACRO, ARGS)  MACRO ARGS
#define MSVC_BUG7(MACRO, ARGS)  MACRO ARGS
#define MSVC_BUG8(MACRO, ARGS)  MACRO ARGS
#define MSVC_BUG9(MACRO, ARGS)  MACRO ARGS
#define MSVC_BUG10(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG11(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG12(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG13(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG14(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG15(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG16(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG17(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG18(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG19(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG20(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG21(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG22(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG23(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG24(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG25(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG26(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG27(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG28(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG29(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG30(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG31(MACRO, ARGS) MACRO ARGS
#define MSVC_BUG32(MACRO, ARGS) MACRO ARGS

#define MCOUT_1(_1)             << _1
#define MCOUT_2(_1, ...)        << _1 << " " MSVC_BUG1(MCOUT_1, (__VA_ARGS__))
#define MCOUT_3(_1, ...)        << _1 << " " MSVC_BUG2(MCOUT_2, (__VA_ARGS__))
#define MCOUT_4(_1, ...)        << _1 << " " MSVC_BUG3(MCOUT_3, (__VA_ARGS__))
#define MCOUT_5(_1, ...)        << _1 << " " MSVC_BUG4(MCOUT_4, (__VA_ARGS__))
#define MCOUT_6(_1, ...)        << _1 << " " MSVC_BUG5(MCOUT_5, (__VA_ARGS__))
#define MCOUT_7(_1, ...)        << _1 << " " MSVC_BUG6(MCOUT_6, (__VA_ARGS__))
#define MCOUT_8(_1, ...)        << _1 << " " MSVC_BUG7(MCOUT_7, (__VA_ARGS__))
#define MCOUT_9(_1, ...)        << _1 << " " MSVC_BUG8(MCOUT_8, (__VA_ARGS__))
#define MCOUT_10(_1, ...)       << _1 << " " MSVC_BUG9(MCOUT_9, (__VA_ARGS__))
#define MCOUT_11(_1, ...)       << _1 << " " MSVC_BUG10(MCOUT_10, (__VA_ARGS__))
#define MCOUT_12(_1, ...)       << _1 << " " MSVC_BUG11(MCOUT_11, (__VA_ARGS__))
#define MCOUT_13(_1, ...)       << _1 << " " MSVC_BUG12(MCOUT_12, (__VA_ARGS__))
#define MCOUT_14(_1, ...)       << _1 << " " MSVC_BUG13(MCOUT_13, (__VA_ARGS__))
#define MCOUT_15(_1, ...)       << _1 << " " MSVC_BUG14(MCOUT_14, (__VA_ARGS__))
#define MCOUT_16(_1, ...)       << _1 << " " MSVC_BUG15(MCOUT_15, (__VA_ARGS__))
#define MCOUT_17(_1, ...)       << _1 << " " MSVC_BUG16(MCOUT_16, (__VA_ARGS__))
#define MCOUT_18(_1, ...)       << _1 << " " MSVC_BUG17(MCOUT_17, (__VA_ARGS__))
#define MCOUT_19(_1, ...)       << _1 << " " MSVC_BUG18(MCOUT_18, (__VA_ARGS__))
#define MCOUT_20(_1, ...)       << _1 << " " MSVC_BUG19(MCOUT_19, (__VA_ARGS__))
#define MCOUT_21(_1, ...)       << _1 << " " MSVC_BUG20(MCOUT_20, (__VA_ARGS__))
#define MCOUT_22(_1, ...)       << _1 << " " MSVC_BUG21(MCOUT_21, (__VA_ARGS__))
#define MCOUT_23(_1, ...)       << _1 << " " MSVC_BUG22(MCOUT_22, (__VA_ARGS__))
#define MCOUT_24(_1, ...)       << _1 << " " MSVC_BUG23(MCOUT_23, (__VA_ARGS__))
#define MCOUT_25(_1, ...)       << _1 << " " MSVC_BUG24(MCOUT_24, (__VA_ARGS__))
#define MCOUT_26(_1, ...)       << _1 << " " MSVC_BUG25(MCOUT_25, (__VA_ARGS__))
#define MCOUT_27(_1, ...)       << _1 << " " MSVC_BUG26(MCOUT_26, (__VA_ARGS__))
#define MCOUT_28(_1, ...)       << _1 << " " MSVC_BUG27(MCOUT_27, (__VA_ARGS__))
#define MCOUT_29(_1, ...)       << _1 << " " MSVC_BUG28(MCOUT_28, (__VA_ARGS__))
#define MCOUT_30(_1, ...)       << _1 << " " MSVC_BUG29(MCOUT_29, (__VA_ARGS__))
#define MCOUT_31(_1, ...)       << _1 << " " MSVC_BUG30(MCOUT_30, (__VA_ARGS__))
#define MCOUT_32(_1, ...)       << _1 << " " MSVC_BUG31(MCOUT_31, (__VA_ARGS__))
#define MCOUT_33(_1, ...)       << _1 << " " MSVC_BUG32(MCOUT_32, (__VA_ARGS__))

#define MCOUT_INC(X, Y)         X##Y
#define MCOUT(X, Y)             MCOUT_INC(X, Y)
#define COUT_MACRO(MACRO, ...)  std::cout MSVC_BUG(MCOUT, (MACRO, COUNT(__VA_ARGS__)))(__VA_ARGS__) << std::endl;
#define COUT_IMPL(...)          COUT_MACRO(MCOUT_, __VA_ARGS__)

#define MCOUTM_1(_1)            << #_1 << ": " << _1
#define MCOUTM_2(_1, ...)       << #_1 << ": " << _1 << " # " MSVC_BUG1(MCOUTM_1, (__VA_ARGS__))
#define MCOUTM_3(_1, ...)       << #_1 << ": " << _1 << " # " MSVC_BUG2(MCOUTM_2, (__VA_ARGS__))
#define MCOUTM_4(_1, ...)       << #_1 << ": " << _1 << " # " MSVC_BUG3(MCOUTM_3, (__VA_ARGS__))
#define MCOUTM_5(_1, ...)       << #_1 << ": " << _1 << " # " MSVC_BUG4(MCOUTM_4, (__VA_ARGS__))
#define MCOUTM_6(_1, ...)       << #_1 << ": " << _1 << " # " MSVC_BUG5(MCOUTM_5, (__VA_ARGS__))
#define MCOUTM_7(_1, ...)       << #_1 << ": " << _1 << " # " MSVC_BUG6(MCOUTM_6, (__VA_ARGS__))
#define MCOUTM_8(_1, ...)       << #_1 << ": " << _1 << " # " MSVC_BUG7(MCOUTM_7, (__VA_ARGS__))
#define MCOUTM_9(_1, ...)       << #_1 << ": " << _1 << " # " MSVC_BUG8(MCOUTM_8, (__VA_ARGS__))
#define MCOUTM_10(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG9(MCOUTM_9, (__VA_ARGS__))
#define MCOUTM_11(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG10(MCOUTM_10, (__VA_ARGS__))
#define MCOUTM_12(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG11(MCOUTM_11, (__VA_ARGS__))
#define MCOUTM_13(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG12(MCOUTM_12, (__VA_ARGS__))
#define MCOUTM_14(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG13(MCOUTM_13, (__VA_ARGS__))
#define MCOUTM_15(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG14(MCOUTM_14, (__VA_ARGS__))
#define MCOUTM_16(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG15(MCOUTM_15, (__VA_ARGS__))
#define MCOUTM_17(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG16(MCOUTM_16, (__VA_ARGS__))
#define MCOUTM_18(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG17(MCOUTM_17, (__VA_ARGS__))
#define MCOUTM_19(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG18(MCOUTM_18, (__VA_ARGS__))
#define MCOUTM_20(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG19(MCOUTM_19, (__VA_ARGS__))
#define MCOUTM_21(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG20(MCOUTM_20, (__VA_ARGS__))
#define MCOUTM_22(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG21(MCOUTM_21, (__VA_ARGS__))
#define MCOUTM_23(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG22(MCOUTM_22, (__VA_ARGS__))
#define MCOUTM_24(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG23(MCOUTM_23, (__VA_ARGS__))
#define MCOUTM_25(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG24(MCOUTM_24, (__VA_ARGS__))
#define MCOUTM_26(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG25(MCOUTM_25, (__VA_ARGS__))
#define MCOUTM_27(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG26(MCOUTM_26, (__VA_ARGS__))
#define MCOUTM_28(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG27(MCOUTM_27, (__VA_ARGS__))
#define MCOUTM_29(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG28(MCOUTM_28, (__VA_ARGS__))
#define MCOUTM_30(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG29(MCOUTM_29, (__VA_ARGS__))
#define MCOUTM_31(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG30(MCOUTM_30, (__VA_ARGS__))
#define MCOUTM_32(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG31(MCOUTM_31, (__VA_ARGS__))
#define MCOUTM_33(_1, ...)      << #_1 << ": " << _1 << " # " MSVC_BUG32(MCOUTM_32, (__VA_ARGS__))

#define MCOUTM_INC(X, Y)        X##Y
#define MCOUTM(X, Y)            MCOUTM_INC(X, Y)
#define COUTM_MACRO(MACRO, ...) std::cout MSVC_BUG(MCOUTM, (MACRO, COUNT(__VA_ARGS__)))(__VA_ARGS__) << std::endl;
#define COUTM_IMPL(...)         COUTM_MACRO(MCOUTM_, __VA_ARGS__)

#define COUT(...)               COUT_IMPL(__VA_ARGS__)
#define COUTM(...)              COUTM_IMPL(__VA_ARGS__)
#ifdef DEBUG
#define COUT_D(...)  COUT_IMPL(__VA_ARGS__)
#define COUTM_D(...) COUTM_IMPL(__VA_ARGS__)
#else
#define COUT_D(...)
#define COUTM_D(...)
#endif
#else
#define COUT(...)
#define COUT_D(...)
#define COUTM(...)
#define COUTM_D(...)
#endif

#endif
