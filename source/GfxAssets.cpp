
#include "GfxAssets.h"

namespace gfx {


    //Constuctor 
    GfxAssets::GfxAssets()
        : shaderFileTypes{ fragShaderExtension, vertShaderExtension}
    {
        shaderPaths = loadAllShaderPaths();
    }


    // Returns a vector of all glsl source files (.vs and .fs) found in Shaders/glsl
    // TODO: This should be an interface to a generic file type finder - need to do this before writing a file finder for any other specific format
    // TODO: Integrate standard logging/debug system to replace cout calls
    // TODO: Path (__FILE__) relative to build (compile time) structure, Shaders will need to be loaded at run time 
    std::vector<std::filesystem::path> GfxAssets::loadAllShaderPaths()
    {
        namespace fs = std::filesystem; // declutter code

        std::vector<fs::path> glslSourceFilePaths;

        fs::path sourcePath = __FILE__; //path of current source file

        fs::path currentDir = sourcePath.parent_path(); // removes file name from path

        while (currentDir != "C:\\") //TODO:: Windows specific
        {
            std::cout << " DEBUG::current directory = " << currentDir.string() << std::endl;

            fs::path checkDir = currentDir / "Shaders";

            if(fs::exists(checkDir))
            {
                // Should I check for a .git here and check the project? Possibly even check the commit hash?
                std::cout << "DEBUG::Shaders directory found." << std::endl;

                checkDir = checkDir / "glsl"; // now look for glsl folder

                if (fs::exists(checkDir))
                {
                    for (const auto& file : fs::directory_iterator(checkDir))
                    {
                        if (file.is_regular_file() && (std::ranges::find(this->shaderFileTypes, file.path().extension().string()) != shaderFileTypes.end()))
                        {
                            glslSourceFilePaths.push_back(file);
                        }
                    }
                }
            }

            currentDir = currentDir.parent_path();
        }

        if (glslSourceFilePaths.empty())
        {
            std::cout << "WARNING::There were no shader source files found. (Only the \"Shaders/glsl\" directory will be searched. Please ensure the git module is pulled correctly.)" << std::endl;
        }

        return glslSourceFilePaths;

    } 

    std::filesystem::path GfxAssets::getShaderPath(std::string shaderExtension)
    {
        if (shaderPaths.empty())
        {
            return std::filesystem::path(); // returns empty path object
        }

        for (auto path : shaderPaths)
        {
            if (path.extension().string() == shaderExtension)
            {
                return path;
            }
        }

        return std::filesystem::path(); // returns empty path object
    }

}






