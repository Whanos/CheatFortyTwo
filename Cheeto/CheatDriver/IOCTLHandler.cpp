#include <ntifs.h>
#include <ntddk.h>
#include "IOCTLCodes.h"
#include "Driver.h"

// Performs a memory copy request.
NTSTATUS DriverCopy(IN PDRIVER_COPY_MEMORY copy) {

	DbgPrint(("Something called DriverCopy..."));

	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS process;

	status = PsLookupProcessByProcessId((HANDLE)copy->ProcessId, &process);

	if (NT_SUCCESS(status)) {
		PEPROCESS sourceProcess, targetProcess;
		PVOID sourcePtr, targetPtr;

		if (copy->Write == FALSE) {
			sourceProcess = process;
			targetProcess = PsGetCurrentProcess();
			sourcePtr = (PVOID)copy->Target;
			targetPtr = (PVOID)copy->Source;
		}
		else {
			sourceProcess = PsGetCurrentProcess();
			targetProcess = process;
			sourcePtr = (PVOID)copy->Source;
			targetPtr = (PVOID)copy->Target;
		}

		SIZE_T bytes;
		status = MmCopyVirtualMemory(sourceProcess, sourcePtr, targetProcess, targetPtr, copy->Size, KernelMode, &bytes);

		ObDereferenceObject(process);
	}

	return status;
}

NTSTATUS DriverDispatch(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(Irp);
	PVOID ioBuffer = Irp->AssociatedIrp.SystemBuffer;
	ULONG inputLength = irpStack->Parameters.DeviceIoControl.InputBufferLength;

	if (irpStack->MajorFunction == IRP_MJ_DEVICE_CONTROL) {
		ULONG ioControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;

		if (ioControlCode == IOCTL_DRIVER_COPY_MEMORY) {
			if (ioBuffer && inputLength >= sizeof(DRIVER_COPY_MEMORY)) {
				Irp->IoStatus.Status = DriverCopy((PDRIVER_COPY_MEMORY)ioBuffer);
				Irp->IoStatus.Information = sizeof(DRIVER_COPY_MEMORY);
			}
			else {
				Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
			}
		}
		else {
			Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
		}
	}

	NTSTATUS status = Irp->IoStatus.Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS CreateIo(PDEVICE_OBJECT device_obj, PIRP irp) {
	UNREFERENCED_PARAMETER(device_obj);

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS CloseIo(PDEVICE_OBJECT device_obj, PIRP irp) {
	UNREFERENCED_PARAMETER(device_obj);

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

NTSTATUS UnsupportedIo(PDEVICE_OBJECT device_obj, PIRP irp) {
	UNREFERENCED_PARAMETER(device_obj);

	irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}
