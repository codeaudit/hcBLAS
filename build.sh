#!/bin/bash -e
#This script is invoked to build the hcblas library and test sources

# getconf _NPROCESSORS_ONLN
working_threads=8

# CHECK FOR COMPILER PATH

if [ ! -z $HCC_HOME ];
then
  if [ -x "$HCC_HOME/compiler/bin/clang++" ];
  then
    platform="hcc"
    cmake_c_compiler="$HCC_HOME/bin/clang"
    cmake_cxx_compiler="$HCC_HOME/bin/clang++"
  fi

elif [ -x "/opt/rocm/hcc/bin/clang++" ];
then
  platform="hcc"
  cmake_c_compiler="/opt/rocm/hcc/bin/clang"
  cmake_cxx_compiler="/opt/rocm/hcc/bin/clang++"

elif [ -x "/usr/local/cuda/bin/nvcc" ];
then
  platform="nvcc"
  cmake_c_compiler="/usr/bin/gcc"
  cmake_cxx_compiler="/usr/bin/g++"
else
  echo "Neither clang  or NVCC compiler found"
  echo "Not an AMD or NVCC compatible stack"
  exit 1
fi

if ( [ ! -z $HIP_PATH ] || [ -x "/opt/rocm/hip/bin/hipcc" ] ); then 
  export HIP_SUPPORT=on
elif ( [ "$platform" = "nvcc" ]); then
  echo "HIP not found. Install latest HIP to continue."
  exit 1
fi

#CURRENT_WORK_DIRECTORY
current_work_dir=$PWD

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$current_work_dir/build/lib/src

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`
copt="-O3"
verbose=""
install=0

# Help menu
print_help() {
cat <<-HELP
=============================================================================================================================
This script is invoked to build hcblas library and test sources. Please provide the following arguments:

   ${green}--test${reset}     Test to enable the library testing. 
   ${green}--profile${reset}  Profile to enable profiling of five blas kernels namely SGEMM, CGEMM, SGEMV, SGER and SAXPY.(CodeXL)
   ${green}--bench${reset}    Profile benchmark using chrono timer.
   ${green}--debug${reset}    Compile with debug info (-g)
   ${green}--verbose${reset}  Run make with VERBOSE=1
   ${green}--install${reset}  Install .deb file using dpkg -i.  Requires sudo perms.
   ${green}--examples${reset} To build and run the example files in examples folder (on/off)

NOTE: export CODEXL_PATH=/path/to/profiler before enabling profile variable.
=============================================================================================================================
HELP
exit 0
}

while [ $# -gt 0 ]; do
  case "$1" in
    --test=*)
      testing="${1#*=}"
      ;;
    --profile=*)
      profiling="${1#*=}"
      ;;
    --debug)
      copt="-g"
      ;;
    --verbose)
      verbose="VERBOSE=1"
      ;;
    --install)
      install="1"
      ;;
    --synckernel=*)
      synckernel="${1#*=}"
      ;;
    --bench=*)
      bench="${1#*=}"
      ;;
    --examples=*)
      examples="${1#*=}"
      ;;
    --help) print_help;;
    *)
      printf "************************************************************\n"
      printf "* Error: Invalid arguments, run --help for valid arguments.*\n"
      printf "************************************************************\n"
      exit 1
  esac
  shift
done

if [ -z $bench];then
    bench="off"
fi

if [ "$install" = "1" ]; then
    export INSTALL_OPT=on
fi

if [ "$profiling" = "on" ]; then
    export CODEXL_PATH=/opt/rocm/profiler/bin
fi

current_dir=$PWD

set +e
# MAKE BUILD DIR
mkdir $current_work_dir/build
mkdir $current_work_dir/build/test
set -e

# SET BUILD DIR
build_dir=$current_work_dir/build

# change to library build
cd $build_dir

if [ "$platform" = "hcc" ]; then
  
    if [ "$synckernel" = "on" ]; then
      cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DSERIALIZE_KERNEL=ON -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS="$copt -fPIC" -DCMAKE_INSTALL_PREFIX=/opt/rocm/hcblas $current_work_dir
    elif [ "$synckernel" = "off" ]; then
      cmake -DCMAKE_C_COMPILER=$cmake_c_compiler  -DSERIALIZE_KERNEL=OFF -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS="$copt -fPIC" -DCMAKE_INSTALL_PREFIX=/opt/rocm/hcblas $current_work_dir
    else 
      #default case: Of course there are Compulsory waits on certain kernels 
      cmake -DCMAKE_C_COMPILER=$cmake_c_compiler  -DSERIALIZE_KERNEL=ON -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS="$copt -fPIC" -DCMAKE_INSTALL_PREFIX=/opt/rocm/hcblas $current_work_dir
    fi

  make -j$working_threads package $verbose
  make -j$working_threads $verbose

   if [ "$install" = "1" ]; then
    sudo make -j$working_threads install
   fi
   cd $build_dir/packaging/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_INSTALL_PREFIX=/opt/rocm/hcblas $current_work_dir/packaging/
 
# Various possibilities of test and profile arguments
# Test=OFF and Profile=OFF (Build library and tests)
  if [ "$bench" = "off" ]; then
    if ( [ -z $testing ] && [ -z $profiling ] ) || ( [ "$testing" = "off" ] || [ "$profiling" = "off" ] ); then
      echo "${green}HCBLAS Build Completed!${reset}"
# Test=ON and Profile=OFF (Build and test the library)
    elif ( [ "$testing" = "on" ] && [ -z $profiling ] ) || ( [ "$testing" = "on" ] && [ "$profiling" = "off" ] ); then
# Build Tests
        cd $build_dir/test/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/test/
        set +e
        make -j$working_threads
# Invoke test script 
        printf "* UNIT TESTS *\n"
        printf "**************\n"
        ${current_work_dir}/build/test/unit/bin/unittest
        printf "* UNIT-API TESTS *\n"
        printf "******************\n"
        ${current_work_dir}/build/test/unit-api/bin/unit-api-test
        if [ $HIP_SUPPORT = "on" ]; then
          printf "* UNIT-HIP TESTS *\n"
          printf "******************\n"
          ${current_work_dir}/build/test/unit-hip/bin/unit-hip-test  
        fi
# Test=ON and Profile=ON (Build, test and profile the library)
    elif ( [ "$testing" = "on" ] && [ "$profiling" = "on" ] ) || ( [ "$testing" = "on" ] && [ "$profiling" = "on" ] ); then 
# Build Tests
        cd $build_dir/test/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler  -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/test/
        set +e
        make -j$working_threads
# Invoke test script 
        printf "* UNIT TESTS *\n"
        printf "**************\n"
        ${current_work_dir}/build/test/unit/bin/unittest
        printf "* UNIT-API TESTS *\n"
        printf "******************\n"
        ${current_work_dir}/build/test/unit-api/bin/unit-api-test
        if [ $HIP_SUPPORT = "on" ]; then
          printf "* UNIT-HIP TESTS *\n"
          printf "******************\n"
          ${current_work_dir}/build/test/unit-hip/bin/unit-hip-test
        fi
# Invoke test script
        cd $current_work_dir/test/benchmark/
# Invoke profiling script
        ./runme.sh
# Test=OFF and Profile=ON (Build and profile the library)
    elif ( [ "$profiling" = "on" ] && [ -z $testing ] ) || ( [ "$testing" = "off" ] && [ "$profiling" = "on" ] ); then
      cd $current_work_dir/test/benchmark/
# Invoke profiling script
      ./runme.sh
    fi
  else #bench=on
    cd $current_work_dir/test/BLAS_benchmark_Convolution_Networks/
    ./runme.sh
  fi

#EXAMPLES
#Invoke examples script if --examples=on
  if [ "$examples" = "on" ]; then
    chmod +x $current_work_dir/examples/build.sh
    cd $current_work_dir/examples/
    ./build.sh
  fi
elif [ "$platform" = "nvcc" ]; then
    cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS="$copt -fPIC" -DCMAKE_INSTALL_PREFIX=/opt/rocm/hipblas $current_work_dir
    
    make -j$working_threads package $verbose
    make -j$working_threads $verbose
    
    cd $build_dir/packaging/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_INSTALL_PREFIX=/opt/rocm/hipblas $current_work_dir/packaging/    
    echo "${green}HIPBLAS Build Completed!${reset}"

  if  [ "$testing" = "on" ]; then
       cd $build_dir/test/ && cmake -DCMAKE_C_COMPILER=$cmake_c_compiler  -DCMAKE_CXX_COMPILER=$cmake_cxx_compiler -DCMAKE_CXX_FLAGS=-fPIC $current_work_dir/test/
     set +e
     make -j$working_threads
     ${current_work_dir}/build/test/unit-hip/bin/unit-hip-test
  fi 
fi
