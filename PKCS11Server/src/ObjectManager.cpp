#include "ObjectManager.h"
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

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
	CK_OBJECT_CLASS objectClass;

	CK_ATTRIBUTE attr = { CKA_CLASS, &objectClass, sizeof(objectClass) };

	CK_RV rv = m_library.functions()->C_GetAttributeValue(
		session,
		object,
		&attr,
		1
	);

	if (rv != CKR_OK)
	{
		return (CK_OBJECT_CLASS) -1 ;
	}

	return objectClass;

	
}

vector<CK_BYTE> ObjectManager::getBinaryAttribute(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE object, CK_ATTRIBUTE_TYPE type)
{
	CK_ATTRIBUTE attr = { type, nullptr, 0 };

	CK_RV rv = m_library.functions()->C_GetAttributeValue(
		session,
		object,
		&attr,
		1
	);

	if (rv != CKR_OK)
	{

		return {};
	}

	vector<CK_BYTE> value(attr.ulValueLen);

	attr.pValue = value.data();
	attr.ulValueLen = value.size();

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

void ObjectManager::printObject(CK_SESSION_HANDLE session, CK_OBJECT_HANDLE object)
{

	auto objectClass = getObjectClass(session, object);

	auto label = getStringAttribute(session, object, CKA_LABEL);
	auto id = getBinaryAttribute(session, object, CKA_ID);

	cout << "===============================" << endl;
	cout << "Object Handle: " << object << endl;
	cout << "Class: " << objectClassToString(objectClass) << endl;
	cout << "Label: " << label << endl;
	cout << "ID: ";

	for (CK_BYTE b : id)
	{
		cout << hex
			<< setw(2)
			<< setfill('0')
			<< (int)b
			<< " ";
		
	}

	cout<< "======================="<< endl;
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
	CK_ATTRIBUTE attr = { type, nullptr, 0 };

	CK_RV rv = m_library.functions()->C_GetAttributeValue(
		session,
		object,
		&attr,
		1
	);

	if (rv != CKR_OK)
	{
		
		return "";
	}

	vector<CK_CHAR> value(attr.ulValueLen);

	attr.pValue = value.data();

	attr.ulValueLen = value.size();

	rv = m_library.functions()->C_GetAttributeValue(
		session,
		object,
		&attr,
		1
	);
	if (rv != CKR_OK)
	{

		return "";
	}
	
	return string(value.begin(), value.end());


}

//bool ObjectManager::listObjects(CK_SESSION_HANDLE session)
//{
//
//	
//
//	CK_RV rv = m_library.functions()->C_FindObjectsInit(
//		session,
//		nullptr,
//		0
//	);
//
//	if (rv != CKR_OK)
//	{
//		cout << "C_FindObjectsInit failed with error: " << rv << endl;
//		return false;
//	}
//
//	while (true)
//	{
//		CK_OBJECT_HANDLE objectHandle;
//		CK_ULONG objectCount = 0;
//
//		rv = m_library.functions()->C_FindObjects(
//			session,
//			&objectHandle,
//			1,
//			&objectCount
//		);
//
//		if (rv != CKR_OK)
//		{
//			break;
//		}
//
//		if (objectCount == 0)
//			break;
//
//
//		CK_OBJECT_CLASS objectClass;
//
//		CK_ATTRIBUTE attrbuteTemplate[] = {
//			{ CKA_CLASS, &objectClass, sizeof(objectClass) }
//		};
//
//		rv = m_library.functions()->C_GetAttributeValue(
//			session,
//			objectHandle,
//			attrbuteTemplate,
//			1
//		);
//
//		if (rv != CKR_OK)
//		{
//			cout << "C_GetAttributeValue failed with error: " << rv << endl;
//			return false;
//		}
//
//		cout << "Object Class: " << objectClassToString(objectClass) << endl;
//
//		CK_ATTRIBUTE attr = { CKA_LABEL, nullptr, 0 };
//
//		rv = m_library.functions()->C_GetAttributeValue(
//			session,
//			objectHandle,
//			&attr,
//			1
//		);
//
//		if (rv != CKR_OK)
//		{
//			cout << "C_GetAttributeValue failed with error: " << rv << endl;
//			return false;
//		}
//
//		cout << "lenght of label: " << attr.ulValueLen << endl;
//
//		vector<CK_CHAR> label(attr.ulValueLen);
//
//		attr.pValue = label.data();
//
//		attr.ulValueLen = label.size();
//
//		rv = m_library.functions()->C_GetAttributeValue(
//			session,
//			objectHandle,
//			&attr,
//			1
//		);
//
//		if (rv != CKR_OK)
//		{
//			cout << "C_GetAttributeValue failed with error: " << rv << endl;
//			return false;
//		}
//
//		cout << "Object Label: " << string(label.begin(), label.end()) << endl;
//	}
//
//
//	m_library.functions()->C_FindObjectsFinal(session);	
//
//	if (rv != CKR_OK)
//	{
//		cout << "C_FindObjectsFinal failed with error: " << rv << endl;
//		return false;
//	}
//
//	return true;
//
//}