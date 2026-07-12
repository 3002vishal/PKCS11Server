#include "pkcs11/pkcs11.h"
#include "PKCS11Library.h"
#include <iostream>
#include "TokenManager.h"
#include "SessionManager.h"
#include "ObjectManager.h"
#include "KeyManager.h"
#include "ObjectFinder.h"
#include "SignManager.h"
#include <iomanip>


int main()
{
	PKCS11Library pkcs11library;

	

	pkcs11library.load("C:\\Windows\\System32\\aetpkss1.dll");

	pkcs11library.initialize();

	ObjectFinder objectFinder(pkcs11library);

	TokenManager tokenManager(pkcs11library);

	SignManager signer(pkcs11library);

	auto tokens = tokenManager.getTokens();

	SessionManager sessionManager(pkcs11library);

	cout << "token info" << endl;

	for (auto i : tokens)
	{
		cout << i.label << endl;
	}

	auto session = sessionManager.openSession(tokens[0].slotId);

	sessionManager.login(session, "12345");

	ObjectManager objectManager(pkcs11library);

	//objectManager.listObjects(session);

	//pkcs11library.printMechanisms(tokens[0].slotId);





	KeyManager keyManager(pkcs11library);

	//keyManager.generateRSAKeyPair(session, "Vishal", "Vishal--001");

	//objectManager.listObjects(session);
	CK_OBJECT_HANDLE pirvateKeyHandle = objectFinder.findByLabel(
		session,
		CKO_PRIVATE_KEY,
		"Vishal"
	);

	if (pirvateKeyHandle != CK_INVALID_HANDLE)
		cout << "found handle: " << pirvateKeyHandle << endl;
	else
		cout << "object not found " << endl;

	string message = "hellow";
	vector<CK_BYTE> data(
		message.begin(),
		message.end()
	);
	vector<CK_BYTE> signature;

	signer.signData(
		session,
		pirvateKeyHandle,
		data,
		signature
	);

	cout << "\nSignature\n";

	for (CK_BYTE b : signature)
	{
		cout << hex
			<< setw(2)
			<< setfill('0')
			<< (int)b
			<< " ";
	}

	cout << dec << endl;





	sessionManager.logout(session);

	sessionManager.closeSession(session);

	return 0;

}
