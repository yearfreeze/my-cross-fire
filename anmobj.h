// anmobj.h: interface for the anmobj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANMOBJ_H__46249C44_7C55_4407_AAE7_790A324FB6DE__INCLUDED_)
#define AFX_ANMOBJ_H__46249C44_7C55_4407_AAE7_790A324FB6DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

//////////////////////////////////////////

struct animation
{	char name[16];
	int	 start;		
	int	 end;		
};

////////////////////////////////////////////
class anmobj
{
public:
	anmobj();
	virtual ~anmobj();

public:
	md2_model_t*	model[2];	//����MD2ģ��
	animation*	anim[2];	//����ģ�͵Ķ�������
	int		size[2];	//��������
	
	//��ʼ��ģ��
	void			getobj(CString na);
	//��ȡģ�͵Ķ�����Ϣ
	animation*		animations (md2_model_t* model,int p);
	//����ģ��
	//��� ID	���� ֡
	void drawModel(int id,float x,float y,float z,int iframe,int iAngle);

};

#endif // !defined(AFX_ANMOBJ_H__46249C44_7C55_4407_AAE7_790A324FB6DE__INCLUDED_)
