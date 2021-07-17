# spman
Hello, this is spman, simple automation build, used c++, for AsciiMachine.
```
Executable files, available: 
    For win32: releases/0.0.1/spm32.exe
    For win64: releases/0.0.1/spm.exe
    For linux, x86_64: releases/0.0.1/spm
        (tested on Manjaro Linux x86_64, gnome 3.38.4)
```
```
TODO:
1. Add xcode support.
2. Add BSD Make and NMake support. 
```
   
```
Available generators:

    GNU Make 4.0,
    Visual Studio C++, 2017.
```

I am used two, NOT MY projects:

First, https://github.com/kokke/tiny-regex-c, tiny_regex_c project.

Second, https://github.com/tronkko/dirent, dirent.h(UNIX), for Microsoft Windows.

Very simple example with main.cpp
```lua
Workspace "My_Workspace"

Project "My_Project"
    Files ( "main.cpp" )
```

... And that's example for building spman:
```lua
SetGenerator "gmake4"

Workspace "spm"

Project "spm"
    if Platform() == "LINUX" and not IsCrossCompiler() then
        LinkLibrary ( "dl" )
        if not IsCrossCompiler() then -- On linux you can download lua5.4.so
             LinkLibrary ( "lua5.4" ) 
        else  -- In mingw, I could not download lua
            Files( "lua/onelua.cpp" )
        end
    end
    Files ( "main.cpp", "src/*", "src/astd/*", "src/util/*" )
```
