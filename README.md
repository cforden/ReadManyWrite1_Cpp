C++ code demos the multithreading design pattern of either allowing many threads to read an object simultaneously or, 
when modifying that object, allowing only a single thread to write to it.

This code dispatches read and write messages from a thread-safe queue that allows the programmer to 
prioritize writing, reading, or chronological order.  Sometimes such techniques are called "enforcing fairness."
The message queue allows for a very broad range of possible definitions of what constitutes fair ordering of thread execution.

It takes several minutes for this demo to run and conclude printing to the console.

See also similar demos I wrote in other languages:
- C#:  https://github.com/cforden/ReadManyWrite1_CS
     This C# version is simpler; it has no queue to prioritize one definition of fairness over another.

