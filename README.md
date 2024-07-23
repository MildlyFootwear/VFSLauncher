# VFSLauncher

## Description
Command argument tool for setting up a virtual file system using a text file to store the structure of it and launching a desired executable hooked to it.  
https://github.com/ModOrganizer2/usvfs is used to construct the VFS.

## Function and Use
Profiles are set up by creating a text file with paths to two directories separated by a ";". The first one will be used to store created or modified files and will be "laid over" the second directory in the virtual filespace.

Launch with arguments to load a text file that will determine the structure of the virtual file system, the executable you wish to hook into it, and a second text file containing the arguments you wish to pass to the executable.

For example: VFSLauncher.exe "Profiles\Profile1.txt" "C:\Windows\system32\cmd.exe" 

...will launch the command prompt with the virtual file system specified in the Profiles folder present in the executable's directory.

An example profile to pass as the first argument;

```
C:\mods;C:\Game
C:\saves;C:\Users\ExampleUser\My Documents\My Games
```


would make the launched executable think that whatever is in the mods folder is actually under the Game folder. If the executable were a game and tried to make a save in the directory most games will save to by default, it will instead be redirected to "C:\saves".

Do note that when designing profiles, having a path end in either "/" or "\" will break most file operations the launched executable attempts.

## Building

Built using Visual Studio 2022 and the Visual Studio 17 2022 Win64 CMake generator. You will likely need C++ packages installed into Visual Studio as well as the Windows 10 SDK if you want to build and use as is.

After building, be sure to put "usvfs_64.dll" in the same directory as the built executable.