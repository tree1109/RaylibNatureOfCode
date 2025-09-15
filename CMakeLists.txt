cmake_minimum_required (VERSION 3.10)
project("RaylibNatureOfCode" LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 如果支援，則為 MSVC 編譯器啟用熱重新載入。
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Raylib setup.
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
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

macro(RaylibOtherPlatform EXECUTABLE_NAME)
    # Web Configurations
    if (${PLATFORM} STREQUAL "Web")
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

# My function for main executable.
function(MyAddMainExecutable EXECUTABLE_NAME MAIN_SOURCE)
    add_executable (${EXECUTABLE_NAME} "${MAIN_SOURCE}" ${ARGN})
    target_link_libraries(${EXECUTABLE_NAME} raylib)
endfunction()

# My function for example executables.
function(MyAddExampleExecutable EXECUTABLE_NAME EXAMPLE_SOURCE)
    add_executable (${EXECUTABLE_NAME} "${EXAMPLE_SOURCE}" ${ARGN})
    target_link_libraries(${EXECUTABLE_NAME} raylib)
endfunction()

# Set include paths.
include_directories(${PROJECT_SOURCE_DIR})
# All shared source files.
file(GLOB UTILITY_SRC_CPP "${PROJECT_SOURCE_DIR}/utility/*.cpp")
file(GLOB GAME_OBJECT_SRC_CPP "${PROJECT_SOURCE_DIR}/game_object/*.cpp")
set(SHARED_SOURCE_FILES ${UTILITY_SRC_CPP} ${GAME_OBJECT_SRC_CPP})

# Main executable.
MyAddMainExecutable(${PROJECT_NAME} "src/${PROJECT_NAME}.cpp" ${SHARED_SOURCE_FILES})
RaylibOtherPlatform(${PROJECT_NAME})
message(STATUS "Add executable: ${PROJECT_NAME}")

# Example executables.
set(PLAYGROUND_LIST 1 2 3 4 5 6 7 8 9 10 11 12)
foreach(NUM ${PLAYGROUND_LIST})
    set(EXECUTABLE_NAME "Playground${NUM}")
    MyAddExampleExecutable(${EXECUTABLE_NAME} "examples/${EXECUTABLE_NAME}.cpp" ${SHARED_SOURCE_FILES})
    RaylibOtherPlatform(${EXECUTABLE_NAME})
    message(STATUS "Add executable: ${EXECUTABLE_NAME}")
endforeach()