#pragma once

#include <string>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

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

