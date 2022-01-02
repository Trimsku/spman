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
#include <string>
//#include "astd/string.hpp"
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

extern std::string help[9];
extern astd::container_s libraries_paths;
extern astd::container_s include_paths;
extern std::string global_args;

enum GenType {
    vs2017 = 1,
    gmake4= 2,
};


enum ProjectType {
    App = 1,
    Lib = 2,
};

//! Spman namespace.
namespace spman
{
    //! Util functions
    template<typename... Args>
    void add_compile_options(Args ... args);
    std::string include_subdirectory_files(const char* folder_name_with_pattern);
    std::string include_file(const char* filename);
    void add_library_path(const char* path_to_library);
    void add_include_path(const char* path_to_library);

    namespace priv
    {
        template<typename T>
        T add(T arg_);
 
        template<typename T, typename... Args>
        std::string add(T arg_, Args... args);
    } // namespace priv

    class Filemake {
        private:
            std::string files;
            std::string proj_name;
            std::string uuid;
            astd::container_s library_names;
        public:

            Filemake(const char* project_name_);
            ~Filemake();

            void GenerateFile(int project_generator, int proj_type = ProjectType::App);

            template<typename T, typename... Args>
            void include_files(T first, Args... args);
            void link_library(std::string name);

            std::string getProjectName();
            std::string getUuid();
            template<typename ... Args>
            friend void add_compile_options(Args ... args);
    };

    //! Workspace class

    class MakeWorkspace {
        private:
            astd::container<std::string> uuids_and_names;
            std::string name;
        public:
            MakeWorkspace(std::string name);
            ~MakeWorkspace();
            void AddProject(spman::Filemake &project);
            void GenerateFile(int generator_type, int proj_type = ProjectType::App);
            void createClass();
            void setName(std::string new_name);
    };

    //! Template functions

    template<typename T>
    T priv::add(T arg_){
        return arg_;
    }
    template<typename T, typename... Args>
    std::string priv::add(T arg_, Args... args) {
        std::string new_str(arg_);
        new_str += ' ';
        new_str += add(args...);
        return new_str;
    }

    template<typename T, typename... Args>
    void Filemake::include_files(T first, Args... args) {
        files += first;
        files += ' ';
        files += priv::add(args...); 
    }

    template<typename... Args>
    void add_compile_options(Args ... args) {
        global_args += priv::add(args..., "");
    }
} // namespace spman
#endif // SPMAN_HPP