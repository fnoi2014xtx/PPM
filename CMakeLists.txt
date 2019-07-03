cmake_minimum_required (VERSION 3.5)
project(PPM)
FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
add_compile_options(-std=c++11)
add_compile_options(-O2)
add_compile_options(-fopenmp)
include_directories(
	headers
)
aux_source_directory(
	sources DIR_SRCS
)
add_executable (${PROJECT_NAME} ${DIR_SRCS})

