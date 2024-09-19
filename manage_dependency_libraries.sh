#!/bin/bash

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     MACHINE=linux;;
    Darwin*)    MACHINE=macos;;
    MINGW*)     MACHINE=win;;
    *)          exit 1
esac

BASHSCRIPTDIR=$(cd "$(dirname "$0")" || exit; pwd)
DEPENDENCIESDIR="${BASHSCRIPTDIR}/usr"
LOCALLIBSDIR="${BASHSCRIPTDIR}/cpp/libs"
UPDATE="FALSE"
DOWNLOAD="FALSE"
BUILD="FALSE"
LIBJPGTAG="v9e"
LIBPNGTAG="v1.6.37"
ZLIBTAG="v1.2.12"
GLFWTAG="3.3.7"
GLEWTAG="glew-cmake-2.2.0"
IMGUITAG="v1.87"
IMGUIZMOTAG="v3.0"
IMPLOTTAG="v0.13"
GLMTAG="0.9.9.8"
HDF5="hdf5-1_14_0"
LAPACK_TAG="v3.10.1"

displayusage() {
    echo " ============================================================== "
    echo "|    Usage:                                                    |"
    echo "| manage_dependency_libaries.sh OPTS                           |"
    echo "|    available options [OPTS]:                                 |"
    echo "| -b) --build)    build the libraries                          |"
    echo "| -d) --download) download the libraries                       |"
    echo "| -h) --help)     print this help                              |"
    echo "| -g) --global)   install in USR_LIB_DIR and USR_SRC_DIR       |"
    echo "| -u) --update)   update the Libraries                         |"
    echo "| [no arguments]  print this help                              |"
    echo " ============================================================== "
}

for arg in "$@"; do
    shift
    case "$arg" in
        "--build")         set -- "$@" "-b" ;;
        "--download")      set -- "$@" "-d" ;;
        "--global")        set -- "$@" "-g" ;;
        "--help")          set -- "$@" "-h" ;;
        "--update")        set -- "$@" "-u" ;;
        *)                 set -- "$@" "$arg";;
    esac
done

# Parse short options
OPTIND=1
while getopts "bdghu?" opt
do
    case "$opt" in
        "b") BUILD="TRUE" ;;
        "d") DOWNLOAD="TRUE" ;;
        "g") GLOBALDIR="TRUE" ;;
        "h") displayusage; exit 0;;
        "u") UPDATE="TRUE" ;;
        "?") displayusage; exit 0;;
    esac
done

shift "$((OPTIND-1))"

checkout_revision(){
    local REPO=$1
    local REVISION=$2
    local REMOTE_REPO=$3
    if [[ -d "${REPO}" ]] ; then
        cd "${REPO}" || exit
        git checkout .
        git checkout -
        git pull
    else
        git clone "${REMOTE_REPO}" "${REPO}"
        cd "${REPO}" || exit
    fi
    git checkout "${REVISION}"
    cd .. || exit
}

build_install(){
    local CURLIB="$1"
    if [ -n "$2" ]; then
        local CMAKEOPTS="$2"
    fi
    mkdir -p "${BUILDDIR}/${CURLIB}"
    cd "${BUILDDIR}/${CURLIB}" || exit
    CMAKE_BUILD_PARALLEL_LEVEL=8 cmake "${SRCDIR}/${CURLIB}" -DCMAKE_INSTALL_PREFIX="${INSTALLDIR}" ${CMAKEOPTS:+$CMAKEOPTS}
    CMAKE_BUILD_PARALLEL_LEVEL=8 cmake --build . --target INSTALL --config Debug
    CMAKE_BUILD_PARALLEL_LEVEL=8 cmake --build . --target INSTALL --config Release
    cd .. || exit
}

build_lib(){
    local CURLIB="$1"
    if [ -n "$2" ]; then
        local CMAKEOPTS="$2"
    fi
    mkdir -p "${BUILDDIR}/${CURLIB}"
    cd "${BUILDDIR}/${CURLIB}" || exit
    CMAKE_BUILD_PARALLEL_LEVEL=8 cmake "${SRCDIR}/${CURLIB}" -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="${OUTPUTBINDIR}" -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="${OUTPUTLIBDIR}" -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="${OUTPUTLIBDIR}" ${CMAKEOPTS:+$CMAKEOPTS}
    CMAKE_BUILD_PARALLEL_LEVEL=8 cmake --build . --config Debug
    CMAKE_BUILD_PARALLEL_LEVEL=8 cmake --build . --config Release
    cd .. || exit
}

build_lib_local(){
    local CURLIB="$1"
    if [ -n "$2" ]; then
        local CMAKEOPTS="$2"
    fi
    mkdir -p "${BUILDDIR}/${CURLIB}"
    cd "${BUILDDIR}/${CURLIB}" || exit
    CMAKE_BUILD_PARALLEL_LEVEL=8 cmake "${LOCALLIBSDIR}/${CURLIB}" -DLIBS_ROOT="${SRCDIR}" -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="${OUTPUTBINDIR}" -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="${OUTPUTLIBDIR}" -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="${OUTPUTLIBDIR}" ${CMAKEOPTS:+$CMAKEOPTS}
    CMAKE_BUILD_PARALLEL_LEVEL=8 cmake --build . --config Debug
    CMAKE_BUILD_PARALLEL_LEVEL=8 cmake --build . --config Release
    cd .. || exit
}

if [[ "${BUILD}" == "FALSE" ]] && [[ "${DOWNLOAD}" == "FALSE" ]] && [[ "${UPDATE}" == "FALSE" ]] ; then
    # nothing to do
    displayusage; exit 0;
fi

if [[ "${GLOBALDIR}" == "TRUE" ]] ; then
    if [ -z "$USR_BUILD_DIR" ]; then
        echo "USR_BUILD_DIR not defined. Exiting .."
        exit 1
    else
        BUILDDIR="${USR_BUILD_DIR}/build"
        OUTPUTLIBDIR="${USR_BUILD_DIR}/libs"
        OUTPUTBINDIR="${USR_BUILD_DIR}/bin"
    fi
    if [ -z "$USR_LIB_DIR" ]; then
        echo "USR_LIB_DIR not defined. Exiting .."
        exit 1
    else
        INSTALLDIR="${USR_LIB_DIR}"
    fi
    if [ -z "$USR_SRC_DIR" ]; then
        echo "USR_SRC_DIR not defined. Exiting .."
        exit 1
    else
        SRCDIR="${USR_SRC_DIR}"
    fi
else
    BUILDDIR="${DEPENDENCIESDIR}/build"
    OUTPUTLIBDIR="${DEPENDENCIESDIR}/libs"
    OUTPUTBINDIR="${DEPENDENCIESDIR}/bin"
    INSTALLDIR="${DEPENDENCIESDIR}/local"
    SRCDIR="${DEPENDENCIESDIR}/src"
fi

if [[ "${GLOBALDIR}" != "TRUE" ]] ; then
    mkdir -p "${SRCDIR}"
fi

if [[ "${DOWNLOAD}" == "TRUE" ]] || [[ "${UPDATE}" == "TRUE" ]] ; then
    cd "${SRCDIR}" || exit
    checkout_revision "imgui" "${IMGUITAG}" "https://github.com/ocornut/imgui"
    checkout_revision "imGuIZMO.quat" "${IMGUIZMOTAG}" "https://github.com/BrutPitt/imGuIZMO.quat"
    checkout_revision "implot" "${IMPLOTTAG}" "https://github.com/epezent/implot"
    if [ "${MACHINE}" == "win" ]; then
        patch "${SRCDIR}/imGuIZMO.quat/imGuIZMO.quat/imGuIZMOquat.h" < "${BASHSCRIPTDIR}/patches/imGuIZMO.quat/imGuIZMOquat.h.win.diff"
    else
        patch "${SRCDIR}/imGuIZMO.quat/imGuIZMO.quat/imGuIZMOquat.h" < "${BASHSCRIPTDIR}/patches/imGuIZMO.quat/imGuIZMOquat.h.diff"
    fi
fi

if [[ "${DOWNLOAD}" == "TRUE" ]] ; then
    cd "${BASHSCRIPTDIR}" || exit
    if [ "${MACHINE}" == "macos" ]; then
        brew install cmake
        brew install zlib
        brew install glfw
        brew install glew
        brew install glm
        brew install libpng
        brew install libjpg
        brew install lapack
    elif [ "${MACHINE}" == "linux" ]; then
        echo "not yet implemented"
    elif [ "${MACHINE}" == "win" ]; then
        cd "${SRCDIR}" || exit
        checkout_revision "libjpeg" "${LIBJPGTAG}" "git@github.com:oyatsukai/libjpeg"
        checkout_revision "libpng"  "${LIBPNGTAG}" "https://github.com/glennrp/libpng"
        patch "${SRCDIR}/libpng/CMakeLists.txt" < "${BASHSCRIPTDIR}/patches/libpng/CMakeLists.txt.win.diff"
        checkout_revision "zlib"    "${ZLIBTAG}"      "https://github.com/madler/zlib"
        checkout_revision "glfw"    "${GLFWTAG}"   "https://github.com/glfw/glfw"
        checkout_revision "glew"    "${GLEWTAG}"   "https://github.com/Perlmint/glew-cmake"
        checkout_revision "glm"     "${GLMTAG}"    "https://github.com/g-truc/glm"
        checkout_revision "hdf5"    "${HDF5}"      "https://github.com/HDFGroup/hdf5"
        checkout_revision "lapack"  "${LAPACK_TAG}"   "https://github.com/Reference-LAPACK/lapack"
        cd "${BASHSCRIPTDIR}" || exit
    else
        echo "unsupported architecture"
    fi
fi
if [[ "${UPDATE}" == "TRUE" ]] ; then
    cd "${BASHSCRIPTDIR}" || exit
    if [ "${MACHINE}" == "macos" ]; then
        brew upgrade cmake
        brew upgrade zlib
        brew upgrade libpng
        brew upgrade libjpg
        brew upgrade glfw
        brew upgrade glew
        brew upgrade glm
        brew upgrade lapack
    elif [ "${MACHINE}" == "linux" ]; then
        echo "not yet implemented"
    elif [ "${MACHINE}" == "win" ]; then
        cd "${SRCDIR}" || exit
        checkout_revision "libjpeg"  "${LIBJPGTAG}"
        checkout_revision "libpng"   "${LIBPNGTAG}"
        patch "${SRCDIR}/libpng/CMakeLists.txt" < "${BASHSCRIPTDIR}/patches/libpng/CMakeLists.txt.win.diff"
        checkout_revision "zlib"     "${ZLIBTAG}"
        checkout_revision "glfw"     "${GLFWTAG}"
        checkout_revision "glew"     "${GLEWTAG}"
        checkout_revision "glm"      "${GLMTAG}"
        checkout_revision "hdf5"     "${HDF5}"
        checkout_revision "lapack"   "${LAPACK_TAG}"
        cd "${BASHSCRIPTDIR}" || exit
    else
        echo "unsupported architecture"
    fi
fi

if [[ "${BUILD}" == "TRUE" ]] ; then
    build_lib_local imgui
    build_lib_local implot
    build_lib_local imGuIZMO.quat
    if [ "${MACHINE}" == "macos" ]; then
        :
    elif [ "${MACHINE}" == "linux" ]; then
        echo "not yet implemented"
    elif [ "${MACHINE}" == "win" ]; then
        build_install   zlib
        build_install   libjpeg
        build_lib       glfw "-DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF"
        build_lib_local glew
        export ZLIB_ROOT=${INSTALLDIR}
        build_install   libpng "-DPNG_TESTS=OFF"
        build_lib       hdf5   "-DHDF5_GENERATE_HEADERS=ON -DHDF5_DISABLE_COMPILER_WARNINGS=ON -DBUILD_SHARED_LIBS=ON -DHDF5_BUILD_FORTRAN=ON -DHDF5_BUILD_CPP_LIB=ON -DHDF5_BUILD_TOOLS=OFF -DBUILD_TESTING=OFF -DHDF5_BUILD_EXAMPLES=OFF -DHDF5_ENABLE_Z_LIB_SUPPORT=ON -DZLIB_DIR=${SRCDIR}/zlib"
        mkdir -p "${INSTALLDIR}/include/hdf5"
        cp "${BUILDDIR}/hdf5/src/h5pubconf.h" "${INSTALLDIR}/include/hdf5/h5pubconf.h"
        cp "${SRCDIR}/hdf5/src/H5FDsubfiling/H5FDsubfiling.h" "${INSTALLDIR}/include/hdf5/H5FDsubfiling.h"
        cp "${SRCDIR}/hdf5/src/H5FDsubfiling/H5FDioc.h" "${INSTALLDIR}/include/hdf5/H5FDioc.h"
        cp "${BUILDDIR}/hdf5/mod/shared/Release/"*.mod "${INSTALLDIR}/include/hdf5"
        build_lib       matplot "-DBUILD_EXAMPLES=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON"
        build_install   lapack  "-DCBLAS=ON"
        cd "${BASHSCRIPTDIR}" || exit
    fi
fi
