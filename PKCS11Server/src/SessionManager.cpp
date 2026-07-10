#include "SessionManager.h"
#include <iostream>
using namespace std;

CK_SESSION_HANDLE SessionManager::openSession(CK_SLOT_ID slotId)
{
	CK_SESSION_HANDLE session;
    CK_RV rv =
        m_library.functions()->C_OpenSession(
            slotId,
            CKF_SERIAL_SESSION | CKF_RW_SESSION,
            nullptr,
            nullptr,
            &session);
	if (rv != CKR_OK)
	{
	     cout << "C_OpenSession failed with error: " << rv << endl;
		return CK_INVALID_HANDLE;
	}

	cout << "Session opened successfully. Session handle: " << session << endl;
	return session;

	
}

void SessionManager::closeSession(CK_SESSION_HANDLE session)
{
	m_library.functions()->C_CloseSession(session);
}

bool SessionManager::login(CK_SESSION_HANDLE session, const string& pin)
{
	CK_RV rv = m_library.functions()->C_Login(session,
		CKU_USER,
		reinterpret_cast<CK_UTF8CHAR_PTR>(const_cast<char*>(pin.c_str())),
		pin.length()

		);

	if (rv == CKR_OK)
		return true;

	if (rv == CKR_USER_ALREADY_LOGGED_IN)
	{
		cout << "User already logged in." << endl;
		return true;
	}

	cout << "C_Login failed with error: " << rv << endl;

	return false;

	
}

void SessionManager::logout(CK_SESSION_HANDLE session)
{
	m_library.functions()->C_Logout(session);
}


