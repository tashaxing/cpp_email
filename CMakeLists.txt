project(cpp_email)
cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++11)

include_directories(
	deps/curl-7.60.0/include
)

link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/deps/curl-7.60.0/bin
)

set(SRC 
	src/email_sender.h
	src/email_sender.cpp
	src/main.cpp
)

add_executable(cpp_email ${SRC})
target_link_libraries(cpp_email 
	curl
)
