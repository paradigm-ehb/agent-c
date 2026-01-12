/* ========================================================================
   $File: pkgm_main.cpp $
   $Date: 2026-01-12 $
   $Revision: $
   $Author: nsrddyn@gmail.com $
   $Notice: (C) Copyright 2026 $
   ======================================================================== */

#include "arena.h"
#include "pkgm.hpp"
#include "stdio.h"

int
main()
{
  mem_arena *arena = arena_create(KiB(16));
  printf("distro name %s", find_lxd_pkgm(arena));
}
