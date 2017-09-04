// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

//mfc
#include <afxwin.h>	

#include <windows.h>

#include "resource.h"

#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
//////////////////////
#include "filereport.h"

//3ds
#include "3ds.h"
//md2
#include "md2.h"
#include "anmobj.h"
//mdl
#include "MDLFormat.h"//
#include "MDLModel.h"//


#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "opengl32.lib")
#pragma comment( lib, "glu32.lib")
#pragma comment( lib, "glaux.lib")	

#define RAND_COORD(x)   ((float)rand()/RAND_MAX * (x))
#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)
// camera

//#define MAP		40
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// sky
#define MAP_W       32    
#define MAP_SCALE   24.0f    
#define MAP			MAP_W*MAP_SCALE/2

// window
#define WINH 800
#define WINW 600

#define GAME_NULL -1
#define GAME_PRE_ANI 0
//��Ϸ��ʼ�˵�
#define GAME_MENU 1
//��Ϸ������
#define GAME_IN 2
#define GAME_WIN 3
#define GAME_FAIL 4
//�����ͼǰ��ʼ��
#define GAME_IN_INIT 5
#define GAME_PASS 6
#define GAME_ERR 10

//��������ʱ�� ��λ: ��
#define TIME_PRE_ANI 4

//�˵�����
#define XMENU 150
#define YMENU 100
#define WIDTH_MENU 200
#define HEIGHT_MENU 60
#define MENU_NUM 3
#define Y_2MENU 100

//MENU�˵�����
#define MENU_START 2
#define MENU_HELP 1
#define MENU_QUIT 0

#define MAX_MAP_OBJECT 20

//md2
#define ENEMY_HEALTH 100

#define GAME_TEX_NUM 20

#define PI		(3.14159265359f)

//����
#define FONT0 0
#define FONT1 1
#define FONT2 2
#define FONT3 3
#define FONT4 4
#define FONT5 5
//ģ�Ͷ���
#define ENEMY_DEATH_ANI 17
#define ENEMY_RUN_ANI   1


//�����ƶ�������
#define MOVE_X 0
#define MOVE_Y 1
#define MOVE_Z 2

#define MAX_MATCH 3
//��ʾWORLD X��ʱ��
#define TIME_WORLD_INFO 60
//��ʾ�ӵ�ײ��
#define TIME_BOMB 20

//��Ч���ӵ�����
#define INVALID_ROCKET 1000
//��ͼ·��
//#define PATH_MAP "data\\map\\map.txt"
#define PATH_MAP "data/map/map.txt"

//����߶�
#define VIEW_HEIGHT 1.8f
//enemyģ��ԭ��߶�
#define ENEMY_MODEL_Y 1.4f

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
