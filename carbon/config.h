#pragma once
#include <stdint.h>
// this is a really big fucking hack, im gonna rewrite this later
// edit: That time is now!


// =========  WHAT YOU NEED TO KNOW ABOUT CONFIG  =========
//
// the config class should work for whatever you need
// after creating the object make sure to call 
// initConfig with the name of the config you would like
// to open. if the given config doesnt exist one will
// be created with that name. Read and write your objects
// with ReadData and WriteData, they both are templates
// so specify your own types (and please dont do dumb things with this).

class config {
	bool initConfig(const char[]);
	template<typename T>
	T ReadData(const char[]);
	template<typename T>
	T WriteData(const char[]);
};

static const unsigned int confWidth = 1680;
static const unsigned int confHeight = 1050;