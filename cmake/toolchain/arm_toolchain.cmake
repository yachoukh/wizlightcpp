# CMAKE_SYSTEM_NAME is the CMake-identifier of the target platform to build for.
set(CMAKE_SYSTEM_NAME Linux)
# CMAKE_SYSTEM_PROCESSOR is the CMake-identifier of the target architecture to build for
set(CMAKE_SYSTEM_PROCESSOR arm)


set(TOOLCHAIN_PREFIX /home/devel/gcc-4.7-linaro-rpi-gnueabihf)
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PREFIX}/arm-linux-gnueabihf-gcc/rootfs)
set(CMAKE_SYSROOT ${TOOLCHAIN_PREFIX}/arm-linux-gnueabihf-gcc/rootfs)

# CMAKE_<LANG>_COMPILER variables may be set to full paths, or to names of compilers to search for in standard locations.
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}/bin/arm-linux-gnueabihf-g++)
set(CMAKE_CXX_FLAGS "-Werror -Wall" CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)