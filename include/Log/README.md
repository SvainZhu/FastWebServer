### Overview
The log library is a simple but high-efficient log library to provide the logging
to the file with high concurrency and high availability. The core of log is create
the string and data cache to save the information quickly and append those data into 
the file for persistent storage. By the compress and cache mechanism, save 
log information to improve overall concurrency and consistency asynchronously.

### Log architecture
