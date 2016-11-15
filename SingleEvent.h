#pragma once

class CSingleEvent
{
public:
	explicit CSingleEvent(LPSECURITY_ATTRIBUTES pEventAttributes = NULL, bool bManualReset = false, bool bInitialState = false, LPCTSTR pName = NULL);
	virtual ~CSingleEvent();

	HANDLE	GetHandle() const;
	void	SetEvent() const;
	DWORD	Wait(DWORD Milliseconds = INFINITE, bool bAlertable = false) const;

private:
	bool	Create(LPSECURITY_ATTRIBUTES pEventAttributes, bool bManualReset, bool bInitialState, LPCTSTR pName);

private:
	HANDLE			m_hHandle;
};

inline HANDLE CSingleEvent::GetHandle() const
{
	return m_hHandle;
}

////////////////////////////////////////////////////////////////////////////////

inline void CSingleEvent::SetEvent() const
{
	::SetEvent(m_hHandle);
}
