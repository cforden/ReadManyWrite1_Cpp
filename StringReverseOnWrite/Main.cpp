// Main.cpp
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include "StringReverseGated.h"

using namespace std;

namespace
{
	const int MAX_LOOP = 10*1000;
	const int STATUS_LOOPS = 1000;
	StringReverseSyncd theReverser;
}

class ThreadStopper
{
public:
	void Add(jthread* threadPtr)
	{
		std::lock_guard<std::mutex> lck(mtx);
		if (isStopRequested)
		{
			threadPtr->request_stop();
		}
		threads.push_back(threadPtr);
	}

	void StopAllThreads()
	{
		std::lock_guard<std::mutex> lck(mtx);
		isStopRequested = true;
		for (jthread* thread : threads)
		{
			thread->request_stop();
		}
	}

private:
	vector<jthread*> threads;
	bool isStopRequested = false;
	std::mutex mtx;
}threadStopper;

void reader_thread(std::stop_token stop_token, int ID)
{
	try
	{
		for (int i = 0; i < MAX_LOOP; ++i)
		{
			theReverser.Read();
			if (i % STATUS_LOOPS == 0)
			{
				ostringstream oss;
				oss << "Reader " << ID << " iteration " << i << "\n";
				cout << oss.str();
			}
			if (stop_token.stop_requested())
			{
				break;
			}
		}
	}
	catch (exception)
	{
		threadStopper.StopAllThreads();
	}
}

void writer_thread(std::stop_token stop_token, int ID)
{
	try
	{
		for (int i = 0; i < MAX_LOOP; ++i)
		{
			theReverser.Write();
			if (i % STATUS_LOOPS == 0)
			{
				ostringstream oss;
				oss << "Writer " << ID << " iteration " << i << "\n";
				cout << oss.str();
			}
			if (stop_token.stop_requested())
			{
				break;
			}
		}
	}
	catch (exception)
	{
		threadStopper.StopAllThreads();
	}
}

int main()
{
	jthread t1(reader_thread, 1);
	threadStopper.Add(&t1);

	jthread t2(reader_thread, 2);
	threadStopper.Add(&t2);

	jthread t3(reader_thread, 3);
	threadStopper.Add(&t3);

	jthread t4(writer_thread, 4);
	threadStopper.Add(&t4);

	jthread t5(writer_thread, 5);
	threadStopper.Add(&t5);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	cout << "\n" << "Total Error count: " << theReverser.GetErrorCount() << endl;
}

