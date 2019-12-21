#pragma once

#include "SimpleWebServer.h"

using namespace Wt;

class WebApplication : public WApplication
{
public:
	WebApplication(const WEnvironment& env, SimpleWebServer& server);

private:
	SimpleWebServer& server_;
	Wt::WText *javaScriptError_;
	const WEnvironment& env_;
	Wt::WTimer *timer_;

	/*! \brief Add another chat client.
	*/
	void javaScriptTest();
	void emptyFunc();
};