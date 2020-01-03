#pragma once

#include <string>
#include <sstream>

class TextReader
{
public:
	TextReader();
	~TextReader();
	std::string GetTextItem();

private:
	bool UpdateTextItems();
	void ReadFile();
	std::string GsxTextText;
};

