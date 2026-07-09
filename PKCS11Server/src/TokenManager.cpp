#include "TokenManager.h"
#include <iostream>
#include <vector>
using namespace std;

string trimPKCS11String(const CK_UTF8CHAR* data, size_t lenght)
{
	string str(reinterpret_cast<const char*>(data), lenght);

	size_t end = str.find_last_not_of(' ');

	if (end == string::npos)
		return "";
	return str.substr(0, end + 1);
}

vector<TokenInfo> TokenManager::getTokens()
{
	CK_ULONG slotCount = 0;
	CK_RV rv = m_library.functions()->C_GetSlotList(CK_TRUE, nullptr, &slotCount);
	if (rv != CKR_OK)
	{
		std::cout << "Failed to get solt count. Error = " << rv << std::endl;
	
	}
	std::cout << "Number of Solts = " << slotCount << std::endl;
	std::vector<CK_SLOT_ID> slotList(slotCount);
	rv = m_library.functions()->C_GetSlotList(CK_TRUE, slotList.data(), &slotCount);
	if (rv != CKR_OK)
	{
		std::cout << "Failed to get slot list. Error = " << rv << std::endl;
	
	}
	vector<TokenInfo> tokens;

	for (CK_SLOT_ID slotId : slotList)
	{
		CK_TOKEN_INFO tokenInfo;

		rv = m_library.functions()->C_GetTokenInfo(slotId, &tokenInfo);

		if (rv != CKR_OK)
		{
			continue;
		}
		TokenInfo info;

		info.slotId = slotId;

		info.label = trimPKCS11String(tokenInfo.label , sizeof(tokenInfo.label));

		info.manufacturer = trimPKCS11String(tokenInfo.manufacturerID, sizeof(tokenInfo.manufacturerID));

		info.model = trimPKCS11String(tokenInfo.model, sizeof(tokenInfo.model));

		info.serialNumber = trimPKCS11String(tokenInfo.serialNumber, sizeof(tokenInfo.serialNumber));

		tokens.push_back(info);
		
	}
	return tokens;
} 
