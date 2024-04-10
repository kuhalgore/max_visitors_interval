#include "TimeIntervals.h"
#include <regex>

//helper functions definitions
std::pair<std::string, std::string> splitString(const std::string & arg, char c)
{
	std::pair<std::string, std::string> retVal{};
	auto pos = arg.find(c);
	if (pos != std::string::npos)
	{
		retVal.first = arg.substr(0, pos);
		retVal.second = arg.substr(pos + 1);
	}
	return retVal;	
}

bool isValidTimeFormat(const std::string& arg)
{
	//regular expression to match HH:MM
	std::regex hhmm("([01]?[0-9]|2[0-3]):[0-5][0-9]");
	//Check if the input string is of the format HH:MM
	return std::regex_match(arg, hhmm);
}

//class methods definitions
int TimeIntervals::HHMMToInt(const std::string &arg)
{
	auto val = splitString(arg, minSecondsSeperator);
	return 60 * std::stoi(val.first) + std::stoi(val.second);
	
}
std::string TimeIntervals::intToHHMM(int arg)
{
	if (arg < 0 || arg >= NoOfTimePoints)
	{
		//convert values outside the range of [00:23:59] hours to empty string";
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
				auto val = splitString(line, inputValSeperator);
				m_entryExits.emplace_back(val.first,val.second);
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

	size_t total = m_entryExits.size();
	for (size_t i=0; i < total; ++i)
	{
		auto &obj = m_entryExits[i];
		//add only those time stamps which are in valid HH:MM format
		auto entryTimeValid = isValidTimeFormat(obj.first);
		auto exitTimeValid = isValidTimeFormat(obj.second);
		if (entryTimeValid && exitTimeValid)
		{
			int entryInt = HHMMToInt(obj.first);
			int exitInt = HHMMToInt(obj.second);
			
			if (entryInt <= exitInt)
			{
				//mark the frequencies
				for (auto i = entryInt; i <= exitInt; ++i)
				{
					++m_freq[i];
				}
			}
			else
			{
				//add the enry as invalid one
				m_invalidEntryExits.push_back(i);
			}
		}
		else
		{
			//add the enry as invalid one
			m_invalidEntryExits.push_back(i);
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

	//handle special case - when all elements have same frequencies
	int firstVal = m_freq[0];
	if (std::all_of(m_freq.begin(), m_freq.end(), [&](int i) { return i == firstVal; }))
	{
		//if the frequency is > 0 then we need to consider the whole interval as the max interval
		if (firstVal > 0)
		{
			maxStart = 0;
			maxEnd = NoOfTimePoints - 1;
			
		}
		else // there are no entries for any time points hence set default value
		{
			maxStart = -1;
			maxEnd = -1;
		}
		maxCount = firstVal;
	}
	else
	{
		for (int i = 0; i < NoOfTimePoints; ++i)
		{
			size_t currFreq = m_freq[i];
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

	m_intervalWithMaxVal = std::make_pair(std::make_pair(intToHHMM(maxStart), intToHHMM(maxEnd)), maxCount);
	return m_intervalWithMaxVal;
}