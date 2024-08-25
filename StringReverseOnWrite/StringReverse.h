// StringReverse.h
#pragma once

#include <string>
#include <atomic>
#include "BackwardAndForwardStrings.h"

class StringReverse
{
public:
	StringReverse();
	const std::string& Read();
	const std::string& Write();
	bool IsValid();
	void reverse_string_in_place(std::string& str);
	unsigned long GetErrorCount();

private:
	std::string theString;
	BackwardAndForwardStrings backwardAndForwardStrings;
	std::atomic<unsigned long> errorCount = 0;
};
