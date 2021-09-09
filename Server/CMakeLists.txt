cmake_minimum_required(VERSION 3.19)        # Specifying the Cmake version
project(Server)                             # Project name
set(CMAKE_CXX_STANDARD 11)                  # C++ standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")  # The compilation flag for the c++ compiler
                                                    # that enables parallelization of the program

set(SOURCE_EXE main.cpp)		            # Setting a variable with a list of sources for an executable file
set(SOURCE_LIB_M message.cpp)			    # The same thing, but for the library "message"
set(SOURCE_LIB_C tcp_server.cpp)			# The same thing, but for the library "tcp_server"
set(SOURCE_LIB_F server.cpp)			    # The same thing, but for the library "full server"

add_library(mesg STATIC ${SOURCE_LIB_M})	# Creating a static library named "mesg" - message

add_library(t_sv STATIC ${SOURCE_LIB_C})	# Creating a static library named "t_sv" - tcp server
target_link_libraries(t_sv mesg)		    # Linking the library "t_sv" with the library "mesg"
target_link_libraries(t_sv WSOCK32.LIB)     # Linking the library "t_sv" with the library "WSOCK32"
target_link_libraries(t_sv Ws2_32.lib)      # Linking the library "t_sv" with the library "Ws2_32"

add_library(f_sv STATIC ${SOURCE_LIB_F})	# Creating a static library named "f_sv" - full server
target_link_libraries(f_sv t_sv)            # Linking the library "f_sv" with the library "tcp_server"

add_executable(Server ${SOURCE_EXE})	    # Creates an executable file named Client
target_link_libraries(Server f_sv)          # Linking the program with the library "f_sv"