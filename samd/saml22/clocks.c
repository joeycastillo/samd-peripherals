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

#include "hal_atomic.h"
#include "samd/clocks.h"

bool gclk_enabled(uint8_t gclk) {
    return GCLK->GENCTRL[gclk].bit.GENEN;
}

void disable_gclk(uint8_t gclk) {
    while (GCLK->SYNCBUSY.reg & (1 << (GCLK_SYNCBUSY_GENCTRL0_Pos + gclk)));
    GCLK->GENCTRL[gclk].reg &= ~GCLK_GENCTRL_GENEN;
    while (GCLK->SYNCBUSY.reg & (1 << (GCLK_SYNCBUSY_GENCTRL0_Pos + gclk)));
}

void connect_gclk_to_peripheral(uint8_t gclk, uint8_t peripheral) {
    hri_gclk_write_PCHCTRL_reg(GCLK, peripheral, gclk | GCLK_PCHCTRL_CHEN);
}

void disconnect_gclk_from_peripheral(uint8_t gclk, uint8_t peripheral) {
    hri_gclk_write_PCHCTRL_reg(GCLK, peripheral, gclk);
}

void enable_clock_generator(uint8_t gclk, uint32_t source, uint16_t divisor) {
    uint32_t divsel = 0;
    // The datasheet says 8 bits and max value of 512, how is that possible?
    if (divisor > 255) { // Generator 1 has 16 bits
        divsel = GCLK_GENCTRL_DIVSEL;
        for (int i = 15; i > 0; i--) {
            if (divisor & (1 << i)) {
                divisor = i - 1;
                break;
            }
        }
    }

    GCLK->GENCTRL[gclk].reg = GCLK_GENCTRL_SRC(source) | GCLK_GENCTRL_DIV(divisor) | divsel | GCLK_GENCTRL_OE | GCLK_GENCTRL_GENEN;
}

void disable_clock_generator(uint8_t gclk) {
    GCLK->GENCTRL[gclk].reg = 0;
    while ((GCLK->SYNCBUSY.vec.GENCTRL & (1 << gclk)) != 0) {}
}

static void osc16minit_clock_source_osc16m(void) {
    OSCCTRL->OSC16MCTRL.bit.ONDEMAND = 0;
    OSCCTRL->OSC16MCTRL.bit.FSEL = 3;
    OSCCTRL->OSC16MCTRL.bit.ENABLE = 1;
    while (!OSCCTRL->STATUS.bit.OSC16MRDY) {}
}

static void init_clock_source_osculp32k(void) {
    uint16_t calib = (OSC32KCTRL->OSCULP32K.reg & OSC32KCTRL_OSCULP32K_CALIB_Msk) >> OSC32KCTRL_OSCULP32K_CALIB_Pos;
    OSC32KCTRL->OSCULP32K.reg = OSC32KCTRL_OSCULP32K_CALIB(calib);
}

static void init_clock_source_xosc32k(void) {
    OSC32KCTRL->XOSC32K.reg = OSC32KCTRL_XOSC32K_EN32K |
                              OSC32KCTRL_XOSC32K_XTALEN |
                              OSC32KCTRL_XOSC32K_ENABLE;
    while (!OSC32KCTRL->STATUS.bit.XOSC32KRDY) {}
}

// static void init_clock_source_dfll48m_xosc(void) {
//     OSCCTRL->DFLLCTRL.reg = OSCCTRL_DFLLCTRL_ENABLE;
//     while (!OSCCTRL->STATUS.bit.DFLLRDY) {}
//     OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_CSTEP(0x1f / 4) |
//                            OSCCTRL_DFLLMUL_FSTEP(0xff / 4) |
//                            OSCCTRL_DFLLMUL_MUL(48000000 / 32768);
//     uint32_t coarse = (*((uint32_t *)FUSES_DFLL48M_COARSE_CAL_ADDR) & FUSES_DFLL48M_COARSE_CAL_Msk) >> FUSES_DFLL48M_COARSE_CAL_Pos;
//     if (coarse == 0x3f) {
//         coarse = 0x1f;
//     }
//     OSCCTRL->DFLLVAL.reg = OSCCTRL_DFLLVAL_COARSE(coarse) |
//                            OSCCTRL_DFLLVAL_FINE(512);

//     OSCCTRL->DFLLCTRL.reg = 0;
//     while (!OSCCTRL->STATUS.bit.DFLLRDY) {}
//     OSCCTRL->DFLLCTRL.reg = OSCCTRL_DFLLCTRL_MODE |
//                             OSCCTRL_DFLLCTRL_ENABLE;
//     while (!OSCCTRL->STATUS.bit.DFLLRDY) {}

//     // Wait for the fine lock on the DFLL.
//     while (!OSCCTRL->STATUS.bit.DFLLLCKC || !OSCCTRL->STATUS.bit.DFLLLCKF) {}
// }

static void init_clock_source_dfll48m_usb(uint32_t fine_calibration) {
    OSCCTRL->INTFLAG.reg = OSCCTRL_INTFLAG_DFLLRDY;
    OSCCTRL->DFLLCTRL.reg = 0;
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY));

    // set the coarse and fine values to speed up frequency lock.
    uint32_t coarse =(*((uint32_t *)NVMCTRL_OTP5)) >> 26;
    OSCCTRL->DFLLVAL.reg = OSCCTRL_DFLLVAL_COARSE(coarse) |
                           OSCCTRL_DFLLVAL_FINE(0x200);
    // set coarse and fine steps, and multiplier (48 MHz = 32768 Hz * 1465)
    OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_CSTEP( 1 ) |
                           OSCCTRL_DFLLMUL_FSTEP( 1 ) |
                           OSCCTRL_DFLLMUL_MUL( 1465 );
    // set closed loop mode, chill cycle disable and USB clock recovery mode, and enable the DFLL.
    OSCCTRL->DFLLCTRL.reg = OSCCTRL_DFLLCTRL_MODE | OSCCTRL_DFLLCTRL_CCDIS | OSCCTRL_DFLLCTRL_ONDEMAND | OSCCTRL_DFLLCTRL_RUNSTDBY | OSCCTRL_DFLLCTRL_USBCRM | OSCCTRL_DFLLCTRL_ENABLE;
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY));
}

void clock_init(bool has_crystal, uint32_t dfll48m_fine_calibration)
{
    osc16minit_clock_source_osc16m();
    if (has_crystal) {
        init_clock_source_xosc32k();
    } else {
        init_clock_source_osculp32k();
    }

    init_clock_source_dfll48m_usb(dfll48m_fine_calibration);

    enable_clock_generator(0, GCLK_GENCTRL_SRC_OSC16M_Val, 1);
    enable_clock_generator(1, GCLK_GENCTRL_SRC_DFLL48M_Val, 1);
    if (has_crystal) {
        enable_clock_generator(2, GCLK_GENCTRL_SRC_XOSC32K_Val, 1);
    } else {
        enable_clock_generator(2, GCLK_GENCTRL_SRC_OSCULP32K_Val, 1);
    }

    // Do this after all static clock init so that they aren't used dynamically.
    init_dynamic_clocks();
}

static bool clk_enabled(uint8_t clk) {
    return GCLK->PCHCTRL[clk].bit.CHEN;
}

static uint8_t clk_get_generator(uint8_t clk) {
    return GCLK->PCHCTRL[clk].bit.GEN;
}

static uint8_t generator_get_source(uint8_t gen) {
    return GCLK->GENCTRL[gen].bit.SRC;
}

static bool osc_enabled(uint8_t index) {
    switch (index) {
        case GCLK_SOURCE_XOSC:
            return OSCCTRL->XOSCCTRL.bit.ENABLE;
        case GCLK_SOURCE_OSCULP32K:
            return true;
        case GCLK_SOURCE_XOSC32K:
            return OSC32KCTRL->XOSC32K.bit.ENABLE;
        case GCLK_SOURCE_OSC16M:
            return OSCCTRL->OSC16MCTRL.bit.ENABLE;
        case GCLK_SOURCE_DFLL48M:
            return OSCCTRL->DFLLCTRL.bit.ENABLE;
        case GCLK_SOURCE_DPLL96M:
            return OSCCTRL->DPLLCTRLA.bit.ENABLE;
    };
    return false;
}

static uint32_t osc_get_frequency(uint8_t index) {
    switch (index) {
        case GCLK_SOURCE_XOSC:
            return 0; // unknown
        case GCLK_SOURCE_OSCULP32K:
        case GCLK_SOURCE_XOSC32K:
            return 32768;
        case GCLK_SOURCE_OSC16M:
            return 16000000;
        case GCLK_SOURCE_DFLL48M:
            return 48000000;
        case GCLK_SOURCE_DPLL96M:
            return 96000000;
    }
    return 0;
}

bool clock_get_enabled(uint8_t type, uint8_t index) {
    if (type == 0)
        return osc_enabled(index);
    if (type == 1)
        return clk_enabled(index);
    if (type == 2)
        return SysTick->CTRL & SysTick_CTRL_ENABLE_Msk;
    return false;
}

bool clock_get_parent(uint8_t type, uint8_t index, uint8_t *p_type, uint8_t *p_index) {
    if (type == 1 && index <= 0x24 && clk_enabled(index)) {
        *p_type = 0;
        *p_index = generator_get_source(clk_get_generator(index));
        return true;
    }
    if (type == 2 && index == 0) {
        *p_type = 0;
        *p_index = generator_get_source(0);
        return true;
    }
    return false;
}

static uint32_t generator_get_frequency(uint8_t gen) {
    uint8_t src = GCLK->GENCTRL[gen].bit.SRC;
    uint32_t div;
    if (GCLK->GENCTRL[gen].bit.DIVSEL) {
        div = 1 << (GCLK->GENCTRL[gen].bit.DIV + 1);
    } else {
        div = GCLK->GENCTRL[gen].bit.DIV;
        if (!div)
            div = 1;
    }

    return osc_get_frequency(src) / div;
}

uint32_t clock_get_frequency(uint8_t type, uint8_t index) {
    if (type == 0) {
        return osc_get_frequency(index);
    }
    if (type == 1 && index <= 47 && clk_enabled(index)) {
        return generator_get_frequency(clk_get_generator(index));
    }
    if (type == 2) {
        switch (index) {
            case 0:
                return clock_get_frequency(0, generator_get_source(0)) / SysTick->LOAD;
            case 1:
                return clock_get_frequency(0, generator_get_source(0)) / MCLK->CPUDIV.bit.CPUDIV;
            case 2:
                switch (OSC32KCTRL->RTCCTRL.bit.RTCSEL) {
                    case 0:
                    case 4:
                        return 1024;
                    case 1:
                    case 5:
                        return 32768;
                }
        }
    }
    return 0;
}

uint32_t clock_get_calibration(uint8_t type, uint8_t index) {
    if (type == 0) {
        switch (index) {
            case GCLK_SOURCE_OSCULP32K:
                return OSC32KCTRL->OSCULP32K.bit.CALIB;
        };
    }
    if (type == 2 && index == 0) {
        return SysTick->LOAD + 1;
    }
    return 0;
}

int clock_set_calibration(uint8_t type, uint8_t index, uint32_t val) {
    if (type == 0) {
        switch (index) {
            case GCLK_SOURCE_OSCULP32K:
                if (val > 0x3f)
                    return -1;
                OSC32KCTRL->OSCULP32K.bit.CALIB = val;
                return 0;
        };
    }
    if (type == 2 && index == 0) {
        if (val < 0x1000 || val > 0x1000000)
            return -1;
        SysTick->LOAD = val - 1;
        return 0;
    }
    return -2;
}
