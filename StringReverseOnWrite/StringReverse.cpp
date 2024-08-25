// StringReverseOnWrite.cpp 
//
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include "StringReverse.h"
#include "StringReverseGated.h"
#include "BackwardAndForwardStrings.h"

using namespace std;

namespace {
	const int REVERSALS_PER_WRITE = 10 + 1;
}


StringReverse::StringReverse() : backwardAndForwardStrings("Hello World!")
{
	theString = backwardAndForwardStrings.GetForwardString();
}

const string& StringReverse::Read()
{
	IsValid();
	return theString;
}

const string& StringReverse::Write()
{
	for (int i = 0; i < REVERSALS_PER_WRITE; ++i)
	{
		IsValid();
		reverse_string_in_place(theString);
	}
	IsValid();
	return theString;
}

bool StringReverse::IsValid()
{
	string testedString = theString;
	if (backwardAndForwardStrings.GetBackwardString() == testedString)
	{
		return true;
	}
	if (backwardAndForwardStrings.GetForwardString() == testedString)
	{
		return true;
	}

	// Error detected!
	unsigned long errCnt = errorCount.fetch_add(1);
	errCnt++;
	ostringstream oss;
	oss << "\nError #" << errCnt << "  with string value : " << testedString << "\n";
	cout << oss.str() << endl;
	if (errCnt > 10)
	{
		throw exception("Enough errors");
	}
}

void StringReverse::reverse_string_in_place(std::string& str)
{
	int left = 0;
	int right = str.length() - 1;

	while (left < right)
	{
		if (1 == left % 3)
		{
			// An occasional wait to provide ample opportunity for corruption due to race conditions
			std::this_thread::sleep_for(std::chrono::nanoseconds(10));
		}
		// Swap characters at left and right indices
		std::swap(str[left], str[right]);

		// Move pointers towards the middle
		left++;
		right--;
	}
}

unsigned long StringReverse::GetErrorCount()
{
	return errorCount.load();
}

