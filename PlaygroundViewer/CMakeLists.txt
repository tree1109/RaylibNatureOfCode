# Note: I need use stb_perlin.h inside raylib.
set(USE_DOWNLOAD_RAYLIB ON)

# Raylib setup.
set(RAYLIB_VERSION 5.5)
if (NOT USE_DOWNLOAD_RAYLIB)
    find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
endif()
if (USE_DOWNLOAD_RAYLIB OR NOT raylib_FOUND) # If there's none, fetch and build raylib
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
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

macro(RaylibOtherPlatform EXECUTABLE_NAME)
    # Web Configurations
    if (PLATFORM STREQUAL "Web")
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    endif()

    # Checks if OSX and links appropriate frameworks (Only required on MacOS)
    if (APPLE)
        target_link_libraries(${EXECUTABLE_NAME} "-framework IOKit")
        target_link_libraries(${EXECUTABLE_NAME} "-framework Cocoa")
        target_link_libraries(${EXECUTABLE_NAME} "-framework OpenGL")
    endif()
endmacro()

# My function.
function(MyAddExecutable EXECUTABLE_NAME)
    add_executable (${EXECUTABLE_NAME} "${EXECUTABLE_NAME}.cpp" ${ARGN})
    target_link_libraries(${EXECUTABLE_NAME} raylib)
endfunction()

# Set include paths.
include_directories(.)
# All source files.
file(GLOB UTILITY_SRC_CPP "utility/*.cpp")
file(GLOB GAME_OBJECT_SRC_CPP "game_object/*.cpp")
file(GLOB PLAYGROUND_SRC_CPP "playground/*.cpp")
file(GLOB MANAGER_SRC_CPP "manager/*.cpp")
file(GLOB SERVICE_SRC_CPP "service/*.cpp")
set(SOURCE_FILES
    ${UTILITY_SRC_CPP}
    ${GAME_OBJECT_SRC_CPP}
    ${PLAYGROUND_SRC_CPP}
    ${MANAGER_SRC_CPP}
    ${SERVICE_SRC_CPP}
)

# Main executable.
MyAddExecutable(${PROJECT_NAME} ${SOURCE_FILES})
RaylibOtherPlatform(${PROJECT_NAME})
message(STATUS "Add executable: ${PROJECT_NAME}")
