cmake_minimum_required(VERSION 3.20)

project(VFSLauncher)



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_executable(VFSLauncher VFSLauncher.cpp)
add_custom_command(
    TARGET VFSLauncher POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/usvfs_x64.dll
                ${CMAKE_BINARY_DIR}/Debug/usvfs_x64.dll)
target_include_directories(VFSLauncher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(VFSLauncher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/usvfs_x64.lib)
set_target_properties(VFSLauncher PROPERTIES
 RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}
 LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR})
