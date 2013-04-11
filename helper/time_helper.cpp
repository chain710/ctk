#include "time_helper.h"

using namespace ctk;

int ctk::timeval_minus( struct timeval t1, struct timeval t2 )
{
    return 1000*1000*(t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
}

timeval ctk::timeval_diff( struct timeval t1, struct timeval t2 )
{
    int diff = timeval_minus(t1, t2);
    timeval res;
    res.tv_sec = diff / 1000000;
    res.tv_usec = diff % 1000000;
    return res;
}

int ctk::timeval_cmp( struct timeval t1, struct timeval t2 )
{
    int diff = timeval_minus(t1, t2);
    if (diff > 0)
    {
        return -1;
    }
    else if (0 == diff)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void ctk::timeval_add( struct timeval& dst, struct timeval add )
{
    dst.tv_sec += add.tv_sec;
    dst.tv_usec += add.tv_usec;
    int add_sec = dst.tv_usec / 1000000;
    dst.tv_usec %= 1000000;
    dst.tv_sec += add_sec;
}
