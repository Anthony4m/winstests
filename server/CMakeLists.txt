project(server)

add_executable(server server.cpp)

# Include the Tnet directory to access Tnet.h
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/Tnet)

# Link the server executable with the Tnet library
target_link_libraries(server PRIVATE Tnet)
