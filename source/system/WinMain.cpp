#include	"iextreme.h"
#include	"Framework.h"
#include	"../sceneMain.h"
#include	"Control.h"
#include	"../sceneTitle.h"
#include	"../DataOwner.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

char*	AppTitle = "IEX2010";

BOOL	bFullScreen = FALSE;
DWORD	ScreenMode = SCREEN720p;

Framework*	MainFrame = NULL;


//*****************************************************************************************************************************
//		アプリケーション初期化
//*****************************************************************************************************************************

BOOL	InitApp(HWND hWnd)
{
	//	IEXシステム初期化
	IEX_Initialize(hWnd, bFullScreen, ScreenMode);
	IEX_InitAudio();
	IEX_InitInput();

	//	システムの初期化
	SYSTEM_Initialize();
	//	メインフレームワーク生成
	MainFrame = new Framework();
	//	初期シーン登録
	DataOwner::GetInst()->Init();
	DataOwner::GetInst()->stageNo = 1;
	MainFrame->ChangeScene(new sceneTitle());
	return TRUE;
}

//*****************************************************************************************************************************
//		ウィンドウプロシージャ
//*****************************************************************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	const float leftWndFrame = 8.0f;
	const float topWndFrame = 29;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONUP:
		//Mouse::lbClick = true;
		//Mouse::cursor.x = LOWORD(lParam);
		//Mouse::cursor.y = HIWORD(lParam);
		//ScreenToClient(hWnd, &Mouse::cursor);

		////窓位置と縁による調整
		//RECT rc;
		//GetWindowRect(iexSystem::Window, &rc);

		//Mouse::cursor.x += rc.left - leftWndFrame;
		//Mouse::cursor.y += rc.top - topWndFrame;

		

		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;
		case VK_F1:		iexSystem::OpenDebugWindow(); return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//*****************************************************************************************************************************
//		ウィンドウ作成
//*****************************************************************************************************************************
HWND InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND			hWnd;

	//	スクリーンサイズ取得
	RECT	WindowSize;
	iexSystem::GetScreenRect(ScreenMode, WindowSize);

	//	ウィンドウクラス設定
	WNDCLASS	wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "IEX2010";
	RegisterClass(&wc);

	//	ウィンドウ作成
	if (!bFullScreen){
		AdjustWindowRect(&WindowSize, WS_OVERLAPPEDWINDOW, FALSE);
		hWnd = CreateWindow("IEX2010", AppTitle, WS_OVERLAPPEDWINDOW,
			0, 0, WindowSize.right - WindowSize.left, WindowSize.bottom - WindowSize.top,
			NULL, NULL, hInstance, NULL);
	}
	else {
		hWnd = CreateWindow("IEX2010", AppTitle, WS_POPUP, 0, 0, WindowSize.right, WindowSize.bottom, NULL, NULL, hInstance, NULL);
		ShowCursor(FALSE);
	}
	if (!hWnd) return NULL;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

//*****************************************************************************************************************************
//		WinMain
//*****************************************************************************************************************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetBreakAlloc(543);

	MSG		msg;
	HWND	hWnd;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000) bFullScreen = TRUE;

	hWnd = InitWindow(hInstance, nCmdShow);
	InitApp(hWnd);

	//	メインループ
	for (;;){
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (MainFrame->Update()) MainFrame->Render();
		}
	}

	//	全解放	
	delete	MainFrame;
	SYSTEM_Release();
	IEX_ReleaseAudio();
	iexParticle::Release();
	iexSystem::CloseDebugWindow();
	IEX_ReleaseInput();
	IEX_Release();

	return 0;
}
