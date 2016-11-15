// test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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

