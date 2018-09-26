#include "ipmac.h"
#include <stdio.h>
#include <stdlib.h>

void getIpMac(char *ipmac)
{
	// Ref: GetAdaptersInfo function
	// https://docs.microsoft.com/en-us/windows/desktop/api/iphlpapi/nf-iphlpapi-getadaptersinfo

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i, type;
	char *searchVirtual, *searchSangfor;
	char macBytes[3] = {0};
	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	int hw_info_len = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return;
	}

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			searchVirtual = strstr(pAdapter->Description, "Virtual");
			if (searchVirtual != NULL) {
				pAdapter = pAdapter->Next;
				continue;
			}
			searchSangfor = strstr(pAdapter->Description, "Sangfor");
			if (searchSangfor != NULL) {
				pAdapter = pAdapter->Next;
				continue;
			}
			type = pAdapter->Type;
			if (type != MIB_IF_TYPE_ETHERNET) {
				pAdapter = pAdapter->Next;
				continue;
			}
			strncat(ipmac, pAdapter->Description, strlen(pAdapter->Description));
			strncat(ipmac, ", MAC: ", 16);
			
			for (i = 0; i < pAdapter->AddressLength; i++) {
				if (i == (pAdapter->AddressLength - 1))
					_snprintf(macBytes, 3, "%.2X\0", (int) pAdapter->Address[i]);
				else
					_snprintf(macBytes, 3, "%.2X:", (int) pAdapter->Address[i]);
				strncat(ipmac, macBytes, 3);
			}

			strncat(ipmac, ", IP Address: ", 16);
			strncat(ipmac, pAdapter->IpAddressList.IpAddress.String, 16);

			pAdapter = pAdapter->Next;
			strncat(ipmac, ", ", 2);
		}
	} else {
		printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
	}

	hw_info_len = strlen(ipmac);
	ipmac[hw_info_len-2] = 0;

	if (pAdapterInfo)
		FREE(pAdapterInfo);
}
