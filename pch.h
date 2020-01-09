// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WEnvironment.h>
#include <Wt/WServer.h>
#include <Wt/WTimer.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WSignal.h>
#include <Wt/WString.h>
#include <Wt/WJavaScript.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WFitLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>
#include <Wt/WPushButton.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WMenu.h>
#include <Wt/WGroupBox.h>
#include <Wt/WGoogleMap.h>
#include <Wt/WDialog.h>
#include <Wt/WLabel.h>
#include <Wt/WTable.h>


#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

#include <chrono>
#include <sstream>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#include "gsxStruct.h"
#include "SimulatorDataStruct.h"

#include "inih/INIReader.h"


#endif //PCH_H
