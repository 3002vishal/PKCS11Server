#include "SignManager.h"
#include <iostream>
using namespace std;

bool SignManager::signData(
	CK_SESSION_HANDLE session,
	CK_OBJECT_HANDLE privateKey,
	const vector<CK_BYTE>& data,
	vector<CK_BYTE>& signature
)
{
	CK_MECHANISM mechanism =
	{
		CKM_SHA256_RSA_PKCS,
		nullptr,
		0
	};

	CK_RV rv =
		m_library.functions()->C_SignInit(
			session,
			&mechanism,
			privateKey
		);
	if (rv != CKR_OK)
	{
		cout << "C_SignInit Failed: " << rv << endl;
		return false;
	}

	CK_ULONG signatureLength = 0;

	rv = m_library.functions()->C_Sign(
		session,
		const_cast<CK_BYTE*> (data.data()),
		static_cast<CK_ULONG> (data.size()),
		nullptr,
		&signatureLength
	);

	if (rv !=  CKR_OK)
	{
		cout << "Failed to get signature lenght :" << rv << endl;
		return false;

	}

	signature.resize(signatureLength);


	rv = m_library.functions()->C_Sign(
		session,
		const_cast<CK_BYTE*>(data.data()),
		static_cast<CK_ULONG> (data.size()),
	    signature.data(),
		&signatureLength
		);

	if (rv != CKR_OK)
	{
		cout << "Signing Failed :" << rv << endl;
		return false;
	}

	signature.resize(signatureLength);

	cout << "Signature Generated Successfully" << endl;
	cout << "Signature Size :" << signature.size() << "bytes" << endl;

	return true;

}

