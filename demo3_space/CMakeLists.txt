file(GLOB_RECURSE CMAKE_CURRENT_SOURCE_DIR "*.cpp")

add_executable(demo3_space ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(demo3_space PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(demo3_space PRIVATE engine)