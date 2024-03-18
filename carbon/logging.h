#pragma once
#include <time.h>
static bool isLogging;
static time_t start;
enum type{
	INFO,
	WARNING
};
static const char* typeStrs[] = {"I", "W"};
enum severity {
	LOW,
	HIGH,
	CRITICAL
};
static size_t strLen;
static const char* severityStrs[] = {"L", "H", "C"};
bool StartLog();
void logMessage(type logType, const char* message);
void logError(severity severityLevel, const char* message);