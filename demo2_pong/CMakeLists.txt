file(GLOB_RECURSE CMAKE_CURRENT_SOURCE_DIR "*.cpp")

add_executable(demo2_pong ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(demo2_pong PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(demo2_pong PRIVATE engine)