#pragma once

/************************************************************************/
/*
	CWndFrameBase窗口
*/
/************************************************************************/

#define KEY_MAINWINDOW "main"
#define KEY_CHILDWINDOW "child"
#define KEY_PRINTWINDOW "print"

#include <algorithm>
#include "base/Cef3/common/client_callback_handler.h"

class CDuilibWindowDemo;

class CWndFrameBase
	: public WindowImplBase
	, public ICallbackDelegate
{
public:
	// This interface is implemented by the owner of the CWndFrameBase. The methods
	// of this class will be called on the main thread.
	class Delegate {
	public:
		// Called to retrieve the CefRequestContext for browser. Only called for
		// non-popup browsers. May return NULL.
		virtual CefRefPtr<CefRequestContext> GetRequestContext(
			CWndFrameBase* root_window) = 0;
		// Called to exit the application.
		virtual void OnExit(CWndFrameBase* root_window) = 0;
		// Called when the RootWindow has been destroyed.
		virtual void OnRootWindowDestroyed(CWndFrameBase* root_window) = 0;

	protected:
		virtual ~Delegate() {}
	};

	CWndFrameBase(void);
	virtual ~CWndFrameBase(void);
	
	virtual LPCTSTR GetWindowClassName(void) const { return  L"{A5F70D30-7F37-4567-A829-7F1F9E4F659E}"; }
    virtual CDuiString GetSkinFile();
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();
	//消息集成处理函数
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetMinInfo(int cx, int cy);
	void OnPrepare();
	virtual string GetBrowserWindowID() {return KEY_MAINWINDOW;}
	virtual void Minimize();
	virtual void Maximize();
	virtual void Restore();
	//virtual void Exit();
	virtual void LoadUrl(wstring url);

	// ICallbackDelegate methods
	void OnSetAddress(const std::wstring& url) OVERRIDE;
	// Set the window title.
	void OnSetTitle(const std::wstring& title) OVERRIDE;
	// Set fullscreen mode.
	void OnSetFullscreen(bool fullscreen) OVERRIDE;
	// Set the loading state.
	void OnSetLoadingState(bool isLoading,
		bool canGoBack,
		bool canGoForward) OVERRIDE;
	// Set the draggable regions.
	void OnSetDraggableRegions(
		const std::vector<CefDraggableRegion>& regions) OVERRIDE;
	// Set load error message log
	void OnLoadError(CefRefPtr<CefFrame> frame
		, int errorCode
		, const CefString& errorText
		, const CefString& failedUrl);
	// Minimize window.
	void OnMinimized() OVERRIDE;
	// Exit application.
	void OnExit() OVERRIDE;

	bool IsDragRect(POINT pt);
	void SetDelegate(CWndFrameBase::Delegate* wndDelegate){ m_pDelegate = wndDelegate;}
    void OnFinalMessage(HWND hWnd);

	virtual void NotifyWindowDestroyed();
	wstring GetUrl();
	wstring GetTitle();

protected:
	CWndFrameBase::Delegate* m_pDelegate;
	CCefBrowserUI* m_pBrowserUI;

private:
	// 定义导航和地址栏控件
	CButtonUI* m_pOkBtn;
	CEditUI* m_pAddressEdit;
	CButtonUI* m_pBackBtn;
	CButtonUI* m_pForwardBtn;
	CButtonUI* m_pRefreshBtn;
    CButtonUI* m_pRefreshBtnNew;
	CButtonUI* m_pStopBtn;
	// 定义导航和地址栏控件结束

    CLabelUI* m_title_label;

    CDuilibWindowDemo* pwnd;
};

	