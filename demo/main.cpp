#include "stdafx.h"

#include "base/Cef3/browser/cef_main_handler.h"
#include "main/WndSimpleFrame.h"
#include "main/WndFrameBase.h"

template <class T>
CWindowWnd* CreateMainWnd(LPCTSTR pstrName)
{
	CWindowWnd *mainFrame = new T;
	mainFrame->Create(NULL, pstrName
		, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
		, WS_EX_OVERLAPPEDWINDOW
		, 0, 0, 0, 0);
	mainFrame->CenterWindow();
	mainFrame->ShowWindow();

	return mainFrame;
}

void InitResource()
{
    // 资源类型

    DuiLib::CPaintManagerUI::SetResourceType(DuiLib::UILIB_FILE);
    //DuiLib::CPaintManagerUI::SetResourceType(DuiLib::UILIB_ZIPRESOURCE);

    // 资源路径
    DuiLib::CDuiString strResourcePath = DuiLib::CPaintManagerUI::GetInstancePath();
    // 加载资源
    switch(DuiLib::CPaintManagerUI::GetResourceType())
    {
    case DuiLib::UILIB_FILE:
        {
            strResourcePath += _T("skin\\");
            DuiLib::CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
            // 加载资源管理器
            DuiLib::CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
            break;
        }
    case DuiLib::UILIB_RESOURCE:
        {
            strResourcePath += _T("skin\\");
            DuiLib::CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
            // 加载资源管理器
            DuiLib::CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
            break;
        }
    case DuiLib::UILIB_ZIP:
        {
            strResourcePath += _T("skin\\");
            DuiLib::CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
            // 加密
            //CPaintManagerUI::SetResourceZip(_T("XXXXXX.zip"), true, _T("XXXXXXX"));
            //CPaintManagerUI::SetResourceZip(_T("XXXXXX.zip"), true);
            // 加载资源管理器
            DuiLib::CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
            break;
        }
    case DuiLib::UILIB_ZIPRESOURCE:
        {
            strResourcePath += _T("skin\\");
            DuiLib::CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
            HRSRC hResource = ::FindResource(DuiLib::CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
            if( hResource != NULL ) {
                DWORD dwSize = 0;
                HGLOBAL hGlobal = ::LoadResource(DuiLib::CPaintManagerUI::GetResourceDll(), hResource);
                if( hGlobal != NULL ) {
                    dwSize = ::SizeofResource(DuiLib::CPaintManagerUI::GetResourceDll(), hResource);
                    if( dwSize > 0 ) {
                        DuiLib::CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
                        // 加载资源管理器
                        DuiLib::CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
                    }
                }
                ::FreeResource(hResource);
            }
        }
        break;
    }
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{ 
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ------------------CEF初始化------------------
	CCefMainHandler CefMainHandler;
	if (!CefMainHandler.Initialize(hInstance, TRUE))
	{
		return -1;
	}

	// ------------------duilib初始化------------------
	CPaintManagerUI::SetInstance(hInstance);
    // 初始化资源
    InitResource();
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Skin\\"));

	if (FAILED(::CoInitialize(NULL)))
	{
		return -1;
	}
	
	// ------------------显示主窗口------------------
	//CreateMainWnd<CWndSimpleFrame>(_T("SimpleWnd"));
    
    CWndFrameBase *mainFrame = new CWndFrameBase();
    //CDuilibWindowDemo *mainFrame = new CDuilibWindowDemo();
    mainFrame->Create(NULL, _T("测试窗口")
        , WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
        , WS_EX_OVERLAPPEDWINDOW
        , 0, 0, 0, 0);
    mainFrame->CenterWindow();
    mainFrame->ShowWindow();

    

	// ------------------开启消息循环------------------
	CefMainHandler.RunMessageLoop();
	

	// ------------------结束关闭资源------------------
	CefMainHandler.Shutdown();

	::CoUninitialize();
	
	return 0;
}
