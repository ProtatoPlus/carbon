#include "Windows.h"
#include <iostream>
#include <format>
#include <string>
#include "logging.h"
bool StartLog() {
	if (!isLogging) {
		start = time(0);
		if (AllocConsole()) {
			isLogging = true;
			return true;
		}
		printf("Error initializing log: %lu", GetLastError());
		return false;
	}
}
void logMessage(type logType, const char* message) {
	char buffer[500];
	sprintf_s(buffer, "%s :: %1f || %s", typeStrs[logType], difftime(time(0), start), message);
	std::cout << buffer << std::endl;
}
void logError(severity severityLevel, const char* message) {
	char buffer[500];
	sprintf_s(buffer, "ERROR :: %s :: %1f || %s", severityStrs[severityLevel], difftime(time(0), start), message);
	std::cerr << buffer << std::endl;
}