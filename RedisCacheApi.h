#pragma once

#ifdef REDISCACHE_EXPORTS
#define REDISCACHE_API __declspec(dllexport)
#else
#define REDISCACHE_API __declspec(dllimport)
#endif