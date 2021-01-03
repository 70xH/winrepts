#include "Windows.h"
#include <iostream>
#include <strings.h>

using namespace std;

/*
    RegCreateKeyExA Function -
    
    LSTATUS RegCreateKeyExA(
        HKEY                        hKey,
        LPCSTR                      lpSubKey,
        DWORD                       Reserved,
        LPSTR                       lpClass,
        DWORD                       dwOptions,
        REGSAM                      samDesired,
        const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
        PHKEY                       phkResult,
        LPDWORD                     lpdwDisposition
    );
*/

/*
    RegSetKeyValueA Function -

    LSTATUS RegSetKeyValueA(
        HKEY    hKey,
        LPCSTR  lpSubKey,
        LPCSTR  lpValueName,
        DWORD   dwType,
        LPCVOID lpData,
        DWORD   cbData
    );
*/

HKEY disableBackupPolicy(HKEY hKey)
{
    cout << "Disabling backup policy..." << endl;

    LPCSTR lpSubKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\SettingSync";

    long lstatus = RegCreateKeyExA(
        HKEY_CURRENT_USER,
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
        cout << "Creating the key failed and exited with error code: " << GetLastError() << endl;
        exit(0);
    }

    return hKey;
}

void disableSync()
{
    cout << "Disable synchronisation settings" << endl;
    cout << endl;

    HKEY hKey;

    hKey = disableBackupPolicy(hKey);

    RegCloseKey(hKey);
}

void turnOffSmartFilter()
{
    cout << "Turning of web content smart filter..." << endl;

    LPCSTR lpSubKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AppHost";
    HKEY hKey;

    long lstatus = RegCreateKeyExA(
        HKEY_CURRENT_USER,
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
        cout << "Creating the key failed and exited with error code: " << GetLastError() << endl;
        exit(0);
    }

    LPCSTR lpValueName = "EnableWebContentEvaluation";
    DWORD dwData = 0;

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
        cout << "Setting the value failed and exited with error code: " << GetLastError() << endl;
        exit(0);
    }

    RegCloseKey(hKey);
}

void advertisingInfo()
{
    cout << "Stop collecting advertising info..." << endl;

    LPCSTR lpSubKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AdvertisingInfo";
    HKEY hKey;

    long lstatus = RegCreateKeyExA(
        HKEY_CURRENT_USER,
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
        cout << "Creating the key failed and exited with error code: " << GetLastError() << endl;
        exit(0);
    }

    LPCSTR lpValueName = "Enabled";
    DWORD dwData = 0;

    lstatus = RegSetKeyValueA(
        hKey,
        NULL,
        lpValueName,
        REG_DWORD,
        (LPBYTE)&dwData,
        sizeof(dwData)
    );

    if ( lstatus != ERROR_SUCCESS )
    {
        cout << "Setting the value failed and exited with error code: " << GetLastError() << endl;
        exit(0);
    }

    RegCloseKey(hKey);
}

void writingInfo()
{
    cout << "Stop sending inking and typing info to Microsoft..." << endl;

    LPCSTR lpSubKey = "SOFTWARE\\Microsoft\\Input\\TIPC";
    HKEY hKey;

    long lstatus = RegCreateKeyExA(
        HKEY_CURRENT_USER,
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
        cout << "Creating the key failed and exited with error code: " << GetLastError() << endl;
        RegCloseKey(hKey);
        exit(0);
    }

    LPCSTR lpValueName = "Enabled";
    DWORD dwData = 0;

    lstatus = RegSetKeyValueA(
        hKey,
        NULL,
        lpValueName,
        REG_DWORD,
        (LPBYTE)&dwData,
        sizeof(dwData)
    );

    if ( lstatus != ERROR_SUCCESS )
    {
        cout << "Setting the value failed and exited with error code: " << GetLastError() << endl;
        RegCloseKey(hKey);
        exit(0);
    }

    RegCloseKey(hKey);
}

void locationPrinting()
{
    cout << "Change the location aware printing..." << endl;

    LPCSTR lpSubKey = "Printers\\Defaults";
    HKEY hKey;

    long lstatus = RegCreateKeyExA(
        HKEY_CURRENT_USER,
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
        cout << "Creating the key failed and exited with error code: " << GetLastError() << endl;
        RegCloseKey(hKey);
        exit(0);
    }

    LPCSTR lpValueName = "NetID";
    LPCSTR dwData = "{00000000-0000-0000-0000-000000000000}";

    lstatus = RegSetValueEx(
        hKey,
        lpValueName,
        0,
        REG_SZ,
        (LPBYTE)dwData,
        strlen(dwData)+1
    );

    if ( lstatus != ERROR_SUCCESS )
    {
        cout << "Setting the value failed and exited with error code: " << GetLastError() << endl;
        RegCloseKey(hKey);
        exit(0);
    }

    RegCloseKey(hKey);
}

void langOptOut()
{
    cout << "Opting out from Language based reccomendations..." << endl;

    LPCSTR lpSubKey = "Control Panel\\International\\User Profile";
    HKEY hKey;

    long lstatus = RegCreateKeyExA(
        HKEY_CURRENT_USER,
        lpSubKey,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS | KEY_WOW64_64KEY,
        NULL,
        &hKey,
        NULL
    );

    if(lstatus != ERROR_SUCCESS)
    {
        cout << "Creating the key failed and exited with error code: " << GetLastError() << endl;
        RegCloseKey(hKey);
        exit(0);
    }

    LPCSTR lpValueName = "HttpAcceptLanguageOptOut";
    DWORD dwData = 1;

    lstatus = RegSetKeyValueA(
        hKey,
        NULL,
        lpValueName,
        REG_DWORD,
        (LPBYTE)&dwData,
        sizeof(dwData)
    );

    if ( lstatus != ERROR_SUCCESS )
    {
        cout << "Setting the key failed and exited with error code: " << GetLastError() << endl;
        RegCloseKey(hKey);
        exit(0);
    }

    RegCloseKey(hKey);
}

int main(int argc, char *argv[])
{
    cout << "Fixing the registry to improve privacy" << endl;
    cout << endl;

    langOptOut();
    locationPrinting();
    writingInfo();
    advertisingInfo();
    turnOffSmartFilter();

    cout << endl;
    disableSync();

    cout << "Done and Done" << endl;
    return 0;
}