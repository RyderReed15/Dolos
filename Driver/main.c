







#include <ntdef.h>
#include <ntifs.h>
#include <windef.h>

#include <ntddk.h>
#include <ntstrsafe.h>
#include <stdlib.h>
#include <wdf.h>

#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <windef.h>
#include <ntstrsafe.h>


#define PROCESS_NAME "csgo.exe"
#define IO_READ_MEM CTL_CODE(FILE_DEVICE_UNKNOWN, 0x701, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_WRITE_MEM CTL_CODE(FILE_DEVICE_UNKNOWN, 0x702, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_GET_ADDRESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0x703, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

_Dispatch_type_(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH DriverIO;
_Dispatch_type_(IRP_MJ_CREATE)
DRIVER_DISPATCH CreateCall;
_Dispatch_type_(IRP_MJ_CLOSE)
DRIVER_DISPATCH CloseCall;
DRIVER_INITIALIZE DriverInitialize;
DRIVER_INITIALIZE DriverEntry;
#pragma alloc_text(INIT, DriverEntry)

DWORD ProcessID = 0;
DWORD ClientAddress = 0;
DWORD EngineAddress = 0;

PDEVICE_OBJECT deviceObject;


NTSTATUS NTAPI MmCopyVirtualMemory
(
	PEPROCESS Source,
	PVOID SourceAddress,
	PEPROCESS Target,
	PVOID TargetAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T ReturnSize
);

NTKERNELAPI
NTSTATUS
PsLookupProcessByProcessId(
	_In_ HANDLE ProcessId,
	_Outptr_ PEPROCESS* Process
);


NTSTATUS NTAPI ZwQuerySystemInformation(ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
NTKERNELAPI PPEB PsGetProcessPeb(
	IN PEPROCESS Process);
NTKERNELAPI PVOID NTAPI PsGetProcessWow64Process(IN PEPROCESS Process);

typedef struct _KERNEL_READ_REQUEST
{
	ULONG PID;
	ULONG Address;
	ULONG Out;
	ULONG Size;

} KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;

typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG PID;
	ULONG Address;
	ULONG In;
	ULONG Size;

} KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;

typedef struct _KERNEL_ADDRESS_REQUEST
{
	ULONG PID;
	ULONG ClientAdd;
	ULONG EngineAdd;

} KERNEL_ADDRESS_REQUEST, * PKERNEL_ADDRESS_REQUEST;

typedef struct _SYSTEM_THREAD {



	LARGE_INTEGER           KernelTime;
	LARGE_INTEGER           UserTime;
	LARGE_INTEGER           CreateTime;
	ULONG                   WaitTime;
	PVOID                   StartAddress;
	CLIENT_ID               ClientId;
	KPRIORITY               Priority;
	LONG                    BasePriority;
	ULONG                   ContextSwitchCount;
	ULONG                   State;
	KWAIT_REASON            WaitReason;

} SYSTEM_THREAD, * PSYSTEM_THREAD;

typedef struct _SYSTEM_PROCESS_INFORMATION {



	ULONG                   NextEntryOffset;
	ULONG                   NumberOfThreads;
	LARGE_INTEGER           Reserved[3];
	LARGE_INTEGER           CreateTime;
	LARGE_INTEGER           UserTime;
	LARGE_INTEGER           KernelTime;
	UNICODE_STRING          ImageName;
	KPRIORITY               BasePriority;
	HANDLE                  ProcessId;
	HANDLE                  InheritedFromProcessId;
	ULONG                   HandleCount;
	ULONG                   Reserved2[2];
	ULONG                   PrivatePageCount;
	VM_COUNTERS             VirtualMemoryCounters;
	IO_COUNTERS             IoCounters;
	SYSTEM_THREAD           Threads[0];

} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

typedef struct _PEB_LDR_DATA
{
	unsigned int Length;
	int Initialized;
	void* SSHandle;
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
} PEB_LDR_DATA, * PPEB_LDR_DATA;
typedef struct _PEB_LDR_DATA32
{
	ULONG Length;
	UCHAR Initialized;
	ULONG SsHandle;
	LIST_ENTRY32 InLoadOrderModuleList;
	LIST_ENTRY32 InMemoryOrderModuleList;
	LIST_ENTRY32 InInitializationOrderModuleList;
} PEB_LDR_DATA32, * PPEB_LDR_DATA32;

typedef struct _PEB {
	BYTE                          Reserved1[2];
	BYTE                          BeingDebugged;
	BYTE                          Reserved2[1];
	PVOID                         Reserved3[2];
	PPEB_LDR_DATA                 Ldr;
	PVOID						  ProcessParameters;
	PVOID                         Reserved4[3];
	PVOID                         AtlThunkSListPtr;
	PVOID                         Reserved5;
	ULONG                         Reserved6;
	PVOID                         Reserved7;
	ULONG                         Reserved8;
	ULONG                         AtlThunkSListPtr32;
	PVOID                         Reserved9[45];
	BYTE                          Reserved10[96];
	PVOID						  PostProcessInitRoutine;
	BYTE                          Reserved11[128];
	PVOID                         Reserved12[1];
	ULONG                         SessionId;
} PEB, * PPEB;
typedef struct _PEB32
{
	UCHAR InheritedAddressSpace;
	UCHAR ReadImageFileExecOptions;
	UCHAR BeingDebugged;
	UCHAR BitField;
	ULONG Mutant;
	ULONG ImageBaseAddress;
	ULONG Ldr;
	ULONG ProcessParameters;
	ULONG SubSystemData;
	ULONG ProcessHeap;
	ULONG FastPebLock;
	ULONG AtlThunkSListPtr;
	ULONG IFEOKey;
	ULONG CrossProcessFlags;
	ULONG UserSharedInfoPtr;
	ULONG SystemReserved;
	ULONG AtlThunkSListPtr32;
	ULONG ApiSetMap;
} PEB32, * PPEB32;

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	WORD LoadCount;
	WORD TlsIndex;
	union
	{
		LIST_ENTRY HashLinks;
		struct
		{
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union
	{
		ULONG TimeDateStamp;
		PVOID LoadedImports;
	};
	PVOID EntryPointActivationContext;
	PVOID PatchInformation;
	LIST_ENTRY ForwarderLinks;
	LIST_ENTRY ServiceTagLinks;
	LIST_ENTRY StaticLinks;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _LDR_DATA_TABLE_ENTRY32
{
	LIST_ENTRY32 InLoadOrderLinks;
	LIST_ENTRY32 InMemoryOrderLinks;
	LIST_ENTRY32 InInitializationOrderLinks;
	ULONG DllBase;
	ULONG EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING32 FullDllName;
	UNICODE_STRING32 BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	LIST_ENTRY32 HashLinks;
	ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY32, * PLDR_DATA_TABLE_ENTRY32;

//NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject);
NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP irp);
NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP irp);



NTKERNELAPI
NTSTATUS IoCreateDriver(
	IN PUNICODE_STRING DriverName, OPTIONAL
	IN PDRIVER_INITIALIZE InitializationFunction
);

NTSTATUS KeReadProcessMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size)
{

	PEPROCESS Target = PsGetCurrentProcess();
	SIZE_T Bytes;

	if (NT_SUCCESS(MmCopyVirtualMemory(Process, SourceAddress, Target, TargetAddress, Size, KernelMode, &Bytes)))
		return STATUS_SUCCESS;
	else
		return STATUS_ACCESS_DENIED;
}
NTSTATUS KeWriteProcessMemory(PEPROCESS Process, PVOID TargetAddress, PVOID SourceAddress, SIZE_T Size)
{
	KAPC_STATE state;
	PSIZE_T Bytes;
	//KeStackAttachProcess(Process, &state);
	if (NT_SUCCESS(MmCopyVirtualMemory(PsGetCurrentProcess(), SourceAddress, Process, TargetAddress, Size, KernelMode, &Bytes))) {

		//KeUnstackDetachProcess(&state);
		return STATUS_SUCCESS;
	}
	else {

		//KeUnstackDetachProcess(&state);
		return STATUS_ACCESS_DENIED;
	}



}

NTSTATUS GetProcessInfo() {
	ClientAddress = 0; EngineAddress = 0; ProcessID = 0;
	ULONG returnSize = 0;

	if (ZwQuerySystemInformation(5, NULL, 0, &returnSize) == STATUS_INFO_LENGTH_MISMATCH) {
		if (returnSize) {
			PVOID pool = ExAllocatePoolWithTag(PagedPool, returnSize, "tag");
			if (pool) {
				if (NT_SUCCESS(ZwQuerySystemInformation(5, pool, returnSize, &returnSize))) {
					PSYSTEM_PROCESS_INFORMATION processEntry = pool;
					do {
						if (processEntry->ImageName.Length) {

							if (wcsstr(processEntry->ImageName.Buffer, L"csgo.exe")) {
								ProcessID = processEntry->ProcessId;
							}
						}
						processEntry = (PSYSTEM_PROCESS_INFORMATION)((BYTE*)processEntry + processEntry->NextEntryOffset);
					} while (processEntry->NextEntryOffset && ProcessID == 0);

					if (ProcessID == 0) {
						ExFreePoolWithTag(pool, "tag");
						return STATUS_ACCESS_DENIED;
					}
				}
			}
			ExFreePoolWithTag(pool, "tag");
		}
	}
	HANDLE file;
	IO_STATUS_BLOCK ioStatusBlock;
	UNICODE_STRING uniName = RTL_CONSTANT_STRING(L"\\SystemRoot\\KernelProcessList.txt");
	OBJECT_ATTRIBUTES objAttr;

	InitializeObjectAttributes(&objAttr, &uniName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL, NULL);
	ZwCreateFile(&file,
		GENERIC_WRITE,
		&objAttr, &ioStatusBlock, NULL,
		FILE_ATTRIBUTE_NORMAL,
		0,
		FILE_OVERWRITE_IF,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL, 0);

	PEPROCESS proc;
	UNICODE_STRING clientName;
	RtlInitUnicodeString(&clientName, L"client.dll");

	UNICODE_STRING engineName;
	RtlInitUnicodeString(&engineName, L"engine.dll");

	if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)ProcessID, &proc)))
	{
		PPEB32 pPeb = (PPEB32)PsGetProcessWow64Process(proc);
		//PPEB32 pPeb = PsGetProcessPeb(proc) + 0x1000;
		KAPC_STATE state;

		KeStackAttachProcess(proc, &state);
		PPEB_LDR_DATA32 pLdr = (PPEB_LDR_DATA32)pPeb->Ldr;
		for (PLIST_ENTRY32 pListEntry = (PLIST_ENTRY32)pLdr->InLoadOrderModuleList.Flink; (ClientAddress == 0 || EngineAddress == 0) && pListEntry != &pLdr->InLoadOrderModuleList; pListEntry = (PLIST_ENTRY32)pListEntry->Flink)
		{
			PLDR_DATA_TABLE_ENTRY32 pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);
			CHAR string[100];
			RtlStringCbPrintfA(string, _countof(string), "%ws\n", pEntry->FullDllName.Buffer);
			size_t length;
			RtlStringCbLengthA(string, _countof(string), &length);
			if (wcsstr(pEntry->FullDllName.Buffer, L"client.dll")) {
				ClientAddress = (UINT32)pEntry->DllBase;
			}if (wcsstr(pEntry->FullDllName.Buffer, L"engine.dll")) {
				EngineAddress = (UINT32)pEntry->DllBase;
			}
			ZwWriteFile(file, NULL, NULL, NULL, &ioStatusBlock, string, (ULONG)length, NULL, NULL);
		}

		KeUnstackDetachProcess(&state);
	}
	ZwClose(file);
	return STATUS_SUCCESS;
	if (ClientAddress && EngineAddress) {
		return STATUS_SUCCESS;
	}
	else {
		return STATUS_ACCESS_DENIED;
	}

}

NTSTATUS DriverIO(PDEVICE_OBJECT DevObj, PIRP irp)
{

	UNREFERENCED_PARAMETER(DevObj);
	NTSTATUS status;
	ULONG BytesIO = 0;

	PIO_STACK_LOCATION stackLoc = IoGetCurrentIrpStackLocation(irp);

	ULONG ControlCode = stackLoc->Parameters.DeviceIoControl.IoControlCode;


	if (ControlCode == IO_READ_MEM) {

		PKERNEL_READ_REQUEST ReadIn = (PKERNEL_READ_REQUEST)irp->AssociatedIrp.SystemBuffer;
		PKERNEL_READ_REQUEST ReadOut = (PKERNEL_READ_REQUEST)irp->AssociatedIrp.SystemBuffer;

		PEPROCESS Process;
		
		if (NT_SUCCESS(PsLookupProcessByProcessId(ReadIn->PID, &Process)))
			KeReadProcessMemory(Process, ReadIn->Address, (PVOID)ReadIn->Out, ReadIn->Size);
		status = STATUS_SUCCESS;
		BytesIO = sizeof(KERNEL_READ_REQUEST);

	}
	else if (ControlCode == IO_WRITE_MEM) {
		PKERNEL_WRITE_REQUEST WriteIn = (PKERNEL_WRITE_REQUEST)irp->AssociatedIrp.SystemBuffer;

		PEPROCESS Process;

		if (NT_SUCCESS(PsLookupProcessByProcessId(WriteIn->PID, &Process))) {
			if (NT_SUCCESS(KeWriteProcessMemory(Process, WriteIn->Address, (PVOID)WriteIn->In, WriteIn->Size)))
				status = STATUS_SUCCESS;
			else
				status = STATUS_ACCESS_DENIED;
		}
		else {
			status = STATUS_ACCESS_DENIED;
		}
		BytesIO = sizeof(KERNEL_WRITE_REQUEST);
	}
	else if (ControlCode == IO_GET_ADDRESS) {
		PKERNEL_ADDRESS_REQUEST address = (PKERNEL_ADDRESS_REQUEST)irp->AssociatedIrp.SystemBuffer;

		PEPROCESS Process;
		PSIZE_T Bytes;
		if (NT_SUCCESS(GetProcessInfo())) {
			address->ClientAdd = ClientAddress;
			address->EngineAdd = EngineAddress;
			address->PID = ProcessID;
			status = STATUS_SUCCESS;

		}
		else {
			status = STATUS_ACCESS_DENIED;
		}
		BytesIO = sizeof(KERNEL_ADDRESS_REQUEST);
	}
	else {
		status = STATUS_INVALID_PARAMETER;
		BytesIO = 0;
	}
	irp->IoStatus.Status = status;
	irp->IoStatus.Information = BytesIO;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return status;
}
NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP irp)
{

	UNREFERENCED_PARAMETER(DeviceObject);

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}
NTSTATUS DriverInitialize(
	_In_  struct _DRIVER_OBJECT* DriverObject,
	_In_  PUNICODE_STRING RegistryPath) {

	NTSTATUS status;
	UNICODE_STRING SymLink, devName;
	ULONG t;

	UNREFERENCED_PARAMETER(RegistryPath);

	RtlInitUnicodeString(&devName, L"\\Device\\InfoDriver");

	status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &deviceObject);
	if (!NT_SUCCESS(status)) {
		return status;
	}
	RtlInitUnicodeString(&SymLink, L"\\DosDevices\\InfoDriver");
	status = IoCreateSymbolicLink(&SymLink, &devName);

	DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverIO;
	DriverObject->DriverUnload = NULL;

	deviceObject->Flags |= DO_BUFFERED_IO;
	deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
	return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,
	PUNICODE_STRING pRegistryPath)
{	//make driverless? use example drivers for tdl
	NTSTATUS status;
	UNICODE_STRING drvName;
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pRegistryPath);
	DbgPrintEx(0, 0, "Driver Loaded\n");




	RtlInitUnicodeString(&drvName, L"\\Driver\\InfoDriver");
	status = IoCreateDriver(&drvName, &DriverInitialize);



	return status;
}




/**NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{

	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
	//IoDeleteSymbolicLink(&dosNam);
	IoDeleteDevice(pDriverObject->DeviceObject);
}*/

