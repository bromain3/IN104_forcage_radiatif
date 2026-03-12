cmake_minimum_required(VERSION 4.1)  # Cmake minimal version for compatibility
project(code_C C)  # project name and languages

set(CMAKE_C_STANDARD 23)  # C version (2023)

# Deactivating CURL stuff for improving compile time
set(BUILD_SHARED_LIBS OFF)
set(BUILD_CURL_EXE OFF)
set(BUILD_TESTING OFF)
set(CURL_DISABLE_DOCS ON)  # ajouter avant add_subdirectory(curl)
    # protocoles
set(CURL_DISABLE_FTP ON)
set(CURL_DISABLE_LDAP ON)
set(CURL_DISABLE_TELNET ON)
set(CURL_DISABLE_DICT ON)
set(CURL_DISABLE_TFTP ON)
set(CURL_DISABLE_FILE ON)
    # features
set(CURL_ZLIB OFF)
set(CURL_BROTLI OFF)
set(CURL_ZSTD OFF)
set(USE_NGHTTP2 OFF)
set(CURL_USE_LIBPSL OFF)

# activating the right library for SSL handling depending on the plateforme
if(WIN32)
    set(CURL_USE_SCHANNEL ON)   # native Windows library
    set(CURL_USE_OPENSSL OFF)
    set(CURL_USE_MBEDTLS OFF)
else()
    set(CURL_USE_OPENSSL ON)    # native unix like lib
    set(CURL_USE_MBEDTLS OFF)
    set(CURL_USE_SCHANNEL OFF)
endif()

add_subdirectory(curl)  # adding curl to the list of file to look in for libraries

add_executable(download download.c)  # defining a new executable and the files to compile for making it

target_link_libraries(download libcurl)  # linking curl libraries to the download code