

#include <ObjectFinder.h>
#include <iostream>
using namespace std;

ObjectFinder::ObjectFinder(PKCS11Library& library)
    : m_library(library)
{
}

CK_OBJECT_HANDLE ObjectFinder::findByLabel(
    CK_SESSION_HANDLE session,
    CK_OBJECT_CLASS objectClass,
     const string& label
)
{
    CK_ATTRIBUTE searchTemplate[]
    {
        { CKA_CLASS,
          &objectClass,
          sizeof(objectClass)
        },

        { CKA_LABEL,
          (void*)label.c_str(),
          (CK_ULONG)label.size()
        }
    };

    CK_RV rv = m_library.functions()->C_FindObjectsInit(
        session,
        searchTemplate,
        sizeof(searchTemplate) / sizeof(CK_ATTRIBUTE)
    );

    if (rv != CKR_OK)
        return CK_INVALID_HANDLE;
    CK_OBJECT_HANDLE object;

    CK_ULONG count = 0;

    rv = m_library.functions()->C_FindObjects(
        session,
        &object,
        1,
        &count
    );

    if (rv != CKR_OK)
    {
        cout << "did not fond the object";
    }

    rv = m_library.functions()->C_FindObjectsFinal(session);

    if (count == 0)
        return CK_INVALID_HANDLE;
    return object;

}