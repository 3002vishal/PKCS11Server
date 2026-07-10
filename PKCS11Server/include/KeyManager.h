#pragma once 
#include "PKCS11Library.h"
#include <string>
using namespace std;

class KeyManager
{
public:
	KeyManager(PKCS11Library& library) : m_library(library) {}

	bool generateRSAKeyPair(CK_SESSION_HANDLE session,const  string& lable,const string& id);
private:

	PKCS11Library& m_library;


};
