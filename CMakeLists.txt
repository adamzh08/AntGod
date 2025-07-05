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

find_package(OpenMP REQUIRED)

add_executable(AntGod main.cpp
        neuralengine/Activation.h
        neuralengine/NEAT_Network.cpp
        neuralengine/NEAT_Network.h
        worldengine/Population.cpp
        worldengine/Population.h
        worldengine/Ant.cpp
        worldengine/Ant.h
        worldengine/UI/TextureCollection.h
        worldengine/UI/TextureCollection.cpp
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

target_compile_options(AntGod PRIVATE -fopenmp)
target_link_options(AntGod PRIVATE -fopenmp)

# Link OpenMP for parallel processing
target_link_libraries(AntGod PRIVATE OpenMP::OpenMP_CXX)

# Link RayLib library
target_link_libraries(AntGod PRIVATE raylib)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})