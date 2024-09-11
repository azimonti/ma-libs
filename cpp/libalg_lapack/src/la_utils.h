#ifndef _LA_UTILS_H_06260E8E7A7C4D93A44DEE9D02BC5A06_
#define _LA_UTILS_H_06260E8E7A7C4D93A44DEE9D02BC5A06_

/************************/
/*     la_utils.h       */
/*    Version 1.1       */
/*     2023/04/29       */
/************************/

#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <string>
#include "la_decomposition.h"
#include "matrix.h"
#include "matrix_operations.h"

namespace la
{
    namespace flags
    {

        enum MatrixFlags {
            ALL           = 1 << 0,
            BASIC         = 1 << 1,
            RANK          = 1 << 2,
            RREF          = 1 << 3,
            BASIS         = 1 << 4,
            BASISROW      = 1 << 5,
            NULLSPACE     = 1 << 6,
            LEFTNULLSPACE = 1 << 7,
            PIVOTCOLS     = 1 << 8,
            FREECOLS      = 1 << 9,
            PIVOTROWS     = 1 << 10,
            FREEROWS      = 1 << 11,
            ATA           = 1 << 12,
            QR            = 1 << 13,
            EIGENVALUES   = 1 << 14,
        };
    }

    template <typename S> void cout_spaces(const la::Matrix<S>& m, const size_t flags, const std::string& title)
    {
        const size_t r_ = m.GetRowsNb(), c_ = m.GetColsNb();
        if (flags & (flags::BASIC | flags::ALL)) { std::cout << title << "\n" << m; }
        if (flags & (flags::RANK | flags::ALL))
        {
            size_t ra_;
            ra_ = la::MatRank(m);
            std::cout << "Rank\n" << ra_ << "\n";
        }
        if (flags & (flags::RREF | flags::ALL))
        {
            la::Matrix<S> m1r_{r_, c_};
            la::MatRref(m1r_, m);
            std::cout << "rref\n" << m1r_;
        }
        if (flags & (flags::BASIS | flags::ALL))
        {
            la::Matrix<S> m1b_{r_, 1};
            la::MatBasis(m1b_, m);
            std::cout << "Basis\n" << m1b_;
        }
        if (flags & (flags::BASISROW | flags::ALL))
        {
            la::Matrix<S> m1br_{c_, 1};
            la::MatBasisRow(m1br_, m);
            std::cout << "Basis Row\n" << m1br_;
        }
        if (flags & (flags::NULLSPACE | flags::ALL))
        {
            la::Matrix<S> m1n_{c_, 1};
            la::MatNullspace(m1n_, m);
            std::cout << "Nullspace\n" << m1n_;
        }
        if (flags & (flags::LEFTNULLSPACE | flags::ALL))
        {
            la::Matrix<S> m1ln_{r_, 1};
            la::MatLeftNullspace(m1ln_, m);
            std::cout << "Left nullspace\n" << m1ln_;
        }
        if (flags & (flags::PIVOTCOLS | flags::ALL))
        {
            std::vector<size_t> m1pc_{la::MatPivotColsNb(m)};
            std::cout << "Pivot Cols\n";
            for (auto i : m1pc_) { std::cout << i << " "; }
            std::cout << "\n";
        }
        if (flags & (flags::FREECOLS | flags::ALL))
        {
            std::vector<size_t> m1fc_{la::MatFreeColsNb(m)};
            std::cout << "Free Cols\n";
            for (auto i : m1fc_) { std::cout << i << " "; }
            std::cout << "\n";
        }
        if (flags & (flags::PIVOTROWS | flags::ALL))
        {
            std::vector<size_t> m1pr_{la::MatPivotRowsNb(m)};
            std::cout << "Pivot Rows\n";
            for (auto i : m1pr_) { std::cout << i << " "; }
            std::cout << "\n";
        }
        if (flags & (flags::FREEROWS | flags::ALL))
        {
            std::vector<size_t> m1fr_{la::MatFreeRowsNb(m)};
            std::cout << "Free Rows\n";
            for (auto i : m1fr_) { std::cout << i << " "; }
            std::cout << "\n";
        }
        if (flags & (flags::ATA | flags::ALL))
        {
            la::Matrix<S> m1a_{c_, c_};
            la::MatAtA(m1a_, m);
            std::cout << "A^T A\n" << m1a_;
        }
        if (flags & (flags::QR | flags::ALL))
        {
            la::Matrix<S> m1q_{r_, c_}, m1r_{c_, c_}, m1qr_{r_, c_};
            la::MatQR(m1q_, m1r_, m);
            la::MatMult(m1qr_, m1q_, m1r_);
            std::cout << "Q\n" << m1q_;
            std::cout << "R\n" << m1r_;
            std::cout << "Q R\n" << m1qr_;
        }
        if (flags & (flags::EIGENVALUES | flags::ALL))
        {
            std::vector<S> m1e_{};
            la::MatEigen(m1e_, m, false);
            std::cout << "Eigenvalues\n";
            for (auto i : m1e_) { std::cout << i << "\n"; }
        }
    }

    template <typename S> Matrix<S>& MatReadFile(Matrix<S>& res, const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) { throw std::runtime_error("File not found"); }
        size_t r_, c_;
        file >> r_ >> c_;
        res = la::Matrix<S>(r_, c_);
        for (size_t i = 0; i < r_; ++i)
        {
            for (size_t j = 0; j < c_; ++j) { file >> res(i, j); }
        }
        return res;
    }

    template <typename S> Matrix<S> MatReadFile(const std::string& filename)
    {
        la::Matrix<S> res;
        return read_matrix(res, filename);
    }
} // namespace la

namespace MFLAGS = la::flags;

#endif
