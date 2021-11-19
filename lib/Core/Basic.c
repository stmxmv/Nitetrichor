/*!
* Created by Molybdenum on 9/9/21.
*
*/
#include <stdlib.h>
#include "Core/Basic.h"

void WNRelease(WNTypeRef obj) {
    WNRuntimeBase* base = (WNRuntimeBase*)obj;
    base->release(obj);
    free(obj);
}



static WNRuntimeClass * WNRuntimeClassTable[WNMaxRuntimeTypes] = {NULL};


void WNInitialize(void) {

}