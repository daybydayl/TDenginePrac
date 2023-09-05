#pragma once
#include "CCommonHead.h"
class CGlobal
{
public:
	CGlobal();
	~CGlobal();

public:
	static CGlobal* IntoInstance();					//进入实例
	static void ExitInstance();						//退出实例
	void Init();									//初始化

	void executeSQL(TAOS* taos, const char* sql);	//官方函数
	void Get_TaosInfo(TAOS* taos);					//获取taos相关信息
	void SynQuery();								//同步查询api
	void UnsynQuery();

private:
	static CGlobal* m_pGlobal;
};

