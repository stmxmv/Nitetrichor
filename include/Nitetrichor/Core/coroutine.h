/*!
* Created by Molybdenum on 10/8/21.
*
*/
#ifndef NITETRICHOR_COROUTINE_H
#define NITETRICHOR_COROUTINE_H

/*
 * `scr' macros for static coroutines.
 */

#define scrBegin         static int scrLine = 0; switch(scrLine) { case 0:;
#define scrFinish(z)     } return (z)
#define scrFinishV       } return

#define scrReturn(z)     \
        do {\
            scrLine=__LINE__;\
            return (z); case __LINE__:;\
        } while (0)
#define scrReturnV       \
        do {\
            scrLine=__LINE__;\
            return; case __LINE__:;\
        } while (0)

/*
 * `ccr' macros for re-entrant coroutines.
 */

/*
 * `ccr' macros for re-entrant coroutines.
 */

#define ccrContParam     void **ccrParam

#define ccrBeginContext  struct ccrContextTag { int ccrLine
#define ccrEndContext(x) } *x = (struct ccrContextTag *)*ccrParam

#define ccrBegin(x)      if(!x) {x= *ccrParam=malloc(sizeof(*x)); x->ccrLine=0;}\
                         if (x) switch(x->ccrLine) { case 0:;
#define ccrFinish(z)     } free(*ccrParam); *ccrParam=0; return (z)
#define ccrFinishV       } free(*ccrParam); *ccrParam=0; return

#define ccrReturn(z)     \
        do {\
            ((struct ccrContextTag *)*ccrParam)->ccrLine=__LINE__;\
            return (z); case __LINE__:;\
        } while (0)
#define ccrReturnV       \
        do {\
            ((struct ccrContextTag *)*ccrParam)->ccrLine=__LINE__;\
            return; case __LINE__:;\
        } while (0)

#define ccrStop(z)       do{ free(*ccrParam); *ccrParam=0; return (z); }while(0)
#define ccrStopV         do{ free(*ccrParam); *ccrParam=0; return; }while(0)

#define ccrContext       void *
#define ccrAbort(ctx)    do { free (ctx); ctx = 0; } while (0)

/// example
//int function(void) {
//    static int i, state = 0;
//    switch (state) {
//        case 0:
//            for (i = 0; i < 20; i++) {
//                state = 1;
//                return i;
//                case 1:;
//            }
//        default:
//            break;
//    }
//}
//
//
//
//
//
//int ascending (void) {
//    static int i;
//    scrBegin;
//        for (i = 1; i <= 10; i++) {
//            scrReturn(i);
//        }
//    scrFinish(0);
//}

#endif //NITETRICHOR_COROUTINE_H
