#include <iostream>
#include <string>
#include "catch.hpp"
#include "TimeIntervals.h"


TEST_CASE("test 01", "finding max visitors interval with 3 overlapping intervals") 
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
	REQUIRE(testObj.getFrequency("9:57") == 0);

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


TEST_CASE("test 02", "max visitors calculated as zero when there are no entry and exits")
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

TEST_CASE("test 03", "finding max visitors interval with 2 overlapping and 2 non-overlapping intervals")
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

TEST_CASE("test 04", "finding max visitors when entry exit intervals are in entered in reverse order according to entry times")
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

TEST_CASE("test 05", "finding max visitors when there are 2 intervals with same frequency of visitors")
{
	SECTION("2 intervals with same frequency, second is of larger duration")
	{
		TimeIntervals testObj;
		std::vector<std::pair<std::string, std::string>> data =
		{
			{"09:58","11:14"},
			{"10:15","11:10"},
			{"11:12","15:22"},
			{"11:19","12:15"}
		};

		testObj.setEntryExits(data);
		testObj.calculateFrequencies();
		auto val = testObj.findMaxIntervalAndCount();

		// in above case - there are  2 intervals with max frequency as 2 wiz [10:15,11:10] and [11:19, 12:15]
		//however second one is of length 56 mins, first one is of lengh  55 mins, hence we should get second interval
		// in the output as answer
		REQUIRE(val.first.first == "11:19");
		REQUIRE(val.first.second == "12:15");
		REQUIRE(val.second == 2);
	}

	SECTION("2 intervals with same frequency, first one and second one are of same duration")
	{
		TimeIntervals testObj;
		std::vector<std::pair<std::string, std::string>> data =
		{
			{ "09:58","11:14" },
			{ "10:14","11:10" },
			{ "11:12","15:22" },
			{ "11:19","12:15" }
		};

		testObj.setEntryExits(data);
		testObj.calculateFrequencies();
		auto val = testObj.findMaxIntervalAndCount();

		// in above case - there are  2 intervals with max frequency as 2 wiz [10:14,11:10] and [11:19, 12:15]
		// however first one is of length 56 mins and first one is of lengh  56 mins, hence we should get the first interval
		// in the output as answer

		REQUIRE(val.first.first == "10:14");
		REQUIRE(val.first.second == "11:10");
		REQUIRE(val.second == 2);
	}


}

TEST_CASE("test 06", "finding max visitors when there entry and exit times contains time in single digit and when it is padded with zero")
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

TEST_CASE("test 07", "finding max visitors when all 5 visitors eneter at 00 and leave at 23:59")
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

TEST_CASE("test 08", "max visitors should be calculated as zero when entry and exit times are in incorrect format")
{
	TimeIntervals testObj;
 	std::vector<std::pair<std::string, std::string>> data =
	{
		{ "00:00","27:21" },
		{ "-23:00","22:59" },
		
	};

	testObj.setEntryExits(data);
	testObj.calculateFrequencies();

	auto val = testObj.findMaxIntervalAndCount();
	REQUIRE(val.first.first == "");
	REQUIRE(val.first.second == "");
	REQUIRE(val.second == 0);

}

TEST_CASE("test 09", "helper function splitString should split the string correclty based on passed arg and a sepearator char")
{
	SECTION("empty string")
	{
		std::string s1{};
		auto val = splitString(s1, ':');
		REQUIRE(val.first == "");
		REQUIRE(val.second == "");
	}

	SECTION("normal string")
	{
		std::string s1{ "14:02" };
		auto val = splitString(s1, ':');
		REQUIRE(val.first == "14");
		REQUIRE(val.second == "02");
	}

	SECTION("repeatating split char")
	{
		std::string s1{ "14:01:02" };
		auto val = splitString(s1, ':');
		REQUIRE(val.first == "14");
		REQUIRE(val.second == "01:02");

		std::string s2{ "00:00,,," };
		auto val2 = splitString(s2, ',');
		REQUIRE(val2.first == "00:00");
		REQUIRE(val2.second == ",,");
	}
	SECTION("different char")
	{
		std::string s1{ "00:00,23:59" };
		auto val = splitString(s1, ',');
		REQUIRE(val.first == "00:00");
		REQUIRE(val.second == "23:59");
	}
}


TEST_CASE("test 10", "helper function isValidTimeFormat shuld return true when the arg passed is in valid HH:MM format")
{
	SECTION("some invalid time formats")
	{
		REQUIRE(isValidTimeFormat("") == false);
		REQUIRE(isValidTimeFormat(":") == false);
		REQUIRE(isValidTimeFormat("0") == false);
		REQUIRE(isValidTimeFormat("235") == false);
		REQUIRE(isValidTimeFormat("abcd") == false);
		
		REQUIRE(isValidTimeFormat("24:00") == false); //rather we should use 00:00
		REQUIRE(isValidTimeFormat("24:01") == false); 
		REQUIRE(isValidTimeFormat("27:02") == false); 
	}
	SECTION("some valid time formats")
	{
		REQUIRE(isValidTimeFormat("00:00") == true);
		REQUIRE(isValidTimeFormat("00:01") == true);
		REQUIRE(isValidTimeFormat("23:59") == true);
		REQUIRE(isValidTimeFormat("23:59") == true);
	}
	
}
