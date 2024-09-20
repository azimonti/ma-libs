#!/bin/bash

unameOut="$(uname -s)"
case "${unameOut}" in
	Linux*)		MACHINE="$(lsb_release -is)";;
	Darwin*)	MACHINE=mac;;
	CYGWIN*)	MACHINE=cygwin;;
	MINGW*)		MACHINE=mingw;;
	*)		MACHINE="UNKNOWN:${unameOut}"
esac

MACHINE=$(echo "${MACHINE}" | awk '{print tolower($0)}')

if [ "$MACHINE" == "mingw" ]; then
	SCRIPTDIR=Scripts
	PYTHON=python
else
	SCRIPTDIR=bin
	PYTHON=$(which python3)
fi

# Define directories with full paths
BASHSCRIPTDIR=$(cd "$(dirname "$0")" || exit; pwd)
BUILDDIR=$(realpath "${BASHSCRIPTDIR}/../../build")
VENVDIR=$(realpath "${BUILDDIR}/python_env_libnn")

# Check if the virtual environment already exists
if [ -d "${VENVDIR}" ]; then
    echo "Virtual environment already exists at ${VENVDIR}. Activating it..."
else
    # Create build directory if it doesn't exist
    mkdir -p "${BUILDDIR}"

    # Create a virtual environment
    "${PYTHON}" -u -m venv "${VENVDIR}"

    # Activate the virtual environment and install pybind11
    echo "Creating virtual environment at ${VENVDIR} and installing pybind11..."
    # shellcheck source=/dev/null
    source "${VENVDIR}/${SCRIPTDIR}/activate"
    pip install --upgrade pip
    pip install pybind11
    pip install h5py
fi

# Print the activation message
echo "You can activate the environment with 'source ${VENVDIR}/${SCRIPTDIR}/activate' and then build."

