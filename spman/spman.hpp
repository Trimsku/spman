#ifndef SPMAN_HPP
#define SPMAN_HPP
#include "astd/Platform.hpp"

#if defined(P_WINDOWS) && !defined(P_WIN_UNIX)
    #include "windows_required/dirent.h"
#elif defined(P_UNIX) && !defined(P_WIN_UNIX)
    #include <dirent.h>
#elif defined(P_MINGW)
    #include "/usr/i686-w64-mingw32/include/dirent.h"
#else
    #error "Your platform not support, please contact with developer. https://github.com/Trimsku/AsciiMachine/issue"
#endif

//! Includes

#include <stdio.h>
#include "astd/string.hpp"
#include "astd/regex.hpp"
#include "astd/container.hpp"
#include "util/gen_uuid.hpp"
#include <errno.h>

//! Defines
#define Workspace(name) spman::MakeWorkspace main_workspace(name);

#if defined(CMAKE_LIKE) && !defined(PREMAKE_LIKE)

#define project_name(name) {\
    spman::Filemake file(#name);
#define projectEnd(...) file.GenerateFile(generator); \
                    main_workspace.AddProject(file); \
                        }

#define GenerateWorkspaceFile() main_workspace.GenerateFile(generator);

#define Set(variable_name, variable_data) auto variable_name = variable_data;
//#ifdef SPM_WINDOWS_ 
//#define Set_Include_Files(...) file.set_path_to_hpp_files(__VA_ARGS__);
#define Compile_Options(...) file.set_args(__VA_ARGS__);

/*#elif SPM_UNIX_
    #define win_hpp_files(fidir_names)
#endif*/
// Create instruction for create 
#define Add_Files(first, ...) file.include_files(first, __VA_ARGS__);
#define Set_Generator(type) \
    int generator; \
    if(type == generator_type::make4) { \
        generator = generator_type::make4; \
    } else if(type == generator_type::vc2017) { \
        generator = generator_type::vc2017; \
    } else { \
        println("In line %d, file %s, function Set_Generator(),\nSelect a generator with generator_type::<type>", __LINE__, __FILE__); \
        puts("Available generators:"); \
        puts("\tgenerator_type::vc2017"); \
        puts("\tgenerator_type::make4"); \
        exit(0); \
    }
// Create instrcution for;library.
// type - use macros SHARED or STATIC.
// first - main_file.
// . . . - files
// Example use: Add_Library(SHARED, "lib/main.cpp", "lib/example.cpp", "lib/example2.cpp")
#define Add_Library(type, first, ...) file.create_library(type, first, __VA_ARGS__);
#endif // #ifdef CMAKE_LIKE
#define SHARED 1
#define STATIC 0
// spman private macros
#define sfputs(text) fputs(text "\n", fptr)
// spman private macros
#define println(text, ...) printf(text "\n", __VA_ARGS__)
// spman private macros
#define putsln(text) puts(text "\n")
// spman private macros
#define sfprintf(text, ...) fprintf(fptr, text "\n", __VA_ARGS__)

extern astd::string help[9];

enum generator_type {
    vc2017 = 1,
    make4 = 2,
};

//! Spman namespace.
namespace spman
{
    astd::string include_subdirectory_files(const char* folder_name, const char* expression = "");
    astd::string include_file(const char* filename);
    class Filemake {
        private:
            template<typename T>
            T add(T arg_);
 
            template<typename T, typename... Args>
            astd::string add(T arg_, Args... args);

            astd::string files;
            astd::string proj_name;
            astd::string uuid;
            astd::string arg;
        public:

            Filemake(const char* project_name_);
            ~Filemake();

            void GenerateFile(int project_generator);

            template <typename... Args>
            void set_args(Args... args) noexcept;

            template<typename T, typename... Args>
            void include_files(T first, Args... args);

            astd::string getArgs();
            astd::string getProjectName();
            astd::string getUuid();
    };

    //! Workspace class

    class MakeWorkspace {
        private:
            astd::container<astd::string> uuids_and_names;
            astd::string name;
        public:
            MakeWorkspace(astd::string name);
            ~MakeWorkspace();
            void AddProject(spman::Filemake &project);
            void GenerateFile(int generator_type);
            void createClass();
            friend class Filemake;
    };

    //! Template functions

    template<typename T>
    T Filemake::add(T arg_){
        return arg_;
    }
    template<typename T, typename... Args>
    astd::string Filemake::add(T arg_, Args... args) {
        astd::string new_str(arg_);
        new_str += ' ';
        new_str += add(args...);
        return new_str;
    }

    template <typename... Args>
    void Filemake::set_args(Args... args) noexcept {
        arg += add(args...);
    }

    template<typename T, typename... Args>
    void Filemake::include_files(T first, Args... args) {
        files += first;
        files += ' ';
        files += add(args...); 
    }
} // namespace spman
#endif // SPMAN_HPP