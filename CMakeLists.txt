add_library(AssetsLib 
    class/source/AssetRegistry.cpp
)
target_link_libraries(AssetsLib
    PRIVATE TypeLib
    PUBLIC  ShaderLib
)

target_include_directories(AssetsLib 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/class/source
)

