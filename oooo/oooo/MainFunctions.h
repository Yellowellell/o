#pragma once;
#pragma comment (lib, "ntdll.lib")

#include <Windows.h>
#include <winternl.h>
#include <stdio.h>

#define SystemExtendedProcessInformation 57
#define MAX_BUFFER_SIZE					1024*1024
#define MAX_BUFFER_LENGTH				1024

#define TIMER_DELAY_LARGE				2000
#define TIMER_DELAY_NORMAL				1000
#define TIMER_DELAY_SMALL				500
#define DEFAULT_PROCESS_NAME			L"Бездействие системы"
 
struct CLIENT_ID
{
	DWORD UniqueProcess;
	DWORD UniqueThread;
};
struct VM_COUNTERS
{
	SIZE_T         PeakVirtualSize;
	SIZE_T         VirtualSize;
	ULONG          PageFaultCount;
	SIZE_T         PeakWorkingSetSize;
	SIZE_T         WorkingSetSize;
	SIZE_T         QuotaPeakPagedPoolUsage;
	SIZE_T         QuotaPagedPoolUsage;
	SIZE_T         QuotaPeakNonPagedPoolUsage;
	SIZE_T         QuotaNonPagedPoolUsage;
	SIZE_T         PagefileUsage;
	SIZE_T         PeakPagefileUsage;
};
struct SYSTEM_THREAD_INFORMATION
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	LONG Priority;
	LONG BasePriority;
	ULONG ContextSwitches;
	ULONG ThreadState;
	ULONG WaitReason;
};
struct SYSTEM_EXTENDED_THREAD_INFORMATION
{
	SYSTEM_THREAD_INFORMATION ThreadInfo;
	PVOID StackBase;
	PVOID StackLimit;
	PVOID Win32StartAddress;
	PVOID TebAddress;
	ULONG Reserved1;
	ULONG Reserved2;
	ULONG Reserved3;
};
struct SYSTEM_PROCESS_INFO
{
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER SpareLi1;
	LARGE_INTEGER SpareLi2;
	LARGE_INTEGER SpareLi3;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	PVOID BasePriority;
	ULONG UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	PVOID PageDirectoryBase;
	VM_COUNTERS VirtualMemoryCounters;
	SIZE_T PrivatePageCount;
	IO_COUNTERS IoCounters;
	SYSTEM_EXTENDED_THREAD_INFORMATION Threads[1];
}; 

struct ProcessInformation
{
	CString		Name;
	CString		FileName;
	int			Id;
	int			OldId;
	int			TotalMemory;
	double		KernelLoad;
	long long	DeltaKernelTime;
	long long	OveralKernelTime;
};

extern int		TimerDelay;

CString			GetProcessFileName(int ProcessId);
bool			GetProcessInformation(int& ProcessesCount, ProcessInformation* Proceses);
long long		GetDeltaKernelTime(long long &OldKernelTime);
bool			KillProcess(int ProcessId);