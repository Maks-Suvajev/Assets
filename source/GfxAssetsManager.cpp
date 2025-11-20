
#include "GfxAssetsManager.h"

namespace gfx {

//Constuctor 
GfxAssetsManager::GfxAssetsManager()
: textureFileTypes(supportedTextureFileTypes)
{
    //loadShaderPaths();
}

std::vector<ShaderPaths> GfxAssetsManager::loadShaderPathSet(std::vector<ShaderFilenameStrings> shaderFilenames)
{
    std::vector<ShaderPaths> returnVector;

    for (const auto& shaderFilename : shaderFilenames)
    {
        ShaderPaths pulledPaths = loadShaderPaths(shaderFilename);

        if (!pulledPaths.fragmentShader.empty() && !pulledPaths.vertexShader.empty())
        {
            returnVector.push_back(pulledPaths);
        }
    }

    return returnVector;
} 



ShaderPaths GfxAssetsManager::loadShaderPaths(ShaderFilenameStrings sourceFileNames)
{
    if (shaderPaths.contains(sourceFileNames.setName))
    {
        #ifdef ENABLE_DEBUG_MESSAGES

            if (shaderPaths[sourceFileNames.setName].vertexShader.filename().string() != sourceFileNames.vertexShader)
            {
                    std::cout << "ERROR::Shader set name found but with differing vertex shader:" << std::endl;
                    std::cout << "ERROR::Loaded vertex shader in set = " << shaderPaths[sourceFileNames.setName].vertexShader.filename().string() << std::endl;
                    std::cout << "ERROR::Requested vertex shader     =  " << sourceFileNames.vertexShader << std::endl;
                    std::cout << "ERROR::Please explicitly delete and load new set or change the name if you want different files." << std::endl;
            }

            if (shaderPaths[sourceFileNames.setName].fragmentShader.filename().string() != sourceFileNames.fragmentShader)
            {
                    std::cout << "ERROR::Shader set name found but with differing fragment shader:" << std::endl;
                    std::cout << "ERROR::Loaded fragment shader in set = " << shaderPaths[sourceFileNames.setName].fragmentShader.filename().string() << std::endl;
                    std::cout << "ERROR::Requested fragment shader     =  " << sourceFileNames.fragmentShader << std::endl;
                    std::cout << "ERROR::Please explicitly delete and load new set or change the name if you want different files." << std::endl;
            }

        #endif

        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "DEBUG::Returning already loaded set." << std::endl;
        #endif

        return shaderPaths[sourceFileNames.setName];
    }

    ShaderPaths foundPaths;
    foundPaths.setName = sourceFileNames.setName; 

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

                        if (file.path().filename().string() == sourceFileNames.vertexShader)
                        {
                            #ifdef ENABLE_DEBUG_MESSAGES
                                std::cout << "DEBUG::Vertex shader: " << file.path().filename() << std::endl;
                            #endif

                            foundPaths.vertexShader = file.path();
                        }

                        if (file.path().filename().string() == sourceFileNames.fragmentShader)
                        {
                            #ifdef ENABLE_DEBUG_MESSAGES
                                std::cout << "DEBUG::Fragment shader: " << file.path().filename() << std::endl;
                            #endif

                            foundPaths.fragmentShader = file.path();
                        }
                    }
                }
            }
        }

        currentDir = currentDir.parent_path();
    }

    bool missingData = false; // Only store the set if there is no data missing

    if (foundPaths.fragmentShader.empty() == true)
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::Fragment shader path not found." << std::endl;
        #endif

        missingData = true;
    }
    
    if (foundPaths.vertexShader.empty() == true)
    {
        std::cout << "ERROR::Vertex shader path not found." << std::endl;
        missingData = true;
    }

    if (missingData == false)
    {
        shaderPaths[sourceFileNames.setName] = foundPaths;
    }

    return foundPaths;
} 

void GfxAssetsManager::loadTexturePaths()
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


std::vector<std::filesystem::path> GfxAssetsManager::getTexturePaths()
{
    if (texturePaths.empty())
    {
        loadTexturePaths();
    }

    return texturePaths;
}


void GfxAssetsManager::resetShaderPaths()
{
    shaderPaths.clear();
}

}






