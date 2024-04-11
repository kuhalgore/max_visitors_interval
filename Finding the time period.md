# Finding interval of the day with maximum visitors

This is a C++ program to find time interval of the day with maximum number of visitors 
present in the museum.

Visiting times are given in a file in comma separated format.
This filename can be passed into the program as a first argument.

This program prints the interval in below format.
<start time>-<end time>;<number of visitors>. 

**For example,**
If the file contains this:
10:15,14:20
11:10,15:22
09:58,11:14
Then output will be
11:10-11:14;3

## Logic
It is assumed that time is represented in HH:MM format, \n
If there is any entry where time is not in HH:MM format, that particular entry is ignored
(Corresponding info is logged on std:err).
Now, once we know that the time is in HH:MM format,
Therefore if we divide the whole day (24 hours) into minutes, 
then there will be 24*60 = 1440 distinct time points.


### Populating frequency for each time point
We have taken an array of 1440 elements where each element represents 
a frequency of that time point appearing in any of the intervals from the input file.
We traverse the input file line by line - on each line we get [start time, end time].
For every point between start time and end time (both inclusive), 
we increment the frequency for that point.\n

** space complexity:
n entry and end points are store in a vector, so space complexity would be 
n + 1440 elements for storing frequencies  = O(n)  \n

** Time complexity:
Being stored in contiguous memory (array), we can access each element in constant time with its index.\n
Each element can be increased at max n times, we will need 1440*n operations \n
(considering hypothetical case where all visitors entered and exited at same time) \n
Also n insertions in a vector to store the entry and exit pairs \n
Hence the value would be 1440*n + n = O(n)
\n
\n
### finding the interval with max value
Using array to store frequencies of the time points makes sure that all the time points are stored in ascending order.
To find the interval which contains maximum number of visitors, 
we iterate over each element of the array and find group of adjacent elements with same value (same frequency)
out of all such groups we take the one with maximum value.
say  (start, end, frequency).

If there is more than one such group, then we will consider the group where difference between last and first element is largest.
This way we guarantee that always largest interval with maximum visitors is found.

** Time complexity :
K*1440 comparisons + at max 1440 comparisons in find_if applied on frequencies
= O(1)


*Note* - Here is K > 0 is some constant representing no of comparisons per iteration, see findMaxIntervalAndCount for more details

## Example
consider below input file \n
input file: \n
09:58,11:14 \n
10:15,11:10 \n
11:12,15:22 \n
11:19,12:15 \n

Here, \n
[10:15,11:10] is common between first and second entry \n
Hence its frequency is 2 \n
[11:19,12:15] is common between third and forth entry \n
Hence its frequency is 2 \n
But length of [11:19,12:15] = 56 >  length of [10:15,11:10] \n
Hence our program will output 11:19-12:15;2 \n

## Unit testing:
We have used Catch2 - a header only unit testing framework to write the test cases.
The header file catch.hpp is included in the src directory solely to able to write unit test cases using the Catch2 framework.\n
When the main program is executed without any arguments it will run all the unit test cases and provide the output on std::out console.\n
All unit tests are written in UnitTests.cpp file


## Data validation:
The program will print on standard error the warning about invalid entry and exit times - \n
if they are not in HH:MM format or if entry time > exit time