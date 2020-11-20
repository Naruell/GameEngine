cmake_minimum_required( VERSION 3.17 )
project( Untitled )

set( CMAKE_CXX_STANDARD 17 )

# External Library #
set( EXTERNAL_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib )
include_directories( ${EXTERNAL_LIBRARY_DIR} )

# jsoncpp #
set( JSON_CODE_DIR  ${EXTERNAL_LIBRARY_DIR}/dist )
include_directories( ${JSON_CODE_DIR} )
include_directories( ${JSON_CODE_DIR}/json )

set( JSON_CODE
        ${JSON_CODE_DIR}/json/json.h
        ${JSON_CODE_DIR}/json/json-forwards.h
        ${JSON_CODE_DIR}/jsoncpp.cpp
        )

# Engine #
set( ENGINE_CODE_DIR ${PROJECT_SOURCE_DIR}/engine )
include_directories( ${ENGINE_CODE_DIR} )

set( ENGINE_CODE
        ${ENGINE_CODE_DIR}/Engine.h
        ${ENGINE_CODE_DIR}/Engine.cpp
        ${ENGINE_CODE_DIR}/Logger.h
        ${ENGINE_CODE_DIR}/Logger.cpp
        ${ENGINE_CODE_DIR}/Timer.h
        ${ENGINE_CODE_DIR}/Timer.cpp
        ${ENGINE_CODE_DIR}/GameState.h
        ${ENGINE_CODE_DIR}/GameState.cpp
        ${ENGINE_CODE_DIR}/Loader.h
        ${ENGINE_CODE_DIR}/Loader.cpp
        ${ENGINE_CODE_DIR}/DataType.h
        ${ENGINE_CODE_DIR}/DataType.inl
        ${ENGINE_CODE_DIR}/DataType.cpp
        )

# Game #
set( GAME_CODE_DIR ${PROJECT_SOURCE_DIR}/game )
include_directories( ${GAME_CODE_DIR} )

set( GAME_CODE
        )

# Subdirectory #
add_subdirectory( test )

# Executable #
add_executable( Untitled main.cpp ${ENGINE_CODE} ${JSON_CODE} ${GAME_CODE} )