/*#ifndef DDC_HANDLE
typedef int DDC_HANDLE;
#endif*/

#ifdef DDC_HANDLE
#undef DDC_HANDLE
#endif

#define DDC_HANDLE void *
#define STD_OUTPUT_HANDLE 123L


#ifndef _REENTRANT
#define _REENTRANT /* Stones and Matthew: Beginning Linux Programming 3rd.Ed. p.480. */
#endif
#define LPTHREAD_START_ROUTINE void*

/* key input */
int kbhit();
int getch();
void Sleep(unsigned long int dwMs);

#ifdef DDC_LINUX_THREAD
#ifndef _REENTRANT
#define _REENTRANT /* Stones and Matthew: Beginning Linux Programming 3rd.Ed. p.480. */
#endif
#define LPTHREAD_START_ROUTINE void*
pthread_t* CreateThread(
                void* pAttr,                    /* default security attributes */                int DefaultStackSize,           /* use default stack size */
                void*(*ThreadFunction)(void*),  /* thread function */
                void* ThreadFunctionArg,        /* no thread function argument */                int CreationFlags,              /* use default creation flags */
                pthread_t* pThreadID);          /* Thread ID pointer.*/

pthread_t GetCurrentThreadId();
#endif
