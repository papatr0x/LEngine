file(GLOB_RECURSE CMAKE_CURRENT_SOURCE_DIR "*.cpp")

add_executable(demo3_space ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(demo3_space PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(demo3_space PRIVATE engine)

# Increment build version (AAAAMMDD.NNN) and regenerate BuildInfo.h before every build.
# The script lives at the project root and is shared across all demo projects.
add_custom_target(bump_build_number_demo3 ALL
    COMMAND python3 "${CMAKE_SOURCE_DIR}/bump_build.py" "${CMAKE_CURRENT_LIST_DIR}"
    COMMENT "Bumping build version for demo3_space"
)
add_dependencies(demo3_space bump_build_number_demo3)