cmake_minimum_required(VERSION 3.16)
project(TreeVisualizer)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/Visual Studio/SD_proj_bonus/DataStructures_Tree/lib/SFML-3.0.0/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(gui
    gui.cpp
    "C:/Visual Studio/SD_proj_bonus/DataStructures_Tree/src/avl_tree.cpp"
    "C:/Visual Studio/SD_proj_bonus/DataStructures_Tree/src/splay_tree.cpp"
    "C:/Visual Studio/SD_proj_bonus/DataStructures_Tree/src/dancing_tree.cpp"
    "C:/Visual Studio/SD_proj_bonus/DataStructures_Tree/src/t_tree.cpp"
)

target_link_libraries(gui
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)