// BackwardAndForwardStrings.h
#pragma once

#include <string>

using std::string;

/// <summary>
/// Just a fancy way of getting predetermined const strings
/// </summary>
class BackwardAndForwardStrings
{
public:
	BackwardAndForwardStrings(const string& forwardString);

	const std::string& GetBackwardString()
	{
		return theBackwardString;
	}

	const std::string& GetForwardString()
	{
		return theForwardString;
	}

	static std::string ReverseACopy(const std::string& input);

private:
	const std::string theForwardString;
	const std::string theBackwardString;
};

