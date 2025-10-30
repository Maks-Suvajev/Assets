
#include "GfxAssets.h"

namespace gfx {

//Constuctor 
GfxAssets::GfxAssets()
: textureFileTypes({".png", ".jpg"})
{
    loadShaderPaths();
}


void GfxAssets::loadShaderPaths()
{
    namespace fs = std::filesystem; // declutter code

    fs::path sourcePath = __FILE__; //path of current source file

    fs::path currentDir = sourcePath.parent_path(); // removes file name from path

    bool shaderProjectFound = false;

    while (currentDir != rootString && !shaderProjectFound) // Searching to root may perhaps be over kill
    {

        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "DEBUG::current directory = " << currentDir.string() << std::endl;
        #endif

        fs::path checkDir = currentDir / shaderModuleProjectName;

        if(fs::exists(checkDir))
        {
            shaderProjectFound = true; // Kill the loop once the Shader project is found.

            #ifdef ENABLE_DEBUG_MESSAGES
                // Should I check for a .git here and check the project? Possibly even check the commit hash?
                std::cout << "DEBUG::" << shaderModuleProjectName << " directory found." << std::endl;
            #endif

            checkDir = checkDir / shaderSourceFolderName; // now look for glsl folder

            if (fs::exists(checkDir))
            {
                #ifdef ENABLE_DEBUG_MESSAGES
                    std::cout << "DEBUG::" << shaderSourceFolderName << " directory found." << std::endl;
                    std::cout << "DEBUG::Source files found:" << std::endl;
                #endif

                for (const auto& file : fs::directory_iterator(checkDir))
                {
                    if (file.is_regular_file())
                    {
                        if (file.path().extension().string() == vertShaderExtension)
                        {
                            #ifdef ENABLE_DEBUG_MESSAGES
                                std::cout << "DEBUG::Vertex shader: " << file.path().filename() << std::endl;
                            #endif

                            shaderPaths.vertexShader = file.path();
                        }
                        else if (file.path().extension().string() == fragShaderExtension)
                        {
                            #ifdef ENABLE_DEBUG_MESSAGES
                                std::cout << "DEBUG::Fragment shader: " << file.path().filename() << std::endl;
                            #endif

                            shaderPaths.fragmentShader = file.path();
                        }
                    }
                }
            }
        }

        currentDir = currentDir.parent_path();
    }
} 


void GfxAssets::loadTexturePaths()
{
    namespace fs = std::filesystem; 

    fs::path sourcePath = __FILE__; //path of current source file

    fs::path currentDir = sourcePath.parent_path(); // removes file name from path

    bool textureFolderFound = false;

    while (currentDir != rootString && !textureFolderFound) // Searching to root may perhaps be over kill
    {
        fs::path checkDir = currentDir / texturesFolderName;

        if (fs::exists(checkDir) && fs::is_directory(checkDir))
        {
            textureFolderFound = true;

            for (const auto& file : fs::directory_iterator(checkDir))
            {
                if (file.is_regular_file())
                {
                    if (std::find(textureFileTypes.begin(), textureFileTypes.end(), file.path().extension().string()) != textureFileTypes.end())
                    {
                        #ifdef ENABLE_DEBUG_MESSAGES
                            std::cout << "DEBUG::Texture file found: " << file.path().filename() << std::endl;
                        #endif

		                texturePaths.push_back(file.path());
                    }
                }
            }

        }

        currentDir = currentDir.parent_path(); 
    }

    if (texturePaths.empty())
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::No textures detected! Please check texture path is configured correctly" << std::endl;
        #endif
    }
}


std::vector<std::filesystem::path> GfxAssets::getTexturePaths()
{
    if (texturePaths.empty())
    {
        loadTexturePaths();
    }

    return texturePaths;
}


void GfxAssets::resetShaderPaths()
{
    shaderPaths.vertexShader = std::filesystem::path(); 
    shaderPaths.fragmentShader = std::filesystem::path(); 
}

}






