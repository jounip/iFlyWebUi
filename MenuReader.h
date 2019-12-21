#pragma once

#include <string>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "gsxStruct.h"

class MenuReader
{
public:
	MenuReader();
	~MenuReader();
	bool GetMenuItems(gsxlinesStruct *);

private:
	bool UpdateMenuItems();
	void ReadFile();
	gsxlinesStruct gsxLines;
};

