#pragma once
#include "PKCS11Library.h"
#include <vector>
using namespace std;

class SignManager
{
public:
	SignManager(PKCS11Library& library): m_library(library){}

	bool signData(
		CK_SESSION_HANDLE session,
		CK_OBJECT_HANDLE privateKey,
		const vector<CK_BYTE>& data,
		vector<CK_BYTE>& signature
	);

private:
	PKCS11Library& m_library;
};