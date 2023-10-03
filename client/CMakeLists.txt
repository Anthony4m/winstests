project(client)

add_executable(client_exe client.cpp)

# Include the Tnet directory to access Tnet.h
target_include_directories(client_exe PRIVATE ${CMAKE_SOURCE_DIR}/Tnet)

# Link the client_exe executable with the Tnet library
target_link_libraries(client_exe PRIVATE Tnet)
