#include "stdafx.h"
#include "ServiceStatus.h"

#define SERV_ERROR(x)		OutputDebugString(TEXT("CServiceStatus ERROR :"));\
	OutputDebugString(x);

#define VERIFY(x) \
if (!(x)) \
	{ \
	SERV_ERROR(TEXT(#x)); \
	}


#define VERIFY_2(condition, ErrorCode) \
if (!(condition)) \
	{ \
	OutputDebugString(TEXT("CSingleEvent ERROR :"));	\
	OutputDebugString(TEXT(#condition));	\
	}


#define VERIFY_CHECKLASTERROR(x)	VERIFY_2(x, ::GetLastError())

CServiceStatus::CServiceStatus(bool bServiceMode, DWORD ServiceType, DWORD ControlsAccepted)
: m_bServiceMode(bServiceMode)
, m_hServiceStatus()
, m_Event(NULL, false, true)
{
	dwServiceType = ServiceType;
	dwCurrentState = SERVICE_START_PENDING;
	dwControlsAccepted = ControlsAccepted;
	dwWin32ExitCode = NO_ERROR;
	dwServiceSpecificExitCode = 0;
	dwCheckPoint = 0;
	dwWaitHint = 0;
}


CServiceStatus::~CServiceStatus()
{
}


bool CServiceStatus::RegisterServiceCtrl(LPCTSTR ServiceName, LPHANDLER_FUNCTION_EX pHandlerProc, LPVOID pContext)
{
	if (!m_bServiceMode)
	{
		return true;
	}

	m_hServiceStatus = ::RegisterServiceCtrlHandlerEx(ServiceName, pHandlerProc, pContext);

	return (0 != m_hServiceStatus);
}

bool CServiceStatus::BeginStateTransition(DWORD UltimateState, DWORD WaitHint)
{
	m_Event.Wait();

	DWORD OriginalState = dwCurrentState;

	switch (UltimateState)
	{
	case SERVICE_PAUSED:
		dwCurrentState = SERVICE_PAUSE_PENDING;
		break;

	case SERVICE_RUNNING:
		dwCurrentState = (SERVICE_PAUSED == dwCurrentState) ? SERVICE_CONTINUE_PENDING : SERVICE_START_PENDING;
		break;

	case SERVICE_STOPPED:
		dwCurrentState = SERVICE_STOP_PENDING;
		break;

	default:
		VERIFY((SERVICE_PAUSED == UltimateState) || (SERVICE_RUNNING == UltimateState) || (SERVICE_STOPPED == UltimateState));
		m_Event.SetEvent();
		return false;
	}

	dwWin32ExitCode = NO_ERROR;
	dwServiceSpecificExitCode = 0;
	dwCheckPoint = 1;
	dwWaitHint = WaitHint;

	if (!SetServiceStatus())
	{
		dwCurrentState = OriginalState;
		m_Event.SetEvent();
		return false;
	}

	return true;
}

bool CServiceStatus::AdvanceStateTransition(DWORD WaitHint, DWORD CheckPoint)
{
	dwCheckPoint = (0 == CheckPoint) ? (dwCheckPoint + 1) : CheckPoint;
	dwWaitHint = WaitHint;

	dwWin32ExitCode = NO_ERROR;
	dwServiceSpecificExitCode = 0;

	return SetServiceStatus();
}

bool CServiceStatus::EndStateTransition()
{
	switch (dwCurrentState)
	{
	case SERVICE_PAUSE_PENDING:
		dwCurrentState = SERVICE_PAUSED;
		break;

	case SERVICE_CONTINUE_PENDING:
	case SERVICE_START_PENDING:
		dwCurrentState = SERVICE_RUNNING;
		break;

	case SERVICE_STOP_PENDING:
		dwCurrentState = SERVICE_STOPPED;
		break;

	default:
		VERIFY((SERVICE_PAUSE_PENDING == dwCurrentState) || (SERVICE_CONTINUE_PENDING == dwCurrentState) ||
			(SERVICE_START_PENDING == dwCurrentState) || (SERVICE_STOP_PENDING == dwCurrentState));
		m_Event.SetEvent();
		return false;
	}

	dwWin32ExitCode = NO_ERROR;
	dwServiceSpecificExitCode = 0;
	dwCheckPoint = 0;
	dwWaitHint = 0;

	bool bEndTransitionResult = SetServiceStatus();

	m_Event.SetEvent();

	return bEndTransitionResult;
}

void CServiceStatus::StateTransitionToStop()
{
	switch (dwCurrentState)
	{
	case SERVICE_CONTINUE_PENDING:
	case SERVICE_PAUSE_PENDING:
	case SERVICE_START_PENDING:
		EndStateTransition();
		BeginStateTransition(SERVICE_STOPPED);
		EndStateTransition();
		break;

	case SERVICE_PAUSED:
	case SERVICE_RUNNING:
		BeginStateTransition(SERVICE_STOPPED);
		EndStateTransition();
		break;

	case SERVICE_STOP_PENDING:
		EndStateTransition();
		break;
	}
}

bool CServiceStatus::SetServiceStatus()
{
	if (!m_bServiceMode)
	{
		return true;
	}

	bool bSetStatusResult = false;

	VERIFY_CHECKLASTERROR(false != (bSetStatusResult = (FALSE != ::SetServiceStatus(m_hServiceStatus, this))));

	return bSetStatusResult;
}