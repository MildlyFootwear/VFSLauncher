# VFSLauncher

## Description
Command argument tool for setting up a virtual file system using a text file to store the structure of it and launching a desired executable hooked to it.

## Function and Use
Profiles are set up by creating a text file with paths to two directories separated by a ";". The first one will be used to store created or modified files and will be "laid over" the second directory in the virtual filespace.

Loading a profile with a text file such as


`C:\mods;C:\Game`
`C:\saves;HOMEPATH\My Documents\My Games`


will make the example game executable think that whatever is in the mods folder is actually under the Game folder and will the game will save info to "C:\saves" instead of the default folder under My Documents.

Launch with command arguments to load a profile and the executable you wish to hook into it.

For example: VFSLauncher.exe "Profiles\Profile1.txt" "C:\Windows\system32\cmd.exe" will launch the command prompt with the virtual file system specified in the Profiles folder present in the executable's directory.

## Building

Built using Visual Studio 2022 and the Visual Studio 17 2022 Win64 CMake generator. You will likely need C++ packages installed into Visual Studio as well as the Windows 10 SDK if you want to build and use as is.