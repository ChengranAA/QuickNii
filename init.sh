#!/bin/bash

# Store the current directory as the project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PROJECT_ROOT="${SCRIPT_DIR}"

# Define paths for the include and lib directories
INCLUDE="${PROJECT_ROOT}/include"
LIB="${PROJECT_ROOT}/lib"

# Create the include and lib directories
mkdir -p "${INCLUDE}"
mkdir -p "${LIB}"

git clone https://github.com/ocornut/imgui "${INCLUDE}/imgui"
git clone https://github.com/epezent/implot "${INCLUDE}/implot"
git clone https://github.com/NIFTI-Imaging/nifti_clib

# Ensure the directory exists before continuing
cd nifti_clib || exit
make


mkdir -p "${INCLUDE}/nifti_clib" "${LIB}/nifti_clib"
mv ./include/*.h "${INCLUDE}/nifti_clib"
mv ./lib/*.a "${LIB}/nifti_clib"

cd ..
rm -rf nifti_clib