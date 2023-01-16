#pragma once
#include <ntddk.h>
#include "IOCTLHandler.h"

extern "C" {
	NTSTATUS
		IoCreateDriver(
			_In_ PUNICODE_STRING DriverName, OPTIONAL
			_In_ PDRIVER_INITIALIZE InitializationFunction
		);
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
