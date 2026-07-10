#pragma once 

#include "PKCS11Library.h"
#include <string>
#include <vector>
using namespace std;

class ObjectManager
{
public:
	ObjectManager(PKCS11Library& library) : m_library(library) {}
	bool listObjects(CK_SESSION_HANDLE session);

	bool getBoolAttribute(
		CK_SESSION_HANDLE session,
		CK_OBJECT_HANDLE object,
		CK_ATTRIBUTE_TYPE type);
	CK_OBJECT_CLASS getObjectClass(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE objectHandle);

	string getStringAttribute(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE objectHandle, CK_ATTRIBUTE_TYPE attributeType);

	vector<CK_BYTE> getBinaryAttribute(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE objectHandle, CK_ATTRIBUTE_TYPE attributeType);

	void printObject(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE object);
private:
	PKCS11Library& m_library;

	vector<CK_BYTE> readAttribute(
		CK_SESSION_HANDLE session,
		CK_OBJECT_HANDLE object,
		CK_ATTRIBUTE_TYPE type
	);

	
};