#include "stdafx.h"

#include "cef_client_handler.h"

#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#include "../browser/main_message_loop.h"
#include "../control/UICefBrowser.h"

void CCefClientHandler::CreateBrowser(HWND hParentWnd, RECT rc)
{
	CEF_REQUIRE_MAIN_THREAD();

	CefBrowserSettings settings;
	CefWindowInfo info;
	info.SetAsChild(hParentWnd, rc);

	CefBrowserHost::CreateBrowser(info, this, _CEF_BLANK_, settings, NULL);
}

void CCefClientHandler::CloseBrowser(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_MAIN_THREAD();

	browser->GetHost()->CloseBrowser(true);
}

void CCefClientHandler::Close()
{
	CEF_REQUIRE_MAIN_THREAD();

	m_pOwner = nullptr;
}

bool CCefClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser
	, CefProcessId source_process
	, CefRefPtr<CefProcessMessage> message)
{
	CEF_REQUIRE_UI_THREAD();

	CefRefPtr<CefProcessMessage> msg = message->Copy();
	exec_on_main_thread([this, msg]
	{
		if (m_pOwner != nullptr)
		{
			m_pOwner->OnProcessMessageReceived(msg);
		}
	});

	return true;
}

void CCefClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	//exec_on_main_thread([this, browser]
	//{
	//	m_pOwner->OnAfterCreated(browser);
	//});
}

void CCefClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser
	, CefRefPtr<CefFrame> frame
	, CefRefPtr<CefContextMenuParams> params
	, CefRefPtr<CefMenuModel> model)
{
	//不显示右键菜单
#ifndef _DEBUG
	for (int i = 0; i < model->GetCount(); i++)
	{
		model->RemoveAt(i);
	}
#endif
}

void CCefClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
	CEF_REQUIRE_UI_THREAD();
}

void CCefClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser
	, CefRefPtr<CefFrame> frame
	, int httpStatusCode)
{
	CEF_REQUIRE_UI_THREAD();

	exec_on_main_thread([this, browser]
	{
		m_pOwner->OnAfterCreated(browser);
	});
}

void CCefClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser
	, CefRefPtr<CefFrame> frame
	, ErrorCode errorCode
	, const CefString& errorText
	, const CefString& failedUrl)
{
	CEF_REQUIRE_UI_THREAD();
}

bool CCefClientHandler::OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask)
{
	CEF_REQUIRE_UI_THREAD();

	// Forbid dragging of link URLs.
	if (mask & DRAG_OPERATION_LINK)
		return true;

	return false;
}

void CCefClientHandler::exec_on_main_thread(std::function<void(void)> task)
{
	if (m_pOwner == nullptr)
		return;

	if (!CEF_RUN_ON_MAIN_THREAD())
	{
		CEF_POST_MAIN_THREAD_TASK(base::Bind(&CCefClientHandler::exec_on_main_thread, this, task)); 
	}
	else
	{
		task();
	}
}
bool CCefClientHandler::OnBeforePopup(
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
	bool* no_javascript_access) {
		CEF_REQUIRE_IO_THREAD();

		//// Return true to cancel the popup window.
		//return !CreatePopupWindow(browser, false, popupFeatures, windowInfo, client,
		//                          settings);

#ifdef OPEN_NEW_WINDOW
		// 创建子窗口的逻辑交由TrayWindowManager
		exec_on_main_thread([this, target_url]
		{
			TrayWindowManager::getInstance()->CreateRootWindowAsPopup(target_url);
		});
#else
		//////////////////////////////////////////////////////////////////////////
		// 禁止弹出新窗口，在当前窗口内跳转
		switch (target_disposition) 
		{ 
		case WOD_NEW_FOREGROUND_TAB: 
		case WOD_NEW_BACKGROUND_TAB: 
		case WOD_NEW_POPUP: 
		case WOD_NEW_WINDOW: 
			browser->GetMainFrame()->LoadURL(target_url); 
			return true; 
			//cancel create 
		} 
		//////////////////////////////////////////////////////////////////////////
#endif
		return true;
	}


void CCefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	
	// Remove and delete message router handlers.
	//auto it = message_handler_set_.begin();
	//for (; it != message_handler_set_.end(); ++it) {
	//	message_router_->RemoveHandler(*(it));
	//	delete *(it);
	//}
	//message_handler_set_.clear();
	//message_router_ = NULL;
}
