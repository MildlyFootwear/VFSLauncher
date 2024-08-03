#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex>
#include "usvfs.h"

bool debug = false;
bool debugParse = false;
bool debugEnv   = false;

/*
char* CheckForEnv(char* charArray, const char* charArray2)
{
    int loc = 0;
    char* finalchar = strdup(charArray);

    while (charArray[loc] == charArray2[loc]) {
        loc++;
    }

    if (charArray2[loc] == NULL && getenv(charArray2) != NULL) {
        char env[60];
        sprintf(env, getenv(charArray2));
        char sep           = '\\';
        char* charArrayAlt = strdup(charArray);
        char* trimmed      = strchr(charArrayAlt, sep);
        sprintf(finalchar, "%s%s\%s", getenv("HOMEDRIVE"), env, trimmed);

    } else {
        sprintf(finalchar, charArray);
    }
    if (debugEnv)
        printf("returning %s for %s with env check %s\n", finalchar, charArray,
               charArray2);

    return finalchar;

}
*/
wchar_t* ToW(const char* charArray)
{

    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;

}

int main(int argc, char* argv[])
{

    bool profilevalid = false;
    bool exevalid     = false;
    bool hasname         = false;
    bool error        = false;
    WCHAR * command   = new WCHAR[4096];
    char* inname      = new char[256];

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
        inname = argv[3];
        hasname = true;
    }
    if (hasname == false)
    {
        printf("No instance name given.");
    }
    if (argc > 4)
    {
        FILE* argsfile;
        argsfile               = fopen(argv[4], "r");
        if (argsfile)
        {

            int const bufferLength = 510;
            char buffer[bufferLength];
            fgets(buffer, bufferLength, argsfile);
            command = ToW(buffer);
            wprintf(L"Passing commands %ls\n", command);
        }
    }
    if (argc > 5)
    {
        ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
        debug = true;
    }

    if (debug == false)
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE); 

    if (profilevalid && exevalid && hasname)
    {

        auto parameters = usvfsCreateParameters();
        usvfsSetInstanceName(parameters, inname);
        usvfsSetDebugMode(parameters, false);
        usvfsSetLogLevel(parameters, LogLevel::Warning);
        usvfsSetCrashDumpType(parameters, CrashDumpsType::None);
        usvfsSetCrashDumpPath(parameters, "");
        usvfsSetProcessDelay(parameters, 200);

        usvfsInitLogging(false);
        usvfsCreateVFS(parameters);

        usvfsDisconnectVFS();
        usvfsFreeParameters(parameters);

        parameters = usvfsCreateParameters();
        printf("instance name %s\n", inname);
        usvfsSetInstanceName(parameters, inname);
        usvfsSetDebugMode(parameters, false);
        usvfsSetLogLevel(parameters, LogLevel::Warning);
        usvfsSetCrashDumpType(parameters, CrashDumpsType::None);
        usvfsSetCrashDumpPath(parameters, "");
        usvfsSetProcessDelay(parameters, 200);

        usvfsInitLogging(false);
        usvfsCreateVFS(parameters);

        // map stuff

        FILE* profile;
        int const bufferLength = 16384;
        char buffer[bufferLength];
        profile = fopen(argv[1], "r");
        fgets(buffer, bufferLength, profile);

            printf("\n\nbuffer %s\n", buffer);
            const char* sep = ";";
            char* pntsource = strtok(buffer, sep);
            while (pntsource != NULL)
            {
                char* pntdest = strtok(NULL, sep);

                LPWSTR source      = ToW(pntsource);
                LPWSTR destination = ToW(pntdest);

                if (destination != NULL && source != NULL) 
                {

                    if (debug)
                        wprintf(L"Linking %ls and %ls\n", source, destination);

                    usvfsVirtualLinkDirectoryStatic(source, destination, LINKFLAG_RECURSIVE);
                    usvfsVirtualLinkDirectoryStatic(destination, source, LINKFLAG_MONITORCHANGES);
                    usvfsVirtualLinkDirectoryStatic(source, destination, LINKFLAG_CREATETARGET);

                }
                pntsource = strtok(NULL, sep);
            }
            
        
        fclose(profile); 

            char tree[4096];
            size_t fuck      = 4096;
            size_t* treesize = &fuck;
            usvfsCreateVFSDump(tree, treesize);
            printf(tree);
        
            

        // spawn process
        {
            STARTUPINFOW si{0};
            si.cb = sizeof(si);
            PROCESS_INFORMATION pi{0};

            if (usvfsCreateProcessHooked(ToW(argv[2]), command, nullptr, nullptr, TRUE, 0, 0, nullptr, &si, &pi)) {

                WaitForSingleObject(pi.hProcess, INFINITE);

                DWORD exit = 99;
                if (!GetExitCodeProcess(pi.hProcess, &exit)) {
                    std::cerr << "process failed\n";
                    error = true;
                }

                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            } else {
                std::cerr << "create process failed\n";
                error = true;
            }
        }
        // free stuff
        usvfsDisconnectVFS();
        usvfsFreeParameters(parameters);
        
            printf("\nVFS ended.\n");
        
        
    } else {
        error = true;
    }

    if (error || debug) {
        ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
        printf("\n");
        Sleep(10000);
    }
        
        
        
}

