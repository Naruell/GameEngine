cmake_minimum_required( VERSION 3.17 )
project( Untitled )

set( CMAKE_CXX_STANDARD 17 )

set( ENGINE_CODE_DIR ${PROJECT_SOURCE_DIR}/engine )
set( GAME_CODE_DIR ${PROJECT_SOURCE_DIR}/game )

include_directories( ${ENGINE_CODE_DIR} )
include_directories( ${GAME_CODE_DIR} )

set( ENGINE_CODE
        ${ENGINE_CODE_DIR}/Engine.h
        ${ENGINE_CODE_DIR}/Engine.cpp
        ${ENGINE_CODE_DIR}/Logger.h
        ${ENGINE_CODE_DIR}/Logger.cpp
        ${ENGINE_CODE_DIR}/Timer.h
        ${ENGINE_CODE_DIR}/Timer.cpp
        )

set( GAME_CODE )

add_subdirectory( test )

add_executable( Untitled main.cpp ${ENGINE_CODE} ${GAME_CODE} )