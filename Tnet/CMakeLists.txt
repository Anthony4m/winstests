project(Tnet)

add_library(Tnet STATIC
        Tnet.h
)

target_include_directories(Tnet PUBLIC ${CMAKE_SOURCE_DIR}/Tnet)
