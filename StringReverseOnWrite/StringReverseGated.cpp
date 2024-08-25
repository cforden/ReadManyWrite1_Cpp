// StringReverseGated.cpp
#include <string>
#include <atomic>
#include <thread>
#include <sstream>
#include <iostream>
#include "StringReverseGated.h"

using std::atomic;
using std::string;
using std::cout;
using std::ostringstream;
using std::lock_guard;

StringReverseSyncd::StringReverseSyncd()
{
	readersCount.store(0);
	isWriting.store(false);
}

void StringReverseSyncd::Read()
{
	QueuedData readCmd(reading);
	{
		lock_guard<mutex> lock(inputQMtx);
		inputQueue.push_front(readCmd);
	}

	bool canRead = false;
	while (!canRead)
	{
		while (isWriting.load())
		{
			isWriting.wait(true);
		}
		lock_guard<mutex> lock(inputQMtx);
		if (!inputQueue.empty())
		{
			if (reading == inputQueue.back().theActivity)
			{
				canRead = true;
				inputQueue.pop_back();
				readersCount.fetch_add(1);
			}
			else
			{
				// Yield so more writing cmds to be popped off after "while (!canRead)", above
				continue;
			}
		}
		else
		{
			ostringstream oss;
			oss << "\nERROR: inputQueue.was empty after putting in a readCmd\n";
			cout << oss.str();
		}
	}
	reverser.Read();
	readersCount.fetch_sub(1);
	readersCount.notify_one();
	DbgReadValidity(readCmd);
}

void StringReverseSyncd::DbgReadValidity(const QueuedData& readData)
{
	if (reading != readData.theActivity)
	{
		ostringstream oss;
		oss << "ERROR: readData.theActivity is " << int(readData.theActivity) << "\n";
		cout << oss.str();
	}

	static int max_readers = 0;
	int currentReaders = readersCount.load();
	if (currentReaders > max_readers)
	{
		max_readers = currentReaders; // Not strictly thread-safe but harmless and unimportant
	}

	if (currentReaders > 3 || currentReaders < 0)
	{
		ostringstream oss;
		oss << "WARNING: currentReaders: " << currentReaders << "\n";
		cout << oss.str();
	}
}

void StringReverseSyncd::Write()
{
	QueuedData writeCmd(writing);
	{
		lock_guard<mutex> lock(inputQMtx);
		inputQueue.push_front(writeCmd);
	}
	bool canWrite = false;
	while (!canWrite)
	{
		lock_guard<mutex> lock(inputQMtx);
		if (!inputQueue.empty())
		{
			int readers = readersCount.load();
			while (readers > 0)
			{
				readersCount.wait(readers);
				readers = readersCount.load();
			}
			if (writing == inputQueue.back().theActivity)
			{
				canWrite = true;
				isWriting.store(true);
				reverser.Write();
				inputQueue.pop_back();
			}
		}
		else
		{
			ostringstream oss;
			oss << "\nERROR: inputQueue.was empty after putting in a writeCmd\n";
			cout << oss.str();
		}
	}
	isWriting.store(false);
	isWriting.notify_all();
}

unsigned long StringReverseSyncd::GetErrorCount()
{
	return reverser.GetErrorCount();
}
