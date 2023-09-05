#include "CGlobal.h"
extern TAOS* taos;

CGlobal* CGlobal::m_pGlobal = 0;
CGlobal::CGlobal()
{
	
}

CGlobal::~CGlobal()
{
	ExitInstance();
}

CGlobal* CGlobal::IntoInstance()
{
	if (m_pGlobal == nullptr)
		m_pGlobal = new CGlobal;
	else
		return m_pGlobal;
	return m_pGlobal;
}

void CGlobal::ExitInstance()
{
	if (m_pGlobal)
		delete m_pGlobal;
	m_pGlobal = 0;
}

void CGlobal::Init()
{
	Get_TaosInfo(taos);
	SynQuery();
}

void CGlobal::executeSQL(TAOS* taos, const char* sql) {
	TAOS_RES* res = taos_query(taos, sql);
	int       code = taos_errno(res);
	if (code != 0) {
		printf("Error code: %d; Message: %s\n", code, taos_errstr(res));
		taos_free_result(res);
		taos_close(taos);
		exit(EXIT_FAILURE);
	}
	int affectedRows = taos_affected_rows(res);
	//printf("affected rows %d\n", affectedRows);
	taos_free_result(res);
}

void CGlobal::Get_TaosInfo(TAOS* taos)
{
	printf("客户端版本为%s\n", taos_get_client_info());
	//printf("服务端版本为：%s\n", taos_get_server_info(taos));
}

void CGlobal::SynQuery()
{
	int taosRet = 0;		//返回值判断
	char databaseName[20];	//数据库名,这里暂时长度给20
	int len = 20;			//数据库名长度，传参需要
	int* required = (int*)malloc(sizeof(int));

	executeSQL(taos, "USE lzj");
	//获取当前数据库名，taos句柄(in) 数据库名接收(out) 接收长度(in)  数据名大小(out)
	taosRet = taos_get_current_db(taos, databaseName, len, required);
	printf("当前使用数据库%s，", databaseName);

	TAOS_RES* res = NULL;								//查询的结果集
	TAOS_ROW row = NULL;								//一条记录的结果数据
	int rows = 0;										//查询结果的记录条数
	res = taos_query(taos, "select * from meas001");

	//返回结果集时间戳字段的精度，0 代表毫秒，1 代表微秒，2 代表纳秒。
	int time_pre = taos_result_precision(res);
	if (time_pre == 0)
		printf("时间戳为毫秒\n");
	int fields_num = taos_field_count(res);				//查询表的域个数，等同taos_num_fields
	TAOS_FIELD* fields = taos_fetch_fields(res);		//域结构信息
	printf("域数量 = %d\n\n", fields_num);

	//获取结果集中每个字段的长度。返回值是一个数组，其长度为结果集的列数。
	//int* fields_len = taos_fetch_lengths(res);//？没理解

	int affect_row = 0;
	affect_row = taos_affected_rows(res);				//获取被所执行的 SQL 语句影响的行数。

	printf("执行select * from meas001\n");
	//批量获取查询结果集中的数据，返回值为获取到的数据的行数。
	//int taos_fetch_block(TAOS_RES * res, TAOS_ROW * rows);

	for (int i = 0; i < fields_num; i++)				//打印域名，表头
		printf("% s\t\t", fields[i].name);
	printf("\n");

	while ((row = taos_fetch_row(res))) {				//按行获取查询结果集中的数据。
		char temp[1024] = { 0 };
		rows++;
		taos_print_row(temp, row, fields, fields_num);	//官方解析一条记录输出
		printf("%s\n", temp);
		//if (rows == 5)
		//	taos_stop_query(res);						//停止当前查询的执行。
	}
	printf("查询到数据 %d 行\n", rows);

	taos_free_result(res);								//释放查询结果集以及相关的资源。

	//获取最近一次 API 调用失败的原因, 返回值为字符串标识的错误提示信息。
	//char* taos_errstr(TAOS_RES * res);
	//获取最近一次 API 调用失败的原因，返回值为错误代码。
	//int taos_errno(TAOS_RES * res);
}

