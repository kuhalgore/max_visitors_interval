#include "TimeIntervals.h"
#include <iostream>
#include<exception>

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

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
	//print the output on standard output in expted format
	std::cout << val.first.first<<outputIntervalSeperator<< val.first.second<<outputValSeperator<< val.second;

	//print on standard error the warning about invalid entry and exit points if any
	const auto &invalidPositions = obj.getInvalidEntryExitsPositions();
	if (invalidPositions.size())
	{
		std::cerr << "\n\nWarning : Following entry and exit times are ignored because \n"
			"they were not in HH:MM format\n"
			"or\n"
			"entry time > exit time\n";
		const auto& enryExits = obj.getEntryExits();
		for (auto &i : invalidPositions)
		{
			std::cerr <<i+1<< " --- "<<enryExits[i].first << ',' << enryExits[i].second<<'\n';
		}
	}
	return 0;
}