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
	printf("�ͻ��˰汾Ϊ%s\n", taos_get_client_info());
	//printf("����˰汾Ϊ��%s\n", taos_get_server_info(taos));
}

void CGlobal::SynQuery()
{
	int taosRet = 0;		//����ֵ�ж�
	char databaseName[20];	//���ݿ���,������ʱ���ȸ�20
	int len = 20;			//���ݿ������ȣ�������Ҫ
	int* required = (int*)malloc(sizeof(int));

	executeSQL(taos, "USE lzj");
	//��ȡ��ǰ���ݿ�����taos���(in) ���ݿ�������(out) ���ճ���(in)  ��������С(out)
	taosRet = taos_get_current_db(taos, databaseName, len, required);
	printf("��ǰʹ�����ݿ�%s��", databaseName);

	TAOS_RES* res = NULL;								//��ѯ�Ľ����
	TAOS_ROW row = NULL;								//һ����¼�Ľ������
	int rows = 0;										//��ѯ����ļ�¼����
	res = taos_query(taos, "select * from meas001");

	//���ؽ����ʱ����ֶεľ��ȣ�0 ������룬1 ����΢�룬2 �������롣
	int time_pre = taos_result_precision(res);
	if (time_pre == 0)
		printf("ʱ���Ϊ����\n");
	int fields_num = taos_field_count(res);				//��ѯ������������ͬtaos_num_fields
	TAOS_FIELD* fields = taos_fetch_fields(res);		//��ṹ��Ϣ
	printf("������ = %d\n\n", fields_num);

	//��ȡ�������ÿ���ֶεĳ��ȡ�����ֵ��һ�����飬�䳤��Ϊ�������������
	//int* fields_len = taos_fetch_lengths(res);//��û���

	int affect_row = 0;
	affect_row = taos_affected_rows(res);				//��ȡ����ִ�е� SQL ���Ӱ���������

	printf("ִ��select * from meas001\n");
	//������ȡ��ѯ������е����ݣ�����ֵΪ��ȡ�������ݵ�������
	//int taos_fetch_block(TAOS_RES * res, TAOS_ROW * rows);

	for (int i = 0; i < fields_num; i++)				//��ӡ��������ͷ
		printf("% s\t\t", fields[i].name);
	printf("\n");

	while ((row = taos_fetch_row(res))) {				//���л�ȡ��ѯ������е����ݡ�
		char temp[1024] = { 0 };
		rows++;
		taos_print_row(temp, row, fields, fields_num);	//�ٷ�����һ����¼���
		printf("%s\n", temp);
		//if (rows == 5)
		//	taos_stop_query(res);						//ֹͣ��ǰ��ѯ��ִ�С�
	}
	printf("��ѯ������ %d ��\n", rows);

	taos_free_result(res);								//�ͷŲ�ѯ������Լ���ص���Դ��

	//��ȡ���һ�� API ����ʧ�ܵ�ԭ��, ����ֵΪ�ַ�����ʶ�Ĵ�����ʾ��Ϣ��
	//char* taos_errstr(TAOS_RES * res);
	//��ȡ���һ�� API ����ʧ�ܵ�ԭ�򣬷���ֵΪ������롣
	//int taos_errno(TAOS_RES * res);
}

