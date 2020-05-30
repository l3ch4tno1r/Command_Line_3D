#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <tchar.h>

#include <process.h>
#include <iostream>
#include <Windows.h>
#include "dbghelp.h"

using namespace std;

const int MaxNameLen = 256;

#pragma comment(lib,"Dbghelp.lib")

void printStack(CONTEXT* ctx) //Prints stack trace based on context record
{
	BOOL    result;
	HANDLE  process;
	HANDLE  thread;
	HMODULE hModule;

	STACKFRAME64        stack;
	ULONG               frame;
	DWORD64             displacement;

	DWORD disp;
	IMAGEHLP_LINE64 *line;

	char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	char name[MaxNameLen];
	char module[MaxNameLen];
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

	memset(&stack, 0, sizeof(STACKFRAME64));

	process = GetCurrentProcess();
	thread = GetCurrentThread();
	displacement = 0;
#if !defined(_M_AMD64)
	stack.AddrPC.Offset = (*ctx).Eip;
	stack.AddrPC.Mode = AddrModeFlat;
	stack.AddrStack.Offset = (*ctx).Esp;
	stack.AddrStack.Mode = AddrModeFlat;
	stack.AddrFrame.Offset = (*ctx).Ebp;
	stack.AddrFrame.Mode = AddrModeFlat;
#endif

	SymInitialize(process, NULL, TRUE); //load symbols

	for (frame = 0; ; frame++)
	{
		//get next call from stack
		result = StackWalk64
		(
#if defined(_M_AMD64)
			IMAGE_FILE_MACHINE_AMD64
#else
			IMAGE_FILE_MACHINE_I386
#endif
			,
			process,
			thread,
			&stack,
			ctx,
			NULL,
			SymFunctionTableAccess64,
			SymGetModuleBase64,
			NULL
		);

		if (!result) break;

		//get symbol name for address
		pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		pSymbol->MaxNameLen = MAX_SYM_NAME;
		SymFromAddr(process, (ULONG64)stack.AddrPC.Offset, &displacement, pSymbol);

		line = (IMAGEHLP_LINE64 *)malloc(sizeof(IMAGEHLP_LINE64));
		line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		//try to get line
		if (SymGetLineFromAddr64(process, stack.AddrPC.Offset, &disp, line))
		{
			printf("\tat %s in %s: line: %lu: address: 0x%0X\n", pSymbol->Name, line->FileName, line->LineNumber, pSymbol->Address);
		}
		else
		{
			//failed to get line
			printf("\tat %s, address 0x%0X.\n", pSymbol->Name, pSymbol->Address);
			hModule = NULL;
			lstrcpyA(module, "");
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				(LPCTSTR)(stack.AddrPC.Offset), &hModule);

			//at least print module name
			if (hModule != NULL)GetModuleFileNameA(hModule, module, MaxNameLen);

			printf("in %s\n", module);
		}

		free(line);
		line = NULL;
	}
}

//******************************************************************************

void function2()
{
	int a = 0;
	int b = 0;
	throw exception();
}

void function1()
{
	int a = 0;
	function2();
}

void function0()
{
	function1();
}

int seh_filter(_EXCEPTION_POINTERS* ex)
{
	printf("*** Exception 0x%x occured ***\n\n", ex->ExceptionRecord->ExceptionCode);
	printStack(ex->ContextRecord);

	return EXCEPTION_EXECUTE_HANDLER;
}

static void threadFunction(void *param)
{

	__try
	{
		function0();
	}
	__except (seh_filter(GetExceptionInformation()))
	{
		printf("Exception \n");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//_beginthread(threadFunction, 0, NULL);
	threadFunction(nullptr);
	printf("Press any key to exit.\n");
	cin.get();
	return 0;
}