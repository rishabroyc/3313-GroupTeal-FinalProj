cmake_minimum_required(VERSION 3.15)
project(MyCrowServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Crow as a subdirectory (expects crow to be inside lib/)
add_subdirectory(crow)
add_subdirectory(cpr)

# Define your executable
add_executable(server main.cpp)

target_link_libraries(server PRIVATE Crow::Crow cpr::cpr)

# Link it to the Crow library
target_link_libraries(server PRIVATE Crow::Crow)
