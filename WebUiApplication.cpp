#include "pch.h"
#include "WebUiApplication.h"

#include "WebUiServer.h"
#include "WebUiWidget.h"



ChatApplication::ChatApplication(const Wt::WEnvironment& env,
    WebUiServer& server)
    : WApplication(env),
    server_(server),
    env_(env)
{
    setTitle("iFlyWebUi 4.0");
    useStyleSheet("iflyapp.css");

    auto theme = std::make_shared<Wt::WBootstrapTheme>();
    theme->setVersion(Wt::BootstrapVersion::v3);
    setTheme(theme);

    javaScriptTest();

    auto vLayout = root()->setLayout(Wt::cpp14::make_unique<Wt::WFitLayout>());

    WebUiWidget* webUiWidget =
        vLayout->addWidget(Wt::cpp14::make_unique<WebUiWidget>(server_));
}

void ChatApplication::javaScriptTest()
{
    if (!env_.javaScript()) {
        javaScriptError_ =
            root()->addWidget(Wt::cpp14::make_unique<Wt::WText>("JavaScript not found..."));

        // The 5 second timer is a fallback for real server push. The updated
        // server state will piggy back on the response to this timeout.
        timer_ = Wt::cpp14::make_unique<Wt::WTimer>();
        timer_->setInterval(std::chrono::milliseconds{ 5000 });
        timer_->timeout().connect(this, &ChatApplication::emptyFunc);
        timer_->start();
    }
}

void ChatApplication::emptyFunc()
{}

