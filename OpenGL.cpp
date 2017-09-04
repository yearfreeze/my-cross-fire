//
// ����  programking 2009��3��
// ����  http://blog.csdn.net/programking
//=======================================
// OpenGL.cpp: implementation of the OpenGL class.
// ѧ�������Ϸϵ�д���
// ������ E_mail: cqtmL@163.com
//====================================================================
#include "stdafx.h"
#include "OpenGL.h"
#include "myclock.h"
#include "gamemap.h"

extern HWND	hWnd;

extern MYCLOCK c1;

extern GAMEMAP gm;

extern CGLFont myfont;

CString	strTest;  
//////////////////////////////////////////////////////////////////////
OpenGL::OpenGL()
{
	
}

OpenGL::~OpenGL()
{

	CleanUp();
}

//�������ظ�ʽ������RC��ѡ��RC
BOOL OpenGL::SetupPixelFormat(HDC hDC0)
{
	int nPixelFormat;
	hDC=hDC0;
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR), 
	    1,     
	    PFD_DRAW_TO_WINDOW | 
	    PFD_SUPPORT_OPENGL | 
	    PFD_DOUBLEBUFFER, 
	    PFD_TYPE_RGBA,  
	    16,  
	    0, 0, 0, 0, 0, 0, 
	    0,     
	    0, 
	    0,
	    0, 0, 0, 0, 
	    16,   
	    0, 
	    0,  
	    PFD_MAIN_PLANE, 
	    0,
	    0, 0, 0 
	}; 
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
		{ MessageBox(NULL,"û�ҵ����ʵ���ʾģʽ","Error",MB_OK|MB_ICONEXCLAMATION);
	      return FALSE;
		}
	SetPixelFormat(hDC,nPixelFormat,&pfd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);   
	
	return TRUE;
}

//�����ӿ�
//����ӳ�䷽ʽ
void OpenGL::init()
{
	glViewport(0,0,RCwidth,RCheight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective	
		( 54.0f,
		  (GLfloat)RCwidth/(GLfloat)RCheight,
		  0.1f,	
		  3000.0f
		);
	
	//ָ��ģ����ͼ��ջ
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	//��ʼ���ƹ�
	gm.light0();

//====================================================
}

void OpenGL::SetViewSize(int w,int h)
{
	RCwidth=w;
	RCheight=h;
}

void OpenGL::initMenu()
{
	glViewport(0, 0, RCwidth, RCheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, RCwidth, 0, RCheight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//��Ⱦ���
void OpenGL::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();		
	text();	//���ʱ�Ӽ�ʱ
	gm.show();	//

	glFlush();		
	SwapBuffers(hDC);	
}

void OpenGL::CleanUp()
{ 
	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
}

void OpenGL::text()
{
	char str[128];

	sprintf(str, "C %d ",c1.iNumClip);
	myfont.settext(400,550,str,FONT0,1,1,1.0f);
}


