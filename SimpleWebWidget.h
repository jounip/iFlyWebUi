#ifndef SIMPLECHATWIDGET_H_
#define SIMPLECHATWIDGET_H_

#include <Wt/WContainerWidget>

#include "SimpleWebServer.h"

namespace Wt {
	class WApplication;
	class WPushButton;
	class WText;
	class WLineEdit;
	class WTextArea;
}

class ChatEvent;

/*! \brief A self-contained chat widget.
*/
class SimpleWebWidget : public Wt::WContainerWidget,
	public SimpleWebServer::Client
{
public:
	/*! \brief Create a chat widget that will connect to the given server.
	*/
	SimpleWebWidget(SimpleWebServer& server, Wt::WContainerWidget *parent = 0);

	/*! \brief Delete a chat widget.
	*/
	~SimpleWebWidget();

	SimpleWebServer& server() { return server_; }

	void login();
	void logout();
	void connect();
	void disconnect();
	bool startChat();

private:
	SimpleWebServer&     server_;

	Wt::WPushButton *btnGroundPower_;
	Wt::WPushButton *btnGroundAir_;

	Wt::WPushButton *btnDoor1_;
	Wt::WPushButton *btnDoor2_;
	Wt::WPushButton *btnDoor3_;

	Wt::WPushButton *btnPushBack1_;
	Wt::WPushButton *btnPushBack2_;
	Wt::WPushButton *btnPushBack3_;
	Wt::WPushButton *btnPushBack4_;

	Wt::WPushButton *btnVirtualKey1_;
	Wt::WPushButton *btnVirtualKey2_;
	Wt::WPushButton *btnVirtualKey3_;
	Wt::WPushButton *btnVirtualKey4_;
	Wt::WPushButton *btnVirtualKey5_;
	Wt::WPushButton *btnVirtualKey6_;
	Wt::WPushButton *btnVirtualKey7_;
	Wt::WPushButton *btnVirtualKey8_;
	Wt::WPushButton *btnVirtualKey9_;
	Wt::WPushButton *btnVirtualKey0_;
	Wt::WPushButton *btnVirtualKeyF12_;

	Wt::WPushButton *btnProcess1_;

	Wt::WText *txtWxrDep;
	Wt::WText *txtWxrArr;
	Wt::WText *txtWxrTafDep;
	Wt::WText *txtWxrTafArr;

	Wt::WGoogleMap *map;

	Wt::WText *txtAlt;
	Wt::WText *txtSpeed;
	Wt::WText *txtTemperature;
	Wt::WText *txtOnGround;

	Wt::WText *txtStatus;

	Wt::WDialog *dialog;

	Wt::WText *txtFlightInfo;

	Wt::WGroupBox *infoBox;

	Wt::WGroupBox *gsxBox;

	Wt::WGroupBox *externalBox;


	Wt::WText *txtOut;
	Wt::WText *txtOff;
	Wt::WText *txtOn;
	Wt::WText *txtIn;

	Wt::WPushButton *btnResetOooi_;

	Wt::WText *txtDestinationDTG;
	Wt::WText *txtDestinationETA;
	Wt::WText *txtDestinationFuel;

	Wt::WText *txtLastName;
	Wt::WText *txtLastAlt;
	Wt::WText *txtLastATA;
	Wt::WText *txtLastFuel;
	Wt::WText *txtLastDTG;

	Wt::WText *txtActName;
	Wt::WText *txtActETA;
	Wt::WText *txtActFuel;
	Wt::WText *txtActDTG;

	Wt::WText *txtNextName;
	Wt::WText *txtNextETA;
	Wt::WText *txtNextFuel;
	Wt::WText *txtNextDTG;

	Wt::WText *txtGSXText;

	/* called from another session */
	void processChatEvent(const ChatEvent& event);

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
	void pressedVirtualKeyF12();

	void pressedProcess1();

};

#endif // SIMPLECHATWIDGET