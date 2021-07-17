#include "spman/spman.hpp"
extern "C" {
    #include "lua/lua.h"
    #include "lua/lauxlib.h"
    #include "lua/lualib.h"
}

/*** Project ***/
int generator;
int filemake_index = 0;
astd::string files;
astd::string name;
bool cc = false;
astd::container<const char*> library_names;
/*** Project end ***/
/*** Workspace ***/
spman::MakeWorkspace mWorkspace("Workspace");
/*** Workspace end ***/

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
static int SetGenerator(lua_State * L) {
    if(strcmp(lua_tostring(L, 1), "gmake4") == 0) {
        generator = generator_type::gmake4;
    } else if(strcmp(lua_tostring(L, 1), "vs2017") == 0) {
        generator = generator_type::vs2017;
    } else {
        printf("Your generator type don't support. Available: vs2017, gmake4");
    }
    return 1;
}
static int LinkLibrary(lua_State * L) {
    library_names.push(lua_tostring(L, 1));
    return 1;
}
static int AddLibraryPath(lua_State * L) {
    spman::add_library_path(lua_tostring(L, 1));
    return 1;
}
static int Project(lua_State * L) {
    if(filemake_index == 0 ) {
        filemake_index++;
        name = lua_tostring(L, 1);
    } else {
        spman::Filemake file(name.c_str());
        file.include_files(files, " "); // All files;
        for(int i = 0; i < library_names.size(); i++) {
            file.link_library(library_names[i]);
        }
        file.GenerateFile(generator);
        mWorkspace.AddProject(file);

        name = lua_tostring(L, 1);
        files = "";
        filemake_index++;
        library_names.~container();
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

int main(int argc, char** argv) {
    astd::string path = ".";
    int rgenerator = -1;
    for(int i = 1; i < argc; i++) {
        if(astd::split(argv[i], '=')[0] == "--path-to-file" || astd::split(argv[i], '=')[0] == "-p") {
            path = astd::split(argv[i], '=')[1];
        } else if( argv[i] == "--cc" ) {
            cc = true;
        } else if(astd::split(argv[i], '=')[0] == "--generator" || astd::split(argv[i], '=')[0] == "-g") {
            if(astd::split(argv[i], '=')[1] == "gmake4") {
                rgenerator = generator_type::gmake4;
            } else if(astd::split(argv[i], '=')[1] == "vs2017") {
                rgenerator = generator_type::vs2017;
            } else {
                printf("Your generator type don't support. Available: vs2017, gmake4");
            }
        }
    }
    int iErr = 0;
    lua_State *lua = luaL_newstate();
    lua_pushcfunction(lua, Workspace);
    lua_setglobal(lua, "Workspace");
    lua_pushcfunction(lua, SetGenerator);
    lua_setglobal(lua, "SetGenerator");
    lua_pushcfunction(lua, Project);
    lua_setglobal(lua, "Project");
    lua_pushcfunction(lua, Files);
    lua_setglobal(lua, "Files");
    lua_pushcfunction(lua, Platform);
    lua_setglobal(lua, "Platform");
    lua_pushcfunction(lua, LinkLibrary);
    lua_setglobal(lua, "LinkLibrary");
    lua_pushcfunction(lua, AddLibraryPath);
    lua_setglobal(lua, "AddLibraryPath");
    lua_pushcfunction(lua, IsCrossCompiler);
    lua_setglobal(lua, "IsCrossCompiler");

    luaopen_base (lua);
    
    iErr = luaL_dofile(lua, (path + (path[path.len()] == '/'?"spm_main.lua":"/spm_main.lua")).c_str());
    if (iErr != 0) {
        printf("LoadingError: %s\n", lua_tostring(lua, -1));
    }
    generator = (rgenerator?rgenerator:generator);
    printf("Generator: %s\n", generator==generator_type::gmake4?"gmake4":"Visual Studio 2017");
    lua_close (lua);
    if(filemake_index == 1) {
        spman::Filemake file(name.c_str());
        file.include_files(files, " "); // All files;
        for(int i = 0; i < library_names.size(); i++) {
            file.link_library(library_names[i]);
        }
        file.GenerateFile(generator);
        mWorkspace.AddProject(file);
    }
    mWorkspace.GenerateFile(generator);
    return 0;
}