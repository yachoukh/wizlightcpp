cmake_minimum_required(VERSION 3.0.0)
project(wizlightcpp VERSION 1.0.0)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror -Wall -std=c++17")
SET(CMAKE_CXX_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/thirdparty)

add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/src/wizControl.cpp
    ${CMAKE_SOURCE_DIR}/src/log.cpp
    ${CMAKE_SOURCE_DIR}/src/udp.cpp
    ${CMAKE_SOURCE_DIR}/src/bulb.cpp 
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Jansson JSON library
find_package(Jansson REQUIRED) 
target_link_libraries(${PROJECT_NAME}
    jansson
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
