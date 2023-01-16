#pragma once

#include "IOCTLCodes.h"
#include "DriverConfig.h"
#include <ntddk.h>

#define IOCTL_DRIVER_COPY_MEMORY ((ULONG)CTL_CODE(DRIVER_DEVICE_TYPE, 0x808, METHOD_BUFFERED, FILE_SPECIAL_ACCESS))

typedef struct _DRIVER_COPY_MEMORY {
	ULONGLONG Source; // Source buffer address.
	ULONGLONG Target; // Target buffer address.
	ULONGLONG Size; // Buffer size.
	ULONG ProcessId; // Target process ID.
	BOOLEAN Write; // TRUE if writing, FALSE if reading.
} DRIVER_COPY_MEMORY, * PDRIVER_COPY_MEMORY;
