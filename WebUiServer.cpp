/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "pch.h"

#include "WebUiServer.h"

const Wt::WString WebUiEvent::formattedHTML(const Wt::WString& user,
				       Wt::TextFormat format) const
{
  switch (type_) {
  case Message:{
    Wt::WString result;

    result = Wt::WString("<span class='")
      + ((user == user_) ?
	 "chat-self" :
	 "chat-user")
      + "'>" + Wt::WWebWidget::escapeText(user_) + ":</spasssn>";

    Wt::WString msg
      = (format == Wt::TextFormat::XHTML ? message_ : Wt::WWebWidget::escapeText(message_));

    if (message_.toUTF8().find(user.toUTF8()) != std::string::npos)
      return result + "<span class='chat-highlight'>" + msg + "</span>";
    else
      return result + msg;
  }
  default:
    return "";
  }
}


WebUiServer::WebUiServer(Wt::WServer& server)
  : server_(server)
{ }

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

void WebUiServer::sendMessage(const Wt::WString& user, const Wt::WString& message)
{
  postWebUiEvent(WebUiEvent(user, message));
}

void WebUiServer::postWebUiEvent(const WebUiEvent& event)
{
  std::unique_lock<std::recursive_mutex> lock(mutex_);

  Wt::WApplication *app = Wt::WApplication::instance();

  for (ClientMap::const_iterator i = clients_.begin(); i != clients_.end();
       ++i) {
    /*
     * If the user corresponds to the current application, we directly
     * call the call back method. This avoids an unnecessary delay for
     * the update to the user causing the event.
     *
     * For other uses, we post it to their session. By posting the
     * event, we avoid dead-lock scenarios, race conditions, and
     * delivering the event to a session that is just about to be
     * terminated.
     */
    if (app && app->sessionId() == i->second.sessionId)
      i->second.eventCallback(event);
    else
      server_.post(i->second.sessionId,
                   std::bind(i->second.eventCallback, event));
  }
}

WebUiServer::UserSet WebUiServer::users()
{
  std::unique_lock<std::recursive_mutex> lock(mutex_);

  UserSet result = users_;

  return result;
}

