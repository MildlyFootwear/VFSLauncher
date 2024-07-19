#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <memory>
#include <stdio.h>
#include <string.h>

#include "usvfs.h"

wchar_t* ToLPCWSTR(const char* charArray)
{
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}

int main(int argc, char* argv[])
{
    bool profilevalid = false;
    bool exevalid     = false;
    bool debug        = false;
    if (argc > 1) {
        char sep = '.';
        char fullpath[255];
        sprintf(fullpath, argv[1]);
        char* extension = strrchr(argv[1], sep) + 1;
        FILE* file;
        file = fopen(argv[1], "r");
        if (extension != nullptr) {
            if (strcmp(extension, "txt") == 0 && file) {
                fclose(file);
                profilevalid = true;
            } else if (strcmp(extension, "txt")) {
                printf("%s is not a text file.\n", fullpath);
            } else {
                printf("Profile file %s is not accessible or does not exist.\n",
                       argv[1]);
            }
        } else {
            printf("Profile file %s is not a file.\n", argv[1]);
        }

    } else {
        printf("Profile argument missing.\n");
    }
    if (profilevalid) {
        printf("Using profile %s.\n", argv[1]);
    }
    if (argc > 2) {
        char sep = '.';
        char fullpath[255];
        sprintf(fullpath, argv[2]);
        char* extension = strrchr(argv[2], sep) + 1;
        FILE* file;
        if (extension != nullptr) {

            if (strcmp(extension, "exe") == 0 && (file = fopen(argv[2], "r"))) {
                fclose(file);
                exevalid = true;
            } else if (strcmp(extension, "exe")) {
                printf("%s is not an executable.\n", fullpath);
            } else {
                printf("Executable %s is not accessible or does not exist.\n", argv[2]);
            }

        } else {
            printf("Executable %s is not a file.\n", argv[2]);
        }

    } else {
        printf("Executable argument missing.\n");
    }
    if (exevalid) {
        printf("Using executable %s.\n", argv[2]);
    }
    if (argc > 3) {

        if (strcmp(argv[3], "debug") == 0)
            debug = true;
    }


    if (profilevalid && exevalid)
    {
        auto parameters = usvfsCreateParameters();

        usvfsSetInstanceName(parameters, "instance");
        usvfsSetDebugMode(parameters, false);
        usvfsSetLogLevel(parameters, LogLevel::Warning);
        usvfsSetCrashDumpType(parameters, CrashDumpsType::None);
        usvfsSetCrashDumpPath(parameters, "");
        usvfsSetProcessDelay(parameters, 200);

        usvfsInitLogging(false);
        auto instance = usvfsCreateVFS(parameters);

        // map stuff

        FILE* profile;
        int const bufferLength = 510;
        char buffer[bufferLength];
        profile = fopen(argv[1], "r");
        while (fgets(buffer, bufferLength, profile)) {

            const char* sep = ";";
            char* pntsource = strtok(buffer, sep);
            char* pntdest       = strtok(NULL, sep);
            LPCWSTR source    = ToLPCWSTR(pntsource);
            LPCWSTR destination = ToLPCWSTR(pntdest);
            if (destination != NULL) {
                if (debug)
                    printf("%s to %s", pntsource, pntdest);
                usvfsVirtualLinkDirectoryStatic(source, destination,LINKFLAG_RECURSIVE);
                usvfsVirtualLinkDirectoryStatic(destination, source, LINKFLAG_MONITORCHANGES); 
                usvfsVirtualLinkDirectoryStatic(source, destination, LINKFLAG_CREATETARGET);
            }
        }
        fclose(profile); 

        if (debug)
        {
            char tree[4096];
            size_t fuck      = 4096;
            size_t* treesize = &fuck;
            usvfsCreateVFSDump(tree, treesize);
            printf(tree);
        }
            

        // spawn process
        {
            STARTUPINFOW si{0};
            si.cb = sizeof(si);
            PROCESS_INFORMATION pi{0};
            WCHAR command[] = L"--skip-launcher";

            if (usvfsCreateProcessHooked(ToLPCWSTR(argv[2]), command, nullptr, nullptr,
                                                                     TRUE, 0, 0, nullptr, &si, &pi)) {
                WaitForSingleObject(pi.hProcess, INFINITE);

                DWORD exit = 99;
                if (!GetExitCodeProcess(pi.hProcess, &exit)) {
                    std::cerr << "process failed\n";
                }

                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            } else {
                std::cerr << "create process failed\n";
            }
        }
        // free stuff
        usvfsDisconnectVFS();
        usvfsFreeParameters(parameters);
        printf("\n%s ended.", argv[2]);
        Sleep(2000);
    } else {
        Sleep(10000);
    }
        
        
}

