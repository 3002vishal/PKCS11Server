
#include "PKCS11Library.h"

class SessionManager
{
public:

	SessionManager(PKCS11Library& library) : m_library(library) {}

	CK_SESSION_HANDLE openSession(CK_SLOT_ID slotId);

	bool login(CK_SESSION_HANDLE session, const std::string& pin);

	void logout(CK_SESSION_HANDLE session);

	void closeSession(CK_SESSION_HANDLE session);



private : 
	 
	PKCS11Library& m_library;


};