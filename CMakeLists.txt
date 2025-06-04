cmake_minimum_required(VERSION 3.16)
project(ESVcrypt VERSION 0.1.1 LANGUAGES CXX)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Path to Windows Qt and OpenSSL libraries:
set(Qt6_DIR "/path/to/qt6/windows/lib/cmake/Qt6")
set(OPENSSL_ROOT_DIR "/path/to/openssl/windows")

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ESVcrypt
    src/main.cpp
    src/core/utils.cpp
    src/gui/gui.cpp
)

target_include_directories(ESVcrypt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/algo
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hide-methods
)

find_package(Qt6 COMPONENTS Core Widgets Gui REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(ESVcrypt
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    OpenSSL::SSL
    OpenSSL::Crypto
)
