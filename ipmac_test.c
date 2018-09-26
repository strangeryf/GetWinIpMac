#include "ipmac.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char hw_info[255] = {0};
	getIpMac(hw_info);
	printf("%s", hw_info);
	getchar();

    return 0;
}
