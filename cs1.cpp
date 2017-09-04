//
// ����  programking 2009��3��
// ����  http://blog.csdn.net/programking
//=======================================
// cs1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "myclock.h"
#include "opengl.h"
#include "gamemap.h"
#include "glfont.h"

#include "filereport.h"

//ʱ��
MYCLOCK c1;

OpenGL* m_OpenGL;
CGLFont myfont;

HDC		hDC;
HWND	hWnd=NULL;
//���ڿ��
int		WinWidth = WINH;
int		WinHeight= WINW;

GAMEMAP gm;

//����Ƿ���
int Lbutdown=0;

void GameLoop()
{
	MSG msg; 
    BOOL fMessage;

    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
    while(msg.message != WM_QUIT)
    {   
		fMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

        if(fMessage)
		{
			TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
        else  
		{
			if(c1.clockcount())
			{
				m_OpenGL->Render();				
				gm.check();
			}
			else
			{
				_sleep(0);
			}
		}
    }
}

LRESULT WINAPI MsgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )
{
	switch(message)
	{
		case WM_CREATE:	
			//��ȡDC
			hDC = GetDC(hWnd);	
			//�������ظ�ʽ��ѡ��RC
			m_OpenGL->SetupPixelFormat(hDC);
			//��Ϸ���ݳ�ʼ��
			gm.init();
			return 0;	
			break;

		case WM_CLOSE:			
			m_OpenGL->CleanUp();
			PostQuitMessage(0);
			return 0;
			break;

		case WM_SIZE:		
			WinHeight = HIWORD(lParam);
			WinWidth  = LOWORD(lParam);
			if (WinHeight==0)	
				WinHeight=1;
			//�����ӿں�ӳ�䷽ʽ
			m_OpenGL->SetViewSize(WinWidth,WinHeight);
			gm.initView();					
			return 0;
			break;

		case WM_DESTROY:
			delete m_OpenGL;
            PostQuitMessage(0);
            return 0;
			break;

        case WM_KEYUP:	
            switch (wParam)
            {
			case 0x70:	//F1
				ShowWindow(hWnd, SW_MINIMIZE);	
				break;		

			default:
				gm.keyupproc(wParam);
				break;
            } 
			//ע��break
			break;

		case WM_LBUTTONDOWN: 
			Lbutdown=1;
			break;

		case WM_MOUSEMOVE:
			gm.mouseproc(lParam);
			break;
		
		case WM_LBUTTONUP:
			Lbutdown=0;
			gm.lbuttonproc(lParam);
			break;

		default:			
			break;
	}

	return (DefWindowProc(hWnd, message, wParam, lParam));
}
 
int APIENTRY WinMain(HINSTANCE hInst,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	DWORD	dwStyle;

	int	nX=0;
	int nY=0;

	//dwExStyle=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
	dwStyle=WS_OVERLAPPEDWINDOW;			

	int wid=GetSystemMetrics(SM_CXSCREEN);
	int hei=GetSystemMetrics(SM_CYSCREEN);

	nX=(wid-WinWidth)/2;
	nY=(hei-WinHeight)/2;	

	//-------------------------------------------------------------------
	//begin �ļ����
	char errstr[]="ȱ�������ļ������޸���������Ϸ";
	if(!gm.haveDataFile())
	{
		MessageBox(NULL,errstr, "CS",MB_OK);
		return 0;
	}
	//end �ļ����

	char cc[]="cs";

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), 
		CS_CLASSDC,
		MsgProc,
		0L, 0L, 
		GetModuleHandle(NULL), 
		//
		LoadIcon(hInst, (LPCTSTR)IDI_ICON1),
		LoadCursor(NULL, IDC_ARROW),
		NULL, NULL,
        cc, 
		LoadIcon(hInst, (LPCTSTR)IDI_ICON3)
	};
    
	RegisterClassEx( &wc );

	m_OpenGL=new OpenGL();//

	hWnd = CreateWindowEx(NULL,
		cc,
		"CS : �����й� ��ǿ��Ϣ",
		WS_POPUP,	
		//WS_OVERLAPPEDWINDOW,
		nX, nY,
		WinWidth, 
		WinHeight,
		NULL,NULL,
		hInst,NULL);

	//ShowWindow( hWnd, SW_SHOWDEFAULT );	//Ĭ�ϴ�С
	ShowWindow( hWnd, SW_MAXIMIZE );	//�����ʾ
	UpdateWindow( hWnd );	

	// init����������ʱ
	c1.ReStart(TIME_PRE_ANI);

	GameLoop();	
    
	return 0;
}



