project( test )

include_directories( ../lib )
include_directories( ../engine )
include_directories( ../game )


add_executable( test test_main.cpp ${ENGINE_CODE} ${GAME_CODE} )