#include <iostream>
#include <string>
#include "catch.hpp"
#include "TimeIntervals.h"

TEST_CASE("test 01", "3 oerlapping intervals") 
{
	TimeIntervals testObj;
	std::vector<std::pair<std::string, std::string>> data =
	{
		{"10:15","14:20"},
		{"11:10","15:22"},
		{"09:58","11:14"}
	};

	testObj.setEntryExits(data);
	testObj.calculateFrequencies();
	REQUIRE(testObj.getFrequency("09:57") == 0);

	REQUIRE(testObj.getFrequency("09:58") == 1);
	REQUIRE(testObj.getFrequency("10:14") == 1);

	REQUIRE(testObj.getFrequency("10:15") == 2);
	REQUIRE(testObj.getFrequency("11:09") == 2);

	
	REQUIRE(testObj.getFrequency("11:10") == 3);
	REQUIRE(testObj.getFrequency("11:14") == 3);

	REQUIRE(testObj.getFrequency("11:15") == 2);
	REQUIRE(testObj.getFrequency("14:20") == 2);

	REQUIRE(testObj.getFrequency("14:21") == 1);
	REQUIRE(testObj.getFrequency("15:22") == 1);

	REQUIRE(testObj.getFrequency("15:23") == 0);

	auto val = testObj.findMaxIntervalAndCount();
	REQUIRE(val.first.first == "11:10");
	REQUIRE(val.first.second == "11:14");
	REQUIRE(val.second == 3);

}


TEST_CASE("test 02", "zero intervals")
{
	TimeIntervals testObj;
	std::vector<std::pair<std::string, std::string>> data ={};

	testObj.setEntryExits(data);

	testObj.calculateFrequencies();

	REQUIRE(testObj.getEntryExits().size() == 0);
	REQUIRE(testObj.getFrequency("11:01") == 0);

	auto val = testObj.findMaxIntervalAndCount();
	REQUIRE(val.first.first == "");
	REQUIRE(val.first.second == "");
	REQUIRE(val.second == 0);
}

TEST_CASE("test 03", "2 oerlapping and 2 non-overelapping intervals")
{
	TimeIntervals testObj;
	std::vector<std::pair<std::string, std::string>> data =
	{
		{ "09:00","10:20" },
		{ "10:00","11:50" },
		{ "12:10","12:50" },
		{ "13:00","13:10" }
		
	};

	testObj.setEntryExits(data);
	testObj.calculateFrequencies();
	REQUIRE(testObj.getFrequency("9:59") == 1);

	REQUIRE(testObj.getFrequency("10:00") == 2);
	REQUIRE(testObj.getFrequency("10:20") == 2);

	REQUIRE(testObj.getFrequency("11:51") == 0);
	REQUIRE(testObj.getFrequency("12:09") == 0);


	REQUIRE(testObj.getFrequency("12:10") == 1);
	REQUIRE(testObj.getFrequency("12:50") == 1);

	REQUIRE(testObj.getFrequency("13:00") == 1);
	REQUIRE(testObj.getFrequency("13:10") == 1);

	REQUIRE(testObj.getFrequency("13:11") == 0);
	
	
	auto val = testObj.findMaxIntervalAndCount();
	REQUIRE(val.first.first == "10:00");
	REQUIRE(val.first.second == "10:20");
	REQUIRE(val.second == 2);

}

TEST_CASE("test 04", "intervals in reverse order")
{
	TimeIntervals testObj;
	std::vector<std::pair<std::string, std::string>> data =
	{
		{ "13:00","13:10" },
		{ "12:10","12:50" },
		{ "10:00","11:50" },
		{ "09:00","10:20" },

	};

	testObj.setEntryExits(data);
	testObj.calculateFrequencies();

	auto val = testObj.findMaxIntervalAndCount();
	REQUIRE(val.first.first == "10:00");
	REQUIRE(val.first.second == "10:20");
	REQUIRE(val.second == 2);

}

TEST_CASE("test 05", "time stamp is padded with zero if there are single digit numbers")
{
	TimeIntervals testObj;
	std::vector<std::pair<std::string, std::string>> data =
	{
		{ "00:00","02:57" },
		{ "00:00","01:02" },
		{ "10:00","11:50" },
		{ "09:00","10:20" },

	};

	testObj.setEntryExits(data);
	testObj.calculateFrequencies();

	auto val = testObj.findMaxIntervalAndCount();
	REQUIRE(val.first.first == "00:00");
	REQUIRE(val.first.second == "01:02");
	REQUIRE(val.second == 2);

}

TEST_CASE("test 06", "all 5 visitors eneter at 00 and leave at 23:59")
{
	TimeIntervals testObj;
	std::vector<std::pair<std::string, std::string>> data =
	{
		{ "00:00","23:59" },
		{ "00:00","23:59" },
		{ "00:00","23:59" },
		{ "00:00","23:59" },
		{ "00:00","23:59" }
	
	};

	testObj.setEntryExits(data);
	testObj.calculateFrequencies();

	auto val = testObj.findMaxIntervalAndCount();
	REQUIRE(val.first.first == "00:00");
	REQUIRE(val.first.second == "23:59");
	REQUIRE(val.second == data.size());

}