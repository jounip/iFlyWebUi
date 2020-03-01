// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#ifndef WebUiWIDGET_H_
#define WebUiWIDGET_H_

#include "WebUiServer.h"

class WebUiEvent;

class WebUiWidget : public Wt::WContainerWidget,
			 public WebUiServer::Client
{
public:
  WebUiWidget(WebUiServer& server);
  ~WebUiWidget();
  void connect();
  void disconnect();
  WebUiServer& server() { return server_; }

private:

  WebUiServer&         server_;

  Wt::WPushButton* btnGroundPower_;
  Wt::WPushButton* btnGroundAir_;

  Wt::WPushButton* btnDoor1_;
  Wt::WPushButton* btnDoor2_;
  Wt::WPushButton* btnDoor3_;

  Wt::WPushButton* btnPushBack1_;
  Wt::WPushButton* btnPushBack2_;
  Wt::WPushButton* btnPushBack3_;
  Wt::WPushButton* btnPushBack4_;
  
  Wt::WPushButton* btnVirtualKey1_;
  Wt::WPushButton* btnVirtualKey2_;
  Wt::WPushButton* btnVirtualKey3_;
  Wt::WPushButton* btnVirtualKey4_;
  Wt::WPushButton* btnVirtualKey5_;
  Wt::WPushButton* btnVirtualKey6_;
  Wt::WPushButton* btnVirtualKey7_;
  Wt::WPushButton* btnVirtualKey8_;
  Wt::WPushButton* btnVirtualKey9_;
  Wt::WPushButton* btnVirtualKey0_;
  Wt::WPushButton* btnVirtualKeyF10_;
  Wt::WPushButton* btnVirtualKeyF11_;

  Wt::WGroupBox* infoBox_;
  Wt::WGroupBox* gsxBox_;

  Wt::WText* txtWxrMetarDep_;
  Wt::WText* txtWxrMetarArr_;
  Wt::WText* txtWxrTafDep_;
  Wt::WText* txtWxrTafArr_;

  Wt::WText* txtAlt_;
  Wt::WText* txtSpeed_;
  Wt::WText* txtTemperature_;
  Wt::WText* txtOnGround_;

  Wt::WText* txtFlightInfo_;

  Wt::WText* txtOut_;
  Wt::WText* txtOff_;
  Wt::WText* txtOn_;
  Wt::WText* txtIn_;

  Wt::WText* txtDestinationDTG_;
  Wt::WText* txtDestinationETA_;
  Wt::WText* txtDestinationFuel_;

  Wt::WText* txtLastName_;
  Wt::WText* txtLastAlt_;
  Wt::WText* txtLastATA_;
  Wt::WText* txtLastFuel_;
  Wt::WText* txtLastDTG_;

  Wt::WText* txtActualName_;
  Wt::WText* txtActualETA_;
  Wt::WText* txtActualFuel_;
  Wt::WText* txtActualDTG_;

  Wt::WText* txtNextName_;
  Wt::WText* txtNextETA_;
  Wt::WText* txtNextFuel_;
  Wt::WText* txtNextDTG_;

  Wt::WText* txtGSXText_;

  Wt::WGroupBox* GSXBox_;


  /* called from another session */
  void processWebUiEvent(const WebUiEvent& event);

  void timerTimeout();

  void pressedGroundPower();
  void pressedGroundAir();

  void pressedButtonDoor1();
  void pressedButtonDoor2();
  void pressedButtonDoor3();

  void pressedPushBack1();
  void pressedPushBack2();
  void pressedPushBack3();
  void pressedPushBack4();

  void pressedVirtualKey1();
  void pressedVirtualKey2();
  void pressedVirtualKey3();
  void pressedVirtualKey4();
  void pressedVirtualKey5();
  void pressedVirtualKey6();
  void pressedVirtualKey7();
  void pressedVirtualKey8();
  void pressedVirtualKey9();
  void pressedVirtualKey0();
  void pressedVirtualKeyF10();
  void pressedVirtualKeyF11();
  void pressedVirtualKeyF12();

  void hideVirtualButtons();

};

/*@}*/

#endif // WebUiWIDGET
