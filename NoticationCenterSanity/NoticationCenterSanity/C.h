//This file just helps me overcome the shitty nonJavaNonC#-esque experience of C/C++
#pragma once 
#include <Windows.h>
#include <stdio.h>

//Ensure __func__ is defined, someway or another. This is the name of the currently executing function.
#ifndef __func__
#define __func__ __FUNCTION__
#endif

//Turn stuff into strings!
#define STRINGIFY(s) #s

//These macros make it so that you can just type w(SOME_RANDOM_POUND_DEFINE) to turn a #define'd char string into a wchar_t string. 
#define __L(x) L ## x
#define w(x)  __L(x)

//Make printing errors easier.
#define PRINT_INT(val) printf("%s = %d\n", STRINGIFY(val), val);
#define PRINT_INT(val) printf("%s = %d\n", STRINGIFY(val), val);
#define PRINT_LONG(val) printf("%s = %i\n", STRINGIFY(val), val);
#define PRINT_INT64(val) printf("%s = %I64d\n", STRINGIFY(val), val);
#define PRINT_DOUBLE(val) printf("%s = %f\n", STRINGIFY(val), val);
#define PRINT_HEX(val) printf("%s = 0x%X\n", STRINGIFY(val), val);
//#define PRINT_HEX(val) printf("%s = 0x%016llX\n" STRINGIFY(val), val);
#define PRINT_BOOL(val) printf("%s = %s\n", STRINGIFY(val), val? "true":"false");
#define PRINT_WSTR(val) printf("%s = '%S'\n", STRINGIFY(val), val);
#define PRINT_POINT(val) printf("%s = (%d, %d)\n", STRINGIFY(val), (val).x, (val).y);
#define PRINT_RECT(val) printf("%s = (%d, %d, %d, %d)\n", STRINGIFY(val), (val).left, (val).top, (val).right, (val).bottom);



//This marco allows you to easily declare a static string constant (wchar_t*) in a class header
//Meaning, instead of this:
//		static const wchar_t *wImmersiveControlPanelUMID() 
//		{
//			static const wchar_t *str = L"windows.immersivecontrolpanel_cw5n1h2txyewy!microsoft.windows.immersivecontrolpanel";
//			return str;
//		}
//
//All you gotta do is this:
//		STATIC_WSTR(wImmersiveControlPanelUMID, L"windows.immersivecontrolpanel_cw5n1h2txyewy!microsoft.windows.immersivecontrolpanel");
//
//But when you do this, you need to reference it like a function. For example (assuming the class name is ModernHelpers): 
//		wcslen(ModernHelpers::wImmersiveControlPanelUMID());
#define STATIC_WSTR(name, text) inline static const wchar_t *(name)() {static const wchar_t *str = (text); return str;}
#define STATIC_DOUBLE(name, val) inline static double (name)(){return (val);}
#define STATIC_POINT(name, x, y) inline static POINT (name)(){return {(x), (y)};}

//Prints an error by calling GetLastError().
inline static void PrintError(wchar_t *wFuncName)
{
	DWORD err = GetLastError();
	wchar_t wErrorText[MAX_PATH];
	::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, err, 0, wErrorText, MAX_PATH, 0);
	printf("%S(): Error %d - '%S'\n", wFuncName, err, wErrorText);
}

//Prints the error specified by @error. If @error is set to ERROR_SUCCESS, it will be populated GetLastError().
#define PRINT_ERROR(error)\
{\
	DWORD err = (error);\
	if (err == ERROR_SUCCESS)\
		err = GetLastError();\
	wchar_t wErrorText[MAX_PATH];\
	::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, err, 0, wErrorText, MAX_PATH, 0);\
	printf("%s(): Error %d - '%S'\n", __func__, err, wErrorText);\
}


//Some macros to help make error checking easier
#define FAIL_IF(statement) if((statement)) {printf("%s: failed because the following was true '%s'\n",__func__, STRINGIFY((statement))); PRINT_ERROR(ERROR_SUCCESS); return false;}
#define CONTINUE_IF(statement) if((statement)) {printf("%s: failed because the following was false '%s'\n",__func__, STRINGIFY((statement))); PRINT_ERROR(ERROR_SUCCESS); return false;}
#define DIE_IF(statement) if((statement)) {printf("%s: dying because the following was true '%s'\n",__func__, STRINGIFY((statement))); _exit(0);}
#define DIE_IF_NOT(statement) if((statement) == false) {printf("%s: dying because the following was true '%s'\n",__func__, STRINGIFY((statement))); _exit(0);}
#define DO_OR_NULL(hr, msg) if(!SUCCEEDED((hr))) {printf("%s");return NULL;}