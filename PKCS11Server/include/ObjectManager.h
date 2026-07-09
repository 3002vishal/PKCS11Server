#pragma once 

#include "PKCS11Library.h"

class ObjectManager
{
public:
	ObjectManager(PKCS11Library& library) : m_library(library) {}
	bool listObjects(CK_SESSION_HANDLE session);
private:
	PKCS11Library& m_library;
};