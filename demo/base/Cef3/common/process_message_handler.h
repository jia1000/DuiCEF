#pragma once

#include "cef_client_handler.h"

//using CefProcessTask = std::function<void(CefRefPtr<CefProcessMessage> message)>;
typedef std::function<void(CefRefPtr<CefProcessMessage> message)> CefProcessTask;

class CProcessMessageHandler: public CefBase
{
public:
	virtual void OnProcessMessageReceived(CefRefPtr<CefProcessMessage> message);

	IMPLEMENT_REFCOUNTING(CProcessMessageHandler);
};