#pragma once
#include <ntddk.h>
#include "IOCTLCodes.h"

NTSTATUS DriverDispatch(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp);
NTSTATUS DriverCopy(IN PDRIVER_COPY_MEMORY copy);
NTSTATUS UnsupportedIo(PDEVICE_OBJECT device_obj, PIRP irp);
NTSTATUS CreateIo(PDEVICE_OBJECT device_obj, PIRP irp);
NTSTATUS CloseIo(PDEVICE_OBJECT device_obj, PIRP irp);