/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "pch.h"
#include "Resource.h"

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

    btnVirtualKey1_ = btnVirtualKey1.get();
    btnVirtualKey2_ = btnVirtualKey2.get();
    btnVirtualKey3_ = btnVirtualKey3.get();
    btnVirtualKey4_ = btnVirtualKey4.get();
    btnVirtualKey5_ = btnVirtualKey5.get();
    btnVirtualKey6_ = btnVirtualKey6.get();
    btnVirtualKey7_ = btnVirtualKey7.get();
    btnVirtualKey8_ = btnVirtualKey8.get();
    btnVirtualKey9_ = btnVirtualKey9.get();
    btnVirtualKey0_ = btnVirtualKey0.get();
    btnVirtualKeyF12_ = btnVirtualKeyF12.get();

    // Setup default Weather text
    auto txtWxrMetarDep = Wt::cpp14::make_unique<Wt::WText>("DEPARTURE AIRPORT IS NOT SET");
    auto txtWxrTafDep = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtWxrMetarArr = Wt::cpp14::make_unique<Wt::WText>("ARRIVAL AIRPORT IS NOT SET");
    auto txtWxrTafArr = Wt::cpp14::make_unique<Wt::WText>("");

    // Setup default info text

    auto txtAlt = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtSpeed = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtTemperature = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtOnGround = Wt::cpp14::make_unique<Wt::WText>("");

    auto txtOut = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtOff = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtOn = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtIn = Wt::cpp14::make_unique<Wt::WText>("");

    auto txtDestinationETA = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtDestinationDTG = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtDestinationFuel = Wt::cpp14::make_unique<Wt::WText>("");

    auto txtLastName = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtLastAlt = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtLastATA = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtLastFuel = Wt::cpp14::make_unique<Wt::WText>("");

    auto txtActualName = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtActualETA = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtActualFuel = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtActualDTG = Wt::cpp14::make_unique<Wt::WText>("");

    auto txtNextName = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtNextETA = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtNextFuel = Wt::cpp14::make_unique<Wt::WText>("");
    auto txtNextDTG = Wt::cpp14::make_unique<Wt::WText>("");

    txtWxrMetarDep_ = txtWxrMetarDep.get();
    txtWxrTafDep_ = txtWxrTafDep.get();
    txtWxrMetarArr_ = txtWxrMetarArr.get();
    txtWxrTafArr_ = txtWxrTafArr.get();

    txtAlt_ = txtAlt.get();
    txtSpeed_ = txtSpeed.get();
    txtTemperature_ = txtTemperature.get();
    txtOnGround_ = txtOnGround.get();

    txtOut_ = txtOut.get();
    txtOff_ = txtOff.get();
    txtOn_ = txtOn.get();
    txtIn_ = txtIn.get();

    txtDestinationETA_ = txtDestinationETA.get();
    txtDestinationDTG_ = txtDestinationDTG.get();
    txtDestinationFuel_ = txtDestinationFuel.get();

    txtLastName_ = txtLastName.get();
    txtLastAlt_ = txtLastAlt.get();
    txtLastATA_ = txtLastATA.get();
    txtLastFuel_ = txtLastFuel.get();

    txtActualName_ = txtActualName.get();
    txtActualETA_ = txtActualETA.get();
    txtActualFuel_ = txtActualFuel.get();
    txtActualDTG_ = txtActualDTG.get();

    txtNextName_ = txtNextName.get();
    txtNextETA_ = txtNextETA.get();
    txtNextFuel_ = txtNextFuel.get();
    txtNextDTG_ = txtNextDTG.get();

    // Setup default GSX text
    auto txtGSXText = Wt::cpp14::make_unique<Wt::WText>("GSX not found.");
    txtGSXText_ = txtGSXText.get();


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

    // Info tab starts here

    auto vinfobox = infoContainer->setLayout(Wt::cpp14::make_unique<Wt::WVBoxLayout>());

    auto hinforow1 = vinfobox->addLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());
    auto hinforow2 = vinfobox->addLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

    // Basic flightinfo

    auto infoBox = Wt::cpp14::make_unique<Wt::WGroupBox>(" (  -  )");
    infoBox_ = infoBox.get();

    hinforow1->addWidget(std::move(infoBox));

    infoBox_->addStyleClass("fieldset-header");

    auto flightinfocontainer = infoBox_->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    auto flightinfocontainerLayout = flightinfocontainer->setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

    auto flightinfocontainer1 = flightinfocontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto flightinfocontainer2 = flightinfocontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto flightinfocontainer3 = flightinfocontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto flightinfocontainer4 = flightinfocontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());


    flightinfocontainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("Alt:"));
    flightinfocontainer1->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    flightinfocontainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("Speed:"));
    flightinfocontainer2->addWidget(std::move(txtAlt));
    flightinfocontainer2->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    flightinfocontainer2->addWidget(std::move(txtSpeed));
    flightinfocontainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("Temp:"));
    flightinfocontainer3->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    flightinfocontainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("On ground:"));
    flightinfocontainer4->addWidget(std::move(txtTemperature));
    flightinfocontainer4->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    flightinfocontainer4->addWidget(std::move(txtOnGround));

    // OOOI

    auto oooiBox = hinforow1->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("OOOI"));
    oooiBox->addStyleClass("fieldset-header");

    auto oooicontainer = oooiBox->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    auto oooicontainerLayout = oooicontainer->setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

    auto oooicontainer1 = oooicontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto oooicontainer2 = oooicontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto oooicontainer3 = oooicontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto oooicontainer4 = oooicontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    oooicontainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("Out:"));
    oooicontainer1->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    oooicontainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("Off:"));
    oooicontainer2->addWidget(std::move(txtOut));
    oooicontainer2->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    oooicontainer2->addWidget(std::move(txtOff));
    oooicontainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("On:"));
    oooicontainer3->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    oooicontainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("In:"));
    oooicontainer4->addWidget(std::move(txtOn));
    oooicontainer4->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    oooicontainer4->addWidget(std::move(txtIn));

    // Destination

    auto destinationBox = hinforow1->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("Destination"));
    destinationBox->addStyleClass("fieldset-header");

    auto destinationcontainer = destinationBox->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    auto destinationcontainerLayout = destinationcontainer->setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

    auto destinationcontainer1 = destinationcontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto destinationcontainer2 = destinationcontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto destinationcontainer3 = destinationcontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto destinationcontainer4 = destinationcontainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());


    destinationcontainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("ETA:"));
    destinationcontainer1->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    destinationcontainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("DTG:"));
    destinationcontainer2->addWidget(std::move(txtDestinationETA));
    destinationcontainer2->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    destinationcontainer2->addWidget(std::move(txtDestinationDTG));
    destinationcontainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("Fuel:"));
    destinationcontainer4->addWidget(std::move(txtDestinationFuel));

    // last waypoint

    auto progressLastBox = hinforow2->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("Last WPT"));
    progressLastBox->addStyleClass("fieldset-header");

    auto lastWptContainer = progressLastBox->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    auto lastWptContainerLayout = lastWptContainer->setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

    auto lastWptContainer1 = lastWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto lastWptContainer2 = lastWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto lastWptContainer3 = lastWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto lastWptContainer4 = lastWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    lastWptContainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("Name:"));
    lastWptContainer1->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    lastWptContainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("ATA:"));
    lastWptContainer2->addWidget(std::move(txtLastName));
    lastWptContainer2->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    lastWptContainer2->addWidget(std::move(txtLastATA));
    lastWptContainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("Fuel:"));
    lastWptContainer3->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    lastWptContainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("Alt:"));
    lastWptContainer4->addWidget(std::move(txtLastFuel));
    lastWptContainer4->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    lastWptContainer4->addWidget(std::move(txtLastAlt));

    // Actual waypoint

    auto progressActBox = hinforow2->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("Actual WPT"));
    progressActBox->addStyleClass("fieldset-header");

    auto actualWptContainer = progressActBox->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    auto actualWptContainerLayout = actualWptContainer->setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

    auto actualWptContainer1 = actualWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto actualWptContainer2 = actualWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto actualWptContainer3 = actualWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto actualWptContainer4 = actualWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    actualWptContainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("Name:"));
    actualWptContainer1->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    actualWptContainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("ETA:"));
    actualWptContainer2->addWidget(std::move(txtActualName));
    actualWptContainer2->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    actualWptContainer2->addWidget(std::move(txtActualETA));
    actualWptContainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("Fuel:"));
    actualWptContainer3->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    actualWptContainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("DTG:"));
    actualWptContainer4->addWidget(std::move(txtActualFuel));
    actualWptContainer4->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    actualWptContainer4->addWidget(std::move(txtActualDTG));

    // Next waypoint

    auto progressNextBox = hinforow2->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("Next WPT"));
    progressNextBox->addStyleClass("fieldset-header");

    auto nextWptContainer = progressNextBox->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    auto nextWptContainerLayout = nextWptContainer->setLayout(Wt::cpp14::make_unique<Wt::WHBoxLayout>());

    auto nextWptContainer1 = nextWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto nextWptContainer2 = nextWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto nextWptContainer3 = nextWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());
    auto nextWptContainer4 = nextWptContainerLayout->addWidget(Wt::cpp14::make_unique<Wt::WContainerWidget>());

    nextWptContainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("Name:"));
    nextWptContainer1->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    nextWptContainer1->addWidget(Wt::cpp14::make_unique<Wt::WText>("ETA:"));
    nextWptContainer2->addWidget(std::move(txtNextName));
    nextWptContainer2->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    nextWptContainer2->addWidget(std::move(txtNextETA));
    nextWptContainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("Fuel:"));
    nextWptContainer3->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    nextWptContainer3->addWidget(Wt::cpp14::make_unique<Wt::WText>("DTG:"));
    nextWptContainer4->addWidget(std::move(txtNextFuel));
    nextWptContainer4->addWidget(Wt::cpp14::make_unique<Wt::WBreak>());
    nextWptContainer4->addWidget(std::move(txtNextDTG));

    // GSX Tab starts here

    auto GSXBox = GSXContainer->addWidget(Wt::cpp14::make_unique<Wt::WGroupBox>("GSX Menu"));
    GSXBox->addStyleClass("fieldset-header");

    auto gsxTable = GSXBox->addWidget(Wt::cpp14::make_unique<Wt::WTable>());
    gsxTable->setWidth(Wt::WLength("100%"));

    gsxTable->elementAt(0, 0)->addWidget(std::move(btnVirtualKey1));
    gsxTable->elementAt(0, 1)->addWidget(std::move(btnVirtualKey2));
    gsxTable->elementAt(0, 2)->addWidget(std::move(btnVirtualKey3));
    gsxTable->elementAt(1, 0)->addWidget(std::move(btnVirtualKey4));
    gsxTable->elementAt(1, 1)->addWidget(std::move(btnVirtualKey5));
    gsxTable->elementAt(1, 2)->addWidget(std::move(btnVirtualKey6));
    gsxTable->elementAt(2, 0)->addWidget(std::move(btnVirtualKey7));
    gsxTable->elementAt(2, 1)->addWidget(std::move(btnVirtualKey8));
    gsxTable->elementAt(2, 2)->addWidget(std::move(btnVirtualKey9));
    gsxTable->elementAt(3, 0)->addWidget(std::move(btnVirtualKey0));
    gsxTable->elementAt(3, 2)->addWidget(std::move(btnVirtualKeyF12));


    // Create 3 main containers from top to bottom.

    auto navigationContainer = Wt::cpp14::make_unique<Wt::WNavigationBar>();
    auto contentsContainer = Wt::cpp14::make_unique<Wt::WContainerWidget>();
    auto statusContainer = Wt::cpp14::make_unique<Wt::WContainerWidget>();

    // Create a navigation bar with a link to a web page.
    navigationContainer->setTitle("iFlyWebUi4.0",
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

    // status row starts here

    statusContainer->addWidget(std::move(txtGSXText));

    // Set Main layout to VBox that has 3 different areas (navi, content, status)
    auto vbox = setLayout(Wt::cpp14::make_unique<Wt::WVBoxLayout>());
    vbox->addWidget(std::move(navigationContainer));
    vbox->addWidget(std::move(contentsContainer), 1);
    vbox->addWidget(std::move(statusContainer), 0);

    // Setup a 60 seconds timer

    auto timer = addChild(std::make_unique<Wt::WTimer>());
    timer->setInterval(std::chrono::seconds(60));
    timer->timeout().connect(this, &WebUiWidget::timerTimeout);
    timer->start();

    // Bind the C++ and JavaScript event handlers.
    btnGroundPower_->clicked().connect(this, &WebUiWidget::pressedGroundPower);
    btnGroundAir_->clicked().connect(this, &WebUiWidget::pressedGroundAir);

    btnDoor1_->clicked().connect(this, &WebUiWidget::pressedButtonDoor1);
    btnDoor2_->clicked().connect(this, &WebUiWidget::pressedButtonDoor2);
    btnDoor3_->clicked().connect(this, &WebUiWidget::pressedButtonDoor3);

    btnPushBack1_->clicked().connect(this, &WebUiWidget::pressedPushBack1);
    btnPushBack2_->clicked().connect(this, &WebUiWidget::pressedPushBack2);
    btnPushBack3_->clicked().connect(this, &WebUiWidget::pressedPushBack3);
    btnPushBack4_->clicked().connect(this, &WebUiWidget::pressedPushBack4);

    btnVirtualKey1_->clicked().connect(this, &WebUiWidget::pressedVirtualKey1);
    btnVirtualKey2_->clicked().connect(this, &WebUiWidget::pressedVirtualKey2);
    btnVirtualKey3_->clicked().connect(this, &WebUiWidget::pressedVirtualKey3);
    btnVirtualKey4_->clicked().connect(this, &WebUiWidget::pressedVirtualKey4);
    btnVirtualKey5_->clicked().connect(this, &WebUiWidget::pressedVirtualKey5);
    btnVirtualKey6_->clicked().connect(this, &WebUiWidget::pressedVirtualKey6);
    btnVirtualKey7_->clicked().connect(this, &WebUiWidget::pressedVirtualKey7);
    btnVirtualKey8_->clicked().connect(this, &WebUiWidget::pressedVirtualKey8);
    btnVirtualKey9_->clicked().connect(this, &WebUiWidget::pressedVirtualKey9);
    btnVirtualKey0_->clicked().connect(this, &WebUiWidget::pressedVirtualKey0);
    btnVirtualKeyF12_->clicked().connect(this, &WebUiWidget::pressedVirtualKeyF12);


    // Connect to server
    connect();
}

void WebUiWidget::timerTimeout()
{
    server_.sendButtonPress(ID_WEBUI_TIMER_TIMEOUT);
}
void WebUiWidget::pressedGroundPower()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_GROUND_POWER);
}

void WebUiWidget::pressedGroundAir()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_GROUND_AIR);
}

void WebUiWidget::pressedButtonDoor1()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_DOOR1);
}

void WebUiWidget::pressedButtonDoor2()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_DOOR2);
}

void WebUiWidget::pressedButtonDoor3()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_DOOR3);
}

void WebUiWidget::pressedPushBack1()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_PUSHBACK1);
}

void WebUiWidget::pressedPushBack2()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_PUSHBACK2);
}

void WebUiWidget::pressedPushBack3()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_PUSHBACK3);
}

void WebUiWidget::pressedPushBack4()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_PUSHBACK4);
}

void WebUiWidget::pressedVirtualKey0()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_0);
}

void WebUiWidget::pressedVirtualKey1()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_1);
}

void WebUiWidget::pressedVirtualKey2()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_2);
}

void WebUiWidget::pressedVirtualKey3()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_3);
}

void WebUiWidget::pressedVirtualKey4()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_4);
}

void WebUiWidget::pressedVirtualKey5()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_5);
}

void WebUiWidget::pressedVirtualKey6()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_6);
}

void WebUiWidget::pressedVirtualKey7()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_7);
}

void WebUiWidget::pressedVirtualKey8()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_8);
}

void WebUiWidget::pressedVirtualKey9()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_9);
}

void WebUiWidget::pressedVirtualKeyF12()
{
    server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_F12);
}

void WebUiWidget::hideVirtualButtons()
{
    gsxBox_->setTitle("GSX Menu");
    btnVirtualKey1_->hide();
    btnVirtualKey2_->hide();
    btnVirtualKey3_->hide();
    btnVirtualKey4_->hide();
    btnVirtualKey5_->hide();
    btnVirtualKey6_->hide();
    btnVirtualKey7_->hide();
    btnVirtualKey8_->hide();
    btnVirtualKey9_->hide();
    btnVirtualKey0_->hide();
}

WebUiWidget::~WebUiWidget()
{
    disconnect();
}

void WebUiWidget::connect()
{
  if (server_.connectClient
      (this, std::bind(&WebUiWidget::processWebUiEvent, this, std::placeholders::_1)))
    Wt::WApplication::instance()->enableUpdates(true);
}

void WebUiWidget::disconnect()
{
  if (server_.disconnectClient(this))
    Wt::WApplication::instance()->enableUpdates(false);
}

void WebUiWidget::processWebUiEvent(const WebUiEvent& event)
{
  Wt::WApplication *app = Wt::WApplication::instance();

  app->triggerUpdate();

  if (event.type() == WebUiEvent::LatLon)
  {
      double tempLat;
      double tempLon;

      tempLat = event.iflydoublevalue();
      tempLon = event.iflydoublevalue2();

      //map->clearOverlays();
      //map->setCenter(Wt::WGoogleMap::Coordinate(tempLat, tempLon));
      //map->addMarker(Wt::WGoogleMap::Coordinate(tempLat, tempLon));

  }

  if (event.type() == WebUiEvent::LastWPT)
  {
      txtLastName_->setText(event.wttext());
      txtLastAlt_->setText(boost::lexical_cast<std::string>(event.iflyvalue()));
      txtLastATA_->setText(event.wttext2());
      txtLastFuel_->setText(boost::lexical_cast<std::string>(event.iflyvalue3()));
  }

  if (event.type() == WebUiEvent::ActualWPT)
  {
      txtActualName_->setText(event.wttext());
      txtActualETA_->setText(event.wttext2());
      txtActualFuel_->setText(boost::lexical_cast<std::string>(event.iflyvalue2()));
      std::stringstream ss;
      ss << std::fixed << std::setprecision(2) << event.iflydoublevalue();
      txtActualDTG_->setText(ss.str());
  }

  if (event.type() == WebUiEvent::NextWPT)
  {
      txtNextName_->setText(event.wttext());
      txtNextETA_->setText(event.wttext2());
      txtNextFuel_->setText(boost::lexical_cast<std::string>(event.iflyvalue2()));

      std::stringstream ss;
      ss << std::fixed << std::setprecision(2) << event.iflydoublevalue();
      txtNextDTG_->setText(ss.str());
  }

  if (event.type() == WebUiEvent::Destination)
  {
      txtDestinationETA_->setText(event.wttext());
      txtDestinationFuel_->setText(boost::lexical_cast<std::string>(event.iflyvalue()));

      std::stringstream ss;
      ss << std::fixed << std::setprecision(2) << event.iflydoublevalue();
      txtDestinationDTG_->setText(ss.str());
  }

  if (event.type() == WebUiEvent::Alt)
  {
      int tempInt;

      tempInt = event.iflyvalue();
      std::string tempString = boost::lexical_cast<std::string>(tempInt);
      txtAlt_->setText(tempString);
  }

  if (event.type() == WebUiEvent::Speed)
  {
      int tempInt;

      tempInt = event.iflyvalue();
      std::string tempString = boost::lexical_cast<std::string>(tempInt);
      txtSpeed_->setText(tempString);
  }

  if (event.type() == WebUiEvent::Temperature)
  {
      int tempInt;

      tempInt = event.iflyvalue();
      std::string tempString = boost::lexical_cast<std::string>(tempInt);
      txtTemperature_->setText(tempString);
  }

  if (event.type() == WebUiEvent::OnGround)
  {
      if (event.iflyvalue() == 1)
      {
          txtOnGround_->setText("Yes");
      }
      else
      {
          txtOnGround_->setText("No");
      }
  }

  if (event.type() == WebUiEvent::GroundPower)
  {
      if (event.iflyvalue() == 0)
      {
          btnGroundPower_->setStyleClass("btn-danger");
          btnGroundPower_->setText("Ground Power NA\nSet Parking Brake");
      }
      if (event.iflyvalue() == 1)
      {
          btnGroundPower_->setStyleClass("btn-success");
          btnGroundPower_->setText("Ground Power\nAvailable");
      }
      if (event.iflyvalue() == 2)
      {
          btnGroundPower_->setStyleClass("btn-primary");
          btnGroundPower_->setText("Ground Power\nConnected");
      }

  }

  if (event.type() == WebUiEvent::GroundAir)
  {
      if (event.iflyvalue() == 0)
      {
          btnGroundAir_->setStyleClass("btn-danger");
          btnGroundAir_->setText("Ground Air NA\nSet Parking Brake");
      }
      if (event.iflyvalue() == 1)
      {
          btnGroundAir_->setStyleClass("btn-success");
          btnGroundAir_->setText("Ground Air\nAvailable");
      }
      if (event.iflyvalue() == 2)
      {
          btnGroundAir_->setStyleClass("btn-primary");
          btnGroundAir_->setText("Ground Air\nConnected");
      }

  }


  if (event.type() == WebUiEvent::Oooi)
  {
      txtOut_->setText(event.wttext());
      txtOff_->setText(event.wttext2());
      txtOn_->setText(event.wttext3());
      txtIn_->setText(event.wttext4());
  }

  if (event.type() == WebUiEvent::Door)
  {
      if (event.iflydata() == 1)
      {
          if (event.iflyvalue() == 0)
          {
              btnDoor1_->setStyleClass("btn-success");
              btnDoor1_->setText("Main door is CLOSED\nPress to open");
          }
          else
          {
              btnDoor1_->setStyleClass("btn-danger");
              btnDoor1_->setText("Main door is OPEN\nPress to close");
          }
      }
      if (event.iflydata() == 2)
      {
          if (event.iflyvalue() == 0)
          {
              btnDoor2_->setStyleClass("btn-success");
              btnDoor2_->setText("Cargo door is CLOSED\nPress to open");
          }
          else
          {
              btnDoor2_->setStyleClass("btn-danger");
              btnDoor2_->setText("Cargo door is OPEN\nPress to close");
          }
      }
      if (event.iflydata() == 3)
      {
          if (event.iflyvalue() == 0)
          {
              btnDoor3_->setStyleClass("btn-success");
              btnDoor3_->setText("Service door is CLOSED\nPress to open");
          }
          else
          {
              btnDoor3_->setStyleClass("btn-danger");
              btnDoor3_->setText("Service door is OPEN\nPress to close");
          }
      }
  }
  if (event.type() == WebUiEvent::WxrMetarDep)
  {
      txtWxrMetarDep_->setText(event.wttext());
  }
  if (event.type() == WebUiEvent::WxrTafDep)
  {
      txtWxrTafDep_->setText(event.wttext());
  }
  if (event.type() == WebUiEvent::WxrMetarArr)
  {
      txtWxrMetarArr_->setText(event.wttext());
  }
  if (event.type() == WebUiEvent::WxrTafArr)
  {
      txtWxrTafArr_->setText(event.wttext());
  }

  if (event.type() == WebUiEvent::FlightInfo)
  {
      infoBox_->setTitle(event.wttext());
  }

  //if (event.type() == WebUiEvent::GsxMenuText)
  //{
//      Wt::WString _temptitle;
//
//      if (event.iflyvalue() == 0) {
//          _temptitle = "GSX Menu / ";
//          _temptitle = _temptitle + event.wttext();
//          gsxBox_->setTitle(_temptitle);
//      }

//      if (event.iflyvalue() == 1) { btnVirtualKey1_->setText(event.wttext()); btnVirtualKey1_->show(); }
//      if (event.iflyvalue() == 2) { btnVirtualKey2_->setText(event.wttext()); btnVirtualKey2_->show(); }
//      if (event.iflyvalue() == 3) { btnVirtualKey3_->setText(event.wttext()); btnVirtualKey3_->show(); }
//      if (event.iflyvalue() == 4) { btnVirtualKey4_->setText(event.wttext()); btnVirtualKey4_->show(); }
//      if (event.iflyvalue() == 5) { btnVirtualKey5_->setText(event.wttext()); btnVirtualKey5_->show(); }
//      if (event.iflyvalue() == 6) { btnVirtualKey6_->setText(event.wttext()); btnVirtualKey6_->show(); }
//      if (event.iflyvalue() == 7) { btnVirtualKey7_->setText(event.wttext()); btnVirtualKey7_->show(); }
//      if (event.iflyvalue() == 8) { btnVirtualKey8_->setText(event.wttext()); btnVirtualKey8_->show(); }
//      if (event.iflyvalue() == 9) { btnVirtualKey9_->setText(event.wttext()); btnVirtualKey9_->show(); }
//      if (event.iflyvalue() == 10) { btnVirtualKey0_->setText(event.wttext()); btnVirtualKey0_->show(); }
//  }

//  if (event.type() == WebUiEvent::GsxTextText)
//  {
//      txtGSXText_->setText(event.wttext());
//  }


}
