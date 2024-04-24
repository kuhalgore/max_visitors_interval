#include "TimeIntervals.h"

//helper functions definitions
std::pair<std::string, std::string> splitString(const std::string & arg, char c)
{
    std::pair<std::string, std::string> retVal{};
    auto pos = arg.find(c);
    if (pos != std::string::npos)
    {
        return { arg.substr(0, pos), arg.substr(pos + 1) };
    }
    return {};  
}

bool isValidTimeFormat(const std::string& arg)
{
    if(arg.empty() || arg.length() !=5 || arg[2] != minSecondsSeperator)
    {
        return false;
    }
    bool retVal = true;
    switch(arg[0])
    {
        case '0':
        case '1':
        {
            retVal = retVal && ( arg[1] >= '0' && arg[1] <= '9');
        }
        break;
        
        case '2':
        {
            retVal = retVal && ( arg[1]>= '0' && arg[1]<= '3');
        }
        break;
        
        default:
            retVal = false;
            
    }
    if (retVal)
    {
        switch(arg[3])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            {
                retVal = retVal && ( arg[4] >= '0' && arg[4] <= '9');
            }
            break;
            
            default:
                retVal = false;
        }
    }
    
    return retVal;
}

//class methods definitions
int TimeIntervals::HHMMToInt(const std::string &arg) const
{
    const auto &val = splitString(arg, minSecondsSeperator);
    
    const auto& it1 = m_timeLiterals.search(val.first);
    const auto& it2 = m_timeLiterals.search(val.second);

    if (it1.first && it2.first)
    {
        return 60 * (it1.second->second) + it2.second->second;
    }
    else
    {
        return 0;
    }
    
}
std::string TimeIntervals::intToHHMM(int arg) const
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
            size_t i = 0;
            while (std::getline(f, line))
            {
                auto val = splitString(line, inputValSeperator);
                calculateFrequenciesForSingleInterval(val.first, val.second, i++);
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

void TimeIntervals::calculateFrequenciesForSingleInterval(const std::string & start, const std::string & end, size_t indx)
{
    //add only those time stamps which are in valid HH:MM format
    auto entryTimeValid = isValidTimeFormat(start);
    auto exitTimeValid = isValidTimeFormat(end);
    if (entryTimeValid && exitTimeValid)
    {
        int entryInt = HHMMToInt(start);
        int exitInt = HHMMToInt(end);

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
            m_invalidEntryExits.push_back(std::make_pair(indx, std::make_pair(start, end)));
        }
    }
    else
    {
        //add the enry as invalid one
        m_invalidEntryExits.push_back(std::make_pair(indx, std::make_pair(start, end)));
    }
}
void TimeIntervals::calculateFrequencies()
{
    size_t total = m_entryExits.size();
    for (size_t i=0; i < total; ++i)
    {
        const auto &obj = m_entryExits[i];
        calculateFrequenciesForSingleInterval(obj.first, obj.second, i);
    }
}

std::pair< std::pair<std::string, std::string>, size_t > TimeIntervals::findMaxIntervalAndCount() const
{
    size_t maxCount = 0;
    int start = -1;
    int end = -1;

    int maxStart = -1;
    int maxEnd = -1;
    size_t prevTimePointFreq = 0;

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

    return std::make_pair(std::make_pair(intToHHMM(maxStart), intToHHMM(maxEnd)), maxCount);
}