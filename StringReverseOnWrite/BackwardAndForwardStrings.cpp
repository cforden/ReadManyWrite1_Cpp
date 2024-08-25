#include <string>
#include <algorithm>
#include "BackwardAndForwardStrings.h"

using std::string;

BackwardAndForwardStrings::BackwardAndForwardStrings(const string& forwardString)
	: theForwardString(forwardString), theBackwardString(ReverseACopy(forwardString))
{}

std::string BackwardAndForwardStrings::ReverseACopy(const string& input)
{
	string copiedString(input);
	std::reverse(copiedString.begin(), copiedString.end());
	return copiedString;
}
