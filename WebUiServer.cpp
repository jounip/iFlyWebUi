#include "pch.h"

#include "Resource.h"
#include "SimulatorDataStruct.h"

#include "WebUiServer.h"

#include "gsxStruct.h"

//#include "iFlyConnect.h"
//#include "MenuReader.h"
//#include "TextReader.h"

#include "gsxStruct.h"


#define KS_METAR_SERVER "tgftp.nws.noaa.gov"
#define KS_METAR_PATH "/data/observations/metar/stations/"
#define KS_TAF_PATH "/data/forecasts/taf/stations/"


WebUiServer::WebUiServer(Wt::WServer& server)
  : server_(server),
    stop_(false)
{
    tempDepMetarString = "";
    tempDepTafString = "";
    tempArrMetarString = "";
    tempArrTafString = "";
    tempInfoString = "( -> )";
    acDepIcao = "";
    acArrIcao = "";
    stateDoor1 = 0;
    stateDoor2 = 0;
    stateDoor3 = 0;
    stateGroundPower = 0;
    stateGroundAir = 0;

    //thread_ = boost::thread(boost::bind(&SimpleWebServer::run, this));

}

WebUiServer::~WebUiServer()
{
    stop_ = true;
    //thread_.join();
}

bool WebUiServer::connect(Client *client,
			       const WebUiEventCallback& handleEvent)
{
  std::unique_lock<std::recursive_mutex> lock(mutex_);

  if (clients_.count(client) == 0) {
    ClientInfo clientInfo;
  
    clientInfo.sessionId = Wt::WApplication::instance()->sessionId();
    clientInfo.eventCallback = handleEvent;

    clients_[client] = clientInfo;

    return true;
  } else
    return false;
}

bool WebUiServer::disconnect(Client *client)
{
  std::unique_lock<std::recursive_mutex> lock(mutex_);

  return clients_.erase(client) == 1;
}

void WebUiServer::postWebUiEvent(const WebUiEvent& event)
{
  std::unique_lock<std::recursive_mutex> lock(mutex_);

  Wt::WApplication *app = Wt::WApplication::instance();

  for (ClientMap::const_iterator i = clients_.begin(); i != clients_.end();
       ++i) {

      server_.post(i->second.sessionId,
          std::bind(i->second.eventCallback, event));
  }
}

void WebUiServer::sendButtonPress(int pressButtonValue)
{
    if (stop_)
    {
        return;
    }

    std::unique_lock<std::recursive_mutex> lock(mutex_);

}

int WebUiServer::GetWeather(int _wxrType, std::string& _icao, std::string& _resultWxr)
{
    return 0;
}



