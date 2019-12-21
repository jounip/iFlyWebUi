/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "pch.h"
#include "WebUiWidget.h"

#include "WebUiServer.h"


WebUiWidget::WebUiWidget(WebUiServer& server)
  : WContainerWidget(),
    server_(server)
{

    // Create Containers for different pages/tabs
    auto groundContainer = Wt::cpp14::make_unique<Wt::WContainerWidget>();
    auto weatherContainer = Wt::cpp14::make_unique<Wt::WContainerWidget>();
    auto infoContainer = Wt::cpp14::make_unique<Wt::WContainerWidget>();
    auto GSXContainer = Wt::cpp14::make_unique<Wt::WContainerWidget>();


    // Setup needed buttons for ground tab
    auto btnGroundPower = Wt::cpp14::make_unique<Wt::WPushButton>("Ground Power\nconnect/disconnect");
    auto btnGroundAir = Wt::cpp14::make_unique<Wt::WPushButton>("Ground Air\n connect/disconnect");
    auto btnDoor1 = Wt::cpp14::make_unique<Wt::WPushButton>("Open/Close\n Door 1");
    auto btnDoor2 = Wt::cpp14::make_unique<Wt::WPushButton>("Open/Close\n Door 2");
    auto btnDoor3 = Wt::cpp14::make_unique<Wt::WPushButton>("Open/Close\n Door 3");
    auto btnPushBack1 = Wt::cpp14::make_unique<Wt::WPushButton>("Start Pushback\nTail Left");
    auto btnPushBack2 = Wt::cpp14::make_unique<Wt::WPushButton>("Start Pushback\nStraight");
    auto btnPushBack3 = Wt::cpp14::make_unique<Wt::WPushButton>("Start Pushback\nTail Right");
    auto btnPushBack4 = Wt::cpp14::make_unique<Wt::WPushButton>("STOP\nPUSHBACK");

    btnGroundPower_ = btnGroundPower.get();
    btnGroundAir_ = btnGroundAir.get();
    btnDoor1_ = btnDoor1.get();
    btnDoor2_ = btnDoor2.get();
    btnDoor3_ = btnDoor3.get();
    btnPushBack1_ = btnPushBack1.get();
    btnPushBack2_ = btnPushBack2.get();
    btnPushBack3_ = btnPushBack3.get();
    btnPushBack4_ = btnPushBack4.get();

    btnGroundPower_->setStyleClass("btn-danger");
    btnGroundAir_->setStyleClass("btn-danger");

    btnDoor1_->setStyleClass("btn-danger");
    btnDoor2_->setStyleClass("btn-danger");
    btnDoor3_->setStyleClass("btn-danger");

    btnPushBack1_->setStyleClass("btn-danger");
    btnPushBack2_->setStyleClass("btn-danger");
    btnPushBack3_->setStyleClass("btn-danger");
    btnPushBack4_->setStyleClass("btn-danger");

    // Setup needed buttons for GSX tab  
    auto btnVirtualKey1 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 1");
    auto btnVirtualKey2 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 2");
    auto btnVirtualKey3 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 3");
    auto btnVirtualKey4 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 4");
    auto btnVirtualKey5 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 5");
    auto btnVirtualKey6 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 6");
    auto btnVirtualKey7 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 7");
    auto btnVirtualKey8 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 8");
    auto btnVirtualKey9 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 9");
    auto btnVirtualKey0 = Wt::cpp14::make_unique<Wt::WPushButton>("Send 0");
    auto btnVirtualKeyF12 = Wt::cpp14::make_unique<Wt::WPushButton>("Send F12");

    // Setup default Weather text
    auto txtWxrMetarDep = Wt::cpp14::make_unique<Wt::WText>("DEPARTURE AIRPORT IS NOT SET");
    auto txtWxrTafDep = Wt::cpp14::make_unique<Wt::WText>("DEPARTURE AIRPORT IS NOT SET");
    auto txtWxrMetarArr = Wt::cpp14::make_unique<Wt::WText>("ARRIVAL AIRPORT IS NOT SET");
    auto txtWxrTafArr = Wt::cpp14::make_unique<Wt::WText>("ARRIVAL AIRPORT IS NOT SET");

    // Setup default GSX text
    auto txtGSXText = Wt::cpp14::make_unique<Wt::WText>("");

    // Setup default statustext
    auto statustext = Wt::cpp14::make_unique<Wt::WText>("");


    // Create Ground tab 
    auto groundBox = groundContainer->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("Ground Equipment"));
    groundBox->addStyleClass("fieldset-header");
    groundBox->addWidget(std::move(btnGroundPower));
    groundBox->addWidget(std::move(btnGroundAir));

    auto doorBox = groundContainer->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("Aircraft Doors"));
    doorBox->addStyleClass("fieldset-header");
    doorBox->addWidget(std::move(btnDoor1));
    doorBox->addWidget(std::move(btnDoor2));
    doorBox->addWidget(std::move(btnDoor3));

    auto pushBox = groundContainer->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("Default Pushback"));
    pushBox->addStyleClass("fieldset-header");
    pushBox->addWidget(std::move(btnPushBack1));
    pushBox->addWidget(std::move(btnPushBack2));
    pushBox->addWidget(std::move(btnPushBack3));
    pushBox->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    pushBox->addWidget(std::move(btnPushBack4));

    auto wxrDepBox = weatherContainer->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("Departure"));
    wxrDepBox->addStyleClass("fieldset-header");
    wxrDepBox->addWidget(std::move(txtWxrMetarDep));
    wxrDepBox->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    wxrDepBox->addWidget(std::move(txtWxrTafDep));

    auto wxrArrBox = weatherContainer->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("Arrival"));
    wxrArrBox->addStyleClass("fieldset-header");
    wxrArrBox->addWidget(std::move(txtWxrMetarArr));
    wxrArrBox->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    wxrArrBox->addWidget(std::move(txtWxrTafArr));

    auto infoBox = infoContainer->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("A group box"));
    infoBox->addStyleClass("fieldset-header");

    auto GSXBox = GSXContainer->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("A group box"));
    GSXBox->addStyleClass("fieldset-header");



    // Create 3 main containers from top to bottom.

    auto navigationContainer = Wt::cpp14::make_unique<Wt::WNavigationBar>();
    auto contentsContainer = Wt::cpp14::make_unique<Wt::WContainerWidget>();
    auto statusContainer = Wt::cpp14::make_unique<Wt::WContainerWidget>();

    // Create a navigation bar with a link to a web page.
    navigationContainer->setTitle("iFlyWebUi4.5",
        "http://jounip.github.io/iFlyWebUi/");
    navigationContainer->setResponsive(true);

    // Setup a Left-aligned menu.
    Wt::WStackedWidget* contentsStack = contentsContainer->addNew<Wt::WStackedWidget>();
    auto leftMenu = Wt::cpp14::make_unique<Wt::WMenu>(contentsStack);
    auto leftMenu_ = navigationContainer->addMenu(std::move(leftMenu));

    leftMenu_->addItem("Ground", std::move(groundContainer));
    leftMenu_->addItem("Weather", std::move(weatherContainer));
    leftMenu_->addItem("Info", std::move(infoContainer));
    leftMenu_->addItem("GSX", std::move(GSXContainer));

    // Set Main layout to VBox that has 3 different areas (navi, content, status)
    auto vbox = setLayout(Wt::cpp14::make_unique<Wt::WVBoxLayout>());
    vbox->addWidget(std::move(navigationContainer));
    vbox->addWidget(std::move(contentsContainer), 1);
    vbox->addWidget(std::move(statustext), 0);

    // Connect to server
    connect();
}

WebUiWidget::~WebUiWidget()
{
    disconnect();
}

void WebUiWidget::connect()
{
  if (server_.connect
      (this, std::bind(&WebUiWidget::processWebUiEvent, this, std::placeholders::_1)))
    Wt::WApplication::instance()->enableUpdates(true);
}

void WebUiWidget::disconnect()
{
  if (server_.disconnect(this))
    Wt::WApplication::instance()->enableUpdates(false);
}

void WebUiWidget::processWebUiEvent(const WebUiEvent& event)
{
  Wt::WApplication *app = Wt::WApplication::instance();

  /*
   * This is where the "server-push" happens. The chat server posts to this
   * event from other sessions, see WebUiServer::postWebUiEvent()
   */

  /*
   * This is the server call: we (schedule to) propagate the updated UI to
   * the client.
   *
   * This schedules an update and returns immediately
   */
  app->triggerUpdate();

}
