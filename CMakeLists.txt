add_library(AssetsLib 
    source/AssetRegistry.cpp
    source/GfxAssetRegistry.cpp
)
target_link_libraries(AssetsLib
    PRIVATE TypeLib
)

target_include_directories(AssetsLib PUBLIC source)

