// test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "SampleService.h"

int main()
{


	bool bServiceMode = true;

	int nCount = __argc;
	if (nCount > 1) bServiceMode = false;

	SampleService cService ;

	cService.Start(bServiceMode);

    return 0;
}

