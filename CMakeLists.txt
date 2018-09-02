cmake_minimum_required(VERSION 3.9)
project(redis-asio
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

if (NOT BOOST_DIR)
    message(FATAL_ERROR "need set BOOST_DIR")
else()
    include_directories(${BOOST_DIR}/include)
    link_directories(${BOOST_DIR}/lib)
endif()

if (NOT HIREDIS_DIR)
    message(FATAL_ERROR "need set HIREDIS_DIR")
else()
    include_directories(${HIREDIS_DIR}/include)
    link_directories(${HIREDIS_DIR}/lib)
endif()

include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

add_library(redis-asio
        src/asyncredis.cpp
        src/asyncredisif.cpp
        src/asyncredissubscriber.cpp
        src/redis.cpp
        )

target_link_libraries(redis-asio libhiredis.a)

install(TARGETS redis-asio
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY include/redis-asio DESTINATION include)