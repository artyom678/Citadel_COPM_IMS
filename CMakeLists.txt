cmake_minimum_required(VERSION 3.28.3)

project(GZProcessor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ZLIB REQUIRED)

add_executable(gzprocessor 
	src/main.cpp
	src/processor.cpp
	src/config.cpp
)

target_include_directories(gzprocessor PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	${ZLIB_INCLUDE_DIRS}
)

target_link_libraries(gzprocessor PUBLIC ${ZLIB_LIBRARIES})
