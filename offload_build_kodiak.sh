#!/bin/bash

# Make sure all the paths are correct

module purge
module use /usr/projects/consult/mcoyne/toss34ofed/modulefiles
module load binutils gcc92_icn
module load cudatoolkit/10.1 mkl

rm -r build
rm -r install

MY_PATH=$(pwd)

export CC=${CC:=gcc}
export FC=${FC:=gfortran}
export CXX=${CXX:=g++}
export BLAS_VENDOR=${BLAS_VENDOR:=MKL}
export BML_OPENMP=${BML_OPENMP:=yes}
export BML_GPU=${BML_GPU:=no}
export INSTALL_DIR=${INSTALL_DIR:="${MY_PATH}/install"}
export BML_TESTING=${BML_TESTING:=yes}
export CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:=Release}
export EXTRA_CFLAGS=${EXTRA_CFLAGS:=""}
export EXTRA_LINK_FLAGS=${EXTRA_LINK_FLAGS:=""}
export CMAKE_Fortran_FLAGS="-fopenmp -foffload=nvptx-none -foffload=-lm"
export CMAKE_C_FLAGS="-O3 -fopenmp -foffload=nvptx-none -foffload=-lm"
export CUDA_TOOLKIT_ROOT_DIR=${CUDATOOLKIT_ROOT}
export BML_INTERNAL_BLAS=${BML_INTERNAL_BLAS:=yes}
./build.sh install

                                                                                                                                                                                              
                                    