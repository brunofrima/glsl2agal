/**************************************************************************
 *
 * Copyright 2009, VMware, Inc.
 * All Rights Reserved.
 * Copyright 2010 George Sapountzis <gsapountzis@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL VMWARE AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/

#include "pipe/p_compiler.h"
#include "util/u_memory.h"

#include "softpipe/sp_public.h"
#include "state_tracker/drm_api.h"
#include "state_tracker/sw_winsys.h"
#include "dri_sw_winsys.h"

static struct pipe_screen *
swrast_create_screen(struct drm_api *api,
                     int drmFD,
                     struct drm_create_screen_arg *arg)
{
   struct sw_winsys *winsys = NULL;
   struct pipe_screen *screen = NULL;

   (void) drmFD;

   if (arg != NULL) {
      switch(arg->mode) {
      case DRM_CREATE_DRISW:
         break;
      default:
         return NULL;
      }
   }

   winsys = dri_create_sw_winsys();
   if (winsys == NULL)
      return NULL;

   screen = softpipe_create_screen( winsys );
   if (screen == NULL)
      goto fail;

   return screen;

fail:
   if (winsys)
      winsys->destroy( winsys );

   return NULL;
}

static void
swrast_drm_api_destroy(struct drm_api *api)
{
   return;
}

static struct drm_api swrast_drm_api =
{
   .name = "swrast",
   .driver_name = "swrast",
   .create_screen = swrast_create_screen,
   .destroy = swrast_drm_api_destroy,
};

struct drm_api *
drm_api_create()
{
   return &swrast_drm_api;
}

/* vim: set sw=3 ts=8 sts=3 expandtab: */