//
// ����  programking 2009��3��
// ����  http://blog.csdn.net/programking
//=======================================
// mdlobj.cpp: implementation of the mdlobj class.
// ѧ�������Ϸϵ�д���
// ������ E_mail: cqtmL@163.com
//====================================================================
#include "stdafx.h"
#include "mdlobj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
TMDLModel		MdlModel[2];
//////////////////////////////////////////////////////////////////////////
mdlobj::mdlobj()
{
	Animation=0;
}
mdlobj::~mdlobj()
{
}
/////////////////////////////////////////////////////////////////////
//��ʼ��
int mdlobj::InitGL(int a,char* na)	
{	
	MdlModel[a].Init(na);
	MdlModel[a].SetSequence(0);

	return TRUE;							// Initialization Went OK
}

//����ģ��
void mdlobj::DrawModels(float x,float y,float z,int a,float size,float rz,float ry)
{
	glPushMatrix();

	glEnable(GL_LIGHTING);            
	glEnable(GL_TEXTURE_2D);     
	//�ƶ���ָ������
	glTranslatef(x, y, z);
	glScalef( size, size,size);	//����
	//��ת
	glRotated(-90,1.0f,0.0f,0.0f);
	//ˮƽ��
	glRotated(rz+5,0.0f,0.0f,1.0f);
	//����
	glRotated(-ry*2/3-5,0.0f,1.0f,0.0f);

	//ˢ�µ�ǰ�������е�֡
	MdlModel[a].AdvanceFrame2();

	//����
	MdlModel[a].DrawModel();///////////////

	if(Animation==1)
	{
		Animation=0;
		MdlModel[a].SetSequence(0);
	}
	if(Animation>0) 
		Animation--;

	glPopMatrix();
}

void mdlobj::fire(int a)
{
	MdlModel[0].SetSequence(a);
	tagMDLSeqDescription *aa=(tagMDLSeqDescription *)((BYTE *)MdlModel[0].Header
							  + MdlModel[0].Header->SequenceOffset)+a;

	//���������� Animation : 5 , NumFrames:29
	Animation=aa->NumFrames/5;
}
