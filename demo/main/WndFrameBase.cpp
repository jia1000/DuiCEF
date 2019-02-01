#include "stdafx.h"

#include "WndFrameBase.h"
#include "base/Cef3/control/UICefBrowser.h"
#include "base/Cef3/browser/cef_main_handler.h"
#include <regex>

//DUI_BEGIN_MESSAGE_MAP(CWndFrameBase, WindowImplBase)
//	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
//DUI_END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
CWndFrameBase::CWndFrameBase(void)
{
	m_pBrowserUI = new CCefBrowserUI(this);
}

CWndFrameBase::~CWndFrameBase(void)
{

}

void CWndFrameBase::OnPrepare()
{

}

CDuiString CWndFrameBase::GetSkinFile()
{
    return _T("main/WndSimpleFrame.xml");
}

void CWndFrameBase::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit"))
		OnPrepare();
	else if (msg.sType == _T("click"))
	{
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("btn_sysmin")) == 0)
		{
			Minimize();
		}
        if (_tcscmp(pszCtrlName, _T("btn_sysmax")) == 0)
        {            
            Maximize();
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
            if( pControl ) pControl->SetVisible(false);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
            if( pControl ) pControl->SetVisible(true);
        }
        if (_tcscmp(pszCtrlName, _T("btn_restore")) == 0)
        {
            Restore();
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
            if( pControl ) pControl->SetVisible(true);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
            if( pControl ) pControl        ->SetVisible(false);
        }
		else if (_tcscmp(pszCtrlName, _T("btn_sysclose")) == 0)
		{
            
            Close(IDOK);
		}
		else if (_tcscmp(pszCtrlName, _T("btn_ok")) == 0)
		{

		}
		else if (_tcscmp(pszCtrlName, _T("btn_back")) == 0)
		{            
            m_pBrowserUI->GetCefBrowser()->GoBack();
			//if (m_pBrowserUI && m_pBrowserUI->CanGoBack())
			//	m_pBrowserUI->GoBack();
		}
		else if (_tcscmp(pszCtrlName, _T("btn_forward")) == 0)
		{
            m_pBrowserUI->GetCefBrowser()->GoForward();
			//if (m_pBrowserUI && m_pBrowserUI->CanGoForward())
			//	m_pBrowserUI->GoForward();
		}
		else if (_tcscmp(pszCtrlName, _T("btn_refresh")) == 0
            || _tcscmp(pszCtrlName, _T("btn_refresh_new")) == 0)
		{
            CDuiString edit = m_pAddressEdit->GetText();
            if (!edit.IsEmpty()) {
	            if (m_pBrowserUI)
	                m_pBrowserUI->GetCefBrowser()->GetMainFrame()->LoadURL(edit.GetData());
            }
		}
		else if (_tcscmp(pszCtrlName, _T("btn_stop")) == 0)
		{
            m_pBrowserUI->GetCefBrowser()->StopLoad();
			//if (m_pBrowserUI)
			//	m_pBrowserUI->StopLoad();
		}
	}
	else if (msg.sType == _T("return"))
	{
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("edit_address")) == 0)
		{
			wstring strUrl = msg.pSender->GetText().GetData();
			if (strUrl.length() == 0){
				return;
			}

			if (strUrl.length() < 5){
				strUrl = L"http://" + strUrl;
			}
			else{
				wstring start = strUrl.substr(0,5);
				transform(start.begin(), start.end(), start.begin(), toupper);

				if (start != L"HTTP:" && start != L"HTTPS"){
					strUrl = L"http://" + strUrl;
				}
			}

			//CGLogger::Debug("load url: " + WStringToString(strUrl));
			LoadUrl(strUrl);	
		}
	}
}

CControlUI* CWndFrameBase::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{
	if( _tcsicmp(pstrClass, _T("Cef")) == 0){
		return m_pBrowserUI;
	}
}

void CWndFrameBase::Minimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void CWndFrameBase::Maximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void CWndFrameBase::Restore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

void CWndFrameBase::SetMinInfo(int cx, int cy)
{
	m_pm.SetMinInfo(cx, cy);
}

void CWndFrameBase::LoadUrl(wstring url)
{
	if (m_pBrowserUI){
		if (url.empty()) {
			std::wstring text = _T("address invalid");
			return;
		}
		m_pBrowserUI->Navigate2(url);
	}
}

wstring CWndFrameBase::GetUrl()
{
	//if (m_pBrowserUI){
	//	return m_pBrowserUI->GetCurrentUrl();
	//}
	return L"";
}

wstring CWndFrameBase::GetTitle()
{
	if (m_title_label){
		return m_title_label->GetText().GetData();
	}
	return L"";
}

LRESULT CWndFrameBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg,wParam,lParam);
}

void CWndFrameBase::InitWindow()
{
	m_pOkBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_ok")));
	m_pAddressEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_address")));
	m_pBackBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_back")));
	m_pForwardBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_forward")));
	m_pRefreshBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_refresh")));
	m_pRefreshBtnNew = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_refresh_new")));
	m_pStopBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_stop")));
 	m_title_label = static_cast<CLabelUI*>(m_pm.FindControl(L"title_label"));
}

LRESULT CWndFrameBase::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed){

		bool isMaximized = !bZoomed;

		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
		if( pControl ) pControl->SetVisible(!isMaximized);
		pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
		if( pControl ) pControl->SetVisible(isMaximized);
	}
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// ICallbackDelegate events
void CWndFrameBase::OnSetAddress(const std::wstring& url)
{
	if (m_pAddressEdit != nullptr){
		m_pAddressEdit->SetText(url.c_str());
	}
}

void CWndFrameBase::OnSetTitle(const std::wstring& title) 
{
	if (m_title_label){
		m_title_label->SetText(title.c_str());
	}
	SetWindowText(m_hWnd, title.c_str());
}

void CWndFrameBase::OnSetFullscreen(bool fullscreen) 
{
	if (fullscreen){
		Maximize();
	}else{
		Restore();
	}
}

void CWndFrameBase::OnSetLoadingState(bool isLoading,
									  bool canGoBack,
									  bool canGoForward)
{
	if (m_pBackBtn){
		m_pBackBtn->SetEnabled(canGoBack);
	}
	if (m_pForwardBtn){
		m_pForwardBtn->SetEnabled(canGoForward);
	}
	if (m_pRefreshBtn){
		m_pRefreshBtn->SetEnabled(!isLoading);
	}
    if (m_pRefreshBtnNew){
        m_pRefreshBtnNew->SetEnabled(!isLoading);
    }
	if (m_pStopBtn){
		m_pStopBtn->SetEnabled(isLoading);
	}
}


//////////////////////////////////////////////////////////////////////////
// ICallbackDelegate methods
void CWndFrameBase::OnSetDraggableRegions(
	const std::vector<CefDraggableRegion>& regions)
{
	
}

// Set load error message log
void CWndFrameBase::OnLoadError(CefRefPtr<CefFrame> frame
				 , int errorCode
				 , const CefString& errorText
				 , const CefString& failedUrl)
{

}

void CWndFrameBase::OnMinimized()
{
	Minimize();
}

void CWndFrameBase::OnExit()
{
	Close(IDOK);
}
//////////////////////////////////////////////////////////////////////////

void CWndFrameBase::OnFinalMessage(HWND hWnd)
{
    //NotifyWindowDestroyed();
    PostQuitMessage(IDOK);
    //__super::OnFinalMessage(hWnd);
    delete this;
}

void CWndFrameBase::NotifyWindowDestroyed()
{
	if (m_pDelegate)
		m_pDelegate->OnRootWindowDestroyed(this);
}
