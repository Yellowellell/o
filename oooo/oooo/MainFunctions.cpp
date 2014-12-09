#include "stdafx.h"
#include "MainFunctions.h"
#include <TlHelp32.h>

int				TimerDelay = TIMER_DELAY_NORMAL;
long long		OldKernelTime = 0;

SYSTEM_PROCESS_INFO*	GetNextBufferEntry(SYSTEM_PROCESS_INFO* CurrentEntry)
{
	return (SYSTEM_PROCESS_INFO*)((LPBYTE)CurrentEntry + CurrentEntry->NextEntryOffset);
}
CString					GetProcessFileName(int ProcessId)
{
	HANDLE hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessId);
	if (hthSnapshot)
	{
		MODULEENTRY32 me = { sizeof(me) };
		if (Module32First(hthSnapshot, &me))
		{
			CloseHandle(hthSnapshot);
			return me.szExePath;
		}
	}

	return L"";
}
bool					GetProcessInformation(int &ProcessesCount, ProcessInformation* Processes)
{
	SYSTEM_PROCESS_INFO* buffer = (SYSTEM_PROCESS_INFO*)VirtualAlloc(NULL, MAX_BUFFER_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	NTSTATUS Status = NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemExtendedProcessInformation, 
											   buffer, MAX_BUFFER_SIZE, NULL);
	if (!NT_SUCCESS(Status))
	{
		VirtualFree(buffer, NULL, MEM_RELEASE);
		return false;
	}
	SYSTEM_PROCESS_INFO* buffer_entry = buffer;

	int i = 0;
	for (i;;buffer = GetNextBufferEntry(buffer), i++)
	{
		Processes[i].Name = (buffer->ImageName.Length == 0) ?
		DEFAULT_PROCESS_NAME : buffer->ImageName.Buffer;
		Processes[i].Id = buffer->UniqueProcessId;

		long long CurrentKernelTime = buffer->UserTime.QuadPart + buffer->KernelTime.QuadPart;
		Processes[i].DeltaKernelTime = CurrentKernelTime - Processes[i].OveralKernelTime;
		Processes[i].OveralKernelTime = CurrentKernelTime;

		if (Processes[i].Id == Processes[i].OldId)
		{
			if (Processes[i].Id != 0)
				Processes[i].KernelLoad = Processes[i].DeltaKernelTime / TimerDelay * 0.01 / 4;
			else 
				Processes[i].KernelLoad = GetDeltaKernelTime(OldKernelTime) / TimerDelay * 0.01;
		}
		Processes[i].OldId = Processes[i].Id;
		
		Processes[i].FileName = GetProcessFileName(Processes[i].Id);
		
		if (buffer->NextEntryOffset == 0)
			break;
	}
	ProcessesCount = i + 1;
	VirtualFree(buffer_entry, NULL, MEM_RELEASE);

	return true;
}
long long				GetDeltaKernelTime(long long &OldKernelTime)
{
	SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION buffer;
	NTSTATUS Status = NtQuerySystemInformation(SystemProcessorPerformanceInformation, &buffer, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION), NULL);
	if (!NT_SUCCESS(Status)) 
		return -1;

	long long DeltaTime = buffer.IdleTime.QuadPart - OldKernelTime;
	OldKernelTime = buffer.IdleTime.QuadPart;
	return DeltaTime;
}
bool					KillProcess(int ProcessId)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if (hProcess == NULL)
		return false;

	bool Result = (bool)TerminateProcess(hProcess, NULL);
	CloseHandle(hProcess);
	return Result;
}