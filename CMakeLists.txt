cmake_minimum_required(VERSION 3.5)

project(server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
add_compile_options(-lboost_thread-mt)
find_package(Threads REQUIRED)
find_package(Boost 1.67.0 REQUIRED system)
include_directories(${Boost_INCLUDE_DIR})

add_executable(server main_server.cpp)
target_link_libraries(server  ${Boost_LIBRARIES})
target_link_libraries(server Threads::Threads)
include_directories(/sqlite-autoconf-3340100/)
link_directories(sqlite-autoconf-3340100/)
target_link_libraries(server sqlite3)
