set(CMAKE_C_COMPILER "/usr/bin/clang-9")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-9")

set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} ${CURSES_LIBRARIES})

add_library(${BINARY}_lib STATIC ${SOURCES})