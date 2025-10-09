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

endfunction()

# All source files.
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")

# Add target.
add_executable (${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)
RaylibOtherPlatform(${PROJECT_NAME})
