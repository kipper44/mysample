#pragma once
//#include <Winsvc.h>
#include "SingleEvent.h"

class CServiceStatus : public SERVICE_STATUS
{
public:
	explicit CServiceStatus(bool bServiceMode = true, DWORD ServiceType = SERVICE_WIN32_OWN_PROCESS, DWORD ControlsAccepted = SERVICE_ACCEPT_STOP);
	virtual ~CServiceStatus();
public:
	bool RegisterServiceCtrl(LPCTSTR ServiceName, LPHANDLER_FUNCTION_EX pHandlerProc, LPVOID pContext);
	bool BeginStateTransition(DWORD UltimateState, DWORD WaitHint = 50000);
	bool AdvanceStateTransition(DWORD WaitHint, DWORD CheckPoint = 0);
	bool EndStateTransition();

	void StateTransitionToStop();
protected:
	bool SetServiceStatus();

private:
	bool					m_bServiceMode;
	SERVICE_STATUS_HANDLE	m_hServiceStatus;
	TCHAR					m_szServiceName[MAX_PATH];

	CSingleEvent			m_Event;
};