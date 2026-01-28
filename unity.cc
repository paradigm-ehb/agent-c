/*-------base files-----------*/
extern "C"
{
#define BASE_UNITY
#include "base/base_include.h"
#undef BASE_UNITY
}

/*----------------------------*/

/*-------common files---------*/
#include "common/logger.cc"
/*----------------------------*/

/*-----resource gathering-----*/
#include "journal/journald.cc"
#include "libnet/net.cc"
#include "libres/resources.cc"
#include "dbus/dbushandler.cc"
/*----------------------------*/

/*------grpc connection-------*/
#include "auth/auth.cc"
#include "grpc/generated/resources/v3/deviceresources.pb.cc"
#include "grpc/generated/resources/v3/deviceresources.grpc.pb.cc"
#include "grpc/handlers/resource_handler.cc"
#include "grpc/server/server.cc"
/*----------------------------*/

/*------commented out---------*/
/*
#include "libvm/qc.c"
#include "libpkg/pkg.c"
#include "libpkg/distro/arch_pac.cc"
#include "libpkg/distro/debian_apt.cc"
#include "libpkg/distro/fedora_dnf.cc"
*/
/*----------------------------*/

#include "core/core.cc"
