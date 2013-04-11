#include <sys/time.h>

namespace ctk
{
    int timeval_minus(struct timeval t1, struct timeval t2);
    timeval timeval_diff(struct timeval t1, struct timeval t2);
    int timeval_cmp(struct timeval t1, struct timeval t2);
    void timeval_add(struct timeval& dst, struct timeval add);
}

