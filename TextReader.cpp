#include "stdafx.h"
#include "TextReader.h"

extern std::string gsxTextPath;

TextReader::TextReader()
{
}


TextReader::~TextReader()
{
}

std::string TextReader::GetTextItem()
{
	bool _newdata;
	_newdata = UpdateTextItems();

	if (_newdata)
	{
		return GsxTextText;
	}
	else
	{
		return "";
	}


}

bool TextReader::UpdateTextItems()
{
	static boost::filesystem::path p = gsxTextPath;
	static bool _fileFirstCheck = TRUE;
	static bool _fileDeleteOnStart = TRUE;
	static bool _fileChanged = FALSE;

	static std::time_t t_last;
	static std::time_t t_new;

	if (boost::filesystem::exists(p))
	{
		if (_fileDeleteOnStart)
		{
			_fileDeleteOnStart = FALSE;
			// don't delete when testing. commenting next row

			// boost::filesystem::remove(p);
		}
		else
		{
			if (_fileFirstCheck)
			{
				t_last = t_new = boost::filesystem::last_write_time(p);
				_fileChanged = TRUE;
				_fileFirstCheck = FALSE;
				ReadFile();
			}
			else
			{
				t_new = boost::filesystem::last_write_time(p);

				if (t_new != t_last)
				{
					t_last = t_new;
					_fileChanged = TRUE;
					ReadFile();

				}
				else
				{
					_fileChanged = FALSE;
				}
			}
		}
	}
	else
	{
		_fileChanged = FALSE;
	}

	return _fileChanged;
}

void TextReader::ReadFile()
{
	static boost::filesystem::path p = gsxTextPath;

	boost::filesystem::ifstream filePlan(p);

	if (filePlan)
	{
		GsxTextText = "";

		getline(filePlan, GsxTextText);
	}

	filePlan.close();

}
