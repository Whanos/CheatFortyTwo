#pragma once
#include "IOCTLHandler.h"

extern "C" {
	NTSTATUS
		IoCreateDriver(
			_In_ PUNICODE_STRING DriverName, OPTIONAL
			_In_ PDRIVER_INITIALIZE InitializationFunction
		);
	NTKERNELAPI NTSTATUS MmCopyVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAddress, PEPROCESS TargetProcess, PVOID TargetAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize);

}

_Dispatch_type_(IRP_MJ_DEVICE_CONTROL)
_Dispatch_type_(IRP_MJ_CREATE)
_Dispatch_type_(IRP_MJ_CLOSE)
DRIVER_DISPATCH DriverDispatch;

DRIVER_INITIALIZE DriverInitialize;

extern "C" DRIVER_INITIALIZE DriverEntry;
#pragma alloc_text(INIT, DriverEntry)

typedef struct _INOUT_PARAM {
	ULONG Param1;
	ULONG Param2;
	ULONG Param3;
	ULONG Param4;
} INOUT_PARAM, * PINOUTPARAM;
