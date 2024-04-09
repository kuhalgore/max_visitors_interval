#include "TimeIntervals.h"
#include <iostream>
#include<exception>

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

/*! \mainpage Finding time interval with maximum number of visitors

This is a C++ program to find time interval of the day with maximum number of visitors
present in the museum.

Visiting times are given in a file in comma separated format.
Name of this file needs to be passed to this program as a first argument


This program prints the interval in below format.
<start time>-<end time>;<number of visitors>.

For example if the file contains this:\n
10:15,14:20 \n
11:10,15:22 \n
9:58,11:14 \n

then output will be,\n
11:10-11:14;3\n


Algorithm logic:\n
We know that the time is entered in HH:MM format, 
therefore if we divide the whole day (24 hours) into minutes, 
then there will be 24*60 = 1440 distinct time points.\n

//populate frequency for each time point ------\n
We have taken an array of 1440 elements where each element represents 
a frequency of that time point appearing in any of the intervals from the input file.
We traverse the input file line by line - on each line we get [start time, end time].
For every point between start time and end time (both inclusive),
we increment the frequency for that point.\n
space complexity:\n
O(1440) = constant\n
Time complexity:\n
Being stored in contiguous memory (array), we can access each element in constant time with its index.\n
Each element can be increased at max n times, we will need 1440*n operations\n
(considering hypothetical case where all visitors entered and exited at same time)\n
Hence the value would be 1440*n = O(n)


//finding the interval with max value ------ \n
Using array to store frequencies of the time points makes sure that all the time points are stored in ascending order.
To find the interval which contains maximum number of visitors,
we iterate over each element of the array and find group of adjacent elements with same value (same frequency)
out of all such groups we take the one with maximum value.
say  (start, end, frequency).\n
If there is more than one such group, then we will consider the group where difference between last and first element is largest. \n
This way we guarantee that always largest interval with maximum visitors is found. \n
/bTime complexity: K*1440 comparisons  = O(1); \n
// here is K > 0 is some constant representing no of comparisons per iteration, see findMaxIntervalAndCount for more details \n

Example, consider below input file \n
input file: \n
9:58,11:14 \n
10:15,11:10 \n
11:12,15:22 \n
11:19,12:15 \n

Here
[10:15,11:10] is common between first and second entry \n
Hence its frequency is 2 \n
[11:19,12:15] is common between third and forth entry \n
Hence its frequency is 2 \n
But length of [11:19,12:15] = 6 >  length of [10:15,11:10] \n
Hence our program will output 11:19-12:15;2 \n

*/


int main(int argc, char *argv[])
{
	std::string inputFile{};
	if (argc == 1)
	{
		std::cout << "No arguments passed, running unit tests using Catch2 framework\n";
		try
		{
			//run all unit test cases using catch2 unit testing framework 
			auto result = Catch::Session().run(argc, argv);
			return result;
		}
		catch (...)
		{
			return -1;
		}
	}
	else if (argc > 1)
	{
		inputFile = argv[1];
	}

	TimeIntervals obj;

	obj.readFromFile(inputFile);
	obj.calculateFrequencies();
 	obj.findMaxIntervalAndCount();
	auto val = obj.getIntervalWithMaxVisitors();
	std::cout << val.first.first<<'-'<< val.first.second<<';'<< val.second;

	return 0;
}