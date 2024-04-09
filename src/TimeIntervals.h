#pragma once
#include <array> 
#include <vector> 
#include <fstream> // for reading from input file
#include <string>
#include <sstream> //for formatting the timestamp in HH::MM format
#include <iomanip> //for setting Time digit to be 2 digit format

//!  TimeIntervals class. 
/*!

This class can read visiting times from an input file.
The path of the input file is given as an argument to its readFromFile() method.
The input file is a text file, where every line is the entering and leaving time values of one visitor separated with commas.
For example:
10:15,14:20
11:10,15:22
Visiting times can be in a random order and not sorted by entering or leaving time values.
method : readFromFile(const std::string & fileName);

Using the data from the input file, this class can find the time period, when there were the most number of visitors and how
many visitors were in the museum in that time period.
method : findMaxIntervalAndCount();

This class can find the results found in the previous step to the standard output in the form <start
time>-<end time>;<number of visitors>. For example: 11:10-14:20;2method: getIntervalWithMaxVisitors();
*/


constexpr int NoOfTimePoints = 24 * 60;
constexpr char inputValSeperator = ',';
constexpr char minSecondsSeperator = ':';
constexpr int TimeDigitPrecision = 2;

class TimeIntervals
{
	//std::vector<std::pair<int, int>> entryExits;
	std::vector<std::pair<std::string, std::string>> entryExits;
	std::array<int, NoOfTimePoints> freq;
	std::pair< std::pair<std::string, std::string>, size_t > intervalWithMaxVal;

public:

	void setEntryExits(std::vector<std::pair<std::string, std::string>> & arg)
	{
		for (auto & obj : arg)
		{
			entryExits.emplace_back(obj.first, obj.second);
		}
	}

	std::vector<std::pair<std::string, std::string>> getEntryExits()
	{
		return entryExits;
	}

	size_t getFrequency(const std::string &arg)
	{
		return freq[HHMMToInt(arg)];
	}

	std::pair< std::pair<std::string, std::string>, size_t > getIntervalWithMaxVisitors() { return intervalWithMaxVal; }

	/*!
	* This function converts the time value in HH:MM string format to Int with logic
	returnVal = no of minutes since 00:00 (60*HH + MM)
	@param arg - 24 hours time in HH:MM format
	******************************************************************************/
	int HHMMToInt(const std::string &arg);
	/*!
	* This function converts the time value in int to HH:MM string format with logic
	returnVal = <arg/60>:<arg-arg/60>
	@param arg - 24 hours time in integer format with value same as no of minutes since 00:00 (60*HH + MM)
	******************************************************************************/
	std::string intToHHMM(int arg);

	void readFromFile(const std::string &inputFile);
	void calculateFrequencies();
	std::pair< std::pair<std::string, std::string>, size_t > findMaxIntervalAndCount();

};
