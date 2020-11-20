project( test )

cmake_minimum_required(VERSION 3.17)

add_subdirectory( LoaderTest )
add_subdirectory( MapEditorTest )
add_subdirectory( PathFindingTest )

add_executable( test test_main.cpp ${ENGINE_CODE} ${GAME_CODE} ${JSON_CODE} )