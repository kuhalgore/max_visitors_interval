This is a C++ program to find time interval of the day with maximum number of visitors 
present in the museum.

visiting times are given in a file in comma separated format.
This program prints the interval in below format.
<start time>-<end time>;<number of visitors>. 

For example if the file contains this:
10:15,14:20
11:10,15:22
9:58,11:14
then output will be 
11:10-11:14;3

logic:
we know that the time is entered in HH:MM format,
Therefore if we divide the whole day (24 hours) into minutes, 
then there will be 24*60 = 1440 distinct time points.

//populate frequency for each time point
We have taken an array of 1440 elements where each element represents 
a frequency of that time point appearing in any of the intervals from the input file.
We traverse the input file line by line - on each line we get [start time, end time].
For every point between start time and end time (both inclusive), 
we increment the frequency for that point.
space complexity: 
O(1440) = constant
Time complexity:
Being stored in contiguous memory (array), we can access each element in constant time with its index
Each element can be increased at max n times, we will need 1440*n operations
(considering hypothetical case where all visitors entered and exited at same time)
Hence the value would be 1440*n = O(n)


//finding the interval with max value
Using array to store frequencies of the time points makes sure that all the time points are stored in ascending order.
To find the interval which contains maximum number of visitors, 
we iterate over each element of the array and find group of adjacent elements with same value (same frequency)
out of all such groups we take the one with maximum value.
say  (start, end, frequency).

If there is more than one such group, then we will consider the group where difference between last and first element is largest.

This way we guarantee that always largest interval with maximum visitors is found.
Time complexity = K*1440 comparisons  = O(1); 
// here is K > 0 is some constant representing no of comparisons per iteration, see findMaxIntervalAndCount for more details

Example, consider below input file 
input file:
9:58,11:14
10:15,11:10
11:12,15:22
11:19,12:15

Here 
[10:15,11:10] is common between first and second entry
Hence its frequency is 2
[11:19,12:15] is common between third and forth entry 
Hence its frequency is 2
But length of [11:19,12:15] = 6 >  length of [10:15,11:10]
Hence our program will output 11:19-12:15;2








