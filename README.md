## A Fast Web Server based on C++ language 
### Overview
This project is a web server using C++14, using IO multiplexing technology 
to improve IO efficiency, and using a small root heap-based timer to close 
timeout requests to avoid excessive resource usage, and implement 
nanosecond-level logs based on Nanolog The system further improves performance.

### The superiority and technology
1. Use the two buffer to cache the message stream for preventing the overflow and data loss
 in the asynchronous logging sytem.
2. Compress the integer format data or message to reduce the data size to build a nanolog system (Under develop).
3. Use new feature in C++ 14 such as smart pointer and so on to reduce the risk of memory leak.

### Architecture
