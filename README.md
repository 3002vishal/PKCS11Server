# PKCS11 Server

A C++ PKCS#11 integration project that provides reusable components for working with cryptographic tokens and HSM-style devices.

## Capabilities

The codebase separates common PKCS#11 operations into focused components:

- PKCS#11 library loading
- Token discovery
- Session management
- Key management
- Object discovery and management
- CSR generation
- Certificate handling
- Signing operations

## Structure

```text
PKCS11Server/
├── include/
│   ├── PKCS11Library.h
│   ├── TokenManager.h
│   ├── SessionManager.h
│   ├── KeyManager.h
│   ├── ObjectFinder.h
│   ├── ObjectManager.h
│   ├── CSRManager.h
│   ├── CertificateManager.h
│   └── SignManager.h
└── src/
    ├── PCKS11Library.cpp
    ├── TokenManager.cpp
    ├── SessionManager.cpp
    ├── KeyManager.cpp
    ├── ObjectFinder.cpp
    ├── ObjectManager.cpp
    ├── CSRManager.cpp
    ├── CertificateManager.cpp
    └── SignManager.cpp
```

## Build

The project uses CMake.

```bash
cmake -S . -B build
cmake --build build
```

A compatible vendor PKCS#11 library and connected token/HSM are required for hardware-backed operations.

## Security Model

Private keys are intended to remain inside the cryptographic device while applications request operations such as signing through PKCS#11.

## Focus

This repository demonstrates lower-level security engineering with **C++, PKCS#11, hardware-backed keys, certificate workflows, and modular cryptographic middleware design**.
