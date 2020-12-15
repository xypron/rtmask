// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright Heinrich Schuchardt, <xypron.glpk@gmx.de>
 *
 * This program allows to determine the supported UEFI runtime services.
 *
 * The UEFI firmware according to UEFI spec 2.8A supplies a configuration table
 * EFI_RT_PROPERTIES_TABLE with field RuntimeServicesSupported. The Linux EFI
 * stub stores this internally.
 *
 * The rtmask program uses an ioctl() call to read the value. The ioctl is
 * expected to be implemented in Linux v5.11 earliest. Cf.
 * https://lore.kernel.org/linux-efi/20201127192051.1430-1-xypron.glpk@gmx.de/
 *
 * The kernel must be configured with CONFIG_EFI_TEST=m.
 *
 * Running rtmask produces an output like the following:
 *
 *     $ rtmask
 *     RuntimeServicesSupported = 0x000005b0
 *     = EFI_RT_SUPPORTED_GET_VARIABLE
 *     | EFI_RT_SUPPORTED_GET_NEXT_VARIABLE_NAME
 *     | EFI_RT_SUPPORTED_SET_VIRTUAL_ADDRESS_MAP
 *     | EFI_RT_SUPPORTED_CONVERT_POINTER
 *     | EFI_RT_SUPPORTED_RESET_SYSTEM
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define EFI_RUNTIME_GET_SUPPORTED_MASK \
	_IOR('p', 0x0C, unsigned int)

static const char *rt_str[] = {
	"EFI_RT_SUPPORTED_GET_TIME",
	"EFI_RT_SUPPORTED_SET_TIME",
	"EFI_RT_SUPPORTED_GET_WAKEUP_TIME",
	"EFI_RT_SUPPORTED_SET_WAKEUP_TIME",
	"EFI_RT_SUPPORTED_GET_VARIABLE",
	"EFI_RT_SUPPORTED_GET_NEXT_VARIABLE_NAME",
	"EFI_RT_SUPPORTED_SET_VARIABLE",
	"EFI_RT_SUPPORTED_SET_VIRTUAL_ADDRESS_MAP",
	"EFI_RT_SUPPORTED_CONVERT_POINTER",
	"EFI_RT_SUPPORTED_GET_NEXT_HIGH_MONOTONIC_COUNT",
	"EFI_RT_SUPPORTED_RESET_SYSTEM",
	"EFI_RT_SUPPORTED_UPDATE_CAPSULE",
	"EFI_RT_SUPPORTED_QUERY_CAPSULE_CAPABILITIES",
	"EFI_RT_SUPPORTED_QUERY_VARIABLE_INFO",
};

int main()
{
	unsigned int i, j, flag;
	int fd, ret;
	unsigned int mask;
	uid_t uid;

	fd = open("/dev/efi_test", O_RDWR);
	if (fd == -1) {
		perror("open");
		if (errno == ENOENT)
			printf("Kernel must be compiled with CONFIG_EFI_TEST\n");
		return 1;
	}

	ret = ioctl(fd, EFI_RUNTIME_GET_SUPPORTED_MASK, &mask);
	if (ret == -1) {
		if (errno = ENOTTY)
			printf("The IOCTL is not implemented\n");
		else
			perror("ioctl");

		return 1;
	}

	printf("RuntimeServicesSupported = 0x%08x\n", mask);

	flag = 1;
	for (i = 1, j = 0; i <= 0x2000; i <<= 1, ++j) {
		if (i & mask) {
			printf("%s %s\n", flag ? "=" : "|", rt_str[j]);
			flag = 0;
		}
	}

	close(fd);

	return 0;
}
