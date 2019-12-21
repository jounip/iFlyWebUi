#include "stdafx.h"
#include "Resource.h"

#include "SimpleWebWidget.h"
#include "SimpleWebServer.h"

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WFitLayout>
#include <Wt/WEnvironment>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WPushButton>
#include <Wt/WNavigationBar>
#include <Wt/WStackedWidget>
#include <Wt/WMenu>
#include <Wt/WGroupBox>
#include <Wt/WGoogleMap>
#include <Wt/WTimer>
#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WTable>

#include <iostream>

using namespace Wt;

SimpleWebWidget::SimpleWebWidget(SimpleWebServer& server, Wt::WContainerWidget *parent)
	: WContainerWidget(parent),
	server_(server)
{
	login();
}

SimpleWebWidget::~SimpleWebWidget()
{
	logout();
}

void SimpleWebWidget::login()
{
	clear();

	btnGroundPower_ = new Wt::WPushButton("Ground Power\nconnect/disconnect");
	btnGroundAir_ = new Wt::WPushButton("Ground Air\n connect/disconnect");
	btnDoor1_ = new Wt::WPushButton("Open/Close\n Door1");
	btnDoor2_ = new Wt::WPushButton("Open/Close\n Door2");
	btnDoor3_ = new Wt::WPushButton("Open/Close\n Door3");
	btnPushBack1_ = new Wt::WPushButton("Start Pushback\nTail Left");
	btnPushBack2_ = new Wt::WPushButton("Start Pushback\nStraight");
	btnPushBack3_ = new Wt::WPushButton("Start Pushback\nTail Right");
	btnPushBack4_ = new Wt::WPushButton("STOP\nPushBack");

	btnVirtualKey1_ = new Wt::WPushButton("Send 1");
	btnVirtualKey2_ = new Wt::WPushButton("Send 2");
	btnVirtualKey3_ = new Wt::WPushButton("Send 3");
	btnVirtualKey4_ = new Wt::WPushButton("Send 4");
	btnVirtualKey5_ = new Wt::WPushButton("Send 5");
	btnVirtualKey6_ = new Wt::WPushButton("Send 6");
	btnVirtualKey7_ = new Wt::WPushButton("Send 7");
	btnVirtualKey8_ = new Wt::WPushButton("Send 8");
	btnVirtualKey9_ = new Wt::WPushButton("Send 9");
	btnVirtualKey0_ = new Wt::WPushButton("Send 0");
	btnVirtualKeyF12_ = new Wt::WPushButton("Send F12");

	btnProcess1_ = new Wt::WPushButton("Start Active Sky");

	txtWxrDep = new Wt::WText("DEPARTURE AIRPORT IS NOT SET");
	txtWxrTafDep = new Wt::WText("DEPARTURE AIRPORT IS NOT SET");
	txtWxrArr = new Wt::WText("ARRIVAL AIRPORT IS NOT SET");
	txtWxrTafArr = new Wt::WText("ARRIVAL AIRPORT IS NOT SET");

	txtGSXText = new Wt::WText("");

	btnGroundPower_->setStyleClass("btn-danger");
	btnGroundAir_->setStyleClass("btn-danger");

	btnDoor1_->setStyleClass("btn-danger");
	btnDoor2_->setStyleClass("btn-danger");
	btnDoor3_->setStyleClass("btn-danger");

	btnPushBack1_->setStyleClass("btn-danger");
	btnPushBack2_->setStyleClass("btn-danger");
	btnPushBack3_->setStyleClass("btn-danger");
	btnPushBack4_->setStyleClass("btn-danger");

	dialog = new Wt::WDialog("No connection");

	Wt::WLabel *label = new Wt::WLabel("Waiting for simulator connection with iFly 737NG",
		dialog->contents());


	Wt::WNavigationBar *navigation = new Wt::WNavigationBar();
	Wt::WStackedWidget *contentsStack = new Wt::WStackedWidget();

	Wt::WContainerWidget *groundContainer = new Wt::WContainerWidget();
	Wt::WContainerWidget *weatherContainer = new Wt::WContainerWidget();
	Wt::WContainerWidget *mapContainer = new Wt::WContainerWidget();
	Wt::WContainerWidget *infoContainer = new Wt::WContainerWidget();
	Wt::WContainerWidget *gsxContainer = new Wt::WContainerWidget();
	Wt::WContainerWidget *startContainer = new Wt::WContainerWidget();

	Wt::WContainerWidget *statusContainer = new Wt::WContainerWidget();

	Wt::WVBoxLayout *mainVLayout = new WVBoxLayout();

	setLayout(mainVLayout);

	// Create a navigation bar with a link to a web page.
	navigation->setTitle("737ng.kapsi.fi",
		"http://737ng.kapsi.fi");
	navigation->setResponsive(true);

	// Setup a Left-aligned menu.
	contentsStack->addStyleClass("contents");
	Wt::WMenu *leftMenu = new Wt::WMenu(contentsStack);
	navigation->addMenu(leftMenu);
	leftMenu->addItem("Ground", groundContainer);
	leftMenu->addItem("Weather", weatherContainer);
	leftMenu->addItem("Info", infoContainer);
	leftMenu->addItem("GSX", gsxContainer);
	leftMenu->addItem("External", startContainer);

	mainVLayout->addWidget(navigation, 0);
	mainVLayout->addWidget(contentsStack, 1);
	mainVLayout->addWidget(statusContainer, 0);

	Wt::WGroupBox *groundBox = new Wt::WGroupBox("Ground Equipment");
	groundBox->addStyleClass("fieldset-header");
	groundBox->addWidget(btnGroundPower_);
	groundBox->addWidget(btnGroundAir_);
	groundContainer->addWidget(groundBox);

	Wt::WGroupBox *doorBox = new Wt::WGroupBox("Aircraft Doors");
	doorBox->addStyleClass("fieldset-header");
	doorBox->addWidget(btnDoor1_);
	doorBox->addWidget(btnDoor2_);
	doorBox->addWidget(btnDoor3_);
	groundContainer->addWidget(doorBox);

	Wt::WGroupBox *pushBox = new Wt::WGroupBox("Default Pushback");
	pushBox->addStyleClass("fieldset-header");
	pushBox->addWidget(btnPushBack1_);
	pushBox->addWidget(btnPushBack2_);
	pushBox->addWidget(btnPushBack3_);
	pushBox->addWidget(new WBreak());
	pushBox->addWidget(new WBreak());
	pushBox->addWidget(btnPushBack4_);
	groundContainer->addWidget(pushBox);

	Wt::WGroupBox *wxrDepMetarBox = new Wt::WGroupBox("Departure Metar");
	Wt::WGroupBox *wxrDepTafBox = new Wt::WGroupBox("Departure TAF");
	Wt::WGroupBox *wxrArrMetarBox = new Wt::WGroupBox("Arrival Metar");
	Wt::WGroupBox *wxrArrTafBox = new Wt::WGroupBox("Arrival TAF");

	wxrDepMetarBox->addStyleClass("fieldset-header");
	wxrDepTafBox->addStyleClass("fieldset-header");
	wxrArrMetarBox->addStyleClass("fieldset-header");
	wxrArrTafBox->addStyleClass("fieldset-header");

	wxrDepMetarBox->addWidget(txtWxrDep);
	wxrDepTafBox->addWidget(txtWxrTafDep);
	wxrArrMetarBox->addWidget(txtWxrArr);
	wxrArrTafBox->addWidget(txtWxrTafArr);

	weatherContainer->addWidget(wxrDepMetarBox);
	weatherContainer->addWidget(wxrDepTafBox);
	weatherContainer->addWidget(wxrArrMetarBox);
	weatherContainer->addWidget(wxrArrTafBox);

	// Flight info data tab starts here

	Wt::WContainerWidget *infoFirstRowContainer = new Wt::WContainerWidget();
	Wt::WHBoxLayout *infoFirstRowHLayout = new WHBoxLayout();
	infoFirstRowContainer->setLayout(infoFirstRowHLayout);

	Wt::WContainerWidget *infoSecondRowContainer = new Wt::WContainerWidget();
	Wt::WHBoxLayout *infoSecondRowHLayout = new WHBoxLayout();
	infoSecondRowContainer->setLayout(infoSecondRowHLayout);

	infoContainer->addWidget(infoFirstRowContainer);
	infoContainer->addWidget(infoSecondRowContainer);

	// infobox with flight no and dep and arr icao codes if available

	txtAlt = new Wt::WText("32013");
	txtSpeed = new Wt::WText("450");
	txtTemperature = new Wt::WText("-55");
	txtOnGround = new Wt::WText("Yes/No");

	Wt::WContainerWidget *flightInfoContainer1 = new Wt::WContainerWidget();
	Wt::WContainerWidget *flightInfoContainer2 = new Wt::WContainerWidget();
	Wt::WContainerWidget *flightInfoContainer3 = new Wt::WContainerWidget();
	Wt::WContainerWidget *flightInfoContainer4 = new Wt::WContainerWidget();

	flightInfoContainer1->addWidget(new Wt::WText("Alt: "));
	flightInfoContainer1->addWidget(new WBreak());
	flightInfoContainer1->addWidget(new Wt::WText("Speed: "));

	flightInfoContainer2->addWidget(txtAlt);
	flightInfoContainer2->addWidget(new WBreak());
	flightInfoContainer2->addWidget(txtSpeed);

	flightInfoContainer3->addWidget(new Wt::WText("Temp: "));
	flightInfoContainer3->addWidget(new WBreak());
	flightInfoContainer3->addWidget(new Wt::WText("On ground: "));

	flightInfoContainer4->addWidget(txtTemperature);
	flightInfoContainer4->addWidget(new WBreak());
	flightInfoContainer4->addWidget(txtOnGround);

	Wt::WHBoxLayout *flightInfoHLayout = new WHBoxLayout();

	flightInfoHLayout->addWidget(flightInfoContainer1);
	flightInfoHLayout->addWidget(flightInfoContainer2);
	flightInfoHLayout->addWidget(flightInfoContainer3);
	flightInfoHLayout->addWidget(flightInfoContainer4);

	Wt::WContainerWidget *flightInfoContainer = new Wt::WContainerWidget();

	flightInfoContainer->setLayout(flightInfoHLayout);

	infoBox = new Wt::WGroupBox(" ( - )");
	infoBox->addStyleClass("fieldset-header");
	infoBox->addWidget(flightInfoContainer);

	// OOOI box with out off on in times if available

	txtOut = new Wt::WText("00:00");
	txtOff = new Wt::WText("00:00");
	txtOn = new Wt::WText("00:00");
	txtIn = new Wt::WText("00:00");

	Wt::WContainerWidget *oooiContainer1 = new Wt::WContainerWidget();
	Wt::WContainerWidget *oooiContainer2 = new Wt::WContainerWidget();
	Wt::WContainerWidget *oooiContainer3 = new Wt::WContainerWidget();
	Wt::WContainerWidget *oooiContainer4 = new Wt::WContainerWidget();

	oooiContainer1->addWidget(new Wt::WText("Out: "));
	oooiContainer1->addWidget(new WBreak());
	oooiContainer1->addWidget(new Wt::WText("Off: "));

	oooiContainer2->addWidget(txtOut);
	oooiContainer2->addWidget(new WBreak());
	oooiContainer2->addWidget(txtOff);

	oooiContainer3->addWidget(new Wt::WText("On: "));
	oooiContainer3->addWidget(new WBreak());
	oooiContainer3->addWidget(new Wt::WText("In: "));

	oooiContainer4->addWidget(txtOn);
	oooiContainer4->addWidget(new WBreak());
	oooiContainer4->addWidget(txtIn);

	Wt::WHBoxLayout *oooiHLayout = new WHBoxLayout();

	oooiHLayout->addWidget(oooiContainer1);
	oooiHLayout->addWidget(oooiContainer2);
	oooiHLayout->addWidget(oooiContainer3);
	oooiHLayout->addWidget(oooiContainer4);

	Wt::WContainerWidget *oooiContainer = new Wt::WContainerWidget();

	oooiContainer->setLayout(oooiHLayout);

	Wt::WGroupBox *oooiBox = new Wt::WGroupBox("OOOI");
	oooiBox->addStyleClass("fieldset-header");
	oooiBox->addWidget(oooiContainer);

	// Destination box

	txtDestinationDTG = new Wt::WText("345");
	txtDestinationETA = new Wt::WText("23:59:03");
	txtDestinationFuel = new Wt::WText("2900");

	Wt::WContainerWidget *destinationContainer1 = new Wt::WContainerWidget();
	Wt::WContainerWidget *destinationContainer2 = new Wt::WContainerWidget();
	Wt::WContainerWidget *destinationContainer3 = new Wt::WContainerWidget();
	Wt::WContainerWidget *destinationContainer4 = new Wt::WContainerWidget();

	destinationContainer1->addWidget(new Wt::WText("ETA: "));
	destinationContainer1->addWidget(new WBreak());
	destinationContainer1->addWidget(new Wt::WText("DTG: "));

	destinationContainer2->addWidget(txtDestinationETA);
	destinationContainer2->addWidget(new WBreak());
	destinationContainer2->addWidget(txtDestinationDTG);

	destinationContainer3->addWidget(new Wt::WText("Fuel: "));

	destinationContainer4->addWidget(txtDestinationFuel);

	Wt::WHBoxLayout *destinationHLayout = new WHBoxLayout();

	destinationHLayout->addWidget(destinationContainer1);
	destinationHLayout->addWidget(destinationContainer2);
	destinationHLayout->addWidget(destinationContainer3);
	destinationHLayout->addWidget(destinationContainer4);

	Wt::WContainerWidget *destinationContainer = new Wt::WContainerWidget();
	destinationContainer->setLayout(destinationHLayout);

	Wt::WGroupBox *destinationBox = new Wt::WGroupBox("Destination");
	destinationBox->addStyleClass("fieldset-header");
	destinationBox->addWidget(destinationContainer);

	// Last waypoint box

	Wt::WContainerWidget *lastWptContainer1 = new Wt::WContainerWidget();
	Wt::WContainerWidget *lastWptContainer2 = new Wt::WContainerWidget();

	txtLastName = new Wt::WText("ROVIL");
	txtLastAlt = new Wt::WText("FL280");
	txtLastATA = new Wt::WText("20:38:51");
	txtLastFuel = new Wt::WText("4300");

	lastWptContainer1->addWidget(new Wt::WText("Name: "));
	lastWptContainer1->addWidget(new WBreak());
	lastWptContainer1->addWidget(new Wt::WText("ATA: "));
	lastWptContainer1->addWidget(new WBreak());
	lastWptContainer1->addWidget(new Wt::WText("Fuel: "));
	lastWptContainer1->addWidget(new WBreak());
	lastWptContainer1->addWidget(new Wt::WText("Alt: "));
	lastWptContainer2->addWidget(txtLastName);
	lastWptContainer2->addWidget(new WBreak());
	lastWptContainer2->addWidget(txtLastATA);
	lastWptContainer2->addWidget(new WBreak());
	lastWptContainer2->addWidget(txtLastFuel);
	lastWptContainer2->addWidget(new WBreak());
	lastWptContainer2->addWidget(txtLastAlt);

	Wt::WHBoxLayout *lastWptHLayout = new WHBoxLayout();

	lastWptHLayout->addWidget(lastWptContainer1);
	lastWptHLayout->addWidget(lastWptContainer2);

	Wt::WContainerWidget *lastWpContainer = new Wt::WContainerWidget();
	lastWpContainer->setLayout(lastWptHLayout);

	Wt::WGroupBox *progressLastBox = new Wt::WGroupBox("Last WPT");
	progressLastBox->addStyleClass("fieldset-header");
	progressLastBox->addWidget(lastWpContainer);

	// Actual waypoint box

	Wt::WContainerWidget *actWpContainer1 = new Wt::WContainerWidget();
	Wt::WContainerWidget *actWpContainer2 = new Wt::WContainerWidget();

	txtActName = new Wt::WText("TESTI");
	txtActETA = new Wt::WText("20:48:32");
	txtActFuel = new Wt::WText("4000");
	txtActDTG = new Wt::WText("23");

	actWpContainer1->addWidget(new Wt::WText("Name: "));
	actWpContainer1->addWidget(new WBreak());
	actWpContainer1->addWidget(new Wt::WText("ETA: "));
	actWpContainer1->addWidget(new WBreak());
	actWpContainer1->addWidget(new Wt::WText("Fuel: "));
	actWpContainer1->addWidget(new WBreak());
	actWpContainer1->addWidget(new Wt::WText("DTG: "));
	actWpContainer2->addWidget(txtActName);
	actWpContainer2->addWidget(new WBreak());
	actWpContainer2->addWidget(txtActETA);
	actWpContainer2->addWidget(new WBreak());
	actWpContainer2->addWidget(txtActFuel);
	actWpContainer2->addWidget(new WBreak());
	actWpContainer2->addWidget(txtActDTG);

	Wt::WHBoxLayout *actWptHLayout = new WHBoxLayout();

	actWptHLayout->addWidget(actWpContainer1);
	actWptHLayout->addWidget(actWpContainer2);

	Wt::WContainerWidget *actWpContainer = new Wt::WContainerWidget();
	actWpContainer->setLayout(actWptHLayout);

	Wt::WGroupBox *progressActBox = new Wt::WGroupBox("Actual WPT");
	progressActBox->addStyleClass("fieldset-header");
	progressActBox->addWidget(actWpContainer);
	
	// Next Waypoint Box

	Wt::WContainerWidget *nextWpContainer1 = new Wt::WContainerWidget();
	Wt::WContainerWidget *nextWpContainer2 = new Wt::WContainerWidget();

	txtNextName = new Wt::WText("FIXME");
	txtNextETA = new Wt::WText("21:02:22");
	txtNextFuel = new Wt::WText("3700");
	txtNextDTG = new Wt::WText("46");

	nextWpContainer1->addWidget(new Wt::WText("Name: "));
	nextWpContainer1->addWidget(new WBreak());
	nextWpContainer1->addWidget(new Wt::WText("ETA: "));
	nextWpContainer1->addWidget(new WBreak());
	nextWpContainer1->addWidget(new Wt::WText("Fuel: "));
	nextWpContainer1->addWidget(new WBreak());
	nextWpContainer1->addWidget(new Wt::WText("DTG: "));
	nextWpContainer2->addWidget(txtNextName);
	nextWpContainer2->addWidget(new WBreak());
	nextWpContainer2->addWidget(txtNextETA);
	nextWpContainer2->addWidget(new WBreak());
	nextWpContainer2->addWidget(txtNextFuel);
	nextWpContainer2->addWidget(new WBreak());
	nextWpContainer2->addWidget(txtNextDTG);

	Wt::WHBoxLayout *nextWptHLayout = new WHBoxLayout();

	nextWptHLayout->addWidget(nextWpContainer1);
	nextWptHLayout->addWidget(nextWpContainer2);

	Wt::WContainerWidget *nextWpContainer = new Wt::WContainerWidget();
	nextWpContainer->setLayout(nextWptHLayout);
	
	Wt::WGroupBox *progressNextBox = new Wt::WGroupBox("Next WPT");
	progressNextBox->addStyleClass("fieldset-header");
	progressNextBox->addWidget(nextWpContainer);

	// add groupboxes to Layouts

	infoFirstRowHLayout->addWidget(infoBox);
	infoFirstRowHLayout->addWidget(oooiBox);
	infoFirstRowHLayout->addWidget(destinationBox);

	infoSecondRowHLayout->addWidget(progressLastBox);
	infoSecondRowHLayout->addWidget(progressActBox);
	infoSecondRowHLayout->addWidget(progressNextBox);

	// GSX tab starts here

	gsxBox = new Wt::WGroupBox("GSX Menu");
	gsxBox->addStyleClass("fieldset-header");
	gsxContainer->addWidget(gsxBox);

	Wt::WTable* gsxTable = new Wt::WTable();
	gsxTable->setWidth(Wt::WLength("100%"));

	btnVirtualKeyF12_->addStyleClass("buttongsx");
	btnVirtualKey1_->addStyleClass("buttongsx");
	btnVirtualKey2_->addStyleClass("buttongsx");
	btnVirtualKey3_->addStyleClass("buttongsx");
	btnVirtualKey4_->addStyleClass("buttongsx");
	btnVirtualKey5_->addStyleClass("buttongsx");
	btnVirtualKey6_->addStyleClass("buttongsx");
	btnVirtualKey7_->addStyleClass("buttongsx");
	btnVirtualKey8_->addStyleClass("buttongsx");
	btnVirtualKey9_->addStyleClass("buttongsx");
	btnVirtualKey0_->addStyleClass("buttongsx");

	gsxTable->elementAt(0, 0)->addWidget(btnVirtualKey1_);
	gsxTable->elementAt(0, 1)->addWidget(btnVirtualKey6_);
	gsxTable->elementAt(1, 0)->addWidget(btnVirtualKey2_);
	gsxTable->elementAt(1, 1)->addWidget(btnVirtualKey7_);
	gsxTable->elementAt(2, 0)->addWidget(btnVirtualKey3_);
	gsxTable->elementAt(2, 1)->addWidget(btnVirtualKey8_);
	gsxTable->elementAt(3, 0)->addWidget(btnVirtualKey4_);
	gsxTable->elementAt(3, 1)->addWidget(btnVirtualKey9_);
	gsxTable->elementAt(4, 0)->addWidget(btnVirtualKey5_);
	gsxTable->elementAt(4, 1)->addWidget(btnVirtualKey0_);

	gsxTable->elementAt(6, 0)->addWidget(btnVirtualKeyF12_);

	gsxBox->addWidget(gsxTable);

	// externaltab starts here

	externalBox = new Wt::WGroupBox("External Programs");
	externalBox->addStyleClass("fieldset-header");
	startContainer->addWidget(externalBox);

	externalBox->addWidget(btnProcess1_);
	btnProcess1_->setStyleClass("btn-success");

	// Bottom statuscontaier with GSX text

	txtGSXText->addStyleClass("gsxtext");
	statusContainer->addWidget(txtGSXText);

	// Bind the C++ and JavaScript event handlers.
	btnGroundPower_->clicked().connect(this, &SimpleWebWidget::pressedGroundPower);
	btnGroundAir_->clicked().connect(this, &SimpleWebWidget::pressedGroundAir);

	btnDoor1_->clicked().connect(this, &SimpleWebWidget::pressedButtonDoor1);
	btnDoor2_->clicked().connect(this, &SimpleWebWidget::pressedButtonDoor2);
	btnDoor3_->clicked().connect(this, &SimpleWebWidget::pressedButtonDoor3);

	btnPushBack1_->clicked().connect(this, &SimpleWebWidget::pressedPushBack1);
	btnPushBack2_->clicked().connect(this, &SimpleWebWidget::pressedPushBack2);
	btnPushBack3_->clicked().connect(this, &SimpleWebWidget::pressedPushBack3);
	btnPushBack4_->clicked().connect(this, &SimpleWebWidget::pressedPushBack4);

	btnVirtualKey1_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey1);
	btnVirtualKey2_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey2);
	btnVirtualKey3_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey3);
	btnVirtualKey4_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey4);
	btnVirtualKey5_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey5);
	btnVirtualKey6_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey6);
	btnVirtualKey7_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey7);
	btnVirtualKey8_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey8);
	btnVirtualKey9_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey9);
	btnVirtualKey0_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKey0);
	btnVirtualKeyF12_->clicked().connect(this, &SimpleWebWidget::pressedVirtualKeyF12);

	btnProcess1_->clicked().connect(this, &SimpleWebWidget::pressedProcess1);

	



	// setup a timer which calls MyClass::timeout() every 60 seconds, until timer->stop() is called.
	Wt::WTimer *timer = new Wt::WTimer();
	timer->setInterval(60000);
	timer->timeout().connect(this, &SimpleWebWidget::timerTimeout);
	timer->start();


	connect();
}

void SimpleWebWidget::timerTimeout()
{
	server_.sendButtonPress(ID_WEBUI_TIMER_TIMEOUT);
}
void SimpleWebWidget::pressedGroundPower()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_GROUND_POWER);
}

void SimpleWebWidget::pressedGroundAir()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_GROUND_AIR);
}

void SimpleWebWidget::pressedButtonDoor1()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_DOOR1);
}

void SimpleWebWidget::pressedButtonDoor2()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_DOOR2);
}

void SimpleWebWidget::pressedButtonDoor3()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_DOOR3);
}

void SimpleWebWidget::pressedPushBack1()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_PUSHBACK1);
}

void SimpleWebWidget::pressedPushBack2()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_PUSHBACK2);
}

void SimpleWebWidget::pressedPushBack3()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_PUSHBACK3);
}

void SimpleWebWidget::pressedPushBack4()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_PUSHBACK4);
}

void SimpleWebWidget::pressedVirtualKey0()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_0);
}

void SimpleWebWidget::pressedVirtualKey1()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_1);
}

void SimpleWebWidget::pressedVirtualKey2()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_2);
}

void SimpleWebWidget::pressedVirtualKey3()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_3);
}

void SimpleWebWidget::pressedVirtualKey4()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_4);
}

void SimpleWebWidget::pressedVirtualKey5()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_5);
}

void SimpleWebWidget::pressedVirtualKey6()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_6);
}

void SimpleWebWidget::pressedVirtualKey7()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_7);
}

void SimpleWebWidget::pressedVirtualKey8()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_8);
}

void SimpleWebWidget::pressedVirtualKey9()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_9);
}

void SimpleWebWidget::pressedVirtualKeyF12()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_F12);
}

void SimpleWebWidget::pressedProcess1()
{
	server_.sendButtonPress(ID_WEBUI_BUTTON_VIRTUAL_KEY_PROC1);
}



void SimpleWebWidget::logout()
{
	disconnect();
}

void SimpleWebWidget::connect()
{
	if (server_.connectWtClient
		(this, boost::bind(&SimpleWebWidget::processChatEvent, this, _1)))
		Wt::WApplication::instance()->enableUpdates(true);
}

void SimpleWebWidget::disconnect()
{
	if (server_.disconnectWtClient(this))
		Wt::WApplication::instance()->enableUpdates(false);
}

void SimpleWebWidget::processChatEvent(const ChatEvent& event)

{

	WApplication *app = WApplication::instance();

	app->triggerUpdate();

	if (event.type() == ChatEvent::Status)
	{
		if (event.iflyvalue() == 0)
		{
			dialog->show();
		}
		else
		{
			dialog->hide();
		}
	}

	if (event.type() == ChatEvent::LastWPT)
	{
		txtLastName->setText(event.wttext());
		txtLastAlt->setText(boost::lexical_cast<std::string>(event.iflyvalue()));
		txtLastATA->setText(event.wttext2());
		txtLastFuel->setText(boost::lexical_cast<std::string>(event.iflyvalue3()));
	}

	if (event.type() == ChatEvent::ActualWPT)
	{
		txtActName->setText(event.wttext());
		txtActETA->setText(event.wttext2());
		txtActFuel->setText(boost::lexical_cast<std::string>(event.iflyvalue2()));
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << event.iflydoublevalue();
		txtActDTG->setText(ss.str());
	}

	if (event.type() == ChatEvent::NextWPT)
	{
		txtNextName->setText(event.wttext());
		txtNextETA->setText(event.wttext2());
		txtNextFuel->setText(boost::lexical_cast<std::string>(event.iflyvalue2()));

		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << event.iflydoublevalue();
		txtNextDTG->setText(ss.str());
	}

	if (event.type() == ChatEvent::Destination)
	{
		txtDestinationETA->setText(event.wttext());
		txtDestinationFuel->setText(boost::lexical_cast<std::string>(event.iflyvalue()));

		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << event.iflydoublevalue();
		txtDestinationDTG->setText(ss.str());
	}

	if (event.type() == ChatEvent::Alt)
	{
		int tempInt;

		tempInt = event.iflyvalue();
		std::string tempString = boost::lexical_cast<std::string>(tempInt);
		txtAlt->setText(tempString);
	}

	if (event.type() == ChatEvent::Speed)
	{
		int tempInt;

		tempInt = event.iflyvalue();
		std::string tempString = boost::lexical_cast<std::string>(tempInt);
		txtSpeed->setText(tempString);
	}

	if (event.type() == ChatEvent::Temperature)
	{
		int tempInt;

		tempInt = event.iflyvalue();
		std::string tempString = boost::lexical_cast<std::string>(tempInt);
		txtTemperature->setText(tempString);
	}

	if (event.type() == ChatEvent::OnGround)
	{
		if (event.iflyvalue() == 1)
		{
			txtOnGround->setText("Yes");
		}
		else
		{
			txtOnGround->setText("No");
		}
	}

	if (event.type() == ChatEvent::GroundPower)
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

	if (event.type() == ChatEvent::GroundAir)
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


	if (event.type() == ChatEvent::Oooi)
	{
		txtOut->setText(event.wttext());
		txtOff->setText(event.wttext2());
		txtOn->setText(event.wttext3());
		txtIn->setText(event.wttext4());
	}

	if (event.type() == ChatEvent::Door)
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
	if (event.type() == ChatEvent::WxrMetarDep)
	{
		txtWxrDep->setText(event.wttext());
	}
	if (event.type() == ChatEvent::WxrTafDep)
	{
		txtWxrTafDep->setText(event.wttext());
	}
	if (event.type() == ChatEvent::WxrMetarArr)
	{
		txtWxrArr->setText(event.wttext());
	}
	if (event.type() == ChatEvent::WxrTafArr)
	{
		txtWxrTafArr->setText(event.wttext());
	}

	if (event.type() == ChatEvent::FlightInfo)
	{
		infoBox->setTitle(event.wttext());
	}

	if (event.type() == ChatEvent::GsxTextText)
	{
		txtGSXText->setText(event.wttext());
	}
}


