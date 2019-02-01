#pragma once

#include <queue>

#include "include/cef_client.h"

#include "../common/process_message_handler.h"
#include "../common/cef_client_handler.h"
#include "base/Cef3/common/client_callback_handler.h"

//using CefCacheTask = std::function<void(void)>;
typedef std::function<void(void)> CefCacheTask;

class CCefBrowserUI : public CControlUI
{
public:
	CCefBrowserUI(ICallbackDelegate* callbackDelegate);
	virtual ~CCefBrowserUI();

	// Control
	void DoInit();
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetVisible(bool bVisible);
	void SetInternVisible(bool bVisible);


	// CefClientHandler�ص��ӿ�
	void OnProcessMessageReceived(CefRefPtr<CefProcessMessage> message);
	void OnAfterCreated(CefRefPtr<CefBrowser> browser);


	// �Զ���ӿ�
	void Navigate2(CefString url);
	void RunJs(CefString JsCode);

    // CefClientHandler�ص��ӿ�
    void OnAddressChange(const CefString& url);
    void OnTitleChange(const CefString& title);
    void OnFullscreenModeChange(bool fullscreen);

	// ������Ϣ����
	void SetProcessMessageHandler(CefRefPtr<CProcessMessageHandler> pHandler);

    CefRefPtr<CefBrowser> GetCefBrowser() { return m_pBrowser; }
private:
	void resize();

private:
	
	CefRefPtr<CCefClientHandler>		m_pClientHandler;
	CefRefPtr<CefBrowser>				m_pBrowser;
	CefRefPtr<CProcessMessageHandler>	m_pProcessMessageHandler;

	// browser�������ǰ���������
	std::queue<CefCacheTask>	m_AfterCreatedCacheTasks;

    ICallbackDelegate*						m_CallbackDelegate;
};