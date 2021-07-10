# spman
Hello, this is spman, simple automation build, used c++, for AsciiMachine.

How to compile with g++?:
```
g++ spm_main.cpp src/spman.cpp src/astd/* src/util/* -O3
./a.out
make
```
Or, soon, you can grab spman.so(or .dll)
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