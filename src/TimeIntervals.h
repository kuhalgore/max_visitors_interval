#pragma once
#include <array> 
#include <vector> 
#include <string>
#include <fstream> //for reading from input file
#include <sstream> //for formatting the timestamp in HH::MM format
#include <iomanip> //for setting Time digit to be 2 digit format

constexpr int NoOfTimePoints = 24 * 60;
constexpr char inputValSeperator = ',';
constexpr char minSecondsSeperator = ':';
constexpr int TimeDigitPrecision = 2;

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
time>-<end time>;<number of visitors>. For example: 11:10-14:20;2
method: getIntervalWithMaxVisitors();

*/
class TimeIntervals
{
	std::vector<std::pair<std::string, std::string>> m_entryExits{};
	std::vector<size_t> m_invalidEntryExits{};
	std::array<unsigned int, NoOfTimePoints> m_freq{};
	std::pair< std::pair<std::string, std::string>, size_t > m_intervalWithMaxVal{};

public:

	/*!
	* This function reads the input file 
	@param inputFile - text file containing in and out timings of visitors in below format \n
	10:15,14:20 \n
	11:10,15:22 \n
	9:58,11:14 \n

	The data is stored internally in a vector of pairs m_entryExits
	******************************************************************************/
	void readFromFile(const std::string &inputFile);

	/*!
	* This function calculates frequecncy for each time point for each element in m_entryExits
	These frequencies are stored in an array of 24*60 elements 
	element in the entryExists is considered only if <start time> and <end time> both are in 
	valid HH:MM format
	Otherwise that element is skipped (an approriate error is printed on std::err for that element)
	******************************************************************************/
	void calculateFrequencies();

	/*!
	* This function iterates over each frequency and keep a track if maximum frequency and 
	correpsponding start and end points (max_start and max_end)
	At the end, it populates those values in m_intervalWithMaxVal.

	This function handles following corner cases
	1) frequency of each time point is same and a nonzero value
	2) frequency of each time point is same and is zero

	******************************************************************************/
	std::pair< std::pair<std::string, std::string>, size_t > findMaxIntervalAndCount();


	/*!
	* This function returns the interval with max visitors calculated in findMaxIntervalAndCount()
	as a pair.
	first element of the pair is <start time, end time>
	second element of the pair os value of max visitors
	******************************************************************************/
	std::pair< std::pair<std::string, std::string>, size_t > getIntervalWithMaxVisitors() { return m_intervalWithMaxVal; }

	/*!
	* This setter function used from unit tests  only- 
	In actual flow, m_entryExits is populated by readFromFile()
	******************************************************************************/
	void setEntryExits(std::vector<std::pair<std::string, std::string>> & arg)
	{
		for (auto & obj : arg)
		{
			m_entryExits.emplace_back(obj.first, obj.second);
		}
	}

	/*!
	* This getter function used from unit tests and from main function to display 
	a warning in case entry and exit times were not in valid HH:MM format
	******************************************************************************/
	std::vector<std::pair<std::string, std::string>> getEntryExits()
	{
		return m_entryExits;
	}

	/*!
	* This getter function used for printing invalid enry exit times on std::err 
	******************************************************************************/
	std::vector<size_t> getInvalidEntryExitsPositions() { return m_invalidEntryExits; }

	/*!
	* This getter function used from unit tests  only-
	******************************************************************************/
	size_t getFrequency(const std::string &arg)
	{
		return m_freq[HHMMToInt(arg)];
	}

	

	/*!
	* This function converts the time value in HH:MM string format to Int using logic - 
	60*HH + MM (number of minutes since 00:00)
	@param arg - 24 hours time in HH:MM format
	******************************************************************************/
	int HHMMToInt(const std::string &arg);
	
	/*!
	* This function converts the time value in int to HH:MM string format using logic
	returnVal = <arg/60>:<arg-arg/60>
	@param arg - 24 hours time in integer format with value same as no of minutes since 00:00 (60*HH + MM)
	note - if arg is not in the range of [0, 24*60-1] , it return empty string
	******************************************************************************/
	std::string intToHHMM(int arg);

};

//helper functions
/*!
* This function splits the string based on passed separator
******************************************************************************/
std::pair<std::string, std::string> splitString(const std::string & arg, char c);


/*!
* This function checks if the passed arg is valid time format in HH:MM format
std::regex is used inside the function to achieve the task
*******************************************************************************/
bool isValidTimeFormat(const std::string& arg);
