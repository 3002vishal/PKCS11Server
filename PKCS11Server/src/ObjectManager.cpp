#include "ObjectManager.h"
#include <iostream>
#include <vector>
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
		CK_OBJECT_HANDLE objectHandle;
		CK_ULONG objectCount = 0;

		rv = m_library.functions()->C_FindObjects(
			session,
			&objectHandle,
			1,
			&objectCount
		);

		if (rv != CKR_OK)
		{
			break;
		}

		if (objectCount == 0)
			break;


		CK_OBJECT_CLASS objectClass;

		CK_ATTRIBUTE attrbuteTemplate[] = {
			{ CKA_CLASS, &objectClass, sizeof(objectClass) }
		};

		rv = m_library.functions()->C_GetAttributeValue(
			session,
			objectHandle,
			attrbuteTemplate,
			1
		);

		if (rv != CKR_OK)
		{
			cout << "C_GetAttributeValue failed with error: " << rv << endl;
			return false;
		}

		cout << "Object Class: " << objectClassToString(objectClass) << endl;

		CK_ATTRIBUTE attr = { CKA_LABEL, nullptr, 0 };

		rv = m_library.functions()->C_GetAttributeValue(
			session,
			objectHandle,
			&attr,
			1
		);

		if (rv != CKR_OK)
		{
			cout << "C_GetAttributeValue failed with error: " << rv << endl;
			return false;
		}

		cout << "lenght of label: " << attr.ulValueLen << endl;

		vector<CK_CHAR> label(attr.ulValueLen);

		attr.pValue = label.data();

		attr.ulValueLen = label.size();

		rv = m_library.functions()->C_GetAttributeValue(
			session,
			objectHandle,
			&attr,
			1
		);

		if (rv != CKR_OK)
		{
			cout << "C_GetAttributeValue failed with error: " << rv << endl;
			return false;
		}

		cout << "Object Label: " << string(label.begin(), label.end()) << endl;
	}


	m_library.functions()->C_FindObjectsFinal(session);	

	if (rv != CKR_OK)
	{
		cout << "C_FindObjectsFinal failed with error: " << rv << endl;
		return false;
	}

	return true;

}