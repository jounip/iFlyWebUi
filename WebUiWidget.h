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
   
  /* called from another session */
  void processWebUiEvent(const WebUiEvent& event);


};

/*@}*/

#endif // WebUiWIDGET
