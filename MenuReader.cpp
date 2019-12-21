#include "stdafx.h"
#include "MenuReader.h"
#include "gsxStruct.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <sstream>

extern std::string gsxMenuPath;

MenuReader::MenuReader()
{
}


MenuReader::~MenuReader()
{
}

bool MenuReader::GetMenuItems(gsxlinesStruct  *_gsxInternal)
{
	bool _newdata;
	_newdata = UpdateMenuItems();

	if (_newdata)
	{
		_gsxInternal->txtMenuSubject = gsxLines.txtMenuSubject;
		_gsxInternal->txtMenuItem1 = gsxLines.txtMenuItem1;
		_gsxInternal->txtMenuItem2 = gsxLines.txtMenuItem2;
		_gsxInternal->txtMenuItem3 = gsxLines.txtMenuItem3;
		_gsxInternal->txtMenuItem4 = gsxLines.txtMenuItem4;
		_gsxInternal->txtMenuItem5 = gsxLines.txtMenuItem5;
		_gsxInternal->txtMenuItem6 = gsxLines.txtMenuItem6;
		_gsxInternal->txtMenuItem7 = gsxLines.txtMenuItem7;
		_gsxInternal->txtMenuItem8 = gsxLines.txtMenuItem8;
		_gsxInternal->txtMenuItem9 = gsxLines.txtMenuItem9;
		_gsxInternal->txtMenuItem10 = gsxLines.txtMenuItem10;
	}
	else
	{
		_gsxInternal->txtMenuSubject = "";
		_gsxInternal->txtMenuItem1 = "";
		_gsxInternal->txtMenuItem2 = "";
		_gsxInternal->txtMenuItem3 = "";
		_gsxInternal->txtMenuItem4 = "";
		_gsxInternal->txtMenuItem5 = "";
		_gsxInternal->txtMenuItem6 = "";
		_gsxInternal->txtMenuItem7 = "";
		_gsxInternal->txtMenuItem8 = "";
		_gsxInternal->txtMenuItem9 = "";
		_gsxInternal->txtMenuItem10 = "";
	}
	return _newdata;
}

bool MenuReader::UpdateMenuItems()
{
	static boost::filesystem::path p = gsxMenuPath;
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
			//boost::filesystem::remove(p);
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

void MenuReader::ReadFile()
{
	static boost::filesystem::path p = gsxMenuPath;

	boost::filesystem::ifstream filePlan(p);

	if (filePlan)
	{
		gsxLines.txtMenuSubject = "";
		gsxLines.txtMenuItem1 = "";
		gsxLines.txtMenuItem2 = "";
		gsxLines.txtMenuItem3 = "";
		gsxLines.txtMenuItem4 = "";
		gsxLines.txtMenuItem5 = "";
		gsxLines.txtMenuItem6 = "";
		gsxLines.txtMenuItem7 = "";
		gsxLines.txtMenuItem8 = "";
		gsxLines.txtMenuItem9 = "";
		gsxLines.txtMenuItem10 = "";

		getline(filePlan, gsxLines.txtMenuSubject);
		getline(filePlan, gsxLines.txtMenuItem1);
		getline(filePlan, gsxLines.txtMenuItem2);
		getline(filePlan, gsxLines.txtMenuItem3);
		getline(filePlan, gsxLines.txtMenuItem4);
		getline(filePlan, gsxLines.txtMenuItem5);
		getline(filePlan, gsxLines.txtMenuItem6);
		getline(filePlan, gsxLines.txtMenuItem7);
		getline(filePlan, gsxLines.txtMenuItem8);
		getline(filePlan, gsxLines.txtMenuItem9);
		getline(filePlan, gsxLines.txtMenuItem10);
	}

	filePlan.close();

}