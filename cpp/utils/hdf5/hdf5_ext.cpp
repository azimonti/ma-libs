/************************/
/*    hdf5_ext.cpp      */
/*    Version 1.0       */
/*     2022/07/15       */
/************************/

#include <cassert>
#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <vector>
#include "std/string.hpp"
#include "hdf5_ext.h"

namespace h5
{
    constexpr size_t MAXDIM = 10;
}

static void checkH5Status(herr_t err, const char* pFile, int line)
{
    if (err < 0) throw std::runtime_error(std::string("H5 error at ") + pFile + " " + std::to_string(line));
}

#define CHECKH5STATUS(err) checkH5Status(err, __FILE__, __LINE__)

h5::H5pp::H5pp(bool isCompressed, bool bFortranInterop) : isCompressed(isCompressed), bFortranInterop(bFortranInterop)
{
    // Overwrite the default error handling function
    H5Eset_auto(H5E_DEFAULT, hdf5_error_handler_s, NULL);
}

void h5::H5pp::ListGroupMembers(const std::string& s, std::vector<std::string>& v)
{
    hid_t grp = H5Gopen2(file, s.c_str(), H5P_DEFAULT);
    if (grp == H5I_INVALID_HID) throw std::runtime_error(std::string("Group not found: ") + s);
    // A more generic version would be passing a callback to this and access the class instance.
    // H5Literate_by_name2(grp, std::string("/" + s).c_str(), H5_INDEX_NAME, H5_ITER_INC, NULL, group_info_s, this,
    // H5P_DEFAULT);
    H5Literate_by_name2(grp, std::string("/" + s).c_str(), H5_INDEX_NAME, H5_ITER_INC, NULL, group_info_s, (void*)&v,
                        H5P_DEFAULT);
    H5Gclose(grp);
}

herr_t h5::H5pp::hdf5_error_handler_s(hid_t err_stack, void* client_data)
{
    (void)client_data;
    H5Eprint2(err_stack, NULL);
    return H5I_INVALID_HID;
}

herr_t h5::H5pp::group_info_s(hid_t loc_id, const char* name, const H5L_info2_t* linfo, void* opdata)
{
    (void)loc_id;
    (void)linfo;
    // ((h5::H5pp *)opdata)->group_info(loc_id, name, linfo);
    ((std::vector<std::string>*)opdata)->emplace_back(name);
    return 0;
}

h5::H5ppWriter::H5ppWriter(const std::string& fname, bool isCompressed, bool bFortranInterop)
    : H5pp(isCompressed, bFortranInterop)
{
    hid_t fapl;
    // Set the file close degree
    fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fclose_degree(fapl, H5F_CLOSE_STRONG);
    // Create a new file. If file exists it appends
    if (std::filesystem::exists(fname))
    {
        file = H5Fopen(fname.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        if (file < 0) throw std::runtime_error("Could not open H5 file for writing.");
    }
    else
    {
        file = H5Fcreate(fname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        if (file < 0) throw std::runtime_error("Could not create H5 file.");
    }
    H5Pclose(fapl);
}

h5::H5ppWriter::~H5ppWriter()
{
    CHECKH5STATUS(H5Fclose(file));
}

h5::H5ppReader::H5ppReader(const std::string& fname, bool isCompressed, bool bFortranInterop)
    : H5pp(isCompressed, bFortranInterop)
{
    hid_t fapl;
    // Set the file close degree
    fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fclose_degree(fapl, H5F_CLOSE_STRONG);
    file = H5Fopen(fname.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file < 0) throw std::runtime_error("Could not open H5 file for reading.");
    H5Pclose(fapl);
}

h5::H5ppReader::~H5ppReader()
{
    CHECKH5STATUS(H5Fclose(file));
}

void h5::H5ppReader::readBase(const std::string& s, const size_t nd, const hid_t type,
                              const std::function<void*(size_t)>& allocFn)
{
    // Open the file and dataset.
    hsize_t dims[MAXDIM], chunk_dims[MAXDIM];
    hid_t dataset   = H5Dopen2(file, s.c_str(), H5P_DEFAULT);

    hid_t filespace = H5Dget_space(dataset);
    hid_t ndims     = H5Sget_simple_extent_dims(filespace, dims, NULL);
    CHECKH5STATUS(ndims > 0);
    if (nd != static_cast<size_t>(ndims))
        throw std::runtime_error("Incompatible number of dimensions in H5ppReader::read.");

    hid_t prop = H5Dget_create_plist(dataset);

    if (H5Pget_layout(prop) == H5D_CHUNKED) { CHECKH5STATUS(H5Pget_chunk(prop, static_cast<int>(nd), chunk_dims)); }

    // Define memory space.
    hid_t memspace = H5Screate_simple(static_cast<int>(nd), dims, NULL);

    void* pMem     = allocFn(dims[0]);

    CHECKH5STATUS(H5Dread(dataset, type, memspace, filespace, H5P_DEFAULT, pMem));

    // Close all objects and file.
    CHECKH5STATUS(H5Pclose(prop));
    CHECKH5STATUS(H5Dclose(dataset));
    CHECKH5STATUS(H5Sclose(filespace));
    CHECKH5STATUS(H5Sclose(memspace));
}

void h5::H5ppReader::read(const std::string& s, const size_t nd, const std::function<bool*(size_t)>& allocFn)
{
    readBase(s, nd, H5T_NATIVE_HBOOL, [&](size_t n) -> void* { return (void*)allocFn(n); });
}

bool& h5::H5ppReader::read(const std::string& s, bool& res)
{
    readBase(s, 1, H5T_NATIVE_HBOOL, [&](size_t n) -> void* {
        (void)n;
        return &res;
    });
    return res;
}

void h5::H5ppReader::read(const std::string& s, const size_t nd, const std::function<char*(size_t)>& allocFn)
{
    readBase(s, nd, H5T_NATIVE_CHAR, [&](size_t n) -> void* { return (void*)allocFn(n); });
}

std::vector<char>& h5::H5ppReader::read(const std::string& s, std::vector<char>& res, const size_t nd)
{
    readBase(s, nd, H5T_NATIVE_CHAR, [&](size_t n) -> void* {
        res.resize(n);
        return res.data();
    });
    return res;
}

char& h5::H5ppReader::read(const std::string& s, char& res)
{
    readBase(s, 1, H5T_NATIVE_CHAR, [&](size_t n) -> void* {
        (void)n;
        return &res;
    });
    return res;
}

std::string& h5::H5ppReader::read(const std::string& s, std::string& res)
{
    std::vector<char*> buf;
    readBase(s, 1, H5T_NATIVE_CHAR, [&](size_t n) -> void* {
        buf.resize(n);
        return reinterpret_cast<char*>(buf.data());
    });
    std::string ss(reinterpret_cast<char*>(buf.data()));
    res = ss;
    return res;
}

void h5::H5ppReader::read(const std::string& s, const size_t nd, const std::function<float*(size_t)>& allocFn)
{
    readBase(s, nd, H5T_NATIVE_FLOAT, [&](size_t n) -> void* { return (void*)allocFn(n); });
}

std::vector<float>& h5::H5ppReader::read(const std::string& s, std::vector<float>& res, const size_t nd)
{
    readBase(s, nd, H5T_NATIVE_FLOAT, [&](size_t n) -> void* {
        res.resize(n);
        return res.data();
    });
    return res;
}

float& h5::H5ppReader::read(const std::string& s, float& res)
{
    readBase(s, 1, H5T_NATIVE_FLOAT, [&](size_t n) -> void* {
        (void)n;
        return &res;
    });
    return res;
}

void h5::H5ppReader::read(const std::string& s, const size_t nd, const std::function<double*(size_t)>& allocFn)
{
    readBase(s, nd, H5T_NATIVE_DOUBLE, [&](size_t n) -> void* { return (void*)allocFn(n); });
}

std::vector<double>& h5::H5ppReader::read(const std::string& s, std::vector<double>& res, const size_t nd)
{
    readBase(s, nd, H5T_NATIVE_DOUBLE, [&](size_t n) -> void* {
        res.resize(n);
        return res.data();
    });
    return res;
}

double& h5::H5ppReader::read(const std::string& s, double& res)
{
    readBase(s, 1, H5T_NATIVE_DOUBLE, [&](size_t n) -> void* {
        (void)n;
        return &res;
    });
    return res;
}

void h5::H5ppReader::read(const std::string& s, const size_t nd, const std::function<int*(size_t)>& allocFn)
{
    readBase(s, nd, H5T_NATIVE_INT, [&](size_t n) -> void* { return (void*)allocFn(n); });
}

std::vector<int>& h5::H5ppReader::read(const std::string& s, std::vector<int>& res, const size_t nd)
{
    readBase(s, nd, H5T_NATIVE_INT, [&](size_t n) -> void* {
        res.resize(n);
        return res.data();
    });
    return res;
}

int& h5::H5ppReader::read(const std::string& s, int& res)
{
    readBase(s, 1, H5T_NATIVE_INT, [&](size_t n) -> void* {
        (void)n;
        return &res;
    });
    return res;
}

void h5::H5ppReader::read(const std::string& s, const size_t nd, const std::function<int64_t*(size_t)>& allocFn)
{
    readBase(s, nd, H5T_NATIVE_LLONG, [&](size_t n) -> void* { return (void*)allocFn(n); });
}

std::vector<int64_t>& h5::H5ppReader::read(const std::string& s, std::vector<int64_t>& res, const size_t nd)
{
    readBase(s, nd, H5T_NATIVE_LLONG, [&](size_t n) -> void* {
        res.resize(n);
        return res.data();
    });
    return res;
}

int64_t& h5::H5ppReader::read(const std::string& s, int64_t& res)
{
    readBase(s, 1, H5T_NATIVE_LLONG, [&](size_t n) -> void* {
        (void)n;
        return &res;
    });
    return res;
}

void h5::H5ppReader::read(const std::string& s, const size_t nd, const std::function<size_t*(size_t)>& allocFn)
{
    if (bFortranInterop) throw std::runtime_error("This function is not working in Fortran Compatibility mode.");
    else readBase(s, nd, H5T_NATIVE_ULLONG, [&](size_t n) -> void* { return (void*)allocFn(n); });
}

std::vector<size_t>& h5::H5ppReader::read(const std::string& s, std::vector<size_t>& res, const size_t nd)
{
    if (bFortranInterop)
    {
        std::vector<int64_t> i64v_;
        read(s, i64v_);
        res.reserve(i64v_.size());
        for (size_t i = 0; i < i64v_.size(); ++i) res.push_back(static_cast<size_t>(i64v_[i]));
    }
    else
    {
        readBase(s, nd, H5T_NATIVE_ULLONG, [&](size_t n) -> void* {
            res.resize(n);
            return res.data();
        });
    }
    return res;
}

size_t& h5::H5ppReader::read(const std::string& s, size_t& res)
{
    if (bFortranInterop)
    {
        int64_t i64_;
        res = static_cast<size_t>(read(s, i64_));
    }
    else
    {
        readBase(s, 1, H5T_NATIVE_ULLONG, [&](size_t n) -> void* {
            (void)n;
            return &res;
        });
    }
    return res;
}

void h5::H5ppWriter::writeBase(const std::string& s, const void* v, const size_t* d, const size_t nd, const hid_t type)
{
    hsize_t dims[MAXDIM];
    assert(nd <= std::size(dims));
    hsize_t maxdims[MAXDIM], chunk_dims[MAXDIM];

    for (size_t i = 0; i < nd; ++i)
    {
        dims[i]       = (hsize_t)d[i];
        maxdims[i]    = H5S_UNLIMITED;
        chunk_dims[i] = 5;
    }

    const std::vector<std::string> vs = std_ext::tokenize(s, '/');
    std::string path;
    for (std::vector<std::string>::const_iterator it = vs.begin(); it != vs.end() - 1; ++it)
    {
        path += (path.empty() ? *it : "/" + *it);
        // Create the groups if not exist
        if (H5Lexists(file, path.c_str(), H5P_DEFAULT) == 0)
        {
            hid_t group = H5Gcreate2(file, path.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
            CHECKH5STATUS(H5Gclose(group));
        }
    }

    // Delete the old dataset if exists
    if (H5Lexists(file, s.c_str(), H5P_DEFAULT) != 0) H5Ldelete(file, s.c_str(), H5P_DEFAULT);

    // Create the data space with unlimited dimensions
    hid_t dataspace = H5Screate_simple(static_cast<int>(nd), dims, maxdims);
    // Modify dataset creation properties, i.e. enable chunking
    hid_t prop      = H5Pcreate(H5P_DATASET_CREATE);
    CHECKH5STATUS(H5Pset_chunk(prop, static_cast<int>(nd), chunk_dims));

    // Set ZLIB / DEFLATE Compression using compression level 9 if compression is enabled
    if (isCompressed) CHECKH5STATUS(H5Pset_deflate(prop, 9));
    hid_t dataset = H5Dcreate2(file, s.c_str(), type, dataspace, H5P_DEFAULT, prop, H5P_DEFAULT);
    if (dataset == H5I_INVALID_HID) throw std::runtime_error(std::string("H5Dcreate2 failed for ") + s);

    // Write data to dataset
    CHECKH5STATUS(H5Dwrite(dataset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, v));

    // Close resources
    CHECKH5STATUS(H5Dclose(dataset));
    CHECKH5STATUS(H5Pclose(prop));
    CHECKH5STATUS(H5Sclose(dataspace));
}

void h5::H5ppWriter::write(const std::string& s, const bool* v, const size_t* d, const size_t nd)
{
    writeBase(s, static_cast<const void*>(v), d, nd, H5T_NATIVE_HBOOL);
}

void h5::H5ppWriter::write(const std::string& s, const char* v, const size_t* d, const size_t nd)
{
    writeBase(s, static_cast<const void*>(v), d, nd, H5T_NATIVE_CHAR);
}

void h5::H5ppWriter::write(const std::string& s, const float* v, const size_t* d, const size_t nd)
{
    writeBase(s, static_cast<const void*>(v), d, nd, H5T_NATIVE_FLOAT);
}

void h5::H5ppWriter::write(const std::string& s, const double* v, const size_t* d, const size_t nd)
{
    writeBase(s, static_cast<const void*>(v), d, nd, H5T_NATIVE_DOUBLE);
}

void h5::H5ppWriter::write(const std::string& s, const int* v, const size_t* d, const size_t nd)
{
    writeBase(s, static_cast<const void*>(v), d, nd, H5T_NATIVE_INT);
}

void h5::H5ppWriter::write(const std::string& s, const int64_t* v, const size_t* d, const size_t nd)
{
    writeBase(s, static_cast<const void*>(v), d, nd, H5T_NATIVE_LLONG);
}

void h5::H5ppWriter::write(const std::string& s, const size_t* v, const size_t* d, const size_t nd)
{
    writeBase(s, static_cast<const void*>(v), d, nd, H5T_NATIVE_ULLONG);
}

void h5::H5ppWriter::write(const std::string& s, const std::vector<size_t>& v)
{
    size_t d_[1] = {};
    d_[0]        = v.size();
    if (bFortranInterop)
    {
        std::vector<int64_t> i64v_;
        i64v_.reserve(v.size());
        for (size_t i = 0; i < v.size(); ++i) i64v_.push_back(static_cast<int64_t>(v[i]));
        write(s, i64v_.data(), d_, std::size(d_));
    }
    else write(s, v.data(), d_, std::size(d_));
}

void h5::H5ppWriter::write(const std::string& s, const size_t& v)
{
    size_t d_[1] = {};
    d_[0]        = 1;
    if (bFortranInterop)
    {
        int64_t i64_;
        i64_ = static_cast<int64_t>(v);
        write(s, &i64_, d_, std::size(d_));
    }
    else write(s, &v, d_, std::size(d_));
}

void h5::H5ppWriter::write(const std::string& s, const std::string& v)
{
    size_t d_[1] = {};
    d_[0]        = v.size();
    write(s, v.c_str(), d_, std::size(d_));
}
