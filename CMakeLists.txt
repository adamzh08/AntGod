cmake_minimum_required(VERSION 3.31)
project(AntGod)

set(CMAKE_CXX_STANDARD 23)

set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
    endif()
endif()

# Find and copy Intel runtime libraries
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "IntelLLVM")
    # Get Intel compiler installation path
    get_filename_component(INTEL_COMPILER_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)

    # Extended Intel DLL search paths
    set(INTEL_DLL_PATHS
            "${INTEL_COMPILER_DIR}"
            "${INTEL_COMPILER_DIR}/../redist/intel64/compiler"
            "${INTEL_COMPILER_DIR}/../compiler/lib/intel64"
            "${INTEL_COMPILER_DIR}/../lib/intel64"
            "$ENV{ONEAPI_ROOT}/compiler/latest/windows/redist/intel64_win/compiler"
            "$ENV{ONEAPI_ROOT}/compiler/latest/lib/intel64"
            "$ENV{ONEAPI_ROOT}/compiler/latest/windows/bin/intel64"
            # Additional common locations
            "${INTEL_COMPILER_DIR}/../redist/intel64_win/compiler"
            "${INTEL_COMPILER_DIR}/../../redist/intel64_win/compiler"
    )

    # Updated list of required DLLs (including both variants)
    set(REQUIRED_DLLS
            libmmd.dll
            libiomp5.dll
            libiomp5md.dll
            svml_dispmd.dll
    )

    # Find and copy each DLL
    foreach(DLL_NAME ${REQUIRED_DLLS})
        find_file(${DLL_NAME}_PATH
                NAMES ${DLL_NAME}
                PATHS ${INTEL_DLL_PATHS}
                NO_DEFAULT_PATH
        )

        if(${DLL_NAME}_PATH)
            file(COPY ${${DLL_NAME}_PATH} DESTINATION ${CMAKE_BINARY_DIR})
            message(STATUS "Found and copied: ${${DLL_NAME}_PATH}")
            # Clear the cache variable for next run
            unset(${DLL_NAME}_PATH CACHE)
        else()
            message(WARNING "Could not find: ${DLL_NAME}")
        endif()
    endforeach()
endif()

find_package(MKL CONFIG)
find_package(OpenMP REQUIRED)

add_executable(AntGod main.cpp
        neuralengine/Layer.cpp
        neuralengine/Layer.h
        neuralengine/Network.cpp
        neuralengine/Network.h
        neuralengine/Activation.h
        worldengine/Population.cpp
        worldengine/Population.h
        worldengine/Ant.cpp
        worldengine/Ant.h
        worldengine/TextureCollection.h
        worldengine/TextureCollection.cpp
        worldengine/World.cpp
        worldengine/World.h
        worldengine/PopulationBuilder.cpp
        worldengine/PopulationBuilder.h
        worldengine/Lines.cpp
        worldengine/Lines.h
        worldengine/LineIntersection.h
        worldengine/UI/Graph.cpp
        worldengine/UI/Graph.h
        worldengine/UI/InfoBox.h
        worldengine/UI/NeuroBox.cpp
        worldengine/UI/NeuroBox.h
        worldengine/UI/RaysEditBox.cpp
        worldengine/UI/RaysEditBox.h
        worldengine/UI/EvolutionEditBox.cpp
        worldengine/UI/EvolutionEditBox.h
        worldengine/UI/ColorEditBox.cpp
        worldengine/UI/ColorEditBox.h
)

target_compile_options(AntGod PRIVATE
        -O3                    # Maximum optimization
        -ffast-math           # Fast math operations
        -march=native         # Use all available CPU instructions
        -mtune=native         # Tune for native CPU
)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "IntelLLVM")
    target_compile_options(AntGod PRIVATE -Qiopenmp)
    target_link_options(AntGod PRIVATE -Qiopenmp)

    # Add static linking for Intel libraries
    target_compile_options(AntGod PRIVATE -static-intel)
    target_link_options(AntGod PRIVATE -static-intel)

    set_target_properties(AntGod PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
    )

    # Link MKL libraries with static linking preference
    target_link_libraries(AntGod PRIVATE MKL::MKL)
else ()
    target_compile_options(AntGod PRIVATE -fopenmp)
    target_link_options(AntGod PRIVATE -fopenmp)

    # target_compile_options(AntGod PRIVATE -static-libgcc -static-libstdc++)
    # target_link_options(AntGod PRIVATE -static-libgcc -static-libstdc++ -static)
endif()

# Link OpenMP for parallel processing
target_link_libraries(AntGod PRIVATE OpenMP::OpenMP_CXX)

# Link RayLib library
target_link_libraries(AntGod PRIVATE raylib)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})