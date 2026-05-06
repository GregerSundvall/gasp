cmake_minimum_required(VERSION 4.2)
project(gasp)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

function(include_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG ${gitTag}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()

set(LIB1 raylib)
find_package(${LIB1} QUIET)
if (NOT ${LIB1}_FOUND)
    include_dependency(${LIB1} https://github.com/raysan5/raylib.git 6.0)
endif()

add_executable(gasp main.cpp
        gasp.cpp
        gasp.h
        Game.cpp
        Game.h)

target_include_directories(gasp PRIVATE ${raylib_INCLUDE_DIRS})

target_link_libraries(gasp PRIVATE ${LIB1})
