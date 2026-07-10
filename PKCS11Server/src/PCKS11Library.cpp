#include "PKCS11Library.h"
#include <iostream>
#include <vector>

PKCS11Library::PKCS11Library()
{
	m_library = nullptr;
	m_functions = nullptr;
} 

bool PKCS11Library::printMechanisms(CK_SLOT_ID slotId)
{
	CK_ULONG count = 0;

	CK_RV  rv = m_functions->C_GetMechanismList(
		slotId,
		nullptr,
		&count
	);

	if (rv != CKR_OK)
	{
		std::cout << " C_GetMechanismList failed: " << rv << std::endl;
		return false;
	}

	std::vector<CK_MECHANISM_TYPE> mechanisms(count);

	rv = m_functions->C_GetMechanismList(
		slotId,
		mechanisms.data(),
		&count
	);

	if (rv != CKR_OK)
	{
		std::cout << "C_GetMechanismList failed: " << rv << std::endl;
		return false;

	}
	std::cout << "\nSupported Mechanisms\n";
	std::cout << "-----------------------------\n";

	for (auto mech : mechanisms)
	{
		std::cout << "Mechanism : 0x"
			<< std::hex
			<< mech
			<< std::dec
			<< std::endl;

		CK_MECHANISM_INFO info;

		rv = m_functions->C_GetMechanismInfo(
			slotId,
			mech,
			&info);

		if (rv == CKR_OK)
		{
			std::cout << "   Min Key Size : "
				<< info.ulMinKeySize << std::endl;

			std::cout << "   Max Key Size : "
				<< info.ulMaxKeySize << std::endl;

			std::cout << "   Flags        : 0x"
				<< std::hex
				<< info.flags
				<< std::dec
				<< std::endl;
		}

		std::cout << std::endl;
	}

	return true;

}

PKCS11Library::~PKCS11Library()
{
	if (m_library)
	{
		FreeLibrary(m_library);
		m_library = nullptr;
	}
} 
CK_FUNCTION_LIST_PTR PKCS11Library::functions() const
{
	return m_functions;
}


bool PKCS11Library::load(const std::string& dllPath)
{
	m_library = LoadLibraryA(dllPath.c_str());
	if (!m_library)
	{
		std::cout << "Failed to load library: " << dllPath << std::endl;
		return false;
	}
	std::cout << "DLL Loaded Successfully\n";
	return true; 
}

bool PKCS11Library::initialize()
{ 
	std::cout << "Getting C_GetFunctionLlist...\n";
	
	auto getFunctionList = reinterpret_cast<CK_C_GetFunctionList>(GetProcAddress(m_library, "C_GetFunctionList"));

	

	if (!getFunctionList)
	{
		std::cout << "Failed to get C_GetFunctionList function pointer.\n";
		return false;
	}
	std::cout << "Found C_GetFunctionList function pointer.\n";

	CK_RV rv = getFunctionList(&m_functions);

	//std::cout << "can you reach here?\n";;
	std::cout << "RV = " << rv << std::endl;
	std::cout << "m_functions = " << m_functions << std::endl;
	std::cout << "C_Initialize ptr = "
		<< reinterpret_cast<void*>(m_functions->C_Initialize)
		<< std::endl;

	if (rv != CKR_OK)
	{
		std::cout << "C_GetFunctionList failed with error: " << rv << std::endl;
		return false;
	}
	std::cout << "can you reach here?\n";
	rv = m_functions->C_Initialize(nullptr);
	std::cout << "can you reach here?\n";

	if (rv != CKR_OK && rv != CKR_CRYPTOKI_ALREADY_INITIALIZED) // if the library is already initialized, we can ignore this error
	{
		std::cout << "C_Initialize failed with error: " << rv << std::endl;
		return false;
	}

	std::cout << "C_Initialize succeeded.\n";


	return true;
}
bool PKCS11Library::listSlots()
{
	CK_ULONG slotCount = 0;

	CK_RV rv = m_functions->C_GetSlotList(CK_TRUE, nullptr, &slotCount);\

		if (rv != CKR_OK)
		{
			std::cout << "Failed to get solt count. Error = " << rv << std::endl;
			return false;
		}

	std::cout << "Number of Solts = " << slotCount << std::endl;
	std::vector<CK_SLOT_ID> slotList(slotCount);

	rv = m_functions->C_GetSlotList(CK_TRUE, slotList.data(), &slotCount);

	if (rv != CKR_OK)
	{
		std::cout << "Failed to get slot list. Error = " << rv << std::endl;
		return false;
	}

	for (CK_SLOT_ID slotId : slotList)
	{
		std::cout << "Slot ID: " << slotId << std::endl;
	}
	return true;
}

bool PKCS11Library::printTokenInfo()
{
	CK_ULONG slotCount = 0;
	CK_RV rv = m_functions->C_GetSlotList(CK_TRUE, nullptr, &slotCount);
	if (rv != CKR_OK)
	{
		std::cout << "Failed to get solt count. Error = " << rv << std::endl;
		return false;
	}
	std::cout << "Number of Solts = " << slotCount << std::endl;
	std::vector<CK_SLOT_ID> slotList(slotCount);
	rv = m_functions->C_GetSlotList(CK_TRUE, slotList.data(), &slotCount);
	if (rv != CKR_OK)
	{
		std::cout << "Failed to get slot list. Error = " << rv << std::endl;
		return false;
	}

	for (CK_SLOT_ID slotId : slotList)
	{
		std::cout << "Slot ID: " << slotId << std::endl;
		CK_TOKEN_INFO tokenInfo;
		rv = m_functions->C_GetTokenInfo(slotId, &tokenInfo);
		if (rv != CKR_OK)
		{
			std::cout << "Failed to get token info for slot " << slotId << ". Error = " << rv << std::endl;
			continue;
		}
		std::cout << "Token Label: " << std::string(reinterpret_cast<char*>(tokenInfo.label), sizeof(tokenInfo.label)) << std::endl;
		std::cout << "Manufacturer ID: " << std::string(reinterpret_cast<char*>(tokenInfo.manufacturerID), sizeof(tokenInfo.manufacturerID)) << std::endl;
		std::cout << "Model: " << std::string(reinterpret_cast<char*>(tokenInfo.model), sizeof(tokenInfo.model)) << std::endl;
		std::cout << "Serial Number: " << std::string(reinterpret_cast<char*>(tokenInfo.serialNumber), sizeof(tokenInfo.serialNumber)) << std::endl;
	}
	return true;
}