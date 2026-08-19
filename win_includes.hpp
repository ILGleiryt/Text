#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOMINMAX
#define NOGDI
#define NOSOUND
#define NOSERVICE
#define NONLS
#define NOCOMM
#define STRICT

#include <Windows.h> // winapi doesnt properly work as header unit
#include <windowsx.h> // macro defines doesnt work with min/max/no[defines]

constexpr int DEFAULT_DPI = USER_DEFAULT_SCREEN_DPI;