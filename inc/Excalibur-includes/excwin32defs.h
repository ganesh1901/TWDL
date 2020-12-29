//Definitions for old compilers
#ifndef ULONG_PTR
#if defined(_WIN64)
    typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;
#else
    typedef unsigned long ULONG_PTR, *PULONG_PTR;
#endif
typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
#endif

