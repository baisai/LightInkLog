
cmake_minimum_required(VERSION 2.8)
project(LightInkLog)

include_directories( ${LightInkLog_SOURCE_DIR}/../../include )

set( sourceRootPath ${LightInkLog_SOURCE_DIR}/../../src )

set( middlePath Common )
set( sourceFile 
	RuntimeError.cpp
	SmallObject.cpp
	LightInkVersion.cpp
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

set( middlePath Atomic )
set( sourceFile 
	mintomic_gcc.c
)
foreach(k ${sourceFile} )
  set( fullSourcePath ${fullSourcePath} ${sourceRootPath}/${middlePath}/${k} )
endforeach(k)


if (CMAKE_BUILD_TYPE STREQUAL Debug)
	add_definitions("-D_DEBUG")
endif ()
add_definitions("-DLIGHTINK_AS_DLL")
add_definitions("-DLIGHTINK_EXPORTS")

add_library( ${PROJECT_NAME} SHARED ${fullSourcePath} )

if (CMAKE_BUILD_TYPE STREQUAL Debug)
set(LIBRARY_OUTPUT_PATH ${LightInkLog_SOURCE_DIR}/../../bin/Debug)
else()
set(LIBRARY_OUTPUT_PATH ${LightInkLog_SOURCE_DIR}/../../bin/Release)
endif()
unset(middlePath)
unset(sourceFile)
unset(fullSourcePath)
unset(sourceRootPath)






