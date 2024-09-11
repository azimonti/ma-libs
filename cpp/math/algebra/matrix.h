#ifndef _MATRiX_H_35E68B297B884F36B7A86B87907C7479_
#define _MATRiX_H_35E68B297B884F36B7A86B87907C7479_

/************************/
/*      matrix.h        */
/*    Version 3.0       */
/*     2023/05/23       */
/************************/

#include <algorithm>
#include <cassert>
#include <complex>
#include <ostream>
#include <type_traits>
#include <typeinfo>
#include <vector>

// clang-format off
#define T_C(x)         static_cast<T>(x)
#define PTRDIFF_C(x)   static_cast<ptrdiff_t>(x)

namespace la
{
    template <typename T> class Matrix
    {
      public:
        class ColumnProxy {
            public:
                inline ColumnProxy(std::vector<T>& data, size_t i, size_t rows) : data_(data), colIndex_(i), rows_(rows) {}
                inline ColumnProxy(const std::vector<T>& data, size_t i, size_t rows) : data_(const_cast<std::vector<T>&>(data)), colIndex_(i), rows_(rows) {}

                inline       T& operator[](size_t j)       { return data_[colIndex_ + j * rows_]; }
                inline const T& operator[](size_t j) const { return data_[colIndex_ + j * rows_]; }

            private:
                std::vector<T>& data_;
                const size_t colIndex_;
                size_t rows_;
        };

      public:
        Matrix() : rows_(0), cols_(0), size_(0) {}
        Matrix(size_t rows, size_t cols=1) : rows_(rows), cols_(cols), size_(rows*cols_), data_(rows*cols) {}
        Matrix(const std::vector<T>& data, size_t rows, size_t cols) : rows_(rows), cols_(cols), size_(rows*cols_) { assert(data.size() == size_); data_.resize(rows*cols); for (size_t i = 0; i < rows_; ++i) for (size_t j = 0; j < cols_; ++j) data_[j*rows_ + i] = data[i*cols_ + j]; }
        Matrix(const std::vector<T>& data, bool bColVector=true) : size_(data.size()), data_(data) { bColVector ? (rows_ = size_, cols_ = 1) : (rows_ = 1, cols_ = size_); }

        // Copy constructor
        Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_), size_(other.size_), data_(other.data_) {}
        // Move constructor
        Matrix(Matrix&& other) : rows_(other.rows_), cols_(other.cols_), size_(other.size_), data_(std::move(other.data_)) {}
        // Copy assignment operator
        Matrix& operator=(const Matrix& other) { rows_ = other.rows_; cols_ = other.cols_; size_ = other.size_; data_ = other.data_; return *this; }
        // Move assignment operator
        Matrix& operator=(Matrix&& other) { rows_ = other.rows_; cols_ = other.cols_; size_ = other.size_; data_ = std::move(other.data_); return *this; }

        inline size_t GetRowsNb() const { return rows_; }
        inline size_t GetColsNb() const { return cols_; }
        inline size_t size()     const { return size_; }
        inline std::vector<T>&       data()       { return data_; }
        inline const std::vector<T>& data() const { return data_; }

        // Scalar Operations
        template <typename S> inline Matrix operator *=( const S& rval ) { for(size_t i=0;i<size_;++i) data_[i]*=T_C(rval); return *this; }
        template <typename S> inline Matrix operator /=( const S& rval ) { for(size_t i=0;i<size_;++i) data_[i]/=T_C(rval); return *this; }

        inline friend Matrix operator * (const Matrix& lval, const T& rval) { Matrix m_{lval.rows_, lval.cols_}; for(size_t i=0;i<m_.size_;++i) m_.data_[i]=lval.data_[i]*rval; return m_; }
        inline friend Matrix operator * (const T& lval, const Matrix& rval) { Matrix m_{rval.rows_, rval.cols_}; for(size_t i=0;i<m_.size_;++i) m_.data_[i]=rval.data_[i]*lval; return m_; }
        inline friend Matrix operator / (const Matrix& lval, const T& rval) { Matrix m_{lval.rows_, lval.cols_}; for(size_t i=0;i<m_.size_;++i) m_.data_[i]=lval.data_[i]/rval; return m_; }
        inline friend Matrix operator / (const T& lval, const Matrix& rval) { Matrix m_{rval.rows_, rval.cols_}; for(size_t i=0;i<m_.size_;++i) m_.data_[i]=rval.data_[i]/lval; return m_; }

        // Matrix operations
        inline Matrix operator + (const Matrix &rval) const { assert(rval.size_==size_); Matrix m_{rows_,cols_}; for(size_t i=0;i<m_.size_;++i) m_.data_[i]=data_[i]+rval.data_[i]; return m_; }
        inline Matrix operator +=(const Matrix &rval) { assert(rval.size_==size_); for(size_t i=0;i<size_;++i) data_[i]+=rval.data_[i]; return *this; }
        template <typename S> inline Matrix operator +=(const std::vector<S> &rval) { assert(rval.size()==size_); for(size_t i=0; i<size_; ++i) data_[i]+=T_C(rval[i]); return *this; }
        inline Matrix operator - (const Matrix &rval) const { assert(rval.size_==size_); Matrix m_{rows_,cols_}; for(size_t i=0;i<m_.size_;++i) m_.data_[i]=data_[i]-rval.data_[i]; return m_; }
        inline Matrix operator -=(const Matrix &rval) { assert(rval.size_==size_); for(size_t i=0;i<size_;++i) data_[i]-=rval.data_[i]; return *this; }
        template <typename S> inline Matrix operator -=(const std::vector<S> &rval)  { assert(rval.size()==size_); for(size_t i=0;i<size_;++i) data_[i]-=T_C(rval[i]); return *this; }

        inline ColumnProxy       operator[](size_t i)                 { return ColumnProxy(data_, i, rows_); }
        inline const ColumnProxy operator[](size_t i)           const { return ColumnProxy(data_, i, rows_); }

        inline T&                operator()(size_t i, size_t j)       { return data_[j*rows_+i]; }
        inline const T&          operator()(size_t i, size_t j) const { return data_[j*rows_+i]; }

        inline Matrix& assign(const std::vector<T>& other) { data_ = other; return *this; }
        inline Matrix& assign(const T* other, size_t otherSize) { assert(otherSize == size_); (void)otherSize; for(size_t i=0;i<size_;++i) data_[i] = T_C(other[i]); return *this; }

        inline Matrix& assignRow(size_t i, const std::vector<T>& other) { assert(other.size() == cols_); for(size_t j=0;j<cols_;++j) (*this)(i,j) = other[j]; return *this; }
        inline Matrix& assignRow(size_t i, const T* other, size_t otherSize) { assert(otherSize == size_); (void)otherSize; for(size_t j=0;j<cols_;++j) (*this)(i,j) = other[j]; return *this; }
        inline Matrix& assignRow(size_t i, const Matrix& other, size_t j) { assert(other.cols_ == cols_); for(size_t k=0;k<cols_;++k) (*this)(i,k) = other(j,k); return *this; }

        inline Matrix& assignCol(size_t j, const std::vector<T>& other) { assert(other.size() == rows_); for(size_t i=0;i<rows_;++i) (*this)(i,j) = other[i]; return *this; }
        inline Matrix& assignCol(size_t j, const T* other, size_t otherSize) { assert(otherSize == rows_); (void)otherSize; for(size_t i=0;i<rows_;++i) (*this)(i,j) = other[i]; return *this; }
        inline Matrix& assignCol(size_t i, const Matrix& other, size_t j) { assert(other.rows_ == rows_); for(size_t k=0;k<rows_;++k) (*this)(k,i) = other(k,j); return *this; }

        inline Matrix& assignRowCol(size_t i, const Matrix& other, size_t j) { assert(other.rows_ == cols_); for(size_t k=0;k<cols_;++k) (*this)(i,k) = other(k,j); return *this; }
        inline Matrix& assignColRow(size_t i, const Matrix& other, size_t j) { assert(other.cols_ == rows_); for(size_t k=0;k<rows_;++k) (*this)(k,i) = other(j,k); return *this; }

        inline Matrix& InsertRow(size_t i){
            return InsertRow(i, std::vector<T>(cols_, T_C(0.0)));
        }

        inline Matrix& InsertRow(size_t i, const std::vector<T>& data){
            assert(i >=0 && i<=rows_); assert(data.size()==cols_);
            ++rows_;
            size_ = rows_*cols_;
            const size_t rowso_ = rows_ - 1;
            data_.resize(rows_*cols_);
            for(size_t j=cols_;j>0;--j)
            {
                std::move(data_.begin()+PTRDIFF_C(i+(j-1)*rowso_),data_.begin()+PTRDIFF_C(j*rowso_),
                        data_.begin()+PTRDIFF_C(i+1+(j-1)*rows_));

                std::move(data_.begin()+PTRDIFF_C((j-1)*rowso_),data_.begin()+PTRDIFF_C(i+(j-1)*rowso_),
                        data_.begin()+PTRDIFF_C((j-1)*rows_));
            }
            for(size_t j=0;j<cols_;j++)
                (*this)(i,j) = data[j];
            return *this;
        }

        inline Matrix& InsertCol(size_t j){
            return InsertCol(j, std::vector<T>(rows_, T_C(0.0)));
        }
        inline Matrix& InsertCol(size_t j, const std::vector<T>& data){
            assert(j >=0 && j<=cols_); assert(data.size()==rows_);
            data_.insert(data_.begin()+PTRDIFF_C(j*rows_), data.begin(), data.end());
            ++cols_;
            size_ = data_.size();
            assert (size_==rows_*cols_);
            return *this;
        }

        inline Matrix& EraseRow(size_t i){
            assert(i >=0 && i<rows_);
            --rows_;
            size_ = rows_*cols_;
            const size_t rowso_ = rows_ + 1;
            for(size_t j=0;j<cols_;++j)
            {

                std::move(data_.begin()+PTRDIFF_C(j*rowso_),data_.begin()+PTRDIFF_C(i+j*rowso_),
                        data_.begin()+PTRDIFF_C(j*rows_));
                std::move(data_.begin()+PTRDIFF_C(i+1+j*rowso_),data_.begin()+PTRDIFF_C((j+1)*rowso_),
                        data_.begin()+PTRDIFF_C(i+j*rows_));
            }
            data_.resize(rows_*cols_);
            return *this;
        }

        inline Matrix& EraseCol(size_t j){
            assert(j >=0 && j<cols_);
            data_.erase(data_.begin()+PTRDIFF_C(j*rows_),data_.begin()+PTRDIFF_C((j+1)*rows_));
            --cols_;
            size_ = data_.size();
            assert (size_==rows_*cols_);
            return *this;
        }

        inline Matrix& SwapRows(size_t i, size_t j){
            assert(i >=0 && i<=rows_); assert(j>=0 && j<=rows_);
            for(size_t k=0;k<cols_;++k) std::swap(data_[i+k*rows_], data_[j+k*rows_]);
            return *this;
        }

        inline Matrix& SwapCols(size_t i, size_t j){
            assert(i >=0 && i<=cols_); assert(j>=0 && j<=cols_);
            const size_t cis=i*rows_, cjs=j*rows_;
            for(size_t k=0;k<rows_;++k) std::swap(data_[cis+k], data_[cjs+k]);
            return *this;
        }

        inline Matrix& Reshape(size_t rows, size_t cols){
            assert(rows*cols == rows_*cols_);
            rows_ = rows;
            cols_ = cols;
            size_ = rows_*cols_;
            return *this;
        }

        inline Matrix& Resize(size_t rows, size_t cols){
            rows_ = rows;
            cols_ = cols;
            size_ = rows_*cols_;
            data_.resize(size_);
            return *this;
        }

        inline Matrix& Transpose(){
            Matrix<T> tmp(cols_, rows_);
            for(size_t i=0;i<rows_;++i)
                for(size_t j=0;j<cols_;++j)
                    tmp(j,i) = (*this)(i,j);
            *this = tmp;
            return *this;
        }

        inline Matrix& Conjugate(){
            // only for complex
            if constexpr (std::is_same_v<T, std::complex<double>>
                    || std::is_same_v<T, std::complex<float>>)
                for(size_t i=0;i<rows_;++i)
                    for(size_t j=0;j<cols_;++j)
                        (*this)(i,j) = std::conj((*this)(i,j));
            return *this;
        }

        inline Matrix& Hermitian(){
            if constexpr (std::is_same_v<T, std::complex<double>>
                    || std::is_same_v<T, std::complex<float>>)
                return Transpose().Conjugate();
            else
                return Transpose();
            return *this;
        }

        inline Matrix& Zeros(){ std::fill(data_.begin(), data_.end(), T_C(0)); return *this; }
        inline Matrix& Ones(){ std::fill(data_.begin(), data_.end(), T_C(1)); return *this; }
        Matrix& Eyes(){
            assert(rows_==cols_);
            Zeros();
            for(size_t i=0;i<rows_;++i) (*this)(i,i) = T_C(1);
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Matrix& m){
            for (size_t i=0;i<m.rows_;++i)
            {
                for(size_t j=0;j<m.cols_;++j) os << m(i,j) << '\t';
                os << std::endl;
            }
            return os;
        }

      private:
        size_t rows_;
        size_t cols_;
        size_t size_;
        std::vector<T> data_;
    };
} // namespace la

#undef PTRDIFF_C
#undef T_C

#endif
