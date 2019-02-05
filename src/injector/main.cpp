#include <windows.h>
#include <string>
//#pragma comment(lib, "user32.lib")

int main(int argc, char** argv)
{
	if(argc < 3) {
		return 1;
	}

	unsigned long processId = std::stoul(argv[1]);
	std::string dllPath(argv[2]);

	if(HANDLE client = OpenProcess(PROCESS_ALL_ACCESS, false, processId)) {
		if(LPVOID LoadLibA = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA")) {
			if(LPVOID allocString = (LPVOID)VirtualAllocEx(client, NULL, dllPath.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) {
				if(WriteProcessMemory(client, (LPVOID)allocString, dllPath.c_str(), dllPath.length(), NULL)) {
					CreateRemoteThread(client, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibA, (LPVOID)allocString, 0, NULL);
					return 0;
				}
			}
		}

		CloseHandle(client);
	}
	return 0;
}