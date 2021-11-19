/*!
* Created by Molybdenum on 9/9/21.
*
*/
#ifndef NITETRICHOR_WNQUEUE_H
#define NITETRICHOR_WNQUEUE_H
#include "Core/Basic.h"

typedef struct s_WNQueue {
    WNRuntimeBase base;
    int elemSize;
    int maxSize;
    int size;
    int front;
    int rear;
    void * queue;
} WNQueue, *WNQueueRef;

WNQueueRef WNQueueCreate(int elemSize, int size);

void WNQueuePush(WNQueueRef self, void * obj);

void WNQueuePop(WNQueueRef self);

WNTypeRef WNQueueGetTop(WNQueueRef self);

bool WNQueueIsEmpty(WNQueueRef self);


#endif //NITETRICHOR_WNQUEUE_H
