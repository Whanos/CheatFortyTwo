#pragma once

#include "IOCTLCodes.h"
#include "DriverConfig.h"
#ifdef SetFlag()
#include <ntddk.h>
#endif

#define IOCTL_DRIVER_COPY_MEMORY ((ULONG)CTL_CODE(DRIVER_DEVICE_TYPE, 0x808, METHOD_BUFFERED, FILE_SPECIAL_ACCESS))

typedef unsigned long long ULONGLONG;
typedef unsigned long ULONG;
typedef unsigned char BOOLEAN;

typedef struct _DRIVER_COPY_MEMORY {
	ULONGLONG Source; // Source buffer address.
	ULONGLONG Target; // Target buffer address.
	ULONGLONG Size; // Buffer size.
	ULONG ProcessId; // Target process ID.
	BOOLEAN Write; // TRUE if writing, FALSE if reading.
} DRIVER_COPY_MEMORY, * PDRIVER_COPY_MEMORY;
