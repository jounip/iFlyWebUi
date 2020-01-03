#pragma once

#include <string>
#include <sstream>

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

