# All source files.
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.hpp"
        "src/*.h")

# Add target.
add_executable (PlaygroundViewer ${SOURCE_FILES})
target_include_directories(PlaygroundViewer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

# Link raylib.
target_link_libraries(PlaygroundViewer raylib)
RaylibOtherPlatform(PlaygroundViewer)

# Link entt.
target_link_libraries(PlaygroundViewer EnTT::EnTT)
