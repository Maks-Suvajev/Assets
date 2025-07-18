
#include "GfxAssets.h"

namespace gfx {


    //Constuctor 
    GfxAssets::GfxAssets()
    {
        loadShaderPaths();
    }

    // Loads shader source file paths into shaderPaths structure (.vs and .fs) found in Shaders/glsl (in current default config)
    // TODO: This should be an interface to a generic file type finder - need to do this before writing a file finder for any other specific format
    // TODO: Integrate standard logging/debug system to replace cout calls
    void GfxAssets::loadShaderPaths()
    {
        namespace fs = std::filesystem; // declutter code

        fs::path sourcePath = __FILE__; //path of current source file TODO: compile time only right now, might need to be more dynamic in future

        fs::path currentDir = sourcePath.parent_path(); // removes file name from path

        while (currentDir != rootString) // Searching to root may perhaps be over kill
        {
            std::cout << " DEBUG::current directory = " << currentDir.string() << std::endl;

            fs::path checkDir = currentDir / shaderModuleProjectName;

            if(fs::exists(checkDir))
            {
                // Should I check for a .git here and check the project? Possibly even check the commit hash?
                std::cout << "DEBUG::" << shaderModuleProjectName << " directory found." << std::endl;

                checkDir = checkDir / shaderSourceFolderName; // now look for glsl folder

                if (fs::exists(checkDir))
                {
                    std::cout << "DEBUG::" << shaderSourceFolderName << " directory found." << std::endl;

                    std::cout << "DEBUG::Source files found:" << std::endl;

                    for (const auto& file : fs::directory_iterator(checkDir))
                    {
                        if (file.is_regular_file())
                        {
                            if (file.path().extension().string() == vertShaderExtension)
                            {
                                std::cout << "Vertex shader: " << file.path().filename() << std::endl;
                                shaderPaths.vertexShader = file.path();
                            }
                            else if (file.path().extension().string() == fragShaderExtension)
                            {
                                std::cout << "Fragment shader: " << file.path().filename() << std::endl;
                                shaderPaths.fragmentShader = file.path();
                            }
                        }
                    }
                }
            }

            currentDir = currentDir.parent_path();
        }
    } 

    void GfxAssets::resetShaderPaths()
    {
        shaderPaths.vertexShader = std::filesystem::path(); 
        shaderPaths.fragmentShader = std::filesystem::path(); 
    }


}






