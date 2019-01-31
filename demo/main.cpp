#include "stdafx.h"

#include "base/Cef3/browser/cef_main_handler.h"
#include "main/WndSimpleFrame.h"

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
    // ��Դ����

    DuiLib::CPaintManagerUI::SetResourceType(DuiLib::UILIB_FILE);
    //DuiLib::CPaintManagerUI::SetResourceType(DuiLib::UILIB_ZIPRESOURCE);

    // ��Դ·��
    DuiLib::CDuiString strResourcePath = DuiLib::CPaintManagerUI::GetInstancePath();
    // ������Դ
    switch(DuiLib::CPaintManagerUI::GetResourceType())
    {
    case DuiLib::UILIB_FILE:
        {
            strResourcePath += _T("skin\\");
            DuiLib::CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
            // ������Դ������
            DuiLib::CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
            break;
        }
    case DuiLib::UILIB_RESOURCE:
        {
            strResourcePath += _T("skin\\");
            DuiLib::CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
            // ������Դ������
            DuiLib::CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
            break;
        }
    case DuiLib::UILIB_ZIP:
        {
            strResourcePath += _T("skin\\");
            DuiLib::CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
            // ����
            //CPaintManagerUI::SetResourceZip(_T("XXXXXX.zip"), true, _T("XXXXXXX"));
            //CPaintManagerUI::SetResourceZip(_T("XXXXXX.zip"), true);
            // ������Դ������
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
                        // ������Դ������
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

	// ------------------CEF��ʼ��------------------
	CCefMainHandler CefMainHandler;
	if (!CefMainHandler.Initialize(hInstance, TRUE))
	{
		return -1;
	}

	// ------------------duilib��ʼ��------------------
	CPaintManagerUI::SetInstance(hInstance);
    // ��ʼ����Դ
    InitResource();
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Skin\\"));

	if (FAILED(::CoInitialize(NULL)))
	{
		return -1;
	}
	
	// ------------------��ʾ������------------------
	CreateMainWnd<CWndSimpleFrame>(_T("SimpleWnd"));


	// ------------------������Ϣѭ��------------------
	CefMainHandler.RunMessageLoop();
	

	// ------------------�����ر���Դ------------------
	CefMainHandler.Shutdown();

	::CoUninitialize();
	
	return 0;
}
