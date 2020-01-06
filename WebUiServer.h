// This may look like C code, but it's really -*- C++ -*-

#ifndef WebUiSERVER_H_
#define WebUiSERVER_H_

#include "Resource.h"

namespace Wt {
  class WServer;
}

class WebUiEvent
{
public:
  enum Type { Door, GroundAir, GroundPower, WxrMetarDep, WxrMetarArr, WxrTafDep, WxrTafArr, LatLon, Alt, OnGround, Status, FlightInfo, LastWPT, ActualWPT, NextWPT, Destination, Oooi, Speed, Temperature, GsxMenuText, GsxTextText };
  Type type() const { return type_; }
  const Wt::WString& wttext() const { return wttext_; }
  const Wt::WString& wttext2() const { return wttext2_; }
  const Wt::WString& wttext3() const { return wttext3_; }
  const Wt::WString& wttext4() const { return wttext4_; }
  int iflydata() const { return iflydata_; }
  int iflyvalue() const { return iflyvalue_; }
  int iflyvalue2() const { return iflyvalue2_; }
  int iflyvalue3() const { return iflyvalue3_; }
  double iflydoublevalue() const { return iflydoublevalue_; }
  double iflydoublevalue2() const { return iflydoublevalue2_; }

private:
    Type type_;
    Wt::WString wttext_;
    Wt::WString wttext2_;
    Wt::WString wttext3_;
    Wt::WString wttext4_;
    int iflydata_;
    int iflyvalue_;
    int iflyvalue2_;
    int iflyvalue3_;
    double iflydoublevalue_;
    double iflydoublevalue2_;

    WebUiEvent(Type type, const int iflyvalue)
        : type_(type), iflyvalue_(iflyvalue)
    { }

    WebUiEvent(Type type, const int iflydata, const int iflyvalue)
        : type_(type), iflydata_(iflydata), iflyvalue_(iflyvalue)
    { }

    WebUiEvent(Type type, const Wt::WString& wttext)
        : type_(type), wttext_(wttext)
    { }

    WebUiEvent(Type type, const double iflydoublevalue)
        : type_(type), iflydoublevalue_(iflydoublevalue)
    { }

    WebUiEvent(Type type, const double iflydoublevalue, const double iflydoublevalue2)
        : type_(type), iflydoublevalue_(iflydoublevalue), iflydoublevalue2_(iflydoublevalue2)
    { }

    WebUiEvent(Type type, const Wt::WString& wttext, const int iflyvalue, const Wt::WString& wttext2, const int iflyvalue3)
        : type_(type), wttext_(wttext), iflyvalue_(iflyvalue), wttext2_(wttext2), iflyvalue3_(iflyvalue3)
    { }

    WebUiEvent(Type type, const Wt::WString& wttext, const Wt::WString& wttext2, const int iflyvalue2, const double iflydoublevalue)
        : type_(type), wttext_(wttext), wttext2_(wttext2), iflyvalue2_(iflyvalue2), iflydoublevalue_(iflydoublevalue)
    { }

    WebUiEvent(Type type, const Wt::WString& wttext, const Wt::WString& wttext2, const Wt::WString& wttext3, const Wt::WString& wttext4)
        : type_(type), wttext_(wttext), wttext2_(wttext2), wttext3_(wttext3), wttext4_(wttext4)
    { }

    WebUiEvent(Type type, const Wt::WString& wttext, const int iflyvalue, const double iflydoublevalue)
        : type_(type), wttext_(wttext), iflyvalue_(iflyvalue), iflydoublevalue_(iflydoublevalue)
    { }


  friend class WebUiServer;
};

typedef std::function<void (const WebUiEvent&)> WebUiEventCallback;

class WebUiServer
{
public:
  class Client
  {
  };
  WebUiServer(Wt::WServer& server);
  WebUiServer(const WebUiServer &) = delete;
  WebUiServer &operator=(const WebUiServer &) = delete;
  ~WebUiServer();
  bool connectClient(Client *client, const WebUiEventCallback& handleEvent);
  bool disconnectClient(Client *client);
  void sendButtonPress(int pressButtonValue);


private:
  struct ClientInfo {
    std::string       sessionId;
    WebUiEventCallback eventCallback;
  };

  typedef std::map<Client *, ClientInfo> ClientMap;

  Wt::WServer&                server_;
  std::recursive_mutex    mutex_;
  ClientMap               clients_;
 
  boost::thread thread_;
  bool  stop_;

  WebUiSimulatorData webuisimulatordata_;

  int simulatorStatus;

  int flightState = FLIGHTSTATE_PREFLIGHT;

  int stateDoor1;
  int stateDoor2;
  int stateDoor3;
  int prevDoor1;
  int prevDoor2;
  int prevDoor3;

  int stateGroundPower;
  int stateGroundAir;
  int prevGroundPower;
  int prevGroundAir;

  int wxrStatus;

  double currentLat;
  double currentLon;
  int currentAlt;
  int currentTemp;
  int currentGS;
  int currentOnGround;

  double prevLat;
  double prevLon;
  int prevAlt;
  int prevTemp;
  int prevGS;
  int prevOnGround;

  Wt::WString tempDepMetarString;
  Wt::WString tempDepTafString;
  Wt::WString tempArrMetarString;
  Wt::WString tempArrTafString;

  Wt::WString tempDepString;
  Wt::WString tempArrString;
  Wt::WString tempFlighNumberString;
  Wt::WString tempInfoStringWt;

  Wt::WString currentGsxMenuSubject;
  Wt::WString currentGsxMenuText1;
  Wt::WString currentGsxMenuText2;
  Wt::WString currentGsxMenuText3;
  Wt::WString currentGsxMenuText4;
  Wt::WString currentGsxMenuText5;
  Wt::WString currentGsxMenuText6;
  Wt::WString currentGsxMenuText7;
  Wt::WString currentGsxMenuText8;
  Wt::WString currentGsxMenuText9;
  Wt::WString currentGsxMenuText0;

  std::string currentGsxTextText;

  std::string tempInfoString;
  std::string tempIcao;
  std::string tempFlightNumber;
  std::string tempWeather;

  std::string acDepIcao;
  std::string acArrIcao;
  std::string acFlightNumber;

  int tempWptTime;
  int tempWptTime2;

  std::string currentLastName;
  int currentLastAltitude;
  std::string currentLastATA;
  int currentLastFuel;

  std::string currentActualName;
  std::string currentActualETA;
  int tempActWPT_ETA;
  int currentActualFuel;
  double currentActualDTG;

  std::string currentNextName;
  std::string currentNextETA;
  int tempNextWPT_ETA;
  int currentNextFuel;
  double currentNextDTG;

  std::string currentDestinationETA;
  int tempDestinationWPT_ETA;
  int currentDestinationFuel;
  double currentDestinationDTG;

  std::string prevLastName;
  int prevLastAltitude;
  std::string prevLastATA;
  int prevLastFuel;

  std::string prevActualName;
  std::string prevActualETA;
  int prevActualFuel;
  double prevActualDTG;

  std::string prevNextName;
  std::string prevNextETA;
  int prevNextFuel;
  double prevNextDTG;

  std::string prevDestinationETA;
  int prevDestinationFuel;
  double prevDestinationDTG;

  std::string currentOooiOut = "N/A";
  std::string currentOooiOff = "N/A";
  std::string currentOooiOn = "N/A";
  std::string currentOooiIn = "N/A";

  std::string prevOooiOut;
  std::string prevOooiOff;
  std::string prevOooiOn;
  std::string prevOooiIn;

  void run();

  void getInitialState();

  void SendGSXMenuTextToWidget();
  void SendGsxTextTextToWidget();

  void SendSimulatorPostitionDataToWidget();
  void SendSimulatorDoorDataToWidget();
  void SendSimulatorGroundDataToWidget();
  void SendSimulatorIcaoDataToWidget();
  void SendSimulatorLastWPTDataToWidget();
  void SendSimulatorActualWPTDataToWidget();
  void SendSimulatorNextWPTDataToWidget();
  void SendSimulatorOooiDataToWidget();
  void SendSimulatorDestinationDataToWidget();

  int GetWeather(int, std::string&, std::string&);

  void postWebUiEvent(const WebUiEvent& event);
};


#endif // WebUiSERVER_H_
