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

add_executable(AntGod main.cpp
        neuralengine/Layer.cpp
        neuralengine/Layer.h
        neuralengine/Network.cpp
        neuralengine/Network.h
        neuralengine/Activation.cpp
        neuralengine/Activation.h
)

target_compile_options(AntGod PRIVATE
        -O3                    # Maximum optimization
        -ffast-math           # Fast math operations
        -march=native         # Use all available CPU instructions
        -mtune=native         # Tune for native CPU
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(AntGod PRIVATE raylib)