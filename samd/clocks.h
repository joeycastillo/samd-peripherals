/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Scott Shawcroft for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MICROPY_INCLUDED_ATMEL_SAMD_CLOCKS_H
#define MICROPY_INCLUDED_ATMEL_SAMD_CLOCKS_H

#include <stdbool.h>
#include <stdint.h>

#include "include/sam.h"

#ifdef SAM_D5X_E5X
#define CLOCK_48MHZ GCLK_GENCTRL_SRC_DFLL_Val
#endif
#ifdef SAMD21
#define CLOCK_48MHZ GCLK_GENCTRL_SRC_DFLL48M_Val
#endif
#ifdef SAML22
#define CLOCK_48MHZ GCLK_GENCTRL_SRC_DFLL48M_Val
#endif

// Pass to clock_init() if fine calibration not known.
#define DEFAULT_DFLL48M_FINE_CALIBRATION 512

#define CORE_GCLK 0

uint8_t find_free_gclk(uint16_t divisor);

bool gclk_enabled(uint8_t gclk);
void disable_gclk(uint8_t gclk);
void reset_gclks(void);

void connect_gclk_to_peripheral(uint8_t gclk, uint8_t peripheral);
void disconnect_gclk_from_peripheral(uint8_t gclk, uint8_t peripheral);

void enable_clock_generator(uint8_t gclk, uint32_t source, uint16_t divisor);
void disable_clock_generator(uint8_t gclk);

void clock_init(bool has_crystal, uint32_t dfll48m_fine_calibration);
void init_dynamic_clocks(void);

bool clock_get_enabled(uint8_t type, uint8_t index);
bool clock_get_parent(uint8_t type, uint8_t index, uint8_t *p_type, uint8_t *p_index);
uint32_t clock_get_frequency(uint8_t type, uint8_t index);
uint32_t clock_get_calibration(uint8_t type, uint8_t index);
int clock_set_calibration(uint8_t type, uint8_t index, uint32_t val);

#endif  // MICROPY_INCLUDED_ATMEL_SAMD_CLOCKS_H
