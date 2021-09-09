cmake_minimum_required(VERSION 3.19)        # Specifying the Cmake version
project(Client)                             # Project name
set(CMAKE_CXX_STANDARD 11)                  # C++ standard

set(SOURCE_EXE main.cpp)		            # Setting a variable with a list of sources for an executable file
set(SOURCE_LIB_M message.cpp)			    # The same thing, but for the library "message"
set(SOURCE_LIB_T test.cpp)			        # The same thing, but for the library "test"
set(SOURCE_LIB_C tcp_client.cpp)			# The same thing, but for the library "tcp_client"
set(SOURCE_LIB_F client.cpp)			    # The same thing, but for the library "full client"

add_library(mesg STATIC ${SOURCE_LIB_M})	# Creating a static library named "mesg" - message
add_library(test STATIC ${SOURCE_LIB_T})	# Creating a static library named "test"
add_library(t_cl STATIC ${SOURCE_LIB_C})	# Creating a static library named "t_cl" - tcp client

add_library(f_cl STATIC ${SOURCE_LIB_F})	# Creating a static library named "f_cl" - full client
target_link_libraries(f_cl mesg)		    # Linking the library "f_cl" with the library "mesg"
target_link_libraries(f_cl test)            # Linking the library "f_cl" with the library "test"
target_link_libraries(f_cl t_cl)            # Linking the library "f_cl" with the library "tcp_client"
target_link_libraries(f_cl WSOCK32.LIB)     # Linking the library "f_cl" with the library "WSOCK32"
target_link_libraries(f_cl Ws2_32.lib)      # Linking the library "f_cl" with the library "Ws2_32"

add_executable(Client ${SOURCE_EXE})	    # Creates an executable file named Client
target_link_libraries(Client f_cl)          # Linking the program with the library "f_cl"