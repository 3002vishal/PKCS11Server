#include "ObjectManager.h"
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

vector<CK_BYTE> ObjectManager::readAttribute(
	CK_SESSION_HANDLE session,
	CK_OBJECT_HANDLE object,
	CK_ATTRIBUTE_TYPE type)
{
	CK_ATTRIBUTE attr =
	{
		type , nullptr, 0
	};

	CK_RV rv = m_library.functions()->C_GetAttributeValue(
		session,
		object,
		&attr,
		1
	);

	if (rv != CKR_OK)
		return {};
	if (attr.ulValueLen == CK_UNAVAILABLE_INFORMATION)
		return {};

	vector<CK_BYTE> value(attr.ulValueLen);

	attr.pValue = value.data();
	attr.ulValueLen = static_cast<CK_ULONG>(value.size());

	rv = m_library.functions()->C_GetAttributeValue(
		session,
		object,
		&attr,
		1
	);
	if (rv != CKR_OK)
	{
		return {};
	}

	return value;

}

string objectClassToString(CK_OBJECT_CLASS objectClass) {
	switch (objectClass)
	{
	case CKO_DATA:
		return "Data";
	case CKO_CERTIFICATE:
		return "Certificate";
	case CKO_PUBLIC_KEY:
		return "Public Key";
	case CKO_PRIVATE_KEY:
		return "Private";
	case CKO_SECRET_KEY:
		return "Secret Key";
	default:
		return "Unknown";
	}
}

CK_OBJECT_CLASS ObjectManager::getObjectClass(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE object)
{
	auto data = readAttribute(
		session,
		object,
		CKA_CLASS
	);

	if (data.size() != sizeof(CK_OBJECT_CLASS))
		return (CK_OBJECT_CLASS)-1;
	 
	return *reinterpret_cast<CK_OBJECT_CLASS*>(data.data());

	
}

vector<CK_BYTE> ObjectManager::getBinaryAttribute(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE object, CK_ATTRIBUTE_TYPE type)
{
	
	return readAttribute(session, object, type);
}

void ObjectManager::printObject(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE object)
{
	auto objectClass = getObjectClass(session, object);

	auto label = getStringAttribute(session, object, CKA_LABEL);
	auto id = getBinaryAttribute(session, object, CKA_ID);

	cout << "===============================" << endl;
	cout << "Object Handle : " << object << endl;
	cout << "Class         : " << objectClassToString(objectClass) << endl;
	cout << "Label         : " << label << endl;

	cout << "ID            : ";

	for (CK_BYTE b : id)
	{
		cout << hex
			<< setw(2)
			<< setfill('0')
			<< (int)b
			<< " ";
	}

	cout << dec << endl;

	// ---------- Boolean Attributes ----------
	cout << "Token         : " << getBoolAttribute(session, object, CKA_TOKEN) << endl;
	cout << "Private       : " << getBoolAttribute(session, object, CKA_PRIVATE) << endl;
	cout << "Sensitive     : " << getBoolAttribute(session, object, CKA_SENSITIVE) << endl;
	cout << "Extractable   : " << getBoolAttribute(session, object, CKA_EXTRACTABLE) << endl;
	cout << "Encrypt       : " << getBoolAttribute(session, object, CKA_ENCRYPT) << endl;
	cout << "Decrypt       : " << getBoolAttribute(session, object, CKA_DECRYPT) << endl;
	cout << "Verify        : " << getBoolAttribute(session, object, CKA_VERIFY) << endl;
	cout << "Sign          : " << getBoolAttribute(session, object, CKA_SIGN) << endl;

	cout << "===============================" << endl;
}



bool ObjectManager::listObjects(CK_SESSION_HANDLE session)
{
	CK_RV rv = m_library.functions()->C_FindObjectsInit(
		session,
		nullptr,
		0
	);
	if (rv != CKR_OK)
	{
		cout << "C_FindObjectsInit failed with error: " << rv << endl;
		return false;
	}
	
	while (true)
	{
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
			
			break;
		}
		if (count == 0)
			break;
		printObject(session, object);

	}

	m_library.functions()->C_FindObjectsFinal(session);
	return true;


}


string ObjectManager::getStringAttribute(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE object, CK_ATTRIBUTE_TYPE type)
{
	auto value = readAttribute(
		session,
		object,
		type
	);

	return string(value.begin(), value.end());

}  

bool ObjectManager::getBoolAttribute(
	CK_SESSION_HANDLE session,
	CK_OBJECT_HANDLE object,
	CK_ATTRIBUTE_TYPE type)
{
	auto data = readAttribute(session, object, type);
	if (data.size() != sizeof(CK_BBOOL))
		return false;

	CK_BBOOL value = *reinterpret_cast<CK_BBOOL*>(data.data());
	return value == CK_TRUE;
}
