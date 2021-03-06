#pragma once

#include "include/cef_client.h"
#include <functional>

#define _CEF_BLANK_ _T("about:blank")

class CCefBrowserUI;

class CCefClientHandler
	: public CefClient
	, public CefLifeSpanHandler
	, public CefContextMenuHandler
	, public CefLoadHandler
	, public CefDragHandler
    , public CefDisplayHandler
{
public:
	CCefClientHandler(CCefBrowserUI* pOwner) :m_pOwner(pOwner){}

	CefRefPtr<CefLifeSpanHandler>		GetLifeSpanHandler()		{ return this; }
	CefRefPtr<CefContextMenuHandler>	GetContextMenuHandler()		{ return this; }
	CefRefPtr<CefLoadHandler>			GetLoadHandler()			{ return this; }
	CefRefPtr<CefDragHandler>			GetDragHandler()			{ return this; }
    CefRefPtr<CefDisplayHandler>		GetDisplayHandler()			{ return this; }

	void CreateBrowser(HWND hParentWnd, RECT rc);
	void CloseBrowser(CefRefPtr<CefBrowser> browser);
	void Close();

	bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser
		, CefProcessId source_process
		, CefRefPtr<CefProcessMessage> message) OVERRIDE;


	// CefLifeSpanHandler methods:
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	bool OnBeforePopup(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) OVERRIDE;
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;


	// CefContextMenuHandler methods:
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefFrame> frame
		, CefRefPtr<CefContextMenuParams> params
		, CefRefPtr<CefMenuModel> model) OVERRIDE;


	// CefLoadHandler methods:
	void OnLoadStart(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefFrame> frame) OVERRIDE;

	void OnLoadEnd(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefFrame> frame
		, int httpStatusCode) OVERRIDE;

	void OnLoadError(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefFrame> frame
		, ErrorCode errorCode
		, const CefString& errorText
		, const CefString& failedUrl) OVERRIDE;


	// CefDragHandler Event
	typedef cef_drag_operations_mask_t DragOperationsMask;

	bool OnDragEnter(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefDragData> dragData
		, DragOperationsMask mask)OVERRIDE;

    // CefDisplayHandler methods
    void OnAddressChange(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& url) OVERRIDE;
    void OnTitleChange(CefRefPtr<CefBrowser> browser,
        const CefString& title) OVERRIDE;
    void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
        bool fullscreen) OVERRIDE;
    bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
        const CefString& message,
        const CefString& source,
        int line) OVERRIDE;

private:
	void exec_on_main_thread(std::function<void(void)> task);

private:
	CCefBrowserUI* m_pOwner;

	IMPLEMENT_REFCOUNTING(CCefClientHandler);
};