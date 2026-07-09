#pragma once 

#include "PKCS11Library.h"
#include <vector>
#include <string>
using namespace std;

struct TokenInfo
{
	CK_SLOT_ID slotId;

	string label;

	string manufacturer;

	string model;

	string serialNumber;

};

class TokenManager
{
public:
	TokenManager(PKCS11Library& library) : m_library(library) 
	{
		
	}

	

	vector<TokenInfo> getTokens();

private :
	PKCS11Library& m_library;
};