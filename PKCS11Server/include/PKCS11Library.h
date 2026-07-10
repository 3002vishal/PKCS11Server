#pragma once

#include <windows.h>
#include <string>

#define CK_PTR *

#define CK_DECLARE_FUNCTION(returnType, name) \
    returnType name

#define CK_DECLARE_FUNCTION_POINTER(returnType, name) \
    returnType (* name)

#define CK_CALLBACK_FUNCTION(returnType, name) \
    returnType (* name)

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#pragma pack(push, cryptoki, 1)
#include "pkcs11/pkcs11.h"
#pragma pack(pop, cryptoki)

class PKCS11Library {
 
public:
	PKCS11Library();
	~PKCS11Library();
	
	bool load(const std::string& dllPath);

	bool initialize();

	void finalize();

	bool listSlots();

	CK_FUNCTION_LIST_PTR functions() const;

	bool printTokenInfo();

	bool printMechanisms(CK_SLOT_ID slotId);

private: 

	HMODULE m_library ;

	CK_FUNCTION_LIST_PTR m_functions;

};