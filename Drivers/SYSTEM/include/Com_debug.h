#ifndef __COM_DEBUG__
#define __COM_DEBUG__

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"


#define DEBUG_ENABLE


#ifdef DEBUG_ENABLE

#define _FILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define FILE_NAME (strrchr(_FILE_NAME, '\\') ? strrchr(_FILE_NAME, '\\') + 1 : _FILE_NAME)

#define debug_init() Debug_Init()
#define debug_printf(format, ...) printf("[%s:%d]" format, FILE_NAME, __LINE__, ##__VA_ARGS__)
#define debug_printfln(format, ...) printf("[%s:%d]" format "\r\n", FILE_NAME, __LINE__, ##__VA_ARGS__)


#else
#define debug_init()
#define debug_printf(...)
#define debug_printfln(format, ...)
#endif

void Debug_Init(void);

#endif // __COM_DEBUG__
