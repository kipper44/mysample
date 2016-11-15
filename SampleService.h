#pragma once
class SampleService : public CServiceModule
{
public:
	SampleService();
	virtual ~SampleService();

private :
	virtual bool Initialize(DWORD Argc, LPTSTR*ppArgv);
	virtual void Main();
	virtual void MakeEnd() ;
	virtual void Cleanup() ;

};

