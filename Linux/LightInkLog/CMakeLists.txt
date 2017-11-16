
cmake_minimum_required(VERSION 2.8)
project(LightInkLog)

include_directories( ${LightInkLog_SOURCE_DIR}/../../include )

set( sourceRootPath ${LightInkLog_SOURCE_DIR}/../../src )

set( middlePath Common )
set( sourceFile 
	LightInkVersion.cpp
	OsHelper.cpp
	RuntimeError.cpp
	SmallObject.cpp
	WinSnprintf.c
)
foreach(k ${sourceFile} )
  set( fullSourcePath ${fullSourcePath} ${sourceRootPath}/${middlePath}/${k} )
endforeach(k)


set( middlePath Log )
set( sourceFile
	LogFormat.cpp
	Logger.cpp
	LoggerMgr.cpp
	LogOption.cpp
	LogThreadType.cpp
	async/AsyncMsg.cpp
	async/AsyncThread.cpp
	channel/DateNameStrategy.cpp
	channel/LogChannelAndroid.cpp
	channel/LogChannelStdColor.cpp
	helper/FileHelper.cpp
	helper/FormatHelper.cpp
)
foreach(k ${sourceFile} )
  set( fullSourcePath ${fullSourcePath} ${sourceRootPath}/${middlePath}/${k} )
endforeach(k)


if (CMAKE_BUILD_TYPE STREQUAL Release)
	add_definitions("-DNDEBUG")
else ()
	add_definitions("-D_DEBUG")
	set( CMAKE_BUILD_TYPE Debug )
endif ()

add_definitions("-DLIGHTINK_AS_DLL")
add_definitions("-DLIGHTINK_EXPORTS")

add_library( ${PROJECT_NAME} SHARED ${fullSourcePath} )

find_library(RT_LIB NAMES rt)
target_link_libraries(${PROJECT_NAME} ${RT_LIB})

if (CMAKE_BUILD_TYPE STREQUAL Debug)
set(LIBRARY_OUTPUT_PATH ${LightInkLog_SOURCE_DIR}/../../bin/Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -W -pipe -fPIC")
else()
set(LIBRARY_OUTPUT_PATH ${LightInkLog_SOURCE_DIR}/../../bin/Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -W -pipe -fPIC")
endif()

unset(middlePath)
unset(sourceFile)
unset(fullSourcePath)
unset(sourceRootPath)






