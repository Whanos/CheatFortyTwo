#include "Driver.h"
#include <ntddk.h>
#include "DriverConfig.h"
#include "IOCTLHandler.h"

NTSTATUS DriverInitialize(
    _In_  struct _DRIVER_OBJECT* DriverObject,
    _In_  PUNICODE_STRING RegistryPath
)
{
    NTSTATUS        status;
    UNICODE_STRING  SymLink;
    UNICODE_STRING  DevName;
    PDEVICE_OBJECT  devobj;

    PETHREAD        ThreadObject;
    PEPROCESS       ProcessObject;

    //RegistryPath is NULL
    UNREFERENCED_PARAMETER(RegistryPath);

    ProcessObject = PsGetCurrentProcess();
    ThreadObject = PsGetCurrentThread();

    DbgPrintEx(DPFLTR_DEFAULT_ID,
        DPFLTR_INFO_LEVEL,
        "[%s] CurrentProcess : %lu (%p) CurrentThread : %lu (%p)\r\n",
        __FUNCTION__,
        HandleToUlong(PsGetCurrentProcessId()),
        ProcessObject,
        HandleToUlong(PsGetCurrentThreadId()),
        ThreadObject);

    RtlInitUnicodeString(&DevName, DRIVER_DEVICE_NAME);
    status = IoCreateDevice(DriverObject, 0, &DevName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, TRUE, &devobj);

    if (!NT_SUCCESS(status)) {
        DbgPrintEx(DPFLTR_DEFAULT_ID,
            DPFLTR_INFO_LEVEL,
            "[%s] Error in IoCreateDevice(%wZ) = %lx\r\n",
            __FUNCTION__,
            &DevName,
            status);
        return status;
    }

    DbgPrintEx(DPFLTR_DEFAULT_ID,
        DPFLTR_INFO_LEVEL,
        "[%s] IoCreateDevice(%wZ) = %lx\r\n",
        __FUNCTION__,
        &DevName,
        status);

    RtlInitUnicodeString(&SymLink, DRIVER_DOS_DEVICE_NAME);
    status = IoCreateSymbolicLink(&SymLink, &DevName);

    if (!NT_SUCCESS(status)) {
        DbgPrintEx(DPFLTR_DEFAULT_ID,
            DPFLTR_INFO_LEVEL,
            "Error creating symlink!");
        return status;
    }

    DbgPrintEx(DPFLTR_DEFAULT_ID,
        DPFLTR_INFO_LEVEL,
        "[%s] IoCreateSymbolicLink(%wZ) = %lx\r\n",
        __FUNCTION__,
        &SymLink,
        status);

    devobj->Flags |= DO_BUFFERED_IO;

    for (int t = 0; t <= IRP_MJ_MAXIMUM_FUNCTION; t++) //set all MajorFunction's to unsupported
        DriverObject->MajorFunction[t] = UnsupportedIo;

    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverDispatch;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateIo;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseIo;
    DriverObject->DriverUnload = NULL; //nonstandard way of driver loading, no unload

    devobj->Flags &= ~DO_DEVICE_INITIALIZING;
    return status;
}

NTSTATUS DriverEntry(
	_In_ struct _DRIVER_OBJECT* DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	NTSTATUS status;
	UNICODE_STRING driverName;

	/* Due to the manual mapping these cannot be used! */
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	RtlInitUnicodeString(&driverName, L"\\Driver\\OwO");
	status = IoCreateDriver(&driverName, &DriverInitialize);

	if (status == STATUS_OBJECT_NAME_COLLISION) {
		DbgPrintEx(DPFLTR_DEFAULT_ID,
			DPFLTR_INFO_LEVEL,
			"[%s] Driver object %wZ already exists!??\r\n",
			__FUNCTION__, &driverName
		);
	}
	else {
		DbgPrintEx(DPFLTR_DEFAULT_ID,
			DPFLTR_INFO_LEVEL,
			"[%s] IoCreateDriver(%wZ) = %lx\r\n",
			__FUNCTION__,
			&driverName,
			status);
	}

	return status;
}
