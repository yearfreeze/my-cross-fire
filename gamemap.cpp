//
// ����  programking 2009��3��
// ����  http://blog.csdn.net/programking
//=======================================

#include "stdafx.h"
#include "gamemap.h"
#include "myclock.h"
#include "glfont.h"
#include "opengl.h"
#include "mykey.h"

//font
extern CGLFont myfont;

//clock
extern MYCLOCK c1;

//mouse
extern int Lbutdown;

extern OpenGL* m_OpenGL;

extern int WinWidth;
extern int WinHeight;

/////////////////////////

GAMEMAP::GAMEMAP()
{
	iGameState=GAME_NULL;	

	xmouse=0;
	ymouse=0;
	xmouseOld=0;
	ymouseOld=0;

	iMenu=-1;

	//camera
	g_eye[0]=50;//
	g_eye[2]=5;//
	g_Angle=0;
	g_elev=0;

	//fire
	IsFire=0;

	rocketX=INVALID_ROCKET;
	rocketY=0.0f;
	rocketZ=0.0f;
	
	iMatch=0;
	iMapRectNum=0;
	iShowHelp=0;
}

GAMEMAP::~GAMEMAP()
{
	delete m_anmobj;
}
//��Ϸ���������ļ����,���ȱ��,������ֹ
int GAMEMAP::haveDataFile()
{
	int ihave=1;
	FILE *fp;
	char *fdata[]={
		//map
		"data/map/map.txt",
		//mdl
		"data/mdl/qian1.mdl",
		//bmp
		"data/images/start1.bmp",
		"data/images/start2.bmp",
		"data/images/box0.bmp",
		"data/images/box01.bmp",
		"data/images/box1.bmp",
		"data/images/box11.bmp",
		"data/images/box2.bmp",
		"data/images/box21.bmp",
		"data/images/map2.bmp",
		"data/images/map5.bmp",
		"data/images/TRIX1.bmp",
		"data/images/button.bmp",
		"data/images/h1.bmp",
		//md2
		"data/md2/role/tris.md2",
		"data/md2/role/weapon.md2",
		//sound
		"data/sound/explode1.wav"
		};
	int i;
	
	for(i=0;i<sizeof(fdata)/sizeof(fdata[0]);i++)
	{
		fp=fopen(fdata[i],"r");
		if(!fp)
		{
			ihave=0;//ȱ���ļ�
			break;
		}
		else
			fclose(fp);
	}

	//�����ǰ�ļ���û��,�����ϼ��ļ���,�ټ��
	if(!ihave)
		SetCurrentDirectory("..");

	for(i=0;i<sizeof(fdata)/sizeof(fdata[0]);i++)
	{
		fp=fopen(fdata[i],"r");
		if(!fp)
		{
			return 0;
		}
		else
			fclose(fp);
	}

	return 1;
}

//���ظ���ͼƬ,ģ��,��ʼ��ȫ������
void GAMEMAP::init()
{
	//mdlģ��Ҫ���ȳ�ʼ��
	m_mdlobj.InitGL(0,"data/mdl/qian1.mdl");	
	
	//load images
	LoadT8("data/images/start1.bmp", g_cactus[0]);
	LoadT8("data/images/start2.bmp", g_cactus[1]);

	LoadT8("data/images/box0.bmp", TexBox[0]);
	LoadT8("data/images/box01.bmp", TexBox[1]);
	LoadT8("data/images/box1.bmp", TexBox[2]);
	LoadT8("data/images/box11.bmp", TexBox[3]);
	LoadT8("data/images/box2.bmp", TexBox[4]);
	LoadT8("data/images/box21.bmp", TexBox[5]);
	//up
	LoadT8("data/images/map2.bmp", g_cactus[4]);
	//ground
	LoadT8("data/images/map5.bmp", g_cactus[7]);

//	g_text = gluNewQuadric();
//	gluQuadricTexture(g_text, GL_TRUE);

	LoadT8("data/images/TRIX1.bmp", g_cactus[8]);
	//button
	LoadT8("data/images/button.bmp", g_cactus[9]);

	LoadT8("data/images/h1.bmp", g_cactus[10]);

	//md2
	m_anmobj=new anmobj;
	m_anmobj->model[0]=NULL;
	m_anmobj->model[1]=NULL;
	m_anmobj->getobj("data/md2/role/");	
	
	///////////////////////////////////////////
	//������ͼ
	glEnable(GL_TEXTURE_2D);

	//��Ϸ״̬
	iGameState=GAME_PRE_ANI;
	iMatch=0;
	//����
	param1=0;
}
//��Ϸ����
void GAMEMAP::showInfo()
{
	char info[50]={0};

	sprintf(info, "enemy number: %d  ",iEnemyNum);
	myfont.settext(50,50,info,FONT1,1,1,1);

	sprintf(info, "HP: %d  ",iHp);
	if(iHp>30)
	{
		//��ɫ��ʾ
		myfont.settext(50,550,info,FONT1,1,1,1);
	}
	else
	{
		myfont.settext(50,550,info,FONT1,1,0,0);	//��ɫ
	}

	//������ʾ ��������
	if(iAttackLife)
	{
		glLoadIdentity();
		//��ͷ��ͼ��
		glDisable(GL_LIGHTING);				
		texture0(g_cactus[10]);	
		glTranslatef(-0.14,0.15,-1.0);
		tPic(0.04f);
		glEnable(GL_LIGHTING);
		//������ֵ		
		showEnemyHp();
	}


	//��ʤ
	if(GAME_WIN==iGameState)
	{
		sprintf(info, "ʤ ��");
		myfont.settext(350,250,info,FONT3,1,1,0);		
	}
	if(GAME_FAIL==iGameState)
	{
		sprintf(info, "ʧ ��");
		myfont.settext(350,250,info,FONT3,1,1,1);		
	}
	if(GAME_PASS==iGameState)
	{
		sprintf(info, "ͨ ��");
		myfont.settext(350,250,info,FONT4,1,1,0);		
	}
	
	if(iShowWorldFrame)
	{
		sprintf(info, "WORLD: %d",iMatch+1);
		myfont.settext(350,250,info,FONT3,1,1,0);		

		iShowWorldFrame++;
		//��ͼ��Ϣ��ʾʱ�� TIME_WORLD_INFO
		if(iShowWorldFrame > TIME_WORLD_INFO)
		{
			iShowWorldFrame=0;
		}
	}
}
//��������ֵ
void GAMEMAP::showEnemyHp()
{
	int i;

	unsigned char rasterallf[12] = {
		0xff, 0xff,0xff, 0xff,0xff, 0xff,
		0xff, 0xff,0xff, 0xff,0xff, 0xff
		};

	glLoadIdentity();
	//���Խ�ջ
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);    
	glDisable(GL_LIGHTING);      	
	//��ɫ��
	//�����ƶ�
	glTranslatef(-0.1f, 0.15, -1.0);

	glColor3f (0.0, 0.0, 0.0);
	//��դλ������ƶ�-0.01����ԭλ�õ���2������
	glRasterPos3f (0.0, 0.0,-0.01);
	for(i=0;i<iAttackMaxLife/10;i++)
	{
		glBitmap (8, 12, 0.0, 0.0, 8.0, 0.0, rasterallf);
	}
	//��ɫ��ʾ����
	glColor3f (0.0, 1.0, 0.0);
	glRasterPos3f (0.0, 0.0,0.0);
	for(i=0;i<iAttackLife/10;i++)
	{
		glBitmap (8, 8, 0.0, 0.0, 8.0, 0.0, rasterallf);
	}

	/////////////////////////
	glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();

}

//׼��
void GAMEMAP::showTarget()
{
	unsigned char rasterallf[12] = {
		0xff, 0xff,0xff, 0xff,0xff, 0xff,
		0xff, 0xff,0xff, 0xff,0xff, 0xff
		};

	glLoadIdentity();
	//���Խ�ջ
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);    
	glDisable(GL_LIGHTING);      	
	//ָ����ɫ
	glColor3f(0.0,1.0,0.0f);             
	//����ת�����ƶ�
	glTranslatef(0.0,-0.033,-1.0f);

	//��RC�����
	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
	//���ù�դλ��
	glRasterPos2i(0,0);                  
	glBitmap (2, 12, 0.0, 0.0, 0.0, 23.0, rasterallf);
	glBitmap (2, 12, 0.0, 0.0, -16.0, -6.0, rasterallf);
	glBitmap (12, 2, 0.0, 0.0, 22.0, 0.0, rasterallf);
	glBitmap (12, 2, 0.0, 0.0, 0.0, 0.0, rasterallf);

	/////////////////////////
	glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();

}
//�������
void GAMEMAP::drawMouse()
{
	glLoadIdentity();
	//���Խ�ջ
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);    
	glDisable(GL_LIGHTING);      	
	
	glTranslatef(xmouse,WinHeight-ymouse,0.1);

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0,0.5,0.0f); 
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(30.0, -15.0,0.0);

	glColor3f(0.0,0.8,0.0f); 
	glVertex3f(12.0, -12.0,0.0);
	glVertex3f(6.0,-20.0,0.0);

	glColor3f(0.0,0.0,0.0f); 
	glVertex3f(3.0,-20.0,0.0);
	glEnd();

	/////////////////////////
	glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();
}

//��������ɫ����
void GAMEMAP::showFire()
{
	unsigned char rasterallf[12] = {
		0xff, 0xff,0xff, 0xff,0xff, 0xff,
		0xff, 0xff,0xff, 0xff,0xff, 0xff
		};
	unsigned char rasterblank[12] = {0};
	int ibombframe;
	int ibombx;
	int ibomby;

	if(!iShowBombFrame)
		return;

	glLoadIdentity();
	//���Խ�ջ
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D);    
	glDisable(GL_LIGHTING);      	
	//ָ����ɫ
	glColor3f(0.0,0.0,0.0f);             
	//����ת�����ƶ�
	glTranslatef(0.0,0.0,-1.0f);

	glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
	//�ø��ֹ�ʽ�������ķ���
	ibombframe=iShowBombFrame-1;

	ibombx=ibombframe*2;
	ibomby=2*(ibombframe-3)*(ibombframe-3)-30;
	glRasterPos2i(0,0);                  
	glBitmap (1,1, 0.0, 0.0, ibombx, -ibomby, rasterblank);	
	glBitmap (3, 4, 0.0, 0.0, 0.0, 0.0, rasterallf);	
	
	ibomby=2*(ibombframe-3)*(ibombframe-3)-18;
	glRasterPos2i(0,0);                  	
	glBitmap (1,1, 0.0, 0.0, -ibombx, -ibomby, rasterblank);	
	glBitmap (4, 3, 0.0, 0.0, 0.0, 0.0, rasterallf);	
	
	ibombx=ibombframe;
	ibomby=(ibombframe-3)*(ibombframe-3);
	glRasterPos2i(0,0);                  	
	glBitmap (1,1, 0.0, 0.0, -ibombx, -ibomby, rasterblank);	
	glBitmap (4, 3, 0.0, 0.0, 0.0, 0.0, rasterallf);	

	/////////////////////////
	glEnable(GL_LIGHTING);         
	glEnable(GL_TEXTURE_2D);          
	glPopAttrib();

	//ˢ��֡
	iShowBombFrame++;
	if(iShowBombFrame>TIME_BOMB)
	{
		iShowBombFrame=0;
	}	
}
//��Ļˢ��
void GAMEMAP::show()
{
	int i;

	float xx0,zz0;	//gun
	
	switch(iGameState)
	{
	case GAME_PRE_ANI:
		showpreani();
		break;

	case GAME_MENU:
		glLoadIdentity();
		showmenu();		
		glLoadIdentity();
		drawMouse();
		break;

	case GAME_IN:
	case GAME_WIN:
	case GAME_FAIL:
	case GAME_PASS:
		//��ʼ����λ����
		glLoadIdentity();
		showTarget();
		showInfo();
		showFire();

		glLoadIdentity();
		DisplayScene();

		showmap();
		//ENEMY
		texture0(g_cactus[8]);
		for(i=0;i<MAX_MAP_OBJECT;i++)
		{
			if(MapEnemyArray[i].show)
			{				
			m_anmobj->drawModel(MapEnemyArray[i].id,
					MapEnemyArray[i].x,
					MapEnemyArray[i].y+ENEMY_MODEL_Y,
					MapEnemyArray[i].z,
					MapEnemyArray[i].iframe,
					MapEnemyArray[i].iAngle);
			}
		}
		//gun
		//��ǰ����λ��
		xx0=g_eye [0];
		zz0=g_eye [2];
		//�������ֵ��Ϊ0�����������λ�ã��Ƕ���ʾ��ǹ
		if(iHp)
		{
			m_mdlobj.DrawModels(xx0,1.8,zz0,
				0,
				0.1f,
				-g_Angle,g_elev);//ˮƽ�Ƕ�ȡ��
		}

		break;

	case GAME_ERR:		
		break;

	default:
		break;
	}
}
//����λͼ��ͼ
bool GAMEMAP::LoadT8(char *filename, GLuint &texture)
{	
	AUX_RGBImageRec *pImage = NULL;
	
	//����ͼƬ����
	pImage = auxDIBImageLoad(filename);	 
	if(pImage == NULL)	
		return false;
	
	//��������
	glGenTextures(1, &texture);		
	glBindTexture    (GL_TEXTURE_2D,texture);
	//����2ά����
	gluBuild2DMipmaps(GL_TEXTURE_2D,4,  
					  pImage->sizeX,    
					  pImage->sizeY,  
					  GL_RGB, GL_UNSIGNED_BYTE,
					  pImage->data    
					 );
	//�ͷ��ڴ�
	free(pImage->data);      
	free(pImage);	
	return true;      
}

//ָ����ͼ
void GAMEMAP::texture(UINT textur)
{
	glBindTexture  (GL_TEXTURE_2D, textur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

//ָ����ͼ
void GAMEMAP::texture0(UINT textur)
{
	glBindTexture  (GL_TEXTURE_2D, textur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//�������� ���߳�2e
void GAMEMAP::tPic(float e)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-e, -e,  0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( e, -e,  0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( e,  e,  0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-e,  e,  0.0f);
	glEnd();
}

//�����Σ����½�����xy�����w����h
void GAMEMAP::tPic(int x,int y,int w,int h)
{ 
	glPushMatrix();

	//����������
	glTranslatef(x,y,-0.1f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0, 0.0, 0.0f);
		
		glTexCoord2f(1.0f, 0.0f);		
		glVertex3f(w, 0.0,  0.0f);		

		//up
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(w, h,  0.0f);
				
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0, h,  0.0f);
	glEnd();	

	glPopMatrix();
}
//����ť
void GAMEMAP::tPicButton(int x,int y,int w,int h,float ytex)
{ 
	glPushMatrix();

	//����������
	glTranslatef(x,y,-0.1f);	//z������ƶ�
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, ytex);
		glVertex3f(0.0, 0.0, 0.0f);
		
		glTexCoord2f(1.0f, ytex);		
		glVertex3f(w, 0.0,  0.0f);		

		//up
		glTexCoord2f(1.0f, ytex+0.5f);
		glVertex3f(w, h,  0.0f);
				
		glTexCoord2f(0.0f, ytex+0.5f);
		glVertex3f(0.0, h,  0.0f);
	glEnd();	

	glPopMatrix();
}

//���õƹ�
void GAMEMAP::light0()
{
	GLfloat light_position[] = {10.0,10.0,3.0,1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	//������ȼ��
	glEnable(GL_DEPTH_TEST);
	//������ɫ����
	glEnable(GL_COLOR_MATERIAL);
}
//��������
void GAMEMAP::showpreani()
{
	//�ƶ�����ǰλ��
	glTranslatef(0,0,-8.0);
	texture0(g_cactus[0]);	
	tPic(2.0f);
	//�ƶ�����
	tSquare(param1*0.2, 0.0, 0.1, 3.0);
}

//ָ��λ�û�һ��������
void GAMEMAP::tSquare(float x, float y, float z, float a)
{
	//���Խ�ջ
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();							
	glDisable(GL_TEXTURE_2D);

	glTranslatef(x,y,z);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
		glVertex3f(-a, -a,  0.0f);
		glVertex3f( a, -a,  0.0f);
		glVertex3f( a,  a,  0.0f);
		glVertex3f(-a,  a,  0.0f);
	glEnd();	

	glEnable(GL_TEXTURE_2D);
	glPopMatrix();							
	glPopAttrib();
}

//ʵʱ���
void GAMEMAP::check()
{
	int i;
	int x,y;

	switch(iGameState)
	{
	case GAME_PRE_ANI:
		if(c1.iNumClip%2==0)
		{
			if(param1<30)
			{			
				param1++;		
			}
		}
		//��ʱ
		if(c1.clipcount())
		{
			iGameState=GAME_MENU;
			//����ͶӰ 
			initView();
		}
		break;

	case GAME_MENU:
		//���˵�ѡ��
		x=xmouse;
		y=WinHeight-ymouse;
		iMenu=-1;//��ʼ�� û��ѡ��
		
		if(x<XMENU	|| x>XMENU+WIDTH_MENU || y<YMENU)
			break;

		for(i=0;i<MENU_NUM;i++)
		{
			if( y>YMENU+i*Y_2MENU && y<YMENU+i*Y_2MENU+HEIGHT_MENU )
			{
				iMenu=i;
				break;
			}
		}
		break;
	
	case GAME_IN_INIT:
		//�����ͼǰ��ʼ��
		initMap();
		iGameState=GAME_IN;
		c1.ReStart(300);
		//������ϷͶӰ
		initView();
		break;

	case GAME_IN:
		//��ʱ
		c1.clipcount();
		//��ɫ���
		checkRole();
		checkFire();		
		//ʤ���ж�
		if(iEnemyNum<=0)
		{
			iMatch++;
			if(iMatch>=MAX_MATCH)
			{
				iGameState=GAME_PASS;
				c1.ReStart(3);
			}
			else
			{
				iGameState=GAME_WIN;
				c1.ReStart(4);
			}
		}
		if(iHp<=0)
		{
			iHp=0;
			iGameState=GAME_FAIL;
			c1.ReStart(3);
		}
		
		break;	

	case GAME_WIN:
		checkRole();
		//��ʱ
		if(c1.clipcount())
		{
			iGameState=GAME_IN_INIT;
		}
		break;

	case GAME_FAIL:
		checkRole();
		//��ʱ
		if(c1.clipcount())
		{
			iGameState=GAME_MENU;
			//����ͶӰ 
			initView();
		}
		break;

	case GAME_PASS:
		checkRole();
		//��ʱ
		if(c1.clipcount())
		{
			iMatch=0;//��0��
			iGameState=GAME_MENU;
			//����ͶӰ 
			initView();
		}
		break;

	default:
		break;
	}
}
//��ʾ�˵�
void GAMEMAP::showmenu()
{
	int i;
	char *menustr[]={"��    ��","����˵��","��ʼ��Ϸ"};
	char *helpstr[]={
		"http://blog.csdn.net/programking",
		"����: programking 2009��3��",
		"  F1 :������С��",
		"  Esc:�����˵�",
		"  ��ǹ:������",
		"  �ӽ�:����ƶ�",
		"����: �ƶ�: W A S D "		
	};

	glLoadIdentity();

	//��glOrthoģʽ�»��Ʋ˵�
	for(i=0;i<MENU_NUM;i++)
	{
		//����
		myfont.settextTest(XMENU+30,YMENU+15+i*Y_2MENU,menustr[i],2,1,1,1);
		//ȡ������
		glDisable(GL_LIGHTING);
		//ͼƬ
		texture0(g_cactus[9]);
		if(iMenu==i)
		{
			tPicButton(XMENU,YMENU+i*Y_2MENU,WIDTH_MENU,HEIGHT_MENU,0.0f);
		}
		else
		{
			tPicButton(XMENU,YMENU+i*Y_2MENU,WIDTH_MENU,HEIGHT_MENU,0.5f);
		}
	}
	//������ͼƬ
	glLoadIdentity();
	glTranslatef(0.0,0.0,-0.1f);
	texture0(g_cactus[1]);
	tPic(0,0,WinWidth,WinHeight);	

	//������˵���˵�
	if(iShowHelp)
	{
		for(i=0;i<sizeof(helpstr)/sizeof(helpstr[0]);i++)
		{
			//����
			myfont.settextTest(370,150+i*40,helpstr[i],1,1,1,1);
		}
	}
	
	glEnable(GL_LIGHTING);
}

//���̴���
void GAMEMAP::keyupproc(int keyparam)
{
    switch (keyparam)
    {
	case KEY_F12:
		//ֱ�ӹ���
		if(iGameState=GAME_IN)
		{
			iEnemyNum=0;
		}		
		break;

	case VK_ESCAPE:
		//�ص��˵�
		iGameState=GAME_MENU;
		initView();
		break;

	default:
		break;
	}
}

//����⺯��
void GAMEMAP::mouseproc(int lparam)
{
	xmouseOld=xmouse;
	ymouseOld=ymouse;
	xmouse=LOWORD(lparam);
	ymouse=HIWORD(lparam);
}
//����ɿ�
void GAMEMAP::lbuttonproc(int lparam)
{
	switch(iGameState)
	{
	case GAME_MENU:
		if(iMenu<0)
			break;

		switch(iMenu)
		{
		case MENU_START:
			iGameState=GAME_IN_INIT;
			break;

		case MENU_HELP:
			iShowHelp=1;
			break;

		case MENU_QUIT:
			m_OpenGL->CleanUp();
			PostQuitMessage(0);				
			break;
		}		
		break;	

	default:
		break;
	}
}

//��������
//��� 	λ�� ����� ��ͼ���� ��ͼ��� iHastop �Ƿ��ж���͵���
void GAMEMAP::showmapBox(float *ppos, float *psize, float *ptex,int itex,int iHastop)
{
	float width =psize[0];	
	float height=psize[2];	
	float length=psize[1];	
	float x = ppos[0];	
	float y = ppos[1];
	float z = ppos[2];
	float fstex=ptex[0];//��x�����ͼ
	float fttex=ptex[1];
	float fhtex=ptex[2];
	///////////////////////////////////////////////////////////////////////////////
	
	//wall ��ֱ��Z��
	texture0(TexBox[itex]);	
	glBegin(GL_QUADS);		
		glTexCoord2f(fstex,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(fstex,fhtex); glVertex3f(x+width,y+height,z); 
		glTexCoord2f(0.0f,fhtex); glVertex3f(x,		y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
	glEnd();

	texture0(TexBox[itex]);	
	glBegin(GL_QUADS);		
		glTexCoord2f(fstex,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(fstex,fhtex); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(0.0f,fhtex); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z+length);
	glEnd();

	if(iHastop)
	{
		//sky
		texture0(g_cactus[4]);	
		glBegin(GL_QUADS);	
			glTexCoord2f(0.0f,fttex); glVertex3f(x+width,y+height,z);
			glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y+height,z+length); 
			glTexCoord2f(fstex,0.0f); glVertex3f(x,		y+height,z+length);
			glTexCoord2f(fstex,fttex); glVertex3f(x,		y+height,z);
		glEnd();
	}
	//wall ��ֱ��x��
	texture0(TexBox[itex+1]);	
	glBegin(GL_QUADS);		
		glTexCoord2f(fttex,fhtex); glVertex3f(x,		y+height,z);	
		glTexCoord2f(0.0f,fhtex); glVertex3f(x,		y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(fttex,0.0f); glVertex3f(x,		y,		 z);		
	glEnd();
	texture0(TexBox[itex+1]);	
	glBegin(GL_QUADS);			
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(fttex,0.0f); glVertex3f(x+width,y,		 z+length);
		glTexCoord2f(fttex,fhtex); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,fhtex); glVertex3f(x+width,y+height,z);
	glEnd();

	//ground
	if(iHastop)
	{
		texture0(g_cactus[7]);	
		glBegin(GL_QUADS);	
			glTexCoord2f(0.0f,fttex); glVertex3f(x+width,y,z);
			glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,z+length); 
			glTexCoord2f(fstex,0.0f); glVertex3f(x,		y,z+length);
			glTexCoord2f(fstex,fttex); glVertex3f(x,		y,z);
		glEnd();
	}
}
//����ͼ����
void GAMEMAP::showmap()
{
	//��ͼ����
	float pmappos[]={0,0,0};
	float pmapsize[]={100,50,5};
	float pmaptex[]={50,25,5};
	int i;
	int iTex;

	//MAP
	//�������
	pmappos[0]=maprect[0].bottom;
	pmappos[2]=maprect[0].left;
	//���ø� ��
	pmapsize[0]=maprect[0].top;
	pmapsize[1]=maprect[0].right;
	//������ͼ����
	pmaptex[0]=pmapsize[0]/2;
	pmaptex[1]=pmapsize[1]/2;
	//������ͼ���
	iTex=maprectTex[0]*2;
	//�������� ������ ����
	showmapBox(pmappos,pmapsize,pmaptex,iTex,1);

	for(i=1;i<iMapRectNum;i++)
	{			
		pmappos[0]=maprect[i].bottom;
		pmappos[2]=maprect[i].left;
		pmapsize[0]=maprect[i].top-maprect[i].bottom;
		pmapsize[1]=maprect[i].right-maprect[i].left;
		pmaptex[0]=pmapsize[0]/2;
		pmaptex[1]=pmapsize[1]/2;
		iTex=maprectTex[i]*2;//ÿ���������Ӧ������ͼ
		showmapBox(pmappos,pmapsize,pmaptex,iTex,0);
	}	
}
//���������
void GAMEMAP::DisplayScene()
{
	float speed=0.5f;					
	float x=g_eye[0],y=g_eye[1],z=g_eye[2];
	
	if (KEY_DOWN(VK_SHIFT))  speed   =speed*2;

	//mouse
/*  */
	g_Angle +=  (xmouse-xmouseOld)*.2f;            
	g_elev  += -(ymouse-ymouseOld)*.2f;          

	if(xmouse<50 || xmouse>750 || ymouse<50 || ymouse>550)
	{
		SetCursorPos(400,300);
	}
	xmouseOld=xmouse;
	ymouseOld=ymouse;

	//
	if (KEY_DOWN(VK_LEFT))   g_Angle-=speed*2;
	if (KEY_DOWN(VK_RIGHT))  g_Angle+=speed*2;
	
	//ת���ɻ���
	rad_xz = float (3.13149* g_Angle/180.0f);	
	//����
	if (KEY_DOWN(33))		   g_elev +=speed;
	if (KEY_DOWN(34))		   g_elev -=speed;
	if (g_elev<-360)		   g_elev  =-360;
	if (g_elev> 360)		   g_elev  = 360;
	
	//w s a d �ƶ� ֧�����¼��ƶ�
	if (KEY_DOWN(KEY_W) || KEY_DOWN(VK_UP) )					
	{
		z+=(float)sin(rad_xz)*speed;	
		x+=(float)cos(rad_xz)*speed;	
	}
	if (KEY_DOWN(KEY_S) || KEY_DOWN(VK_DOWN) )					
	{
		z-=(float)sin(rad_xz)*speed;	
		x-=(float)cos(rad_xz)*speed;
	}
	if (KEY_DOWN(KEY_A))					
	{
		z-=(float)cos(rad_xz)*speed;	
		x+=(float)sin(rad_xz)*speed;	
	}
	if (KEY_DOWN(KEY_D))		
	{
		z+=(float)cos(rad_xz)*speed;	
		x-=(float)sin(rad_xz)*speed;	
	}


	//�ƶ���Χ�ж�
	if(RoleInMap(x,y,z))
	{
		g_eye[0]=x;
		g_eye[2]=z;
	}

	g_eye[1] =VIEW_HEIGHT;

	g_look[0] = (float)(g_eye[0] +100*cos(rad_xz));
	g_look[2] = (float)(g_eye[2] +100*sin(rad_xz));
	g_look[1] = g_eye[1] +g_elev;	
	
	gluLookAt(g_eye[0],g_eye[1],g_eye[2],g_look[0],g_look[1],g_look[2],0.0,1.0,0.0);
	//����
	//�������
	if(Lbutdown && iHp)
	{
		//��ʼ������
		rocketX=g_eye[0];
		rocketZ=g_eye[2];
		rocketY=ENEMY_MODEL_Y;
		//�趨ǹ��ģ�Ͷ�������
		m_mdlobj.fire(1);//
		IsFire=1;//���㣬��ʾ���ӵ�
		Lbutdown=0;
		//sound
		sndPlaySound("data/sound/explode1.wav",SND_ASYNC);
		//bomb ���û��ײ��Ч��������ײ������
		if(!iShowBombFrame)
		{
			iShowBombFrame=1;//��ǹʱ������ײ��Ч��
		}
	}

	return;
}

//��ײ���
int GAMEMAP::hittest()
{
	int i;
	int iHit=0;
	float x,y,z;

	//ԭ���ӵ�ÿ��ˢ������,���Ӿ���7.0
	//		���ʱ���ӵ�ǰ����ѭ��6��,ÿ������һ����λ�����
	if(IsFire)
	{
		x=rocketX;
		y=rocketY;
		z=rocketZ;
		iHit=0;
		for(int k=0;k<6;k++)					
		{	
			for(i=0;i<MAX_MAP_OBJECT;i++)
			{
				if(!MapEnemyArray[i].show)
					continue;

				if(MapEnemyArray[i].hp<=0)
					continue;		 
 		
				if(		abs(MapEnemyArray[i].x-x)<0.1f	
					&&	abs(MapEnemyArray[i].y+ENEMY_MODEL_Y-y)<0.3f
					&&	abs(MapEnemyArray[i].z-z)<0.1f)	
				{				
					MapEnemyArray[i].hp-=20;
					if(MapEnemyArray[i].hp<=0)
					{
						//�趨����
						MapEnemyArray[i].iframe=178;	//death frame
						MapEnemyArray[i].iCurrentAni=ENEMY_DEATH_ANI;
						iEnemyNum--;
					}
					//����������ʾ
					iAttackLife=MapEnemyArray[i].hp;
					iAttackMaxLife=MapEnemyArray[i].hpMax;
					param1=0;
					iHit=1;		//�Ѿ���⵽��ײ
					break;
				}					
			}
			if(iHit)
			{	
				//���
				Lbutdown=0;				
				IsFire=0;	//	
				rocketX=INVALID_ROCKET;		//����ӵ�����
				break;//ֹͣ���
			}
		  
			x+=1.0f*cos(rad_xz);				
	  		z+=1.0f*sin(rad_xz);				
	  		y+=1.0f*tan(g_elev*PI/360);		
		}
	}//end �ӵ���enemy�ļ��

	//�ӵ����ϰ���ļ��
	if(IsFire)
	{
		x=rocketX;
		y=rocketY;
		z=rocketZ;
		iHit=0;
		for(int k=0;k<6;k++)					
		{	
			if(!POINT_IN_RECT(x,z,maprect[0]))
			{			
				iHit=1;
			}	
			for(i=1;i<iMapRectNum;i++)
			{
				//���κ��ϰ�������,�Ƿ�
				if(POINT_IN_RECT(x,z,maprect[i]))
				{
					iHit=1;
				}
			}
			if(iHit)
			{
				//���
				Lbutdown=0;				
				IsFire=0;	//	
				rocketX=INVALID_ROCKET;		//����ӵ�����
				break;//�����ϰ�
			}
			x+=1.0f*cos(rad_xz);				
	  		z+=1.0f*sin(rad_xz);				
	  		y+=1.0f*tan(g_elev*PI/360);		
		}
	}//end �ӵ����ϰ���ļ��

	//enemy����ҵ���ײ
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if(!MapEnemyArray[i].show)
				continue;

		//�����ܶ�״̬, �����
		if(ENEMY_RUN_ANI!=MapEnemyArray[i].iCurrentAni)
				continue;

		if(	abs(MapEnemyArray[i].x-g_eye[0])<1.0f	
			&&	abs(MapEnemyArray[i].z-g_eye[2])<1.0f)	
		{				
			iHp-=10;	//��enemy��ײ,����ֵ����
			break;
		}	
	}
	return 0;
}

void GAMEMAP::checkRole()
{
	int i,j;
	int iTurn;//�Ƿ����˶�
	struct animation	*pAni;

	//ˢ�µ��˵Ķ���֡
	if(c1.iNumClip%3==0)
	{
		//ˢ��֡ ����
		for(i=0;i<MAX_MAP_OBJECT;i++)
		{
			if(!MapEnemyArray[i].show)
				continue;

			//ˢ��֡
			pAni=&(m_anmobj->anim[0][MapEnemyArray[i].iCurrentAni]);
			switch(MapEnemyArray[i].iCurrentAni)
			{
			case 1:
				//run
				MapEnemyArray[i].iframe++;
				if(MapEnemyArray[i].iframe > pAni->end)
				{
					//ѭ������
					MapEnemyArray[i].iframe=pAni->start;
				}
				break;

			default:
				if(MapEnemyArray[i].iframe < pAni->end)
				{
					MapEnemyArray[i].iframe++;
				}
				break;
			}
		}// end for
	}

	//ˢ�µ�������
	if(c1.iNumClip%3==0)
	{
		for(i=0;i<MAX_MAP_OBJECT;i++)
		{
			if(!MapEnemyArray[i].show)
				continue;

			//������ܲ�����, ˢ������
			if(ENEMY_RUN_ANI!=MapEnemyArray[i].iCurrentAni)
				continue;
			
			//run
			switch(MapEnemyArray[i].iMoveDir)
			{
			case MOVE_X:
				MapEnemyArray[i].x+=MapEnemyArray[i].iMoveSpeed;
				break;

			case MOVE_Z:
				MapEnemyArray[i].z+=MapEnemyArray[i].iMoveSpeed;
				break;

			default:
				break;
			}

			iTurn=0;//�Ƿ����˶�
			//��ײ���
			if(!POINT_IN_RECT(MapEnemyArray[i].x,
					MapEnemyArray[i].z,
					maprect[0]))
			{
				iTurn=1;		
			}
			for(j=1;j<iMapRectNum;j++)
			{
				if(POINT_IN_RECT(MapEnemyArray[i].x,
					MapEnemyArray[i].z,
					maprect[j]))
				{
					iTurn=1;		
				}
			}
			if(iTurn)
			{
				MapEnemyArray[i].iMoveSpeed=-MapEnemyArray[i].iMoveSpeed;//�ٶ�ʸ��ȡ��
				//�Ƕ�ȡ����0 1 2 3��ʾ�ĸ����� 0 x������
				MapEnemyArray[i].iAngle=(MapEnemyArray[i].iAngle+2)%4;
			}
		}// end for
	}
}

void GAMEMAP::checkFire()
{
	//��ײ���
	hittest();	
	
	//�ӵ����
	if(IsFire)
	{
		//���겻���ƶ�,���� ǰ��7.0
		rocketX+=7.0f*(float)cos(rad_xz);		
		rocketZ+=7.0f*(float)sin(rad_xz);			
		rocketY+=9.0f*(float)tan(g_elev*PI/360);	

		//ͳ���ӵ��ƶ�֡��
		IsFire++;
		//�ӵ����� IsFire 1��22
		if(IsFire>22)			
		{
			Lbutdown=0;						
			IsFire=0;							
			return;								
		}		
	}

	//������ʾ��Ϣ��ʱ
	if(iAttackLife)
	{
		param1++;
		if(param1>60)
		{
			iAttackLife=0;
			param1=0;
		}
	}

}

int GAMEMAP::RoleInMap(float x,float y,float z)
{
	int i;
	RECT_F role;
	
	role.left=z-0.5f;
	role.right=z+0.5f;

	role.bottom=x-0.5f;
	role.top=x+0.5f;

	//������������
	if(!RECT_IN_RECT(role,maprect[0]))
	{	
		return 0;
	}
	
	for(i=1;i<iMapRectNum;i++)
	{
		//���κ��ϰ�������,�Ƿ�
		if(!RECT_OUT_RECT(role,maprect[i]))
		{	
			return 0;
		}
	}
	return 1;
}
//��ʼ��ĳһ�ص�ͼ ��Ϸ����
void GAMEMAP::initMap()
{

	//camera
	g_eye[0]=50;//
	g_eye[2]=5;//
	g_Angle=0;
	g_elev=0;

	//fire
	IsFire=0;

	rocketX=INVALID_ROCKET;
	rocketY=0.0f;
	rocketZ=0.0f;

	iHp=100;	//����ֵ
	//������ʾ
	iAttackLife=0;
	iAttackMaxLife=0;
	//
	iShowWorldFrame=1;	//��ʾ��ͼ��Ϣ
	iShowBombFrame=0;	//����ʾ�ӵ�ײ��
	//���ص�ͼ
	LoadMap();
}
//���ص�ͼ
int GAMEMAP::LoadMap()
{
	FILE *fp;
	char temp[50]={0};
	int find=0;
	int i;
	
	//����
	memset(MapEnemyArray,0,sizeof(MapEnemyArray));
	iMapEnemyCursor=0;
	iEnemyNum=0;

	//	��������ͼ���
	memset(maprectTex,0,sizeof(maprectTex));
	
	fp=fopen(PATH_MAP,"r");
	if(!fp)
	{
		return 0;
	}

	//�� *��ͼ���
	while(!find && !feof(fp))
	{
		FGetLine(temp,fp);
		if(temp[0]=='*' && temp[1]=='0'+iMatch)
		{
			find=1;
		}
	}
	if(!find)
	{
		return 0;
	}

	//�ҵ���ĳһ�صĵ�ͼ����
	//enemy����
	i=0;
	FGetLineJumpCom(temp,fp);	
	while(temp[0]!='#' && !feof(fp))
	{
		//map data
		sscanf(temp,"%d %d %d %d %d %d %d",
			&MapEnemyArray[i].id ,
			&MapEnemyArray[i].x ,
			&MapEnemyArray[i].z ,
			&MapEnemyArray[i].hp ,
			&MapEnemyArray[i].iAngle ,
			&MapEnemyArray[i].iMoveDir ,
			&MapEnemyArray[i].iMoveSpeed );			

		//�Ƿ���ʾ
		MapEnemyArray[i].show = 1;
		//��ʼ�� ֡
		MapEnemyArray[i].iframe = 40;
		//��ʼ�� ����
		MapEnemyArray[i].iCurrentAni = 1;//run
		//�������ֵ
		MapEnemyArray[i].hpMax=MapEnemyArray[i].hp;
		i++;		
		//��ȡ��һ��
		FGetLineJumpCom(temp,fp);	
	}
	iMapEnemyCursor=i;
	iEnemyNum=iMapEnemyCursor;

	//��ͼ����
	i=0;
	FGetLineJumpCom(temp,fp);	
	while(temp[0]!='#' && !feof(fp))
	{
		//map data
		sscanf(temp,"%d %d %d %d %d",
			&maprect[i].top ,
			&maprect[i].bottom,
			&maprect[i].left,
			&maprect[i].right,
			&maprectTex[i]);			
		i++;		
		//��ȡ��һ��
		FGetLineJumpCom(temp,fp);	
	}
	iMapRectNum=i;

	fclose(fp);

	return 1;
}

//����ͶӰ��ʽ
void GAMEMAP::initView()
{
	ShowCursor(false);
	switch(iGameState)
	{
	case GAME_MENU:
		m_OpenGL->initMenu();
		break;

	default:
		m_OpenGL->init();		
		break;
	}
}

