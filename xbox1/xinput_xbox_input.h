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

#ifndef _XDK_XINPUT1_H
#define _XDK_XINPUT1_H

enum {
   XINPUT1_GAMEPAD_B =			1 << 0,
   XINPUT1_GAMEPAD_A =			1 << 1,
   XINPUT1_GAMEPAD_Y =			1 << 2,
   XINPUT1_GAMEPAD_X =			1 << 3,
   XINPUT1_GAMEPAD_DPAD_UP =		1 << 4,
   XINPUT1_GAMEPAD_DPAD_DOWN =		1 << 5,
   XINPUT1_GAMEPAD_DPAD_LEFT =		1 << 6,
   XINPUT1_GAMEPAD_DPAD_RIGHT =		1 << 7,
   XINPUT1_GAMEPAD_BACK =		1 << 8,
   XINPUT1_GAMEPAD_START =		1 << 9,
   XINPUT1_GAMEPAD_WHITE =		1 << 10,
   XINPUT1_GAMEPAD_LEFT_TRIGGER =	1 << 11,
   XINPUT1_GAMEPAD_LEFT_THUMB =		1 << 12,
   XINPUT1_GAMEPAD_BLACK =		1 << 13,
   XINPUT1_GAMEPAD_RIGHT_TRIGGER =	1 << 14,
   XINPUT1_GAMEPAD_RIGHT_THUMB =	1 << 15,
   XINPUT1_GAMEPAD_LSTICK_LEFT_MASK =	1 << 16,
   XINPUT1_GAMEPAD_LSTICK_RIGHT_MASK =	1 << 17,
   XINPUT1_GAMEPAD_LSTICK_UP_MASK =	1 << 18,
   XINPUT1_GAMEPAD_LSTICK_DOWN_MASK =	1 << 19,
   XINPUT1_GAMEPAD_RSTICK_LEFT_MASK =	1 << 20,
   XINPUT1_GAMEPAD_RSTICK_RIGHT_MASK =	1 << 21,
   XINPUT1_GAMEPAD_RSTICK_UP_MASK =	1 << 22,
   XINPUT1_GAMEPAD_RSTICK_DOWN_MASK =	1 << 23,
};

#endif
