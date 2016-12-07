# LightInkLog

log system from prject LightInk 

#build


##win32

* install vs2012
* open file ./windows/LightInkLua.sln
* build
* the file output in ./bin/Debug(Release)/

##linux

* install cmake
* mkdir ./Linux/build
* cd ./Linux/build
* cmake ../
* make
* the file output in ./bin/Debug(Release)/

# Use

* define LIGHTINK_LOG_MULTITHREAD support multi thread
* but you must support Mutex and Thread
* if you use cxx 11 support multi thread you can define LIGHTINK_LOG_CXX11
* you can define them in include/Log/LogConfig.h
* first call LightInkLogCppInit to init log system
* LightInkLog->init_async can set async available
* last call LightInkLog->release() destroy log system