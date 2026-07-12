#pragma once

#include "PKCS11Library.h"
#include <string>
#include <vector>

class ObjectFinder
{
public:

    ObjectFinder(PKCS11Library& library);

    CK_OBJECT_HANDLE findByLabel(
        CK_SESSION_HANDLE session,
        CK_OBJECT_CLASS objectClass,
        const std::string& label);

    CK_OBJECT_HANDLE findById(
        CK_SESSION_HANDLE session,
        CK_OBJECT_CLASS objectClass,
        const std::vector<CK_BYTE>& id);

private:

    PKCS11Library& m_library;
};