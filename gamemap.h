#ifndef __GAMEMAP
#define __GAMEMAP

#include "mdlobj.h"

typedef struct tagRECT_F
{
    float   left;
    float   top;
    float   right;
    float   bottom;
} RECT_F, *PRECT_F;

struct ROLE
{
	int id;	//ID
	//����
	int x;
	int y;
	int z;

	int w;
	int h;
	
	int iframe;
	int iCurrentAni;	

	//�ƶ�����
	int iAngle;
	int iMoveDir;
	int iMoveSpeed;

	//��������
	int hp;//����ֵ
	int hpMax;

	int show;	//�Ƿ���ʾ

	int r;//��ת�Ƕ�
};

class GAMEMAP
{
public:
	GAMEMAP();
	~GAMEMAP();
	
	void init();
	//
	void initView();
	void initMap();
	int haveDataFile();
	//��ʾ
	void show();
	void showpreani();
	void showmenu();
	
	void mouseproc(int lparam);
	void lbuttonproc(int lparam);
	void keyupproc(int keyparam);
	//ʵʱ���
	void check();
	void checkRole();
	void checkFire();
	int RoleInMap(float x,float y,float z);
	//
	void DisplayScene();
	void showmap();
	void showmapBox(float *ppos,float *psize,float *ptex,int itex,int iHastop);
	void light0();
	//����λͼ
	bool LoadT8(char *filename, GLuint &texture);
	//����ͼ
	void texture(UINT textur);			
	void texture0(UINT textur);			
	//��ʾͼƬ
	void tPic(float e);
	void tPic(int x,int y,int w,int h);
	void tPicButton(int x,int y,int w,int h,float ytex);
	void tSquare(float x, float y, float z, float a);
	//fire
	void showFire();
	//
	void showTarget();
	//hittest
	int hittest();
	//
	void showInfo();
	void showEnemyHp();
	void drawMouse();
	//
	int LoadMap();

	////////////////////////////////////////////////////////////
	//data
	int iGameState;//��Ϸ״̬
	int iMatch;//�ڼ���

	//images
	unsigned int g_cactus[GAME_TEX_NUM];
	unsigned int TexBox[GAME_TEX_NUM];
	GLUquadricObj *g_text; 

	//pre ani
	int param1;
	
	//���λ��
	int xmouse;
	int ymouse;
	int xmouseOld;
	int ymouseOld;
	//��ǰѡ��Ĳ˵���
	int iMenu;

	// look
	float	g_eye[3];		//
	float	g_look[3];		//
	float	rad_xz;	
	float	g_Angle;	
	float	g_elev;	
	
	//enemy
	struct ROLE MapEnemyArray[MAX_MAP_OBJECT];
	int iMapEnemyCursor;

	//md2
	anmobj* m_anmobj;

	//map��������
	RECT maprect[MAX_MAP_OBJECT];
	unsigned int  maprectTex[MAX_MAP_OBJECT];
	int iMapRectNum;
	
	//mdl
	mdlobj	m_mdlobj;

	//FIRE
	int IsFire;//�Ƿ����ڷ����ӵ�
	float rocketX;
	float rocketY;
	float rocketZ;  
	//�ӵ�ײ��Ч��
	int iShowBombFrame;

	//
	int iEnemyNum;	//��������
	int iHp;	//�������ֵ
	//������ʾ
	int iAttackLife;
	int iAttackMaxLife;
	//��ͼ��ʾ
	int iShowWorldFrame;
	//�����˵�
	int iShowHelp;
};

#endif