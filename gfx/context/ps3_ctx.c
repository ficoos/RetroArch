/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2012 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2012 - Daniel De Matteis
 * 
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../driver.h"

#include <stdint.h>

#include <sys/spu_initialize.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../gl_common.h"
#include "../image.h"

#include "ps3_ctx.h"

#ifdef HAVE_OPENGLES11
#define glOrtho glOrthof
#endif

static struct texture_image menu_texture;
static PSGLdevice* gl_device;
static PSGLcontext* gl_context;

// Other vertex orientations
static const GLfloat vertexes_90[] = {
   0, 1,
   1, 1,
   1, 0,
   0, 0
};

static const GLfloat vertexes_180[] = {
   1, 1,
   1, 0,
   0, 0,
   0, 1
};

static const GLfloat vertexes_270[] = {
   1, 0,
   0, 0,
   0, 1,
   1, 1
};

//forward decls
extern const GLfloat *vertex_ptr;
extern const GLfloat *default_vertex_ptr;

void gfx_ctx_set_swap_interval(unsigned interval, bool inited)
{
   (void)inited;

   if (gl_context)
   {
      if (interval)
         glEnable(GL_VSYNC_SCE);
      else
         glDisable(GL_VSYNC_SCE);
   }
}

void gfx_ctx_check_window(bool *quit,
      bool *resize, unsigned *width, unsigned *height, unsigned frame_count)
{
   gl_t *gl = driver.video_data;
   *quit = false;
   *resize = false;

#ifdef HAVE_SYSUTILS
   cellSysutilCheckCallback();
#endif

   if (gl->quitting)
      *quit = true;

   if (gl->should_resize)
      *resize = true;
}

#ifndef HAVE_GRIFFIN
bool gfx_ctx_window_has_focus(void)
{
   return true;
}

void gfx_ctx_swap_buffers(void)
{
   psglSwap();
}
#endif

void gfx_ctx_clear(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
}

void gfx_ctx_set_blend(bool enable)
{
   if(enable)
   {
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
   }
   else
      glDisable(GL_BLEND);
}

void gfx_ctx_set_resize(unsigned width, unsigned height) { }


bool gfx_ctx_menu_init(void)
{
   gl_t *gl = driver.video_data;

   if (!gl)
      return false;

#ifdef HAVE_CG_MENU
   glGenTextures(1, &gl->menu_texture_id);

   RARCH_LOG("Loading texture image for menu...\n");
   if (!texture_image_load(default_paths.menu_border_file, &menu_texture))
   {
      RARCH_ERR("Failed to load texture image for menu.\n");
      return false;
   }

   glBindTexture(GL_TEXTURE_2D, gl->menu_texture_id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_ARGB_SCE, menu_texture.width, menu_texture.height, 0,
		   GL_ARGB_SCE, GL_UNSIGNED_INT_8_8_8_8, menu_texture.pixels);

   glBindTexture(GL_TEXTURE_2D, gl->texture[gl->tex_index]);

   free(menu_texture.pixels);
#endif
	
   return true;
}

void gfx_ctx_update_window_title(bool reset) { }

void gfx_ctx_get_video_size(unsigned *width, unsigned *height)
{
   psglGetDeviceDimensions(gl_device, width, height); 
}

bool gfx_ctx_init(void)
{
   PSGLinitOptions options = {
	   .enable = PSGL_INIT_MAX_SPUS | PSGL_INIT_INITIALIZE_SPUS,
	   .maxSPUs = 1,
	   .initializeSPUs = GL_FALSE,
   };
#if CELL_SDK_VERSION < 0x340000
   options.enable |=	PSGL_INIT_HOST_MEMORY_SIZE;
#endif

   // Initialize 6 SPUs but reserve 1 SPU as a raw SPU for PSGL
   sys_spu_initialize(6, 1);
   psglInit(&options);

   PSGLdeviceParameters params;

   params.enable = PSGL_DEVICE_PARAMETERS_COLOR_FORMAT |
		   PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT |
		   PSGL_DEVICE_PARAMETERS_MULTISAMPLING_MODE;
   params.colorFormat = GL_ARGB_SCE;
   params.depthFormat = GL_NONE;
   params.multisamplingMode = GL_MULTISAMPLING_NONE_SCE;

   if (g_console.triple_buffering_enable)
   {
      params.enable |= PSGL_DEVICE_PARAMETERS_BUFFERING_MODE;
      params.bufferingMode = PSGL_BUFFERING_MODE_TRIPLE;
   }

   if (g_console.current_resolution_id)
   {
      CellVideoOutResolution resolution;
      cellVideoOutGetResolution(g_console.current_resolution_id, &resolution);

      params.enable |= PSGL_DEVICE_PARAMETERS_WIDTH_HEIGHT;
      params.width = resolution.width;
      params.height = resolution.height;
   }

   gl_device = psglCreateDeviceExtended(&params);
   gl_context = psglCreateContext();

   psglMakeCurrent(gl_context, gl_device);
   psglResetCurrentContext();

   return true;
}

bool gfx_ctx_set_video_mode(
      unsigned width, unsigned height,
      unsigned bits, bool fullscreen)
{
   return true;
}

void gfx_ctx_destroy(void)
{
   psglDestroyContext(gl_context);
   psglDestroyDevice(gl_device);

   psglExit();
}

void gfx_ctx_input_driver(const input_driver_t **input, void **input_data) { }

void gfx_ctx_set_filtering(unsigned index, bool set_smooth)
{
   gl_t *gl = driver.video_data;

   if (!gl)
      return;

   if (index == 1)
   {
      // Apply to all PREV textures.
      for (unsigned i = 0; i < TEXTURES; i++)
      {
         glBindTexture(GL_TEXTURE_2D, gl->texture[i]);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, set_smooth ? GL_LINEAR : GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, set_smooth ? GL_LINEAR : GL_NEAREST);
      }
   }
   else if (index >= 2 && gl->fbo_inited)
   {
      glBindTexture(GL_TEXTURE_2D, gl->fbo_texture[index - 2]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, set_smooth ? GL_LINEAR : GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, set_smooth ? GL_LINEAR : GL_NEAREST);
   }

   glBindTexture(GL_TEXTURE_2D, gl->texture[gl->tex_index]);
}

void gfx_ctx_set_fbo(bool enable)
{
   gl_t *gl = driver.video_data;
   gl->fbo_inited = enable;
   gl->render_to_tex = enable;
}

/*============================================================
	MISC
        TODO: Refactor
============================================================ */

void gfx_ctx_get_available_resolutions (void)
{
   bool defaultresolution;
   uint32_t resolution_count;
   uint16_t num_videomodes;

   if (g_console.check_available_resolutions)
      return;

   defaultresolution = true;

   uint32_t videomode[] = {
      CELL_VIDEO_OUT_RESOLUTION_480, CELL_VIDEO_OUT_RESOLUTION_576,
      CELL_VIDEO_OUT_RESOLUTION_960x1080, CELL_VIDEO_OUT_RESOLUTION_720,
      CELL_VIDEO_OUT_RESOLUTION_1280x1080, CELL_VIDEO_OUT_RESOLUTION_1440x1080,
      CELL_VIDEO_OUT_RESOLUTION_1600x1080, CELL_VIDEO_OUT_RESOLUTION_1080
   };

   num_videomodes = sizeof(videomode) / sizeof(uint32_t);

   resolution_count = 0;
   for (unsigned i = 0; i < num_videomodes; i++)
   {
      if (cellVideoOutGetResolutionAvailability(CELL_VIDEO_OUT_PRIMARY, videomode[i], CELL_VIDEO_OUT_ASPECT_AUTO, 0))
         resolution_count++;
   }

   g_console.supported_resolutions = malloc(resolution_count * sizeof(uint32_t));
   g_console.supported_resolutions_count = 0;

   for (unsigned i = 0; i < num_videomodes; i++)
   {
      if (cellVideoOutGetResolutionAvailability(CELL_VIDEO_OUT_PRIMARY, videomode[i], CELL_VIDEO_OUT_ASPECT_AUTO, 0))
      {
         g_console.supported_resolutions[g_console.supported_resolutions_count++] = videomode[i];
         g_console.initial_resolution_id = videomode[i];

         if (g_console.current_resolution_id == videomode[i])
         {
            defaultresolution = false;
            g_console.current_resolution_index = g_console.supported_resolutions_count-1;
         }
      }
   }

   /* In case we didn't specify a resolution - make the last resolution
      that was added to the list (the highest resolution) the default resolution */
   if (g_console.current_resolution_id > num_videomodes || defaultresolution)
      g_console.current_resolution_index = g_console.supported_resolutions_count - 1;

   g_console.check_available_resolutions = true;
}

int gfx_ctx_check_resolution(unsigned resolution_id)
{
   return cellVideoOutGetResolutionAvailability(CELL_VIDEO_OUT_PRIMARY, resolution_id, CELL_VIDEO_OUT_ASPECT_AUTO, 0);
}

const char *ps3_get_resolution_label(uint32_t resolution)
{
   switch (resolution)
   {
      case CELL_VIDEO_OUT_RESOLUTION_480:
	      return  "720x480 (480p)";
      case CELL_VIDEO_OUT_RESOLUTION_576:
	      return "720x576 (576p)"; 
      case CELL_VIDEO_OUT_RESOLUTION_720:
	      return "1280x720 (720p)";
      case CELL_VIDEO_OUT_RESOLUTION_960x1080:
	      return "960x1080";
      case CELL_VIDEO_OUT_RESOLUTION_1280x1080:
	      return "1280x1080";
      case CELL_VIDEO_OUT_RESOLUTION_1440x1080:
	      return "1440x1080";
      case CELL_VIDEO_OUT_RESOLUTION_1600x1080:
	      return "1600x1080";
      case CELL_VIDEO_OUT_RESOLUTION_1080:
	      return "1920x1080 (1080p)";
      default:
	      return "Unknown";
   }
}

void gfx_ctx_set_projection(gl_t *gl, const struct gl_ortho *ortho, bool allow_rotate)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   if (allow_rotate)
   {
      switch (gl->rotation)
      {
         case 90:
            vertex_ptr = vertexes_90;
            break;
         case 180:
            vertex_ptr = vertexes_180;
            break;
         case 270:
            vertex_ptr = vertexes_270;
            break;
         case 0:
         default:
            vertex_ptr = default_vertex_ptr;
            break;
      }
   }

   glVertexPointer(2, GL_FLOAT, 0, vertex_ptr);

   glOrtho(ortho->left, ortho->right, ortho->bottom, ortho->top, ortho->znear, ortho->zfar);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void gfx_ctx_set_aspect_ratio(void *data, unsigned aspectratio_index)
{
   (void)data;
   gl_t *gl = driver.video_data;

   if (g_console.aspect_ratio_index == ASPECT_RATIO_AUTO)
      rarch_set_auto_viewport(g_extern.frame_cache.width, g_extern.frame_cache.height);

   g_settings.video.aspect_ratio = aspectratio_lut[g_console.aspect_ratio_index].value;
   g_settings.video.force_aspect = false;
   gl->keep_aspect = true;
   gl->should_resize = true;
}

void gfx_ctx_set_overscan(void)
{
   gl_t *gl = driver.video_data;
   if (!gl)
      return;

   gl->should_resize = true;
}
