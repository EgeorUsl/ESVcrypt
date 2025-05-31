cmake_minimum_required(VERSION 3.16)
project(ESVcrypt VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(Qt6 COMPONENTS Core REQUIRED)
add_executable(ESVcrypt src/main.cpp src/core/utils.cpp)

target_include_directories(ESVcrypt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/algo
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hide-methods
  )  

find_package(OpenSSL REQUIRED) 
if(NOT OpenSSL_FOUND)
  message(FATAL_ERROR "OpenSSL not found. Please install OpenSSL.")
endif()

target_link_libraries(ESVcrypt ${OpenSSL_LIBRARIES})
target_link_libraries(ESVcrypt ${OPENSSL_CRYPTO_LIBRARY})

target_link_libraries(ESVcrypt Qt6::Core)

