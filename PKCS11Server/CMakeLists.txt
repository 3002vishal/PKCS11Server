cmake_minimum_required(VERSION 3.10)

add_executable(PKCS11Server
    src/main.cpp
 "include/pkcs11/pkcs11.h" "include/pkcs11/pkcs11f.h" "include/pkcs11/pkcs11t.h" "src/PCKS11Library.cpp"  "include/PKCS11Library.h"  "include/SessionManager.h" "include/CSRManager.h" "src/TokenManager.cpp" "src/SessionManager.cpp" "src/CSRManager.cpp" "src/ObjectManager.cpp" "src/KeyManager.cpp" "src/CertificateManager.cpp" "include/ObjectManager.h" "include/pkcs11/TokenManager.h")

target_include_directories(PKCS11Server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_property(TARGET PKCS11Server PROPERTY CXX_STANDARD 20)