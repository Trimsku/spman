#ifndef SPMAN_HPP
#define SPMAN_HPP
#include "astd/Platform.h"

#if defined(P_WINDOWS)
    #include "windows_required/dirent.h"
#elif defined(P_UNIX) 
    #include <dirent.h>
#else
    #error "Your platform not support, please contact with developer. https://github.com/Trimsku/AsciiMachine/issue"
#endif

//! Includes

#include <stdio.h>
#include "astd/string.hpp"
#include "astd/regex.hpp"
#include "astd/container.hpp"
#include "util/gen_uuid.hpp"

// spman private macros
#define sfputs(text) fputs(text "\n", fptr)
// spman private macros
#define println(text, ...) printf(text "\n", __VA_ARGS__)
// spman private macros
#define putsln(text) puts(text "\n")
// spman private macros
#define sfprintf(text, ...) fprintf(fptr, text "\n", __VA_ARGS__)

extern astd::string help[9];
extern astd::container_s libraries_paths;

enum generator_type {
    vs2017 = 1,
    gmake4 = 2,
};

//! Spman namespace.
namespace spman
{
    //! Util functions
    astd::string include_subdirectory_files(const char* folder_name_with_pattern);
    astd::string include_file(const char* filename);
    void add_library_path(const char* path_to_library);

    class Filemake {
        private:
            template<typename T>
            T add(T arg_);
 
            template<typename T, typename... Args>
            astd::string add(T arg_, Args... args);

            astd::string files;
            astd::string proj_name;
            astd::string uuid;
            astd::container<const char*> library_names;
        public:

            Filemake(const char* project_name_);
            ~Filemake();

            void GenerateFile(int project_generator);

            template<typename T, typename... Args>
            void include_files(T first, Args... args);
            void link_library(const char *name);

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
            void setName(astd::string new_name);
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

    template<typename T, typename... Args>
    void Filemake::include_files(T first, Args... args) {
        files += first;
        files += ' ';
        files += add(args...); 
    }
} // namespace spman
#endif // SPMAN_HPP