cmake_minimum_required(VERSION 4.1)  # Cmake minimal version for compatibility
project(code_C C)  # project name and languages

set(CMAKE_C_STANDARD 23)  # C version (2023)

# Deactivating
set(BUILD_SHARED_LIBS OFF)
set(BUILD_CURL_EXE OFF)
set(BUILD_TESTING OFF)

set(CURL_USE_LIBPSL OFF)
set(CURL_ZLIB OFF)
set(CURL_BROTLI OFF)
set(CURL_ZSTD OFF)
set(USE_NGHTTP2 OFF)
set(USE_LIBIDN2 OFF)

add_subdirectory(curl)  # adding curl to the list of file to look in for libraries

add_executable(download download.c)  # defining a new executable and the files to compile for making it

target_link_libraries(download libcurl)  # linking curl libraries to the download code