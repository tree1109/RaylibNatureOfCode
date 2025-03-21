cmake_minimum_required (VERSION 3.8)

# 如果支援，則為 MSVC 編譯器啟用熱重新載入。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Dependencies
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

project ("RaylibNatureOfCode")

add_executable (${PROJECT_NAME} "RaylibNatureOfCode.cpp" "utility/Math.h" "utility/PathFinder.h" "utility/PathFinder.cpp" "utility/CommonType.h")
target_link_libraries(${PROJECT_NAME} raylib)

add_executable ("Chapter1" "Playground1.cpp" "utility/Math.h" "utility/Math.cpp" "utility/PathFinder.h" "utility/PathFinder.cpp" "utility/CommonType.h")
target_link_libraries("Chapter1" raylib)

add_executable ("Chapter2" "Playground2.cpp" "utility/Math.h" "utility/Math.cpp" "utility/PathFinder.h" "utility/PathFinder.cpp" "utility/CommonType.h")
target_link_libraries("Chapter2" raylib)

add_executable ("Chapter3" "Playground3.cpp" "utility/Math.h" "utility/Math.cpp" "utility/PathFinder.h" "utility/PathFinder.cpp" "utility/CommonType.h")
target_link_libraries("Chapter3" raylib)

add_executable ("Chapter4" "Playground4.cpp" "utility/Math.h" "utility/Math.cpp" "utility/PathFinder.h" "utility/PathFinder.cpp" "utility/CommonType.h")
target_link_libraries("Chapter4" raylib)

add_executable ("Chapter5" "Playground5.cpp" "utility/Math.h" "utility/Math.cpp" "utility/PathFinder.h" "utility/PathFinder.cpp" "utility/CommonType.h")
target_link_libraries("Chapter5" raylib)

add_executable ("Chapter6" "Playground6.cpp" "utility/Math.h" "utility/Math.cpp" "utility/PathFinder.h" "utility/PathFinder.cpp" "utility/CommonType.h")
target_link_libraries("Chapter6" raylib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RaylibNatureOfCode PROPERTY CXX_STANDARD 20)
  set_property(TARGET Chapter1 PROPERTY CXX_STANDARD 20)
  set_property(TARGET Chapter2 PROPERTY CXX_STANDARD 20)
  set_property(TARGET Chapter3 PROPERTY CXX_STANDARD 20)
  set_property(TARGET Chapter4 PROPERTY CXX_STANDARD 20)
  set_property(TARGET Chapter5 PROPERTY CXX_STANDARD 20)
  set_property(TARGET Chapter6 PROPERTY CXX_STANDARD 20)
endif()

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()