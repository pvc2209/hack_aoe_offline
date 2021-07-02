// https://codelearn.io/sharing/hack-game-half-life-voi-c
// https://stackoverflow.com/a/13977501

#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>


using namespace std;

// convert 'char*' to 'const wchar_t*
const wchar_t *GetWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}

// Hàm lấy địa chỉ của module "hw.dll"
uintptr_t getModuleBaseAddress(const wchar_t* modName, DWORD procId)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {

                if (!_wcsicmp(GetWC(modEntry.szModule), modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

template<class T>
void change(DWORD pid, LPVOID address, T value) {
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!process) {
        cout << "Couldn't open process" << endl;
        return;
    }
    if (!WriteProcessMemory(process, address, &value, sizeof(value), 0)) {
        cout << "Couldn't write process memory" << endl;
    }
}
 
int main() {
    DWORD pid;
    HWND hGameWindow = FindWindow(NULL, _T("Age of Empires HD"));
    GetWindowThreadProcessId(hGameWindow, &pid);
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    DWORD baseAddress;
    DWORD moduleAddress = getModuleBaseAddress(L"empires.exe", pid);

    if (!ReadProcessMemory(pHandle, (LPVOID)(moduleAddress + 0x00188144), &baseAddress, sizeof(baseAddress), NULL)) {
        cout << "Couldn't read process memory";
        return -1;
    }

    
    
    if (!ReadProcessMemory(pHandle, (LPVOID)(baseAddress + 0x4BC), &baseAddress, sizeof(baseAddress), NULL)) {
        cout << "Couldn't read process memory";
        return -1;
    }

    if (!ReadProcessMemory(pHandle, (LPVOID)(baseAddress + 0xF8), &baseAddress, sizeof(baseAddress), NULL)) {
        cout << "Couldn't read process memory";
        return -1;
    }

    if (!ReadProcessMemory(pHandle, (LPVOID)(baseAddress + 0x50), &baseAddress, sizeof(baseAddress), NULL)) {
        cout << "Couldn't read process memory";
        return -1;
    }

    DWORD meatAddress = baseAddress;
    DWORD goldAddress = baseAddress + 0xC;
    DWORD woodAddress = baseAddress + 0x4;
    DWORD stoneAddress = baseAddress + 0x8;
 



    // cout << "Module address:" << hex << moduleAddress << endl;

    // cout << "Base address:" << hex << baseAddress << endl;

    // cout << "Gold address:" << hex << woodAddress << endl;
    

    float meat;
    float wood;
    float gold;
    float stone;

    meat = wood = gold = stone = 50000;

    change(pid, (LPVOID) meatAddress, meat);
    change(pid, (LPVOID) woodAddress, wood);
    change(pid, (LPVOID) goldAddress, gold);
    change(pid, (LPVOID) stoneAddress, stone);
         
    return 0;
}