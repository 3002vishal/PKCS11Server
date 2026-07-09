#include "pkcs11/pkcs11.h"
#include "PKCS11Library.h"
#include <iostream>
#include "TokenManager.h"
#include "SessionManager.h"
#include "ObjectManager.h"


int main()
{
    PKCS11Library pkcs11library;

    pkcs11library.load("C:\\Windows\\System32\\aetpkss1.dll");
       
    pkcs11library.initialize();

    TokenManager tokenManager(pkcs11library);

	auto tokens = tokenManager.getTokens();

	SessionManager sessionManager(pkcs11library);

	auto session = sessionManager.openSession(tokens[0].slotId);

    sessionManager.login(session, "12345");

	ObjectManager objectManager(pkcs11library);

	objectManager.listObjects(session);

   

	sessionManager.logout(session);

	sessionManager.closeSession(session);

    return 0;
}
