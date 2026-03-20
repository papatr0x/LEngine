file(GLOB_RECURSE CMAKE_CURRENT_SOURCE_DIR "*.cpp")

add_executable(demo1 ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(demo1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(demo1 PRIVATE engine)