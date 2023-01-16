#include <iostream>
#include "../CheatDriver/DriverConfig.h"
#include "../CheatDriver/IOCTLCodes.h"
#include "Process.h"
#include <windows.h>
#include <vector>

void thirdPerson() {

}

int main()
{
	// Open connection to driver
	HANDLE driver = CreateFileW(
		DRIVER_DEVICE_PATH,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr
	);

	if (driver == NULL || driver == 0) {
		printf("Driver handle returned 0! :(\n");
		DWORD error = GetLastError();
		printf("Error: 0x%X\n", error);
		return 1;
	}
	printf("Driver handle: %lu\n", driver);
	DWORD tf2Id = GetProcessId(L"hl2.exe");
	printf("TF2 ID: %lu\n", tf2Id);
	DWORD moduleBase = GetModuleBaseAddress(tf2Id, L"client.dll");
	printf("client.dll Base Address: 0x%X\n", moduleBase);
	printf("Running hack loop...\n");

	DWORD localPlayer = moduleBase + 0xc48470;

	char playerBuffer[1024]{ 0 };

	DRIVER_COPY_MEMORY copy = {};
	copy.ProcessId = tf2Id;
	copy.Source = localPlayer;
	copy.Target = *playerBuffer;
	copy.Size = 300;
	copy.Write = FALSE;

	DeviceIoControl(
		driver,
		IOCTL_DRIVER_COPY_MEMORY,
		&copy,
		sizeof(copy),
		&copy,
		sizeof(copy),
		0,
		0);

	printf("Call finished?\n");

	std::cout << playerBuffer << std::endl;

	while (true) {

	}
}
