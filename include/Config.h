

#ifndef LIGHTINK_COMMON_CONFIG_H_
#define LIGHTINK_COMMON_CONFIG_H_

#if defined(DEBUG) || defined(_DEBUG)
#define LIGHTINK_DEBUG
#endif // DEBUG


#define LightInkNoTrace //关闭trace日志

#ifndef LIGHTINK_DEBUG
#define LightInkNoDebug //关闭debug日志
#endif

//#define LightInkNoMessage
//#define LightInkNoError

#ifndef LIGHTINK_DEBUG
#define LightInkNoScriptDebug
#endif

//#define LightInkNoScriptMessage
//#define LightInkNoScriptError



/////////////////////////////////////////////////////////
//导出,导入变量
////////////////////////////////////////////////////////
#ifdef WIN32

#ifdef LIGHTINK_AS_DLL

#ifdef LIGHTINK_EXPORTS
#define LIGHTINK_DECL __declspec(dllexport)
#define LIGHTINK_TEMPLATE_DECL __declspec(dllexport)
#else
#define LIGHTINK_DECL __declspec(dllimport)
#define LIGHTINK_TEMPLATE_DECL __declspec(dllexport)
#endif

#else
#define LIGHTINK_DECL extern
#define LIGHTINK_TEMPLATE_DECL
#endif // LIGHTINK_BUILD_DLL



#elif defined __GNUC__ && __GNUC__ >= 4
#define LIGHTINK_DECL __attribute__((visibility ("default")))
#define LIGHTINK_TEMPLATE_DECL

#else
#define LIGHTINK_DECL
#define LIGHTINK_TEMPLATE_DECL

#endif



#endif