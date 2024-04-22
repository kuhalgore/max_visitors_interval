cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(max_visitors_interval)
add_executable(
	max_visitors_interval
	src/main.cpp
	src/TimeIntervals.cpp
	src/TimeIntervals.h
	src/catch.hpp
	src/UnitTests.cpp
	)
