// StringReverseGated.h
#pragma once

#include <string>
#include <deque>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "StringReverse.h"

using std::string;
using std::deque;
using std::mutex;
using std::atomic;
using std::condition_variable;

enum class activity_t
{
	undefined,
	reading,
	writing
};

struct QueuedData
{
	// Default ctor required by queue
	QueuedData()
	{
		QueuedData(undefined);
	}

	QueuedData(activity_t action)
	{
		theActivity = action;
	}

	activity_t theActivity;

	typedef int Payload; // A stand-in for some user's needed data
	Payload someArbitraryData = 42;
};

class StringReverseSyncd
{
	using enum activity_t;
public:
	StringReverseSyncd();
	void Read();
	void Write();
	unsigned long GetErrorCount();
	void DbgReadValidity(const QueuedData& readData);

private:
	StringReverse reverser;
	deque<QueuedData> inputQueue;
	mutex inputQMtx;
	atomic<int> readersCount;
	atomic<bool> isWriting;
};
