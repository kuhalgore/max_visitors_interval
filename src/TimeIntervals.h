#pragma once
#include <array> 
#include <vector> 
#include <string>
#include <fstream> //for reading from input file
#include <sstream> //for formatting the timestamp in HH::MM format
#include <iomanip> //for setting Time digit to be 2 digit format

#include <algorithm>

#include "Trie.h"

constexpr int NoOfTimePoints = 24 * 60;
constexpr char inputValSeperator = ',';
constexpr char outputIntervalSeperator = '-';
constexpr char outputValSeperator = ';';
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

typedef std::pair<std::string, std::string> StartEnd;
typedef std::pair<size_t, StartEnd> InvalidEntry;

class TimeIntervals
{
    std::vector<std::pair<std::string, std::string>> m_entryExits{};
    std::vector<InvalidEntry> m_invalidEntryExits{};
    std::array<unsigned int, NoOfTimePoints> m_freq{};
    /*!
        Making use of Trie to store key value mapppings of string representation of digits to thier int value.
        for example "00"-> 00, "23"-->23 and "59"-->59, etc.
        Our keys consists of only digits from[0 to 9] and key length 2.
      
    */
    Trie<unsigned int> m_timeLiterals;
public:

    /*!
    * This function reads the input file 
    @param inputFile - text file containing in and out timings of visitors in below format \n
    10:15,14:20 \n
    11:10,15:22 \n
    9:58,11:14 \n

    Each line is parsed and start and end time from each line is passed to the function calculateFrequenciesForSingleInterval().
    ******************************************************************************/
    void readFromFile(const std::string &inputFile);

    /*!
    This function calculates frequecncy for each time point in the interval [start, end].
    These frequencies are stored in an array of 24 * 60 elements.
    The function calculates the frequencies only if if both start and end times are in
    valid HH : MM format.
    Otherwise that interval is skipped (an approriate error is printed on std::err for that element)

    This is called from readFromFile() in the main flow and from calculateFrequencies() from Unit Test cases.

    */
    void calculateFrequenciesForSingleInterval(const std::string & start, const std::string & end, size_t indx);

    /*!
    * 
    This function calls calculateFrequenciesForSingleInterval() for each element in m_entryExits
    Used only from Unit Test cases 
    ******************************************************************************/
    void calculateFrequencies();

    /*!
    * 
    This function iterates over each frequency and keep a track if maximum frequency and 
    correpsponding start and end points (max_start and max_end)
    At the end, it retrns the interval with max visitors as a pair.
    first element of the pair is <start time, end time>
    second element of the pair os value of max visitors

    This function handles following corner cases
    1) frequency of each time point is same and a nonzero value
    2) frequency of each time point is same and is zero

    ******************************************************************************/
    std::pair< std::pair<std::string, std::string>, size_t > findMaxIntervalAndCount() const;

    /*!
    * This setter function used from unit tests  only- 
    In actual flow, m_entryExits is populated by readFromFile()
    ******************************************************************************/
    void setEntryExits(std::vector<std::pair<std::string, std::string>> & arg)
    {
        for (const auto & obj : arg)
        {
            m_entryExits.emplace_back(obj.first, obj.second);
        }
    }

    /*!
    * This getter function used from unit tests and from main function to display 
    a warning in case entry and exit times were not in valid HH:MM format
    ******************************************************************************/
    std::vector<std::pair<std::string, std::string>> getEntryExits() const
    {
        return m_entryExits;
    }

    /*!
    * This getter function used for printing invalid enry exit times on std::err 
    ******************************************************************************/
    std::vector<InvalidEntry> getInvalidEntryExitsPositions() const { return m_invalidEntryExits; }

    /*!
    * This getter function used from unit tests  only-
    ******************************************************************************/
    size_t getFrequency(const std::string &arg)
    {
        return m_freq[HHMMToInt(arg)];
    }

    

    /*!
    * This function converts the time value in HH:MM string format to Int using logic - 
    60*(integral value of HH) + (integral value of  MM)
    to convert the HH and MM to integer, we have used Trie data structure, see Trie.h for more details.
    @param arg - 24 hours time in HH:MM format
    ******************************************************************************/
    int HHMMToInt(const std::string &arg) const;
    
    /*!
    * This function converts the time value in int to HH:MM string format using logic
    returnVal = <arg/60>:<arg-arg/60>
    @param arg - 24 hours time in integer format with value same as no of minutes since 00:00 (60*HH + MM)
    note - if arg is not in the range of [0, 24*60-1] , it return empty string
    ******************************************************************************/
    std::string intToHHMM(int arg) const;

    /*!
    * Default constructor
    - We have used this to initialize the trie data structure to store key (string) value(int) pairs of all digits between [00 to 59]
    - see Trie.h for more details
    */
    TimeIntervals()
    {
        m_timeLiterals.insert("00", 0);
        m_timeLiterals.insert("01", 1);
        m_timeLiterals.insert("02", 2);
        m_timeLiterals.insert("03", 3);
        m_timeLiterals.insert("04", 4);
        m_timeLiterals.insert("05", 5);
        m_timeLiterals.insert("06", 6);
        m_timeLiterals.insert("07", 7);
        m_timeLiterals.insert("08", 8);
        m_timeLiterals.insert("09", 9);

        m_timeLiterals.insert("10", 10);
        m_timeLiterals.insert("11", 11);
        m_timeLiterals.insert("12", 12);
        m_timeLiterals.insert("13", 13);
        m_timeLiterals.insert("14", 14);
        m_timeLiterals.insert("15", 15);
        m_timeLiterals.insert("16", 16);
        m_timeLiterals.insert("17", 17);
        m_timeLiterals.insert("18", 18);
        m_timeLiterals.insert("19", 19);

        m_timeLiterals.insert("20", 20);
        m_timeLiterals.insert("21", 21);
        m_timeLiterals.insert("22", 22);
        m_timeLiterals.insert("23", 23);
        m_timeLiterals.insert("24", 24);
        m_timeLiterals.insert("25", 25);
        m_timeLiterals.insert("26", 26);
        m_timeLiterals.insert("27", 27);
        m_timeLiterals.insert("28", 28);
        m_timeLiterals.insert("29", 29);

        m_timeLiterals.insert("30", 30);
        m_timeLiterals.insert("31", 31);
        m_timeLiterals.insert("32", 32);
        m_timeLiterals.insert("33", 33);
        m_timeLiterals.insert("34", 34);
        m_timeLiterals.insert("35", 35);
        m_timeLiterals.insert("36", 36);
        m_timeLiterals.insert("37", 37);
        m_timeLiterals.insert("38", 38);
        m_timeLiterals.insert("39", 39);

        m_timeLiterals.insert("40", 40);
        m_timeLiterals.insert("41", 41);
        m_timeLiterals.insert("42", 42);
        m_timeLiterals.insert("43", 43);
        m_timeLiterals.insert("44", 44);
        m_timeLiterals.insert("45", 45);
        m_timeLiterals.insert("46", 46);
        m_timeLiterals.insert("47", 47);
        m_timeLiterals.insert("48", 48);
        m_timeLiterals.insert("49", 49);

        m_timeLiterals.insert("50", 50);
        m_timeLiterals.insert("51", 51);
        m_timeLiterals.insert("52", 52);
        m_timeLiterals.insert("53", 53);
        m_timeLiterals.insert("54", 54);
        m_timeLiterals.insert("55", 55);
        m_timeLiterals.insert("56", 56);
        m_timeLiterals.insert("57", 57);
        m_timeLiterals.insert("58", 58);
        m_timeLiterals.insert("59", 59);

    }

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
