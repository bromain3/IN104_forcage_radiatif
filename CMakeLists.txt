cmake_minimum_required(VERSION 3.25.1)  # Cmake minimal version for compatibility
project(code_C C)  # project name and languages

set(CMAKE_C_STANDARD 23)  # C version (2023)

# Deactivating CURL stuff for improving compile time
set(BUILD_SHARED_LIBS        OFF CACHE BOOL "" FORCE)
set(BUILD_CURL_EXE           OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING            OFF CACHE BOOL "" FORCE)
set(CURL_DISABLE_TESTS       ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_LIBPSL     ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAP       ON  CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAPS      ON  CACHE BOOL "" FORCE)
set(USE_NGHTTP2             OFF CACHE BOOL "" FORCE)
set(CURL_BROTLI             OFF CACHE BOOL "" FORCE)
set(CURL_ZSTD               OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBSSH2        OFF CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL         OFF CACHE BOOL "" FORCE)

# each OS has a builtin SSL system lib that we can use
if(WIN32)
    # Windows : SChannel is builtin
    set(CURL_USE_SCHANNEL   ON  CACHE BOOL "" FORCE)
    set(CURL_USE_OPENSSL    OFF CACHE BOOL "" FORCE)
elseif(APPLE)
    # macOS : Secure Transport is builtin
    set(CURL_USE_SECTRANSP  ON  CACHE BOOL "" FORCE)
    set(CURL_USE_OPENSSL    OFF CACHE BOOL "" FORCE)
else()
    # Linux : OpenSSL, available in each distribution (at least it should be)
    set(CURL_USE_OPENSSL    ON  CACHE BOOL "" FORCE)
endif()


# executable
add_subdirectory(curl)  # adding curl to the list of file to look in for libraries

add_executable(download dataDownloading/download.c dataDownloading/spe_ids_binding.h)  # defining a new executable and the files to compile for making it

target_link_libraries(download PRIVATE CURL::libcurl)  # linking curl libraries to the download code
