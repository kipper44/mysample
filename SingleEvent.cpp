#include "stdafx.h"
#include "SingleEvent.h"

#define VERIFY_2(condition, ErrorCode) \
if (!(condition)) \
	{ \
	OutputDebugString(TEXT("CSingleEvent ERROR :"));	\
	OutputDebugString(TEXT(#condition));	\
	OutputDebugString("\n");	\
	}

#define VERIFY_CHECKLASTERROR(x)	VERIFY_2(x, ::GetLastError())

CSingleEvent::CSingleEvent(LPSECURITY_ATTRIBUTES pEventAttributes, bool bManualReset, bool bInitialState, LPCTSTR pName)
{
	Create(pEventAttributes, bManualReset, bInitialState, pName);
}


CSingleEvent::~CSingleEvent()
{
}

DWORD	CSingleEvent::Wait(DWORD Milliseconds, bool bAlertable) const
{
	DWORD WaitResult;

	VERIFY_CHECKLASTERROR(WAIT_FAILED != (WaitResult = ::WaitForSingleObjectEx(m_hHandle, Milliseconds, false != bAlertable)));

	return WaitResult;

}


bool CSingleEvent::Create(LPSECURITY_ATTRIBUTES pEventAttributes, bool bManualReset, bool bInitialState, LPCTSTR pName)
{
	m_hHandle = ::CreateEvent(pEventAttributes, false != bManualReset, false != bInitialState, pName);

	VERIFY_CHECKLASTERROR(NULL != m_hHandle);

	return (NULL != m_hHandle);
}