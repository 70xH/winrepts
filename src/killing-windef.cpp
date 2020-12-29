#include "Windows.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void executeShellCommands()
{
    string tasks[] = {"\\Microsoft\\Windows\\Windows Defender\\Windows Defender Cache Maintenance",
                    "\\Microsoft\\Windows\\Windows Defender\\Windows Defender Cleanup",
                    "\\Microsoft\\Windows\\Windows Defender\\Windows Defender Scheduled Scan",
                    "\\Microsoft\\Windows\\Windows Defender\\Windows Defender Verification"};
    int size = sizeof(tasks)/sizeof(tasks[0]);
    string command;

    for ( int i=0; i<size ; i++)
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
    remove("temp.ps1");
}

int main()
{
    cout << "Now, Lets kill the windows defender..." << endl;
    cout << endl;

    executeShellCommands();
    return 0;
}