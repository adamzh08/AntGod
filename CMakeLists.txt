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

find_package(MKL CONFIG)
find_package(OpenMP REQUIRED)

add_executable(AntGod main.cpp
        neuralengine/Layer.cpp
        neuralengine/Layer.h
        neuralengine/Network.cpp
        neuralengine/Network.h
        neuralengine/Activation.h
        worldengine/Line.cpp
        worldengine/Line.h
        worldengine/Population.cpp
        worldengine/Population.h
        worldengine/Ant.cpp
        worldengine/Ant.h
        worldengine/Lines.cpp
        worldengine/Lines.h
        worldengine/Textures.h
        worldengine/Textures.cpp
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

    target_compile_options(AntGod PRIVATE -static-libgcc -static-libstdc++)
    target_link_options(AntGod PRIVATE -static-libgcc -static-libstdc++ -static)
endif()

# Link OpenMP for parallel processing
target_link_libraries(AntGod PRIVATE OpenMP::OpenMP_CXX)

# Link RayLib library
target_link_libraries(AntGod PRIVATE raylib)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})