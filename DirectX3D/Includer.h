#ifndef INCLUDER_H
#define INCLUDER_H

//targeting windows 7 or higher version
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>

//redefining macros (Not include those from Windows.h) turning off switches
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOMINMAX
#define NOWH
#define STRICT

#include <Windows.h>
#include <string>
#endif // !INCLUDER_H

