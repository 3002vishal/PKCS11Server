

#include "KeyManager.h"

#include <iostream>
#include <vector>

using namespace std;



bool KeyManager::generateRSAKeyPair(
    CK_SESSION_HANDLE session,
     const std::string& label,
    const std::string& id)
{
    CK_MECHANISM mechanism =
    {
        CKM_RSA_PKCS_KEY_PAIR_GEN,
        nullptr,
        0
    };

    CK_BBOOL ckTrue = CK_TRUE;
    CK_BBOOL ckFalse = CK_FALSE;

    CK_ULONG modulusBits = 2048;

    CK_BYTE publicExponent[] =
    {
        0x01,
        0x00,
        0x01
    };

    CK_OBJECT_CLASS publicClass = CKO_PUBLIC_KEY;
    CK_OBJECT_CLASS privateClass = CKO_PRIVATE_KEY;

    std::vector<CK_BYTE> idBytes(id.begin(), id.end());

    CK_ATTRIBUTE publicTemplate[] =
    {
        { CKA_CLASS, &publicClass, sizeof(publicClass) },

        { CKA_TOKEN, &ckTrue, sizeof(ckTrue) },

        { CKA_PRIVATE, &ckFalse, sizeof(ckFalse) },

        { CKA_LABEL,
            (void*)label.c_str(),
            (CK_ULONG)label.size()
        },

        { CKA_ID,
            idBytes.data(),
            (CK_ULONG)idBytes.size()
        },

        { CKA_MODULUS_BITS,
            &modulusBits,
            sizeof(modulusBits)
        },

        { CKA_PUBLIC_EXPONENT,
            publicExponent,
            sizeof(publicExponent)
        },

        { CKA_VERIFY,
            &ckTrue,
            sizeof(ckTrue)
        }
    };

    CK_ATTRIBUTE privateTemplate[] =
    {
        { CKA_CLASS, &privateClass, sizeof(privateClass) },

        { CKA_TOKEN, &ckTrue, sizeof(ckTrue) },

        { CKA_PRIVATE, &ckTrue, sizeof(ckTrue) },

        { CKA_LABEL,
            (void*)label.c_str(),
            (CK_ULONG)label.size()
        },

        { CKA_ID,
            idBytes.data(),
            (CK_ULONG)idBytes.size()
        },

        { CKA_SIGN,
            &ckTrue,
            sizeof(ckTrue)
        },

        { CKA_DECRYPT,
            &ckTrue,
            sizeof(ckTrue)
        },

        { CKA_SENSITIVE,
            &ckTrue,
            sizeof(ckTrue)
        },

        { CKA_EXTRACTABLE,
            &ckFalse,
            sizeof(ckFalse)
        }
    };

    CK_OBJECT_HANDLE publicKey = CK_INVALID_HANDLE;
    CK_OBJECT_HANDLE privateKey = CK_INVALID_HANDLE;

    CK_RV rv =
        m_library.functions()->C_GenerateKeyPair(
            session,
            &mechanism,

            publicTemplate,
            sizeof(publicTemplate) / sizeof(CK_ATTRIBUTE),

            privateTemplate,
            sizeof(privateTemplate) / sizeof(CK_ATTRIBUTE),

            &publicKey,
            &privateKey
        );

    if (rv != CKR_OK)
    {
        cout << "Key Generation Failed." << endl;
        cout << "CK_RV = " << rv << endl;
        return false;
    }

    cout << endl;
    cout << "==============================" << endl;
    cout << "RSA Key Pair Generated" << endl;
    cout << "Public Handle  : " << publicKey << endl;
    cout << "Private Handle : " << privateKey << endl;
    cout << "==============================" << endl;

    return true;
}