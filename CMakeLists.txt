cmake_minimum_required(VERSION 3.0)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH) 

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(max_visitors_interval)
add_executable(
	max_visitors_interval
	src/main.cpp
	src/TimeIntervals.cpp
	src/TimeIntervals.h
	src/Catch.hpp
	src/UnitTests.cpp
	)
