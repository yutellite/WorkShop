#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define MAX_OID_LEN 512
#define DB_SUCCESS 0
#define DB_ERROR 1
#define SECUREC_MEM_MAX_LEN ((unsigned int)0x7fffffff) //2GB
#define PLUGIN_LOG_LEVEL_INFO "War"
typedef int db_int32;
typedef unsigned int db_uint32;
typedef char db_char;
#define WARING(modulename,str, ...) printf("[%s]"str" Tid=%u,File:%s,Line:%d\n",modulename,##__VA_ARGS__,(db_uint32)pthread_self(),__FILE__,__LINE__)

db_int32 cm_memcpy_s(void* dest, size_t dest_max, const void* src, size_t count)
{
    if (dest_max > SECUREC_MEM_MAX_LEN )
    {
        WARING(PLUGIN_LOG_LEVEL_INFO,  "%s %d.", "The max size of dest string is larger than",SECUREC_MEM_MAX_LEN);
        return DB_ERROR;
    }

    if(0 == count)
    {
        return DB_SUCCESS;
    }

    if (dest == NULL || src == NULL)
    {
        WARING(PLUGIN_LOG_LEVEL_INFO,  "%s", "The input ptr of dest or src string is NULL.");
        return DB_ERROR;
    }

    if (count > dest_max)
    {
        WARING(PLUGIN_LOG_LEVEL_INFO,  "%s %d.", "The size of dest string is larger than .", MAX_OID_LEN);
        return DB_ERROR;
    }

    if (dest == src)
    {
        return DB_SUCCESS;
    }

    if ((dest > src && dest < (void*)((db_char*)src + count)) ||
        (src > dest && src < (void*)((db_char*)dest + count)) )
    {
        WARING(PLUGIN_LOG_LEVEL_INFO,  "%s", "Src and dest string overlap in memcpy.");
        return DB_ERROR;
    }

    (void)memcpy(dest, src, count);

    return DB_SUCCESS;
}

int main()
{
    int ret;
    char src[8] = "abcdefg";
    //char des[8] = "abcdefg";
    ret=cm_memcpy_s(src+2, MAX_OID_LEN, src, 0);
    if(ret!=0)WARING(PLUGIN_LOG_LEVEL_INFO,"test1 failed!");

    ret=cm_memcpy_s(src+2, (unsigned int)0x8fffffff, src, 0);
    if(ret!=0)WARING(PLUGIN_LOG_LEVEL_INFO,"test2 failed!%d",ret);

    ret=cm_memcpy_s(NULL, MAX_OID_LEN, src, 2);
    if(ret!=0)WARING(PLUGIN_LOG_LEVEL_INFO,"test3 failed!");

    ret=cm_memcpy_s(src+2, 1, NULL, 2);
    if(ret!=0)WARING(PLUGIN_LOG_LEVEL_INFO,"test4 failed!");

    ret=cm_memcpy_s(src+2, 1, src, 3);
    if(ret!=0)WARING(PLUGIN_LOG_LEVEL_INFO,"test5 failed!");

    ret=cm_memcpy_s(src+3, 8, src+1, 4);
    if(ret!=0)WARING(PLUGIN_LOG_LEVEL_INFO,"test6 failed!");

    ret=cm_memcpy_s(src+1, 8, src+3, 4);
    if(ret!=0)WARING(PLUGIN_LOG_LEVEL_INFO,"test6 failed!");   

    ret=cm_memcpy_s(src+1, 2, src+4, 0);
    if(ret!=0)WARING(PLUGIN_LOG_LEVEL_INFO,"test6 failed!");
    WARING(PLUGIN_LOG_LEVEL_INFO,"src is %s",src);   
    return 0;
}
