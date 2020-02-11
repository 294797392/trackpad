#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "libhook.h"

typedef struct
{
	HINSTANCE dllHwnd;
	HHOOK hookHwnd;
	libhook_mouse_callback mouse_callback;
}libhook;

static libhook *hook = NULL;

BOOL WINAPI DllMain(HINSTANCE instance, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			hook = (libhook*)malloc(sizeof(libhook));
			memset(hook, 0, sizeof(libhook));
			hook->dllHwnd = instance;
		}
		break;

		case DLL_PROCESS_DETACH:
		{
			
		}
		break;

		default:
			break;
	}
	return TRUE;
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MessageBox(NULL, NULL, NULL, NULL);
	//if (hook->mouse_callback)
	//{
	//	hook->mouse_callback(1, 1, 1);
	//}

	return CallNextHookEx(hook->hookHwnd, nCode, wParam, lParam);
}

void libhook_start()
{
	printf("start\n");
	hook->hookHwnd = SetWindowsHookEx(WH_KEYBOARD, MouseProc, hook->dllHwnd, 0);
}

void libhook_stop()
{
	if (hook->hookHwnd)
	{
		UnhookWindowsHookEx(hook->hookHwnd);
		hook->hookHwnd = NULL;
	}
}

void libhook_set_mouse_callback(libhook_mouse_callback callback)
{
	hook->mouse_callback = callback;
}