cmake_minimum_required(VERSION 3.16)
project(FileEncryptor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_executable(FileEncryptor main.cpp)

target_link_libraries(FileEncryptor Qt6::Core)

