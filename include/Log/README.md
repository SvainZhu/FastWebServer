### Overview
The log library is a simple but high-efficient log library to provide the logging
to the file with high concurrency and high availability. The core of log is create
the string and data cache to save the information quickly and append those data into 
the file for persistent storage. By the compress and cache mechanism, save 
log information to improve overall concurrency and consistency asynchronously.

### Log architecture
![Log architecture](./log_architecture.png)


### The component of log
#### Logger 
The logger is a component that can log a message into a file, which is encapsulated
 into a header file, so the user only needs to call for log the messages into the file.
Logger include an instance of LogStream class to add the fixed format information when logging 
message, for example the line number, log file name, data.

#### Logging
The logging is the core of this log system that will run a log thread to record the message into
 the log file. It will use two tools to finish logging task: 1. Log Buffer, 2. Append Log.
These two utils will complete the set of log buffers to cache the message for improving the 
performance when the system is in high concurrency and append the logging messages in the buffers 
into the log file separately. Also, a Log Message class is defined to format the message and output 
into the standford file descriptor. It overrides the << operator to output the data and set a exclusive
 buffer to get a complete row line before flush. 

#### Log Buffer
The log buffer will set the buffers that use the double cache technology to prevent one of 
the buffer overflows from causing data loss. When the buffers are full or the time is out, the 
logging component will use Append Log component to append the data from buffers into the end of log file.

#### Append Log
The Append Log component defines an append operator that can append the string data into the end of file.
It is based on Append Util class that encapsulates the open and close function of file, use the standard I/O
 layer to append the data into the end of the file. The Append Log also set a counter to process the flush 
the data into file in a fixed number of cycles.