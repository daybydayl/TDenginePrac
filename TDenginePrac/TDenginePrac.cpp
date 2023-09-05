#include <stdio.h>
#include <stdlib.h>
#include "CGlobal.h"


TAOS* taos;//声明成全局，方便跨文件使用

int main() {
    taos = taos_connect("Linx.Linx-lzj", "root", "taosdata", NULL, 6030);
    if (taos == NULL) {
        printf("failed to connect to server\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("connect success！！\n");
        CGlobal::IntoInstance()->Init();
    }


    //测试写库
   /* executeSQL(taos, "CREATE DATABASE power");
    executeSQL(taos, "USE power");
    executeSQL(taos, "CREATE STABLE meters (ts TIMESTAMP, current FLOAT, voltage INT, phase FLOAT) TAGS (location BINARY(64), groupId INT)");
    executeSQL(taos, "INSERT INTO d1001 USING meters TAGS('California.SanFrancisco', 2) VALUES ('2018-10-03 14:38:05.000', 10.30000, 219, 0.31000) ('2018-10-03 14:38:15.000', 12.60000, 218, 0.33000) ('2018-10-03 14:38:16.800', 12.30000, 221, 0.31000)"
        "d1002 USING meters TAGS('California.SanFrancisco', 3) VALUES ('2018-10-03 14:38:16.650', 10.30000, 218, 0.25000)"
        "d1003 USING meters TAGS('California.LosAngeles', 2) VALUES ('2018-10-03 14:38:05.500', 11.80000, 221, 0.28000) ('2018-10-03 14:38:16.600', 13.40000, 223, 0.29000)"
        "d1004 USING meters TAGS('California.LosAngeles', 3) VALUES ('2018-10-03 14:38:05.000', 10.80000, 223, 0.29000) ('2018-10-03 14:38:06.500', 11.50000, 221, 0.35000)");*/
    taos_close(taos);
    taos_cleanup();
}
