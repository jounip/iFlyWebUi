#include "stdafx.h"
#include "WebApplication.h"

#include "SimpleWebServer.h"
#include "SimpleWebWidget.h"

#include <Wt/WEnvironment>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WBootstrapTheme>
#include <Wt/WApplication>

#include <Wt/WFitLayout>
#include <Wt/WVBoxLayout>

#include <Wt/WTimer>

using namespace Wt;

WebApplication::WebApplication(const WEnvironment& env,
	SimpleWebServer& server)
	: WApplication(env),
	server_(server),
	env_(env)
{

	setTitle("iFly 737 Web UI");

	setTheme(new WBootstrapTheme());

	useStyleSheet("iflyapp.css");

	javaScriptTest();

	Wt::WFitLayout* webAppLayout = new Wt::WFitLayout();
	root()->setLayout(webAppLayout);

	Wt::WContainerWidget *webAppContainer = new Wt::WContainerWidget();
	webAppLayout->addWidget(webAppContainer);

	Wt::WVBoxLayout *webVLayout = new WVBoxLayout();
	webAppContainer->setLayout(webVLayout);

	SimpleWebWidget *webWidget =
		new SimpleWebWidget(server_);

	webVLayout->addWidget(webWidget,1);
}

void WebApplication::javaScriptTest()
{
	if (!env_.javaScript()){
		//javaScriptError_ = new WText(WString::tr("serverpushwarning"), root());

		// The 5 second timer is a fallback for real server push. The updated
		// server state will piggy back on the response to this timeout.
		timer_ = new Wt::WTimer(root());
		timer_->setInterval(5000);
		timer_->timeout().connect(this, &WebApplication::emptyFunc);
		timer_->start();
	}
}

void WebApplication::emptyFunc()
{}



