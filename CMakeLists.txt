cmake_minimum_required(VERSION 3.8)
project(Raspberry-LCD-TIC149)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(NOT EXISTS ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE "debug")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -fPIC -pipe -fomit-frame-pointer -funroll-all-loops -s")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")

option(USE_DEFAULT_INSTALL_PREFIX "Use default install prefix 'project_dir/target'?" ON)
option(ENABLE_SANITIZERS "Build with sanitizes: leak, address?" OFF)

if (${ENABLE_SANITIZERS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -fsanitize=address")
endif()

set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")
set(CODE_DIR "${PROJECT_DIR}/code")

if (${USE_DEFAULT_INSTALL_PREFIX})
    set(CMAKE_INSTALL_PREFIX "${PROJECT_DIR}/target")
endif()

set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/..")

find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(TinyXML REQUIRED)

############# BEGIN EASY_BMP ##############
set(EASY_BMP easy_bmp)

set(EASY_BMP_DIR "${CODE_DIR}/EasyBMP")
file(GLOB EASY_BMP_SOURCES
    "${EASY_BMP_DIR}/*.h"
    "${EASY_BMP_DIR}/*.hpp"
    "${EASY_BMP_DIR}/*.cc"
    "${EASY_BMP_DIR}/*.cpp")

add_library(${EASY_BMP} STATIC ${EASY_BMP_SOURCES})
############# END EASY_BMP ##############


############# BEGIN DISPLAY ##############
set(DISPLAY display)
file(GLOB DISPLAY_SRC
    "${CODE_DIR}/*.h"
    "${CODE_DIR}/*.hpp"
    "${CODE_DIR}/*.cc"
    "${CODE_DIR}/*.cpp"
)

set(COMMON_LINK_LIBRARIES
    -Wl,-Bdynamic
        Threads::Threads
        -ldl
        -lrt
)

add_executable(${DISPLAY} ${DISPLAY_SRC})
target_link_libraries(${DISPLAY} 
    ${COMMON_LINK_LIBRARIES}
    ${EASY_BMP}
    -lcurl
    -ltinyxml)
############# END DISPLAY ##############


install(TARGETS
    ${DISPLAY}
    DESTINATION .)