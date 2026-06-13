cmake_minimum_required(VERSION 3.20)
project(asd_course_work)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/model.cpp
        src/hash_table.cpp
        src/avl_tree.cpp
        src/list.cpp
        src/search.cpp
        src/airline.cpp
        src/menu.cpp
)
