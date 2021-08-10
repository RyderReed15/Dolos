#pragma once

#ifndef DRIVER_INTERFACE_H
#define DRIVER_INTERFACE_H
#include "Vector3.h"
#include <Windows.h>

#define IO_READ_MEM CTL_CODE(FILE_DEVICE_UNKNOWN, 0x701, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_WRITE_MEM CTL_CODE(FILE_DEVICE_UNKNOWN, 0x702, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)



enum STATUS { STATUS_NO_GAME, STATUS_SUCESS, STATUS_NO_DRIVER };

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

class DriverInterface {
public:
	DriverInterface();
	template <typename T>
	int WriteMemory(ULONG PID, ULONG Address, T Value);
	template <typename T>
	int WriteMemory(ULONG PID, ULONG Address, T Value, DWORD Size);
	template <typename T>
	T ReadMemory(ULONG PID, ULONG Address, SIZE_T Size);
	Vector3 ReadGappedVec3(ULONG PID, ULONG Address, int gap);
	void getInfo();

private:
	HANDLE driverHandle;

	

};

/*
 Reads a value from the specified location
 @param Id of process to read from
 @param Address of location to read from
 @param Size of value
 @return Status code
*/
template <typename T>
T DriverInterface::ReadMemory(ULONG PID, ULONG Address, SIZE_T Size) {
	if (driverHandle == INVALID_HANDLE_VALUE) {
		int returnCode = STATUS_NO_DRIVER;
		return *(T*)&returnCode;
	}
	T Buffer;
	KERNEL_READ_REQUEST Read;

	Read.PID = PID;
	Read.Address = Address;
	Read.Size = Size;
	Read.Out = (ULONG)&Buffer;
	if (DeviceIoControl(driverHandle, IO_READ_MEM, &Read, sizeof(Read), &Read, sizeof(Read), 0, 0)) {
		

		return Buffer;
	}
	else {
		int returnCode = STATUS_NO_GAME;
		return *(T*)&returnCode;
	}
}

/*
 Writes a value to the specified location
 @param Id of process to write to
 @param Address of location to write to
 @param Value to write
 @param Size of value
 @return Status code
*/
template <typename T>
int DriverInterface::WriteMemory(ULONG PID, ULONG Address, T Value) {
	return WriteMemory(PID, Address, Value, sizeof(Value));
	
}
/*
 Writes a value to the specified location
 @param Id of process to write to
 @param Address of location to write to
 @param Value to write
 @param Size of value
 @return Status code
*/
template <typename T>
int DriverInterface::WriteMemory(ULONG PID, ULONG Address, T Value, DWORD Size) {
	if (driverHandle == INVALID_HANDLE_VALUE)
		return 2;

	DWORD Bytes;
	KERNEL_WRITE_REQUEST write;

	write.PID = PID;
	write.Address = Address;
	write.In = (ULONG)&Value;
	write.Size = Size;
	//std::cout << Value << std::endl;
	return (DeviceIoControl(driverHandle, IO_WRITE_MEM, &write, sizeof(write), 0, 0, &Bytes, NULL));
	

}
// Opens a handle to the driver to read and write memory
DriverInterface::DriverInterface() {
	driverHandle = CreateFileA("\\\\.\\DolosDriver", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
}
/*
 Reads a Vector3 that has gaps between its values from memory
 @param Id of the process to read from
 @param Address of the Vector3
 @param gap between values
 @return Vector3 read from memory
*/
Vector3 DriverInterface::ReadGappedVec3(ULONG PID, ULONG Address, int gap) {
	if (driverHandle == INVALID_HANDLE_VALUE)
		return { -999,0,0 };

	return { ReadMemory<float>(PID, Address, 4),ReadMemory<float>(PID, Address + gap, 4),ReadMemory<float>(PID, Address + 2 * gap, 4) };
}
#endif
