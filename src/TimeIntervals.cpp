#include "TimeIntervals.h"


int TimeIntervals::HHMMToInt(const std::string &arg)
{
	int retVal{};
	auto pos = arg.find(minSecondsSeperator);
	if (pos != std::string::npos)
	{
		std::string min = arg.substr(0, pos);
		std::string sec = arg.substr(pos + 1);
		retVal = 60 * std::stoi(min) + std::stoi(sec);
	}

	return retVal;
}
std::string TimeIntervals::intToHHMM(int arg)
{
	if (arg < 0)
	{
		//convert it to "";
		return {};

	}
	std::string retVal{};
	retVal.reserve(6);
	int minsMultiplier = arg / 60;
	int sec = arg - minsMultiplier * 60;
	std::stringstream ss;
	ss << std::setw(TimeDigitPrecision) << std::setfill('0')<< minsMultiplier;
	ss << ':';
	ss << std::setw(TimeDigitPrecision) << std::setfill('0')<<sec;

	return ss.str();
}

void TimeIntervals::readFromFile(const std::string &inputFile)
{
	std::ifstream f(inputFile.c_str());
	if (f.is_open())
	{
		try 
		{
			std::string line;
			std::string entry, exit;
			while (std::getline(f, line))
			{
				auto pos = line.find(inputValSeperator);
				if (pos != std::string::npos)
				{
					entry = line.substr(0, pos);
					exit = line.substr(pos + 1);
					//addEntryExit(entry, exit);
					entryExits.push_back(std::make_pair(entry, exit));
				}
			}
		}
		
		catch (std::exception & e)
		{
			f.close();
			throw e;
		}
		f.close();

	}

}

void TimeIntervals::calculateFrequencies()
{

	for (auto & obj : entryExits)
	{
		int entryInt = HHMMToInt(obj.first);
		int exitInt = HHMMToInt(obj.second);
		
		//mark the frequencies
		for (auto i = entryInt; i <= exitInt; ++i)
		{
			++freq[i];
		}
	}
}

std::pair< std::pair<std::string, std::string>, size_t > TimeIntervals::findMaxIntervalAndCount()
{
	size_t maxCount = 0;
	int start = -1;
	int end = -1;

	int maxStart = -1;
	int maxEnd = -1;
	int prevTimePointFreq = 0;

	//special case - when all elements have same frequencies
	int firstVal = freq[0];
	if (firstVal!=0 && std::all_of(freq.begin(), freq.end(), [&](int i) { return i == firstVal; }))
	{
		maxStart = 0;
		maxEnd = NoOfTimePoints-1;
		maxCount = firstVal;
	}
	else
	{
		for (int i = 0; i < NoOfTimePoints; ++i)
		{
			size_t currFreq = freq[i];
			if (currFreq > maxCount)
			{
				maxStart = start = i;
				maxEnd = end = i;
				maxCount = currFreq;
			}
			else if (currFreq == maxCount)
			{
				if (prevTimePointFreq == currFreq)
				{
					end = i;
				}

				else
				{
					//start again
					start = i;
					end = i;
				}

			}

			else if (currFreq < maxCount)
			{
				if ((end - start) > (maxEnd - maxStart))
				{
					maxStart = start;
					maxEnd = end;
				}
			}

			prevTimePointFreq = currFreq;
		}
	}

	intervalWithMaxVal = std::make_pair(std::make_pair(intToHHMM(maxStart), intToHHMM(maxEnd)), maxCount);
	return intervalWithMaxVal;
}