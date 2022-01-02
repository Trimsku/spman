#include "spman/spman.hpp"

extern "C" {
    #include "lua/lua.h"
    #include "lua/lauxlib.h"
    #include "lua/lualib.h"
}
#include <cstring>

/*** Project ***/
int generator = -1;
int proj_type = -1;
int filemake_index = 0;
std::string files;
std::string name;
bool cc = false;
astd::container_s library_names;
/*** Project end ***/
/*** Workspace ***/
spman::MakeWorkspace mWorkspace("unnamed_workspace");
/*** Workspace end ***/

static int AddIncludePath(lua_State * L) {
    spman::add_include_path(lua_tostring(L, 1));
    return 1;
}

static int Workspace(lua_State * L) {
    mWorkspace.setName(lua_tostring(L, 1));
    return 1;
}

static int Platform(lua_State * L) {
    #if defined(P_UNIX) && !defined(P_MACOS)
        lua_pushstring(L, "LINUX");
    #elif defined(P_MACOS)
        lua_pushstring(L, "MACOS");
    #elif defined(P_WINDOWS)
        lua_pushstring(L, "WINDOWS");
    #endif
    return 1;
}

static int IsCrossCompiler(lua_State * L) {
    return cc;
}

static int LinkLibrary(lua_State * L) {
    std::string tmp(lua_tostring(L, 1));
    library_names.push(tmp);
    return 1;
}

static int AddLibraryPath(lua_State * L) {
    spman::add_library_path(lua_tostring(L, 1));
    return 1;
}

static int Project(lua_State * L) {
    if(filemake_index == 0 ) {
        ++filemake_index;
        name = lua_tostring(L, 1);
    } else {
        spman::Filemake file(name.c_str());
        file.include_files(files.c_str(), " "); // All files;
        for(int i = 0; i < library_names.size(); i++) {
            file.link_library(library_names[i]);
        }
        file.GenerateFile(generator, proj_type);
        mWorkspace.AddProject(file);

        name = lua_tostring(L, 1);
        files = "";
        ++filemake_index;
    }
    return 1;
}

static int Files(lua_State * L) {
    int args = lua_gettop(L);
    for(int i = 1; i <= args; i++) {
        files += spman::include_subdirectory_files(lua_tostring(L, i));
        files += ' ';
    }
    return 1;
}

static int AddCompileOptions(lua_State * L) {
    int args = lua_gettop(L);
    for(int i = 1; i <= args; i++) {
        if(lua_tostring(L, i)[0] == '`') {
            bool isError = true;
            std::string options = "";
            for(int j = i; j <= args ; j++) {
                std::string option = lua_tostring(L, i);
                if(option.at(option.size()) == '`') {
                    isError = false;
                    break;
                } else {
                    options += option;
                    options += ' ';
                }
            }
            if(isError) {
                printf("SyntaxParseError: expected ` in CompileOptions");
                exit(0);
            } else {
                spman::add_compile_options(options);
            }
        }
        spman::add_compile_options(lua_tostring(L, i));
    }
    return 1;
}

static int SetProjectType(lua_State * L) {
    if(strcmp(lua_tostring(L, 1), "Application") == 0) { 
        proj_type = ProjectType::App;
    } else if(strcmp(lua_tostring(L, 1), "SharedLib") == 0) {
        proj_type = ProjectType::Lib;
    } else {
        printf("ParseError: there isn't `%s` in available project types. Available: Application, SharedLib", lua_tostring(L, 1));
    }
    return 1;
}

int main(int argc, char** argv) {

    std::string help = "all commands; \n\
`--path-to-file=name`, `-p=name` - set path to lua file \n\
`--filename=name`, `-f=name` - set name for lua file \n\
`--generator=[gmake4, vs2017]`, `-g=[gmake4, vs2017]` - set generator\n\
`--spman-functions-help`, `-sfh` - return spman functions\n\
\n";

    std::string functions_help = 
"libs - libraries\n\
decl - declaration \n\n\n\
\tWorkspace \"your_workspace_name\"  - set the main workspace name\n\n\
\tProject \"your_project_name\" - set the project name. Set after `Workspace` decl.\n\n\
\tFiles (\"...\") - set files for compiling, set after `Project` decls. \n\n\
\tPlatform - return platform. Can return: MACOS, LINUX and WINDOWS \n\n\
\tLinkLibrary \"library_name\" - get library from `any path` \n\n\
\tAddLibraryPath \"library_path\" - add library path to project paths of libs\n\n\
\tIsCrossCompiler - return true, if you add option --cc, otherwise false \n\n\
\tAddCompileOptions \"...\" - add compile options \n\n\
\tSetProjectType - set project type of project. Set after project decl. \n\n\
\tAddIncludePath - add include path to project include paths";

    std::string path = ".";
    std::string input_name = "make.lua";

    for(int i = 1; i < argc; i++) {
        if(astd::split(argv[i], '=')[0] == "--path-to-file" || astd::split(argv[i], '=')[0] == "-p") {
            path = astd::split(argv[i], '=')[1];
        } else if( strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0 ) {
            printf(help.c_str());
            exit(0);
        } else if( astd::split(argv[i], '=')[0] == "--filename" || astd::split(argv[i], '=')[0] == "-f") {
            input_name = astd::split(argv[i], '=')[1];
        } else if( strcmp(argv[i], "--cc") == 0 ) {
            input_name = astd::split(argv[i], '=')[1];
        } else if( strcmp(argv[i], "--spman-functions-help") == 0 || strcmp(argv[i], "-sfh") == 0 ) {
            printf(functions_help.c_str());
            exit(0);
        } else if(astd::split(argv[i], '=')[0] == "--generator" || astd::split(argv[i], '=')[0] == "-g") {
            if(astd::split(argv[i], '=')[1] == "gmake4") {
                generator = GenType::gmake4;
            } else if(astd::split(argv[i], '=')[1] == "vs2017") {
                generator = GenType::vs2017;
            } else {
                printf("ParseError: there isn't `%s` in available project generators. Available: gmake4, vs2017", astd::split(argv[i], '=')[1].c_str());
                exit(0);
            }
        }
    }
    int iErr = 0;
    lua_State *lua = luaL_newstate();
    lua_pushcfunction(lua, ::Workspace);
    lua_setglobal(lua, "Workspace");
    lua_pushcfunction(lua, ::Project);
    lua_setglobal(lua, "Project");
    lua_pushcfunction(lua, ::Files);
    lua_setglobal(lua, "Files");
    lua_pushcfunction(lua, ::Platform);
    lua_setglobal(lua, "Platform");
    lua_pushcfunction(lua, ::LinkLibrary);
    lua_setglobal(lua, "LinkLibrary");
    lua_pushcfunction(lua, ::AddLibraryPath);
    lua_setglobal(lua, "AddLibraryPath");
    lua_pushcfunction(lua, ::IsCrossCompiler);
    lua_setglobal(lua, "IsCrossCompiler");
    lua_pushcfunction(lua, ::AddCompileOptions);
    lua_setglobal(lua, "AddCompileOptions");
    lua_pushcfunction(lua, ::SetProjectType);
    lua_setglobal(lua, "SetProjectType");
    lua_pushcfunction(lua, ::AddIncludePath);
    lua_setglobal(lua, "AddIncludePath");
    
    luaopen_base (lua); 

    iErr = luaL_dofile(lua, (path + (path[path.size()] == '/'?input_name:"/"+input_name)).c_str());
    if (iErr != 0) {
        printf("LoadingError: %s\n", lua_tostring(lua, -1));
        exit(0);
    }
    if( generator == -1) {
        printf("Please, set generator with `--generator=x` option");
        exit(0);
    }
    if(proj_type == -1) {
        printf("Please, set project type in top of file. See `spman -h` for more details.");
        exit(0);
    }
    printf("Generator: %s\n", generator== GenType::gmake4?"GNU Make 4":"Visual Studio 2017");
    lua_close (lua);

    spman::Filemake file(name.c_str());
    file.include_files(files.c_str(), " "); // All files;
    for(int i = 0; i < library_names.size(); i++) {
        file.link_library(library_names[i]);
    }
    file.GenerateFile(generator, proj_type);
    mWorkspace.AddProject(file);
    mWorkspace.GenerateFile(generator, proj_type);
    return 0;
}