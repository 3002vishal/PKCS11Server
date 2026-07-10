cmake_minimum_required(VERSION 3.10)

project(PKCS11Server)

add_executable(
    PKCS11Server
    src/main.cpp
    src/PCKS11Library.cpp
    src/TokenManager.cpp
    src/SessionManager.cpp
    src/CSRManager.cpp
    src/ObjectManager.cpp
    src/KeyManager.cpp
    src/CertificateManager.cpp
)

target_include_directories(PKCS11Server PRIVATE
    include
)

set_property(TARGET PKCS11Server PROPERTY CXX_STANDARD 20)