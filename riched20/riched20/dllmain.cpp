// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	_tsetlocale(LC_ALL, _T("chs_china.936"));
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//InitializeCriticalSection(&g_critical);
		break;
	case DLL_THREAD_ATTACH:
		
		break;
	case DLL_THREAD_DETACH:
		
		break;
	case DLL_PROCESS_DETACH:
		//DeleteCriticalSection(&g_critical);
		break;
	}
	return TRUE;
}

