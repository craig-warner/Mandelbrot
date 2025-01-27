cmake_minimum_required(VERSION 3.1...3.16)

# set the project name and version
project(Mandelbrot VERSION 1.0)

configure_file(
	${PROJECT_SOURCE_DIR}/include/MandelbrotConfig.h.in 
	${PROJECT_BINARY_DIR}/include/MandelbrotConfig.h
)

#target_include_directories(Mandelbrot PUBLIC
#                           "${PROJECT_BINARY_DIR}"
#                           )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# The compiled library code is here
add_subdirectory(src)

#find_package(Threads)

# The executable code is here
#add_subdirectory(apps)

add_executable(Mandelbrot 
		${PROJECT_SOURCE_DIR}/src/mandelbrot.c
		${PROJECT_SOURCE_DIR}/src/mandel.c
		${PROJECT_SOURCE_DIR}/src/bmp_wr.c
		${PROJECT_SOURCE_DIR}/src/BMP.c
		${PROJECT_SOURCE_DIR}/src/mandel_txt.c

		${PROJECT_SOURCE_DIR}/src/BMP.h
		${PROJECT_SOURCE_DIR}/src/mandel.h
		${PROJECT_SOURCE_DIR}/src/movie.h
		${PROJECT_SOURCE_DIR}/src/mandel_txt.h
)

#add_executable(Mandelbrot 
#	${PROJECT_SOURCE_DIR}/src/mandel_txt.h
#	${PROJECT_SOURCE_DIR}/src/mandel_txt.c
#)

#target_include_directories(${PROJECT_NAME}  PUBLIC 
#	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include/Mandelbrot
	#	$<INSTALL_INTERFACE>
	#		)

target_include_directories(${PROJECT_NAME}  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}  PUBLIC ${PROJECT_BINARY_DIR}/include)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
