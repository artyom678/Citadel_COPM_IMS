cmake_minimum_required(VERSION 3.28.3)

project(ZProcessor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ZLIB REQUIRED)

add_executable(zprocessor 
	src/main.cpp
	src/processor.cpp
	src/config.cpp
)

target_include_directories(zprocessor PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	${ZLIB_INCLUDE_DIRS}
)

target_link_libraries(zprocessor PUBLIC ${ZLIB_LIBRARIES})