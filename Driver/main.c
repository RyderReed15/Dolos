
#include <ntdef.h>
#include <ntifs.h>
#include <windef.h>


#define IO_READ_MEM CTL_CODE(FILE_DEVICE_UNKNOWN, 0x701, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_WRITE_MEM CTL_CODE(FILE_DEVICE_UNKNOWN, 0x702, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

_Dispatch_type_(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH DriverIO;
_Dispatch_type_(IRP_MJ_CREATE)
DRIVER_DISPATCH CreateCall;
_Dispatch_type_(IRP_MJ_CLOSE)
DRIVER_DISPATCH CloseCall;
DRIVER_INITIALIZE DriverInitialize;
DRIVER_INITIALIZE DriverEntry;
#pragma alloc_text(INIT, DriverEntry)


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
