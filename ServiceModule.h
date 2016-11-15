#pragma once
#include <memory>

#define DBGPRNT(x)	OutputDebugString(TEXT("SERVICE DEBUG:")); \
	OutputDebugString(x);OutputDebugString(TEXT("\n"));
	

class CServiceStatus;
class CServiceModule
{
public:
	explicit CServiceModule();
	virtual ~CServiceModule();

public:
	bool Start(bool bServiceMode = true);
	static void WINAPI ServiceProc(DWORD Argc, LPTSTR* ppArgv);

	static DWORD WINAPI HandlerEx(DWORD Control, DWORD EventType, LPVOID pEventData, LPVOID pContext);


	bool ServiceCtrlDispatcher();
	void ServiceStart(DWORD Argc, LPTSTR*ppArgv);

	DWORD HandlerMain(DWORD Control, DWORD EventType, LPVOID pEventData, CServiceStatus* pServiceStatus);

	virtual bool Initialize(DWORD Argc, LPTSTR*ppArgv) { return true; };
	virtual void Main() 
	{
		if (false == GetMode())
		{
			//키 입력 대기
			int i = getchar();
		}
		else
		{
			while (GetRunning())
			{
				Sleep(1000);
			}
		}
	};
	virtual void MakeEnd() {};
	virtual void Cleanup() {};

	bool GetRunning() { return m_bRunning; }
	bool GetMode() { return m_bServiceMode; }
	void SetMode(bool bServiceMode = true);

protected:
	virtual DWORD OnContinue(CServiceStatus*);
	virtual DWORD OnInterrogate(CServiceStatus*);
	virtual DWORD OnNetBindAdd(CServiceStatus*);
	virtual DWORD OnNetBindDisable(CServiceStatus*);
	virtual DWORD OnNetBindEnable(CServiceStatus*);
	virtual DWORD OnNetBindRemove(CServiceStatus*);
	virtual DWORD OnParamChange(CServiceStatus*);
	virtual DWORD OnPause(CServiceStatus*);
	virtual DWORD OnShutdown(CServiceStatus* pServiceStatus);
	virtual DWORD OnStop(CServiceStatus* pServiceStatus);
	virtual DWORD OnDeviceEvent(CServiceStatus*, DWORD, LPVOID);
	virtual DWORD OnHardwareProfileChange(CServiceStatus*, DWORD);
	virtual DWORD OnPowerEvent(CServiceStatus*, DWORD, LPVOID);
	virtual DWORD OnCustomHandlerEx(CServiceStatus*, DWORD);

private:
	bool						m_bServiceMode;
	static CServiceModule*		m_pServer;
	bool						m_bRunning;
};

inline DWORD CServiceModule::OnContinue(CServiceStatus*)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnInterrogate(CServiceStatus*)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnNetBindAdd(CServiceStatus*)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnNetBindDisable(CServiceStatus*)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnNetBindEnable(CServiceStatus*)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnNetBindRemove(CServiceStatus*)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnParamChange(CServiceStatus*)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnPause(CServiceStatus*)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnShutdown(CServiceStatus* pServiceStatus)
{
	return OnStop(pServiceStatus);
}

inline DWORD CServiceModule::OnDeviceEvent(CServiceStatus*, DWORD, LPVOID)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnHardwareProfileChange(CServiceStatus*, DWORD)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnPowerEvent(CServiceStatus*, DWORD, LPVOID)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

inline DWORD CServiceModule::OnCustomHandlerEx(CServiceStatus*, DWORD)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}