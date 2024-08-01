#pragma once
#include <stdint.h>
#include "SimpleIni.h"
#include "logging.h"

// =========  WHAT YOU NEED TO KNOW ABOUT CONFIG  =========
//
// the config class should work for whatever you need
// after creating the object make sure to call 
// initConfig with the name of the config you would like
// to open. if the given config doesnt exist one will
// be created with that name. Read and write your objects
// with ReadData and WriteData, they both are templates
// so specify your own types (and please dont do dumb things with this).
//
// 7/31/2024
// system information will also be stored in a class here for easy access.

class config {
	CSimpleIniA iniFile;
	SI_Error rc;

	bool initConfig(const char* confName);
};

static const unsigned int confWidth = 1680;
static const unsigned int confHeight = 1050;