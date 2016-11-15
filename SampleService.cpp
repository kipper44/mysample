#include "stdafx.h"
#include "SampleService.h"


SampleService::SampleService()
{
}


SampleService::~SampleService()
{
}

bool SampleService::Initialize(DWORD Argc, LPTSTR*ppArgv)
{
	OutputDebugString("초기화 루틴 필요에 따리 구현\n");
	return true;
}

void SampleService::Main()
{
	OutputDebugString("메인 루프 처리 여기는 반드시 처리 해야만 한다\n");
	while (GetRunning())
	{
		OutputDebugString("서비스 루프\n");
		if (GetMode() == false) //콘솔 모드
		{
			int i = getchar(); //정지 코드
			if (i == 10) break;
		}

		Sleep(1000);
	}
}

void SampleService::MakeEnd()
{
	OutputDebugString("서비스가 종료  알림 필요에 따라 구현\n");
}

void SampleService::Cleanup()
{
	OutputDebugString("서비스 종료후 삭제 해야 할 객체를 처리  필요에 따라 상속 받아 사용\n");
}

