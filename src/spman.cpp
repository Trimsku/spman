#include "../spman/spman.hpp"

astd::string help[9] = {
    "Usage: make [config=name] [target]",
    "",
    "Configurations:",
	"\tdebug",
	"\trelease",
	"",
    "Targets:",
    "\tall (default)",
	"\tclean",    
};

spman::Filemake::Filemake(const char* project_name_) : proj_name(project_name_), uuid(astd::createUUID()) {}

spman::Filemake::~Filemake() {} 

void spman::Filemake::GenerateFile(int project_generator) {
    FILE *fptr;
    astd::string files_save = files;
    if(project_generator == generator_type::make4) {
        println("Generated %s.mk, -G 'GNU Make 4.0'...", proj_name.c_str());
        if((fptr = fopen((proj_name + ".mk").c_str(), "w")) != NULL) {
            sfputs("ifndef verbose");
                sfputs("\tSILENT = @");
            sfputs("endif\n");
            sfputs("ifeq ($(config),debug)");
                sfputs("\tOBJS_DIR := obj/Debug");
                sfputs("\tBIN_DIR := bin/Debug");
                sfputs("\tARGS := -O0 -g -fsanitize=address");
            sfputs("else");
                sfputs("\tOBJS_DIR := obj/Release");
                sfputs("\tBIN_DIR := bin/Release");
                sfputs("\tARGS := -O3");
            sfputs("endif");
            astd::string files_o;
            fprintf(fptr, "%s","OBJS := ");
            for( ;; ) {
                astd::string istr;
                astd::container_s file_s = astd::split(files, ' ');
                if(file_s[1] == "" && file_s[0] == "") break;
                if(file_s[0] != "" ) {
                    istr = file_s[0];
                    files = file_s[1];
                    if(astd::split(istr, '.')[1] == "cpp" || astd::split(istr, '.')[1] == "c") {
                        astd::string fstr;
                        int i = istr.len() - 1;
                        for( ; istr[i] != '/' && i > 0; i--);
                        if(i > 0) i++; 
                        for(int j = i; j < istr.len(); j++) fstr += istr[j];
                        fprintf(fptr, "$(OBJS_DIR)/%s.o ", astd::split(fstr, '.')[0].c_str());
                    }
                } else if(file_s[0] == "") {
                    files = file_s[1];
                }
            }
            sfprintf("\nBINS := $(BIN_DIR)/%s\n", proj_name.c_str());
            sfputs("all: create_dirs $(OBJS) $(BINS)\n");
            sfputs(".PHONY: create_dirs clean");
            sfputs("create_dirs:");
	            sfputs("\t$(SILENT) mkdir -p $(OBJS_DIR)");
	            sfputs("\t$(SILENT) mkdir -p $(BIN_DIR)");
            sfputs("clean:");
	            sfputs("\trm -f $(OBJS_DIR)/*.o\n");
            sfprintf("$(BIN_DIR)/%s:", proj_name.c_str());
	            sfputs("\t$(SILENT) echo Building $@...");
	            sfputs("\t$(SILENT) $(CXX) $(CFLAGS) $(OBJS) $(ARGS) -o $@\n");

            files = files_save;
            for( ;; ) {
                astd::string istr;
                astd::container_s file_s = astd::split(files, ' ');
                if(file_s[1] == "" && file_s[0] == "") break;
                if(file_s[0] != "" ) {
                    istr = file_s[0];
                    files = file_s[1];
                    if(astd::split(istr, '.')[1] == "cpp" || astd::split(istr, '.')[1] == "c") {
                        astd::string fstr;
                        int i = istr.len() - 1;
                        for( ; istr[i] != '/' && i > 0; i--);
                        if(i > 0) i++; 
                        for(int j = i; j < istr.len(); j++) fstr += istr[j];
                        sfprintf("$(OBJS_DIR)/%s.o: %s", astd::split(fstr, '.')[0].c_str(), istr.c_str());
	                        sfputs("\t$(SILENT) echo Building $@...");
	                        sfprintf("\t$(SILENT) $(CXX) $(CFLAGS) -c $< -o $@ $(ARGS) -MMD -MP -MF $(OBJS_DIR)/%s.d\n", astd::split(fstr, '.')[0].c_str());
                    }
                } else if(file_s[0] == "") {
                    files = file_s[1];
                }
            }
            sfputs("-include $(OBJS:%.o=%.d)");
        }
        fclose(fptr);
    }
    if(project_generator == generator_type::vc2017) {
        if((fptr = fopen((proj_name + ".vcxproj").c_str(), "w")) != NULL) {
            println("Generated %s, -G 'vs2017'...", (proj_name + ".vcxproj").c_str());
            // VS C++ config.
            sfputs("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
            sfputs("<Project DefaultTargets=\"Build\" ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">");
            // Project Release|debug for Win32|x64
            sfputs("\t<ItemGroup Label = \"ProjectConfigurations\">");
            sfputs("\t<ProjectConfiguration Include=\"Debug|Win32\">");
                sfputs("\t\t<Configuration>Debug</Configuration>");
                sfputs("\t\t<Platform>Win32</Platform>");
            sfputs("\t</ProjectConfiguration>");
            sfputs("\t<ProjectConfiguration Include=\"Release|Win32\">");
                sfputs("\t\t<Configuration>Release</Configuration>");
                sfputs("\t\t<Platform>Win32</Platform>");
            sfputs("\t</ProjectConfiguration>");
            sfputs("\t<ProjectConfiguration Include=\"Debug|x64\">");
                sfputs("\t\t<Configuration>Debug</Configuration>");
                sfputs("\t\t<Platform>x64</Platform>");
            sfputs("\t</ProjectConfiguration>");
            sfputs("\t<ProjectConfiguration Include=\"Release|x64\">");
                sfputs("\t\t<Configuration>Release</Configuration>");
                sfputs("\t\t<Platform>x64</Platform>");
            sfputs("\t</ProjectConfiguration>");
            sfputs("\t</ItemGroup>");
            
            // Globals: GUID, ...
            sfputs("\t<PropertyGroup Label=\"Globals\"> "); 
            sfprintf("\t\t<ProjectGuid>{%s}</ProjectGuid> ", uuid.c_str());
            sfputs("\t</PropertyGroup>");

            // Other...
            sfputs("\t<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />");
            sfputs("\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">");
                sfputs("\t\t<ConfigurationType>Application</ConfigurationType>");
                sfputs("\t\t<UseDebugLibraries>true</UseDebugLibraries>");
                sfputs("\t\t<PlatformToolset>v141</PlatformToolset>");
                sfputs("\t\t<CharacterSet>Unicode</CharacterSet>");
            sfputs("\t</PropertyGroup>");
            sfputs("\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">");
                sfputs("\t\t<ConfigurationType>Application</ConfigurationType>");
                sfputs("\t\t<UseDebugLibraries>false</UseDebugLibraries>");
                sfputs("\t\t<PlatformToolset>v141</PlatformToolset>");
                sfputs("\t\t<WholeProgramOptimization>true</WholeProgramOptimization>");
                sfputs("\t\t<CharacterSet>Unicode</CharacterSet>");
            sfputs("\t</PropertyGroup>");
            sfputs("\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"Configuration\">");
                sfputs("\t\t<ConfigurationType>Application</ConfigurationType>");
                sfputs("\t\t<UseDebugLibraries>true</UseDebugLibraries>");
                sfputs("\t\t<PlatformToolset>v141</PlatformToolset>");
                sfputs("\t\t<CharacterSet>Unicode</CharacterSet>");
            sfputs("\t</PropertyGroup>");
            sfputs("\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"Configuration\">");
                sfputs("\t\t<ConfigurationType>Application</ConfigurationType>");
                sfputs("\t\t<UseDebugLibraries>false</UseDebugLibraries>");
                sfputs("\t\t<PlatformToolset>v141</PlatformToolset>");
                sfputs("\t\t<WholeProgramOptimization>true</WholeProgramOptimization>");
                sfputs("\t\t<CharacterSet>Unicode</CharacterSet>");
            sfputs("\t</PropertyGroup>");
            sfputs("\t<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />");
            sfputs("\t<ImportGroup Label=\"ExtensionSettings\">");
            sfputs("\t</ImportGroup>");
            sfputs("\t<ImportGroup Label=\"Shared\">");
            sfputs("\t</ImportGroup>");
            sfputs("\t<ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">");
            sfputs("\t\t<Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />");
            sfputs("\t</ImportGroup>");
            sfputs("\t<ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">");
            sfputs("\t\t<Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />");
            sfputs("\t</ImportGroup>");
            sfputs("<ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">");
                sfputs("\t\t<Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />");
            sfputs("\t</ImportGroup>");
            sfputs("\t<ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">");
                sfputs("\t\t<Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />");
            sfputs("\t</ImportGroup>");
            sfputs("\t<PropertyGroup Label=\"UserMacros\" />");
            sfputs("\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">");
                sfputs("\t\t<LinkIncremental>true</LinkIncremental>");
            sfputs("\t</PropertyGroup>");
            sfputs("\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">");
                sfputs("\t\t<LinkIncremental>true</LinkIncremental>");
            sfputs("\t</PropertyGroup>");
            sfputs("\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">");
                sfputs("\t\t<LinkIncremental>true</LinkIncremental>");
            sfputs("\t</PropertyGroup>");
            sfputs("\t<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">");
                sfputs("\t\t<LinkIncremental>true</LinkIncremental>");
            sfputs("\t</PropertyGroup>");
            sfputs("\t<ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">");
            sfputs("\t\t<ClCompile>");
                sfputs("\t\t\t<PrecompiledHeader>NotUsing</PrecompiledHeader>");
                sfputs("\t\t\t<WarningLevel>Level3</WarningLevel>");
                sfputs("\t\t\t<Optimization>Disabled</Optimization>");
                sfputs("\t\t\t<PreprocessorDefinitions>WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>");
                sfputs("\t\t\t<ConformanceMode>true</ConformanceMode>");
            sfputs("\t\t</ClCompile>");
            sfputs("\t\t<Link>");
                sfputs("\t\t\t<SubSystem>Console</SubSystem>");
                sfputs("\t\t\t<GenerateDebugInformation>true</GenerateDebugInformation>");
            sfputs("\t\t</Link>");
            sfputs("\t</ItemDefinitionGroup>");
            sfputs("\t<ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">");
                sfputs("\t\t<ClCompile>");
                    sfputs("\t\t\t<PrecompiledHeader>NotUsing</PrecompiledHeader>");
                    sfputs("\t\t\t<WarningLevel>Level3</WarningLevel>");
                    sfputs("\t\t\t<Optimization>Disabled</Optimization>");
                    sfputs("\t\t\t<PreprocessorDefinitions>_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>");
                    sfputs("\t\t\t<ConformanceMode>true</ConformanceMode>");
                sfputs("\t\t</ClCompile>");
                sfputs("\t\t<Link>");
                    sfputs("\t\t\t<SubSystem>Console</SubSystem>");
                    sfputs("\t\t\t<GenerateDebugInformation>true</GenerateDebugInformation>");
                sfputs("\t\t</Link>");
            sfputs("\t</ItemDefinitionGroup>");
            sfputs("\t<ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">");
                sfputs("\t\t<ClCompile>");
                    sfputs("\t\t\t<PrecompiledHeader>NotUsing</PrecompiledHeader>");
                    sfputs("\t\t\t<WarningLevel>Level3</WarningLevel>");
                    sfputs("\t\t\t<Optimization>MaxSpeed</Optimization>");
                    sfputs("\t\t\t<FunctionLevelLinking>true</FunctionLevelLinking>");
                    sfputs("\t\t\t<IntrinsicFunctions>true</IntrinsicFunctions>");
                    sfputs("\t\t\t<PreprocessorDefinitions>WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>");
                    sfputs("\t\t\t<ConformanceMode>true</ConformanceMode>");
                sfputs("\t\t</ClCompile>");
                sfputs("\t\t<Link>");
                    sfputs("\t\t\t<SubSystem>Console</SubSystem>");
                    sfputs("\t\t\t<EnableCOMDATFolding>true</EnableCOMDATFolding>");
                    sfputs("\t\t\t<OptimizeReferences>true</OptimizeReferences>");
                    sfputs("\t\t<GenerateDebugInformation>true</GenerateDebugInformation>");
                sfputs("\t\t</Link>");
            sfputs("\t</ItemDefinitionGroup>");
            sfputs("\t<ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">");
                sfputs("\t\t<ClCompile>");
                    sfputs("\t\t\t<PrecompiledHeader>NotUsing</PrecompiledHeader>");
                    sfputs("\t\t\t<WarningLevel>Level3</WarningLevel>");
                    sfputs("\t\t\t<Optimization>MaxSpeed</Optimization>");
                    sfputs("\t\t\t<FunctionLevelLinking>true</FunctionLevelLinking>");
                    sfputs("\t\t\t<IntrinsicFunctions>true</IntrinsicFunctions>");
                    sfputs("\t\t\t<PreprocessorDefinitions>NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>");
                    sfputs("\t\t\t<ConformanceMode>true</ConformanceMode>");
                sfputs("\t\t</ClCompile>");
                sfputs("\t\t<Link>");
                    sfputs("\t\t\t<SubSystem>Console</SubSystem>");
                    sfputs("\t\t\t<EnableCOMDATFolding>true</EnableCOMDATFolding>");
                    sfputs("\t\t\t<OptimizeReferences>true</OptimizeReferences>");
                    sfputs("\t\t\t<GenerateDebugInformation>true</GenerateDebugInformation>");
                sfputs("\t\t</Link>");
            sfputs("\t</ItemDefinitionGroup>");
            // File includes
            sfputs("\t<ItemGroup>");
            for(;;) {
                astd::string istr;
                astd::container_s file_s = astd::split(files, ' ');
                if(file_s[1] == "" && file_s[0] == "") break;

                if(file_s[0] != "" ) {
                    istr = file_s[0];
                    //println("CPP_istr: %s", istr.c_str());
                    files = file_s[1];
                    //println("CPP_files: %s", files.c_str());
                    if(astd::split(istr, '.')[1] == "cpp" || astd::split(istr, '.')[1] == "c") {
                        sfprintf("\t\t<ClCompile Include=\"%s\" />", istr.c_str());
                    } else if(astd::split(istr, '.')[1] == "hpp" || astd::split(istr, '.')[1] == "h") {
                        sfprintf("\t\t<ClInclude Include=\"%s\" />", istr.c_str());
                    }
                } else if(file_s[0] == "") {
                    files = file_s[1];
                }
            }for(;;) {
                astd::string istr;
                astd::container_s file_s = astd::split(files, ' ');
                if(file_s[1] == "" && file_s[0] == "") break;

                if(file_s[0] != "" ) {
                    istr = file_s[0];
                    //println("CPP_istr: %s", istr.c_str());
                    files = file_s[1];
                    //println("CPP_files: %s", files.c_str());
                    if(astd::split(istr, '.')[1] == "cpp" || astd::split(istr, '.')[1] == "c") {
                        sfprintf("\t\t<ClCompile Include=\"%s\" />", istr.c_str());
                    } else if(astd::split(istr, '.')[1] == "hpp" || astd::split(istr, '.')[1] == "h") {
                        sfprintf("\t\t<ClInclude Include=\"%s\" />", istr.c_str());
                    }
                } else if(file_s[0] == "") {
                    files = file_s[1];
                }
            }
            sfputs("\t</ItemGroup>");
            sfputs("\t<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />");
            sfputs("\t<ImportGroup Label=\"ExtensionTargets\">");
            sfputs("\t</ImportGroup>");
            sfputs("</Project>");
        }
        fclose(fptr);
        files = files_save;
        if((fptr = fopen((proj_name + ".vcxproj.filters").c_str(), "w")) != NULL) {
            println("Generated %s, -G 'vs2017'...", (proj_name + ".vcxproj.filters").c_str());
        sfputs("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
        sfputs("<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">");
            sfputs("\t<ItemGroup>");
            astd::string dirs;
            astd::container_s dirs_c;
            astd::container_s uuids;
            for( ;; ) {
                if(files == "") break;
                if( astd::split_end(astd::split(files, ' ')[0], '/')[1] != "") dirs += astd::split_end(astd::split(files, ' ')[0], '/')[0];
                dirs += ' ';
                files = astd::split(files, ' ')[1];
            }
            files = files_save;
            for( ;; ) {
                bool in_array = false;
                if( dirs == "") break;
                for(int i = 0; i < dirs_c.size(); i++) if(dirs_c[i] == astd::split(dirs, ' ')[0]) {in_array = true; break; }
                if(!in_array && astd::split(dirs, ' ')[0] != "") dirs_c.push(astd::split(dirs, ' ')[0]);
                dirs = astd::split(dirs, ' ')[1];
            }
            for(int i = 0; i < dirs_c.size(); i++) {
                uuids.push(astd::createUUID());
                sfprintf("\t\t<Filter Include=\"%s\">", dirs_c[i].c_str());
                    sfprintf("\t\t\t<UniqueIdentifier>{%s}</UniqueIdentifier>", uuids[i].c_str());
                sfputs("\t\t</Filter>");
            }
            sfputs("\t</ItemGroup>");
            sfputs("\t<ItemGroup>");
                for( ;; ) {
                    astd::string istr;
                    astd::container_s file_s = astd::split(files, ' ');
                    if(file_s[1] == "" && file_s[0] == "") break;

                    if(file_s[0] != "" ) {
                        istr = file_s[0];
                        files = file_s[1];
                        bool is_on_dir = false;
                        for(int i = 0; i < dirs_c.size(); i++) {
                            if(dirs_c[i] == astd::split_end(istr, '/')[0]) {
                                if(astd::split(istr, '.')[1] == "cpp" || astd::split(istr, '.')[1] == "c") {
                                    sfprintf("\t\t<ClCompile Include=\"%s\">", istr.c_str());
                                } else if(astd::split(istr, '.')[1] == "hpp" || astd::split(istr, '.')[1] == "h") {
                                    sfprintf("\t\t<ClInclude Include=\"%s\">", istr.c_str());
                                }
                                sfprintf("\t\t\t<Filter>%s</Filter>", dirs_c[i].c_str());
                                sfputs("\t\t</ClInclude>");
                                is_on_dir = true;
                                break;
                            }
                        }
                        if(is_on_dir == false) {
                        if(astd::split(istr, '.')[1] == "cpp" || astd::split(istr, '.')[1] == "c") {
                            sfprintf("\t\t<ClCompile Include=\"%s\" />", istr.c_str());
                        } else if(astd::split(istr, '.')[1] == "hpp" || astd::split(istr, '.')[1] == "h") {
                            sfprintf("\t\t<ClInclude Include=\"%s\" />", istr.c_str());
                        }
                        }
                } else if(file_s[0] == "") {
                    files = file_s[1];
                }
                }
        sfputs("</Project>");
        }
        fclose(fptr);
    }
}

astd::string spman::include_subdirectory_files(const char* folder_name, const char* pattern_) {
    astd::string f_p("");
    if(pattern_[0] == '*') {
        f_p += '.';
    } f_p += pattern_;
    const char* formatted_pattern = f_p.c_str();
    int result;
    re_t pattern = re_compile(formatted_pattern);
    astd::string files("");
    DIR* dir;
    struct dirent *ent;
    if((dir = opendir(folder_name)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if(ent->d_type == DT_REG) {
                if(re_matchp(pattern, ent->d_name, &result) != -1) {
                    files += folder_name;
                    files += '/';
                    files += ent->d_name;
                    files += "  ";
                }
            }
        }
        closedir(dir);
    } else {
        printf("\"%s\" not a directory. \n", folder_name);
        closedir(dir);
        exit(0);
    }
    return files;
}

astd::string spman::include_file(const char* filename) {
    FILE *fstr = fopen(filename, "r");
    if(fstr == NULL) {
        printf("File \"%s\" is not found. Check your filename or path.", filename);
        exit(1);
    }
    return filename;
}

astd::string spman::Filemake::getArgs() {
    return arg;
}
astd::string spman::Filemake::getProjectName() {
    return proj_name;
}

astd::string spman::Filemake::getUuid() {
    return uuid;
}

void spman::MakeWorkspace::AddProject(spman::Filemake &project) {
    uuids_and_names.push(project.getUuid());
    uuids_and_names.push(project.getProjectName());
}

spman::MakeWorkspace::MakeWorkspace(astd::string name_) : name(name_) {}
spman::MakeWorkspace::~MakeWorkspace() {}
void spman::MakeWorkspace::GenerateFile(int generator) {
    FILE *fptr;
    if(generator == generator_type::vc2017) {
        if((fptr = fopen((name + ".sln").c_str(), "w")) != NULL) {
            println("Generated %s, -G 'vs2017'...", (name + ".sln").c_str());
            sfputs("\nMicrosoft Visual Studio Solution File, Format Version 12.00");
            sfputs("# Visual Studio 15");
            for(int i = 0; i < uuids_and_names.size(); i+=2) {
                sfprintf("Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"%s\", \"%s\", \"%s\"", uuids_and_names[i+1].c_str(), (uuids_and_names[i+1] + ".vcxproj").c_str(), uuids_and_names[i].c_str());
                sfputs("EndProject");
            }
            sfputs("Global");
                sfputs("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
		            sfputs("\t\tDebug|x64 = Debug|x64");
	    	        sfputs("\t\tDebug|Win32 = Debug|Win32");
		            sfputs("\t\tRelease|x64 = Release|x64");
		            sfputs("\t\tRelease|Win32 = Release|Win32");
    	        sfputs("\tEndGlobalSection");

                sfputs("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
                    for(int i = 0; i < uuids_and_names.size(); i+=2) {
                        sfprintf("\t\t{%s}.Debug|x64.ActiveCfg = Debug|x64", uuids_and_names[i].c_str());
		                sfprintf("\t\t{%s}.Debug|x64.Build.0 = Debug|x64", uuids_and_names[i].c_str());
		                sfprintf("\t\t{%s}.Debug|Win32.ActiveCfg = Debug|Win32", uuids_and_names[i].c_str());
		                sfprintf("\t\t{%s}.Debug|Win32.Build.0 = Debug|Win32", uuids_and_names[i].c_str());
		                sfprintf("\t\t{%s}.Release|x64.ActiveCfg = Release|x64", uuids_and_names[i].c_str());
		                sfprintf("\t\t{%s}.Release|x64.Build.0 = Release|x64", uuids_and_names[i].c_str());
		                sfprintf("\t\t{%s}.Release|Win32.ActiveCfg = Release|Win32", uuids_and_names[i].c_str());
		                sfprintf("\t\t{%s}.Release|Win32.Build.0 = Release|Win32", uuids_and_names[i].c_str());
                    }
                sfputs("\tEndGlobalSection");

                sfputs("\tGlobalSection(SolutionProperties) = preSolution");
		            sfputs("\t\tHideSolutionNode = FALSE");
	            sfputs("\tEndGlobalSection");
            sfputs("EndGlobal");
        } 
    } else if(generator == generator_type::make4) {
        if((fptr = fopen(("Makefile"), "w")) != NULL) {
            puts("Generated Makefile, -G 'GNU Make 4.0'...");
            sfputs("ifndef config");
                sfputs("\tconfig=release");
            sfputs("endif\n");

            sfputs("ifndef verbose");
                sfputs("\tSILENT = @");
            sfputs("endif\n");

            sfputs("ifeq ($(config),debug)");
                sfputs("\tGCONFIG = debug");
            sfputs("else");
                sfputs("\tGCONFIG = release");
            sfputs("endif\n");

            fputs("PROJECTS := ", fptr);
            for(int i = 0; i < uuids_and_names.size(); i+=2) {
                fprintf(fptr, "%s ", uuids_and_names[i+1].c_str());
            }

            sfputs("\n.PHONY: all help $(PROJECTS)\n");

            sfputs("\nall: $(PROJECTS)\n");
            for(int i = 0; i < uuids_and_names.size(); i+=2) {
                sfprintf("%s:", uuids_and_names[i+1].c_str());
                    sfprintf("\t$(SILENT) echo \"==== Building %s ($(GCONFIG)) ====\"", uuids_and_names[i+1].c_str());
                    sfprintf("\t$(SILENT) $(MAKE) --no-print-directory -C . -f %s.mk config=$(GCONFIG)", uuids_and_names[i+1].c_str());
            }
            fputs("\nclean:\n", fptr);
            for(int i = 0; i < uuids_and_names.size(); i += 2) {
                sfprintf("\t$(SILENT) $(MAKE) --no-print-directory -C . -f %s.mk clean", uuids_and_names[i+1].c_str());
            }

            sfputs("help:");
            for(int i = 0; i < 9; i++) {
                sfprintf("\t$(SILENT) echo \"%s\"", help[i].c_str());
            }
            for(int i = 0; i < uuids_and_names.size(); i += 2) {
                sfprintf("\t$(SILENT) echo \"\t%s\"", uuids_and_names[i+1].c_str());
            }
        }
    }
}