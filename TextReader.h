#pragma once

#include <string>
#include <sstream>

class TextReader
{
public:
	TextReader();
	~TextReader();

	bool GetTextItem(gsxlinesStruct*);

private:
	bool UpdateTextItems();
	void ReadFile();
	gsxlinesStruct gsxLines;

};

