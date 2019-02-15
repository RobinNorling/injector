#include <windows.h>
#include <string>

int main(int argc, char** argv) {
	if(argc < 3) {
		return 1;
	}

	std::string dllPath(argv[2]);

	if(void* client = OpenProcess(PROCESS_ALL_ACCESS, false, std::stoul(argv[1]))) {
		if(void* LoadLibA = (void*)GetProcAddress(GetModuleHandle((LPCSTR)"kernel32.dll"), (LPCSTR)"LoadLibraryA")) {
			if(void* allocString = (void*)VirtualAllocEx(client, NULL, dllPath.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) {
				if(WriteProcessMemory(client, (void*)allocString, dllPath.c_str(), dllPath.length(), NULL)) {
					if(CreateRemoteThread(client, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibA, (void*)allocString, 0, NULL)) {
						CloseHandle(client);
						return 0;
					}
				}
			}
		}
		CloseHandle(client);
	}

	return 1;
}
