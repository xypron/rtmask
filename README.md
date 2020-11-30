RTMASK
======

This program allows to determine the supported UEFI runtime services.

The UEFI firmware according to UEFI spec 2.8A supplies a configuration table
EFI\_RT\_PROPERTIES\_TABLE with field RuntimeServicesSupported. The Linux EFI
stub stores this internally.

The rtmask program uses an ioctl() call to read the value. The ioctl is
expected to be implemented in Linux v5.11 earliest. Cf.
https://lore.kernel.org/linux-efi/20201127192051.1430-1-xypron.glpk@gmx.de/

The kernel must be configure with CONFIG\_EFI\_TEST=m.
