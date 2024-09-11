#ifndef _HDF5_EXT_H_4E7F80888C0440DB8D8541EA1DBA398B_
#define _HDF5_EXT_H_4E7F80888C0440DB8D8541EA1DBA398B_

/************************/
/*     hdf5_ext.h       */
/*    Version 1.0       */
/*     2022/07/15       */
/************************/

#include <array>
#include <functional>
#include <string>
#include <vector>
#include "hdf5.h"

namespace h5
{

    class H5pp
    {
      public:
        H5pp(bool isCompressed, bool bFortranInterop);
        virtual ~H5pp() = default;
        void ListGroupMembers(const std::string& s, std::vector<std::string>& v);

      protected:
        hid_t file{};
        bool isCompressed{};
        bool bFortranInterop{};

      private:
        static herr_t group_info_s(hid_t loc_id, const char* name, const H5L_info2_t* linfo, void* opdata);
        static herr_t hdf5_error_handler_s(hid_t err_stack, void* client_data);
    };

    class H5ppWriter : public H5pp
    {
      public:
        H5ppWriter(const std::string& fname, bool isCompressed = true, bool bFortranInterop = true);
        ~H5ppWriter();
        void write(const std::string& s, const bool* v, const size_t* d, const size_t nd);
        void write(const std::string& s, const char* v, const size_t* d, const size_t nd);
        void write(const std::string& s, const float* v, const size_t* d, const size_t nd);
        void write(const std::string& s, const double* v, const size_t* d, const size_t nd);
        void write(const std::string& s, const int* v, const size_t* d, const size_t nd);
        void write(const std::string& s, const int64_t* v, const size_t* d, const size_t nd);
        void write(const std::string& s, const size_t* v, const size_t* d, const size_t nd);

        template <typename S> void write(const std::string& s, const std::vector<S>& v)
        {
            size_t d_[1] = {};
            d_[0]        = v.size();
            write(s, v.data(), d_, std::size(d_));
        }

        template <typename S, size_t N> void write(const std::string& s, const std::vector<std::array<S, N>>& v)
        {
            size_t d_[2] = {0, 0};
            d_[0]        = v.size();
            d_[1]        = v[0].size();
            write(s, v.data()->data(), d_, std::size(d_));
        }

        template <typename S> void write(const std::string& s, const std::vector<std::vector<S>>& v)
        {
            size_t d_[2] = {0, 0};
            d_[0]        = v.size();
            d_[1]        = v[0].size();
            std::vector<S> v_(v.size() * v[0].size());
            for (size_t i = 0; i < v.size(); ++i)
                for (size_t j = 0; j < v[0].size(); ++j) v_[i * v[0].size() + j] = v[i][j];
            write(s, v_.data(), d_, std::size(d_));
        }

        template <typename S> void write(const std::string& s, const S& v)
        {
            size_t d_[1] = {};
            d_[0]        = 1;
            write(s, &v, d_, std::size(d_));
        }

        // template specialization for Fortran interoperability
        void write(const std::string& s, const std::vector<size_t>& v);
        void write(const std::string& s, const size_t& v);
        // template specialization for std::string
        void write(const std::string& s, const std::string& v);

      private:
        void writeBase(const std::string& s, const void* v, const size_t* d, const size_t nd, const hid_t type);
    };

    class H5ppReader : public H5pp
    {
      public:
        H5ppReader(const std::string& fname, bool isCompressed = true, bool bFortranInterop = true);
        ~H5ppReader();
        void read(const std::string& s, const size_t nd, const std::function<bool*(size_t)>& allocFn);
        bool& read(const std::string& s, bool& res);
        void read(const std::string& s, const size_t nd, const std::function<char*(size_t)>& allocFn);
        std::vector<char>& read(const std::string& s, std::vector<char>& res, const size_t nd = 1);
        char& read(const std::string& s, char& res);
        std::string& read(const std::string& s, std::string& res);
        void read(const std::string& s, const size_t nd, const std::function<float*(size_t)>& allocFn);
        std::vector<float>& read(const std::string& s, std::vector<float>& res, const size_t nd = 1);
        float& read(const std::string& s, float& res);
        void read(const std::string& s, const size_t nd, const std::function<double*(size_t)>& allocFn);
        std::vector<double>& read(const std::string& s, std::vector<double>& res, const size_t nd = 1);
        double& read(const std::string& s, double& res);
        void read(const std::string& s, const size_t nd, const std::function<int*(size_t)>& allocFn);
        std::vector<int>& read(const std::string& s, std::vector<int>& res, const size_t nd = 1);
        int& read(const std::string& s, int& res);
        void read(const std::string& s, const size_t nd, const std::function<int64_t*(size_t)>& allocFn);
        std::vector<int64_t>& read(const std::string& s, std::vector<int64_t>& res, const size_t nd = 1);
        int64_t& read(const std::string& s, int64_t& res);
        void read(const std::string& s, const size_t nd, const std::function<size_t*(size_t)>& allocFn);
        std::vector<size_t>& read(const std::string& s, std::vector<size_t>& res, const size_t nd = 1);
        size_t& read(const std::string& s, size_t& res);

      private:
        void readBase(const std::string& s, const size_t nd, const hid_t type,
                      const std::function<void*(size_t)>& allocFn);
    };

} // namespace h5

#endif
