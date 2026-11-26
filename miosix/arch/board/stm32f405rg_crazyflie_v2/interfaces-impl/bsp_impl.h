/***************************************************************************
 *   Copyright (C) 2012 by Terraneo Federico                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/ 

/***********************************************************************
* bsp_impl.h Part of the Miosix Embedded OS.
* Board support package, this file initializes hardware.
************************************************************************/

#ifndef BSP_IMPL_H
#define BSP_IMPL_H

#include "miosix_settings.h"
#include "interfaces/gpio.h"

namespace miosix {

/**
\addtogroup Hardware
\{
*/

typedef Gpio<PC, 0> _led_red_l;
typedef Gpio<PC, 1> _led_green_l;
typedef Gpio<PC, 2> _led_green_r;
typedef Gpio<PC, 3> _led_red_r;
typedef Gpio<PD, 2> _led_blue_l;

#define MAKE_LED_API_ACTIVE_LOW(name, pin) \
    inline void led##name##On() \
    { \
        pin::low(); \
    } \
    inline void led##name##Off() \
    { \
        pin::high(); \
    }

#define MAKE_LED_API_ACTIVE_HIGH(name, pin) \
    inline void led##name##On() \
    { \
        pin::high(); \
    } \
    inline void led##name##Off() \
    { \
        pin::low(); \
    }

MAKE_LED_API_ACTIVE_LOW(RedLeft, _led_red_l)
MAKE_LED_API_ACTIVE_LOW(GreenLeft, _led_green_l)
MAKE_LED_API_ACTIVE_LOW(RedRight, _led_red_r)
MAKE_LED_API_ACTIVE_LOW(GreenRight, _led_green_r)
MAKE_LED_API_ACTIVE_HIGH(BlueLeft, _led_blue_l)

/**
\}
*/

} //namespace miosix

#endif //BSP_IMPL_H
