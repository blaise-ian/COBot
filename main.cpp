#include <windows.h>

#include "CO/Client.h"
#include <map>

using namespace std;

static CO::CClient* Client;

extern "C"
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            // attach to process
            // return FALSE to fail DLL load
            char buff[_MAX_PATH];
            GetModuleFileName(hinstDLL, buff, _MAX_PATH);
            int i = 0, j = -1;
            while (buff[i] != 0)
            {
                if (buff[i] == '\\' || buff[i] == '/')
                    j = i;
                ++i;
            }
            if (j == -1)
            {
                MessageBox(0, "Failed to establish dll directory.", "Error", MB_OK);
                return FALSE;
            }
            buff[j+1] = 0;
            //SetCurrentDirectory(buff);
            Client = new CO::CClient(buff);
            break;
        }
        case DLL_PROCESS_DETACH:
            // detach from process

            delete Client;
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
