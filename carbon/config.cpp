#include "config.h"
class config {
	CSimpleIniA iniFile;
	SI_Error rc;
	bool initConfig(const char* confName) {
		iniFile.SetUnicode();
		rc = iniFile.LoadFile(confName);
		if (rc < 0) {
			logError(HIGH, "Failed to initialize config file.");
			return false;
		};
	}
};