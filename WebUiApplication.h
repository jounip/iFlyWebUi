#pragma once

#include "WebUiServer.h"

class ChatApplication : public Wt::WApplication
{
public:
    /*! \brief Create a new instance.
     */
    ChatApplication(const Wt::WEnvironment& env, WebUiServer& server);

private:
    WebUiServer& server_;
    Wt::WText* javaScriptError_;
    const Wt::WEnvironment& env_;
    std::unique_ptr<Wt::WTimer>   timer_;

    /*! \brief Add another chat client.
     */
    void javaScriptTest();
    void emptyFunc();
};
