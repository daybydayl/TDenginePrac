#pragma once
#include "CCommonHead.h"
class CGlobal
{
public:
	CGlobal();
	~CGlobal();

public:
	static CGlobal* IntoInstance();					//����ʵ��
	static void ExitInstance();						//�˳�ʵ��
	void Init();									//��ʼ��

	void executeSQL(TAOS* taos, const char* sql);	//�ٷ�����
	void Get_TaosInfo(TAOS* taos);					//��ȡtaos�����Ϣ
	void SynQuery();								//ͬ����ѯapi
	void UnsynQuery();

private:
	static CGlobal* m_pGlobal;
};

