cmake_minimum_required(VERSION 3.6)
project(roguelike_sfml)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

set(SOURCE_FILES main.cpp GameSystem.cpp GameSystem.h Level.cpp Level.h Player.cpp Player.h Enemy.cpp Enemy.h)
add_executable(roguelike_sfml ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(roguelike_sfml ${SFML_LIBRARIES})
endif()

target_link_libraries(roguelike_sfml QueryBuilder pthread mysqlcppconn-static dl)
