#ifndef ipmac__h
#define ipmac__h

#ifdef __cplusplus
extern "C"
{
#endif

#include <winsock2.h>
#include <iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

extern void getIpMac(char *ipmac);

#ifdef __cplusplus
}
#endif

#endif