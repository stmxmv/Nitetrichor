/*!
* Created by Molybdenum on 9/9/21.
*
*/
#ifndef NITETRICHOR_BASIC_H
#define NITETRICHOR_BASIC_H
#include <stdint.h>
#include <stdbool.h>


#define WNMaxRuntimeTypes	65535

typedef void* WNTypeRef;

typedef void (*WNReleaseCallback)(WNTypeRef obj);

typedef int WNIndex;

typedef struct s_WNAllocator {

} *WNAllocatorRef;

typedef struct s_WNRuntimeClass {
    WNIndex version;
    const char *className; // must be a pure ASCII string, nul-terminated
    void (*init)(WNTypeRef cf);
    WNTypeRef (*copy)(WNAllocatorRef allocator, WNTypeRef cf);
    void (*finalize)(WNTypeRef cf);
    bool (*equal)(WNTypeRef cf1, WNTypeRef cf2);
//    WNHashCode (*hash)(WNTypeRef cf);
//    WNStringRef (*copyFormattingDesc)(WNTypeRef cf, WNDictionaryRef formatOptions);	// return str with retain
//    WNStringRef (*copyDebugDesc)(WNTypeRef cf);	// return str with retain

    void (*reclaim)(WNTypeRef cf); // Set _kWNRuntimeResourcefulObject in the .version to indicate this field should be used

    uint32_t (*refcount)(intptr_t op, WNTypeRef cf); // Set _kWNRuntimeCustomRefCount in the .version to indicate this field should be used
    // this field must be non-NULL when _kWNRuntimeCustomRefCount is in the .version field
    // - if the callback is passed 1 in 'op' it should increment the 'cf's reference count and return 0
    // - if the callback is passed 0 in 'op' it should return the 'cf's reference count, up to 32 bits
    // - if the callback is passed -1 in 'op' it should decrement the 'cf's reference count; if it is now zero, 'cf' should be cleaned up and deallocated (the finalize callback above will NOT be called unless the process is running under GC, and WN does not deallocate the memory for you; if running under GC, finalize should do the object tear-down and free the object memory); then return 0
    // remember to use saturation arithmetic logic and stop incrementing and decrementing when the ref count hits UINT32_MAX, or you will have a security bug
    // remember that reference count incrementing/decrementing must be done thread-safely/atomically
    // objects should be created/initialized with a custom ref-count of 1 by the class creation functions
    // do not attempt to use any bits within the WNRuntimeBase for your reference count; store that in some additional field in your WN object
} WNRuntimeClass;

typedef struct s_WNRuntimeBase {
    uintptr_t isa;
    u_int8_t info[4];
    WNReleaseCallback release;
    u_int32_t _rc;
} WNRuntimeBase;



void WNRelease(WNTypeRef obj);

#endif //NITETRICHOR_BASIC_H
