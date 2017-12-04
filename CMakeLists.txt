cmake_minimum_required(VERSION 3.6)
project(observer)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(include)
set(SOURCE_FILES include/crypto/rc4.h include/crypto/sha1.h
        include/event.h include/util/log.h
        include/tracker.h include/util/win_help.h
        src/crypto/rc4.c src/crypto/sha1.c
        src/event.c src/util/log.c src/main.c
        src/tracker.c src/util/win_help.c src/config.c include/config.h src/reader.c include/reader.h)


add_executable(observer ${SOURCE_FILES})
target_link_libraries(observer ${CURSES_LIBRARIES})