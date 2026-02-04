
#include "GfxAssetsManager.h"

namespace gfx {

//Constuctor 
GfxAssetsManager::GfxAssetsManager()
: textureFileTypes(supportedTextureFileTypes),
  textureFolderPath(getDefaultTexturesDirPath()),
  shaderFolderPath(getDefaultShaderSourceDirPath())
{
}

std::vector<ShaderProgramFilePaths> GfxAssetsManager::loadShaderPathSet(std::vector<ShaderProgramFilenameStrings> shaderFilenames)
{
    std::vector<ShaderProgramFilePaths> returnVector;

    for (const auto& shaderFilename : shaderFilenames)
    {
        ShaderProgramFilePaths pulledPaths = loadShaderPaths(shaderFilename);

        if (!pulledPaths.fragmentShader.empty() && !pulledPaths.vertexShader.empty())
        {
            returnVector.push_back(pulledPaths);
        }
    }

    return returnVector;
} 

std::filesystem::path GfxAssetsManager::getDefaultShaderSourceDirPath()
{
    #ifdef SHADER_SOURCE_DEFAULT_PATH
        return std::filesystem::path(SHADER_SOURCE_DEFAULT_PATH);
    #else
        // Construct
        std::filesystem::path sourceFilePath = __FILE__;
        std::filesystem::path sourceDirPath = sourceFilePath.parent_path();
        std::filesystem::path assetsBaseDir = sourceDirPath.parent_path();
        std::filesystem::path renderEngineDir = assetsBaseDir.parent_path();

        return renderEngineDir / shaderModuleProjectName / shaderSourceFolderName;
    #endif
}


void GfxAssetsManager::checkPathsAndStore(ShaderProgramFilePaths& foundPaths)
{
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
        shaderPaths[foundPaths.setName] = foundPaths;
    }
}

ShaderProgramFilePaths GfxAssetsManager::loadShaderPaths(ShaderProgramFilenameStrings sourceFileNames)
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

    ShaderProgramFilePaths returnPaths;
    returnPaths.setName = sourceFileNames.setName;

    if (shaderFolderPath.empty())
    {
        shaderFolderPath = getDefaultShaderSourceDirPath();
    }

    if (std::filesystem::exists(shaderFolderPath) && std::filesystem::is_directory(shaderFolderPath))
    {
        for (const auto& file : std::filesystem::directory_iterator(shaderFolderPath))
        {
            if (file.is_regular_file())
            {
                if (file.path().filename().string() == sourceFileNames.vertexShader)
                {
                    #ifdef ENABLE_DEBUG_MESSAGES
                        std::cout << "DEBUG::Vertex shader: " << file.path().filename() << std::endl;
                    #endif

                    returnPaths.vertexShader = file.path();
                }

                if (file.path().filename().string() == sourceFileNames.fragmentShader)
                {
                    #ifdef ENABLE_DEBUG_MESSAGES
                        std::cout << "DEBUG::Fragment shader: " << file.path().filename() << std::endl;
                    #endif

                    returnPaths.fragmentShader = file.path();
                }
            }
        }
    }
    else
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::Current shader source directory is invalid: " << shaderFolderPath.string() << std::endl;
        #endif
    }

    checkPathsAndStore(returnPaths);

    return returnPaths;
} 

std::filesystem::path GfxAssetsManager::getDefaultTexturesDirPath()
{
    #ifdef TEXTURES_DEFAULT_PATH
        return std::filesystem::path(TEXTURES_DEFAULT_PATH);
    #else

        // Construct
        std::filesystem::path sourceFilePath = __FILE__;
        std::filesystem::path sourceDirPath = sourceFilePath.parent_path();
        std::filesystem::path assetsBaseDir = sourceDirPath.parent_path();

        return assetsBaseDir / defaultTexturesFolderName;

    #endif
}

void GfxAssetsManager::populateTexturePaths(std::filesystem::path textureFolderPath)
{
    for (const auto& file : std::filesystem::directory_iterator(textureFolderPath))
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

void GfxAssetsManager::loadTexturePaths()
{
    std::filesystem::path textureFolderPath = getDefaultTexturesDirPath();

    if (std::filesystem::exists(textureFolderPath) && std::filesystem::is_directory(textureFolderPath))
    {
        populateTexturePaths(textureFolderPath);
    }
    else
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::Texture path directory is not valid: " << textureFolderPath.string() << std::endl;
        #endif
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






