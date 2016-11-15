#include "stdafx.h"
#include "ServiceModule.h"
#include "ServiceStatus.h"

#define SERV_ERROR(x)		OutputDebugString(TEXT("CServiceModule ERROR :"));\
	OutputDebugString(x);OutputDebugString(TEXT("\n"));

#define VERIFY(x) \
if (!(x)) \
	{ \
	SERV_ERROR(TEXT(#x)); \
	}


#define VERIFY_2(condition, ErrorCode) \
if (!(condition)) \
	{ \
	OutputDebugString(TEXT("CServiceModule ERROR :"));	\
	OutputDebugString(TEXT(#condition));	\
	OutputDebugString("\n");	 \
	}


#define VERIFY_CHECKLASTERROR(x)	VERIFY_2(x, ::GetLastError())

CServiceModule  * CServiceModule::m_pServer = NULL;

CServiceModule::CServiceModule()
	: m_bServiceMode(false)
	, m_bRunning(false)
{
}


CServiceModule::~CServiceModule()
{
}


bool CServiceModule::Start(bool bServiceMode)
{
	SetMode(bServiceMode);
	m_pServer = this;
	return ServiceCtrlDispatcher();
}


bool CServiceModule::ServiceCtrlDispatcher()
{
	// 서비스 모드가 아닌 경우에는 서비스 메인 함수를 바로 호출
	if (!m_bServiceMode)
	{
		CServiceModule::ServiceStart(__argc, __argv);
		return true;
	}

	// 서비스 모드인 경우에는 서비스 시작시킴
	bool bStartServiceSuccesssful = false;

	SERVICE_TABLE_ENTRY ServiceTable[] = {
		{ TEXT("SeedWar Server"), CServiceModule::ServiceProc },
		{ NULL, NULL }
	};

	VERIFY_CHECKLASTERROR(FALSE != (bStartServiceSuccesssful = (FALSE != ::StartServiceCtrlDispatcher(ServiceTable))));

	return bStartServiceSuccesssful;
}

void WINAPI CServiceModule::ServiceProc(DWORD Argc, LPTSTR*ppArgv)
{
	m_pServer->ServiceStart(Argc, ppArgv);
}


DWORD WINAPI CServiceModule::HandlerEx(DWORD Control, DWORD EventType, LPVOID pEventData, LPVOID pContext)
{
	return m_pServer->HandlerMain(Control, EventType, pEventData, reinterpret_cast<CServiceStatus*>(pContext));
}

void CServiceModule::ServiceStart(DWORD Argc, LPTSTR*ppArgv)
{

	CServiceStatus ServiceStatus(m_bServiceMode);

	if (ServiceStatus.RegisterServiceCtrl(TEXT(""), HandlerEx, &ServiceStatus))
	{
		if (ServiceStatus.BeginStateTransition(SERVICE_RUNNING))
		{
			// 서버 초기화
			if (m_pServer->Initialize(Argc, ppArgv))
			{
				if (ServiceStatus.EndStateTransition())
				{
					m_bRunning = true;
					// 서버 메인
					m_pServer->Main();
				}

				// 서버 정리
				m_pServer->Cleanup();
			}
		}
	}

	ServiceStatus.StateTransitionToStop();
}

DWORD CServiceModule::HandlerMain(DWORD Control, DWORD EventType, LPVOID pEventData, CServiceStatus* pServiceStatus)
{
	//SERV_ERROR("DWORD CService::HandlerMain()");
	DWORD Result = ERROR_CALL_NOT_IMPLEMENTED;

	switch (Control)
	{
	case SERVICE_CONTROL_CONTINUE:
		Result = OnContinue(pServiceStatus);
		break;

	case SERVICE_CONTROL_INTERROGATE:
		Result = OnInterrogate(pServiceStatus);
		break;

	case SERVICE_CONTROL_NETBINDADD:
		Result = OnNetBindAdd(pServiceStatus);
		break;

	case SERVICE_CONTROL_NETBINDDISABLE:
		Result = OnNetBindDisable(pServiceStatus);
		break;

	case SERVICE_CONTROL_NETBINDENABLE:
		Result = OnNetBindEnable(pServiceStatus);
		break;

	case SERVICE_CONTROL_NETBINDREMOVE:
		Result = OnNetBindRemove(pServiceStatus);
		break;

	case SERVICE_CONTROL_PARAMCHANGE:
		Result = OnParamChange(pServiceStatus);
		break;

	case SERVICE_CONTROL_PAUSE:
		Result = OnPause(pServiceStatus);
		break;

	case SERVICE_CONTROL_SHUTDOWN:
		Result = OnShutdown(pServiceStatus);
		break;

	case SERVICE_CONTROL_STOP:
		Result = OnStop(pServiceStatus);
		break;

	case SERVICE_CONTROL_DEVICEEVENT:
		Result = OnDeviceEvent(pServiceStatus, EventType, pEventData);
		break;

	case SERVICE_CONTROL_HARDWAREPROFILECHANGE:
		Result = OnHardwareProfileChange(pServiceStatus, EventType);
		break;

	case SERVICE_CONTROL_POWEREVENT:
		Result = OnPowerEvent(pServiceStatus, EventType, pEventData);
		break;

	default:
		if ((Control >= 128) && (Control <= 255))
		{
			Result = OnCustomHandlerEx(pServiceStatus, Control);
		}
		break;
	}

	return Result;
}

DWORD CServiceModule::OnStop(CServiceStatus* pServiceStatus)
{
	m_bRunning = false;
	
	if (FALSE == pServiceStatus->BeginStateTransition(SERVICE_STOPPED))
	{
		return ERROR_CALL_NOT_IMPLEMENTED;
	}

	m_pServer->MakeEnd();

	//SERV_ERROR("CServiceModule::OnStop");

	return NO_ERROR;
}

void CServiceModule::SetMode(bool bServiceMode )
{
	m_bServiceMode = bServiceMode;
}