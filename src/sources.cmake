set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/infrastructure/network/http/http-server.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/src/infrastructure/network/thread-pool/thread-pool.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/src/infrastructure/network/thread-pool/thread-pool-node.cpp
)

set(exe_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${sources}
)