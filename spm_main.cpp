#define CMAKE_LIKE
#include "spman/spman.hpp"
using namespace spman;

int main() {

    Set_Generator(generator_type::make4)
    Workspace("MyWorkspace")

    project_name( MyProject ) 

    Set(src_astd, include_subdirectory_files("src/astd", "*")) // include all files with .cpp names.
    Set(src, include_subdirectory_files("src", "spman.cpp")) // 
    Set(src_util, include_subdirectory_files("src/util", "*"))
    Set(include_astd, include_subdirectory_files("spman/astd", "*"))
    Set(include_spman, include_subdirectory_files("spman", "spman.hpp"))
    Set(include_util, include_subdirectory_files("spman/util", "*"))

    Set(main_file, include_file("/home/trimsky/spm/spm_main.cpp")) // include main.cpp
    
    #ifdef P_UNIX
        Compile_Options(
            "-Os",
            "-g"
        )
    #endif

    #ifdef P_WINDOWS
        Set(window_req, include_subdirectory_files("spman/windows_required", "*"))
        Add_Files(main_file, src, src_astd, include_astd, include_spman, include_util, window_req) 
    #else
        Add_Files(main_file, src, src_astd, src_util, include_astd, include_spman, include_util) // Add to Makefile command for create binary file.
    #endif
    //Add_Library(SHARED, main_file, src, src_astd)
    projectEnd( MyProject )

    GenerateWorkspaceFile();
    return 0;
}