/*!
* Created by Molybdenum on 9/9/21.
*
*/
#include "LearnKit/WNQueue.h"
#include <stdlib.h>
#include <memory.h>

#define MAXSIZE 10

static
void WNQueueRelease(WNTypeRef obj) {
    free(((WNQueueRef)obj)->queue);
}

WNQueueRef WNQueueCreate(int elemSize, int size) {
    WNQueue *queue = calloc(1, sizeof(WNQueue));
    if (size <= 0) {
        queue->queue = malloc(elemSize * MAXSIZE);
        queue->maxSize = MAXSIZE;
    } else {
        queue->queue = malloc(elemSize * size);
        queue->maxSize = size;
    }
    queue->base.release = WNQueueRelease;
    queue->elemSize = elemSize;
    return queue;
}


void WNQueuePush(WNQueueRef self, void * obj) {
    if (self->size == self->maxSize) {
        self->maxSize *= 2;
        void * queue = malloc(self->elemSize * self->maxSize);
        if (self->front == 0) {
            memcpy(queue, self->queue, self->size * self->elemSize);
        } else {
            int size = (self->size - self->front) * (self->elemSize);
            memcpy(queue, self->queue + self->front * self->elemSize, size);
            memcpy(queue + size, self->queue, self->front * self->elemSize);
        }
        free(self->queue);
        self->front = 0;
        self->rear = self->size;
        self->queue = queue;
    }
    void * rear = self->queue + self->elemSize * self->rear;
    memcpy(rear, obj, self->elemSize);

    self->rear = (++self->rear) % self->maxSize;
    ++self->size;

}

void WNQueuePop(WNQueueRef self) {
    --self->size;
    self->front = (++self->front) % self->maxSize;
}

WNTypeRef WNQueueGetTop(WNQueueRef self) {
    return self->queue + self->elemSize * self->front;
}

bool WNQueueIsEmpty(WNQueueRef self) {
    return self->size == 0;
}

