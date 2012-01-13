prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=@CMAKE_INSTALL_LIBDIR@
includedir=@CMAKE_INSTALL_INCDIR@

Name: @CMAKE_PROJECT_NAME@
Description: The genbb_help API
URL: https://nemo.lpc-caen.in2p3.fr/wiki/genbb_help
Version: @genbb_help_VERSION_STR@
@genbb_help_pkgconfig_Requires@
Libs: -L@CMAKE_INSTALL_LIBDIR@ @genbb_help_pkgconfig_Libs@
Cflags: -I@CMAKE_INSTALL_INCDIR@