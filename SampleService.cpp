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
	OutputDebugString("�ʱ�ȭ ��ƾ �ʿ信 ���� ����\n");
	return true;
}

void SampleService::Main()
{
	OutputDebugString("���� ���� ó�� ����� �ݵ�� ó�� �ؾ߸� �Ѵ�\n");
	while (GetRunning())
	{
		OutputDebugString("���� ����\n");
		if (GetMode() == false) //�ܼ� ���
		{
			int i = getchar(); //���� �ڵ�
			if (i == 10) break;
		}

		Sleep(1000);
	}
}

void SampleService::MakeEnd()
{
	OutputDebugString("���񽺰� ����  �˸� �ʿ信 ���� ����\n");
}

void SampleService::Cleanup()
{
	OutputDebugString("���� ������ ���� �ؾ� �� ��ü�� ó��  �ʿ信 ���� ��� �޾� ���\n");
}

