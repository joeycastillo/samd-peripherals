/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Scott Shawcroft for Adafruit Industries
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

#include "samd/pins.h"

#define SERCOM(sercom_index, p_pad)     \
{ \
  .index = sercom_index,   \
  .pad = p_pad \
}

#define NO_SERCOM \
{ \
  .index = 0x3f, \
  .pad = 0 \
}


#define TCC(p_index, p_wave_output) \
{ \
  .index = p_index, \
  .is_tc = false, \
  .wave_output = p_wave_output \
}

#define TC(p_index, p_wave_output) \
{ \
  .index = p_index - 3, \
  .is_tc = true, \
  .wave_output = p_wave_output \
}

#define NO_TIMER TCC(0xf, 0)

#define TOUCH(y_line) \
    .has_touch = true, \
    .touch_y_line = y_line,

#define NO_TOUCH \
    .has_touch = false,

#define EXTINT_CHANNEL(channel) \
    .has_extint = true, \
    .extint_channel = channel,

#define NO_EXTINT \
    .has_extint = false,

#define ADC_INPUT(input) input

#define NO_ADC 0xff

// This macro is used to simplify pin definition in boards/<board>/pins.c
#define PIN(p_name, p_extint, p_adc, p_touch, \
            p_primary_sercom, p_secondary_sercom, \
            p_primary_timer, p_secondary_timer) \
const mcu_pin_obj_t pin_## p_name = { \
    PIN_PREFIX_VALUES \
    .number = (PIN_## p_name), \
    p_extint \
    p_touch \
    .adc_input = {p_adc}, \
    .timer = { p_primary_timer, p_secondary_timer}, \
    .sercom = {p_primary_sercom, p_secondary_sercom}, \
}

// Pins in datasheet order.
// NOTE(tannewt): TC wave out 0 is commented out because the first channel is
// used to vary the 16 bit timer's frequency.
#if defined(PIN_PA00) && !defined(IGNORE_PIN_PA00)
PIN(PA00, EXTINT_CHANNEL(0), NO_ADC, NO_TOUCH,
    NO_SERCOM,
    SERCOM(1, 0),
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PA01) && !defined(IGNORE_PIN_PA01)
PIN(PA01, EXTINT_CHANNEL(1), NO_ADC, NO_TOUCH,
    NO_SERCOM,
    SERCOM(1, 1),
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PA02) && !defined(IGNORE_PIN_PA02)
PIN(PA02, EXTINT_CHANNEL(2), ADC_INPUT(0), TOUCH(8),
    NO_SERCOM,
    NO_SERCOM,
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PA03) && !defined(IGNORE_PIN_PA03)
PIN(PA03, EXTINT_CHANNEL(3), ADC_INPUT(1), TOUCH(9),
    NO_SERCOM,
    NO_SERCOM,
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PB04) && !defined(IGNORE_PIN_PB04)
PIN(PB04, EXTINT_CHANNEL(4), ADC_INPUT(12), TOUCH(10),
    NO_SERCOM,
    NO_SERCOM,
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PB05) && !defined(IGNORE_PIN_PB05)
PIN(PB05, EXTINT_CHANNEL(5), ADC_INPUT(13), TOUCH(11),
    NO_SERCOM,
    NO_SERCOM,
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PB06) && !defined(IGNORE_PIN_PB06)
PIN(PB06, EXTINT_CHANNEL(6), ADC_INPUT(14), TOUCH(12),
    NO_SERCOM,
    NO_SERCOM,
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PB07) && !defined(IGNORE_PIN_PB07)
PIN(PB07, EXTINT_CHANNEL(7), ADC_INPUT(15), TOUCH(13),
    NO_SERCOM,
    NO_SERCOM,
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PB08) && !defined(IGNORE_PIN_PB08)
PIN(PB08, EXTINT_CHANNEL(8), ADC_INPUT(2), TOUCH(14),
    NO_SERCOM,
    SERCOM(3, 0),
    TC(0, 0),
    NO_TIMER);
#endif
#if defined(PIN_PB09) && !defined(IGNORE_PIN_PB09)
PIN(PB09, EXTINT_CHANNEL(9), ADC_INPUT(3), TOUCH(15),
    NO_SERCOM,
    SERCOM(3, 1),
    TC(0, 1),
    NO_TIMER);
#endif
#if defined(PIN_PA04) && !defined(IGNORE_PIN_PA04)
PIN(PA04, EXTINT_CHANNEL(4), ADC_INPUT(4), NO_TOUCH,
    NO_SERCOM,
    SERCOM(0, 0),
    TCC(0, 0),
    NO_TIMER);
#endif
#if defined(PIN_PA05) && !defined(IGNORE_PIN_PA05)
PIN(PA05, EXTINT_CHANNEL(5), ADC_INPUT(5), NO_TOUCH,
    NO_SERCOM,
    SERCOM(0, 1),
    TCC(0, 1),
    NO_TIMER);
#endif
#if defined(PIN_PA06) && !defined(IGNORE_PIN_PA06)
PIN(PA06, EXTINT_CHANNEL(6), ADC_INPUT(6), NO_TOUCH,
    NO_SERCOM,
    SERCOM(0, 2),
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PA07) && !defined(IGNORE_PIN_PA07)
PIN(PA07, EXTINT_CHANNEL(7), ADC_INPUT(7), NO_TOUCH,
    NO_SERCOM,
    SERCOM(0, 3),
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PA08) && !defined(IGNORE_PIN_PA08)
PIN(PA08, NO_EXTINT, NO_ADC, TOUCH(3),
    SERCOM(0, 0),
    #ifdef SERCOM4
    SERCOM(4, 0),
    #else
    NO_SERCOM,
    #endif
    TCC(0, 0),
    NO_TIMER);
#endif
#if defined(PIN_PA09) && !defined(IGNORE_PIN_PA09)
PIN(PA09, EXTINT_CHANNEL(9), NO_ADC, TOUCH(2),
    SERCOM(0, 1),
    #ifdef SERCOM4
    SERCOM(4, 1),
    #else
    NO_SERCOM,
    #endif
    TCC(0, 1),
    NO_TIMER);
#endif
#if defined(PIN_PA10) && !defined(IGNORE_PIN_PA10)
PIN(PA10, EXTINT_CHANNEL(10), NO_ADC, TOUCH(1),
    SERCOM(0, 2),
    #ifdef SERCOM4
    SERCOM(4, 2),
    #else
    NO_SERCOM,
    #endif
    NO_TIMER,
    TCC(0, 2));
#endif
#if defined(PIN_PA11) && !defined(IGNORE_PIN_PA11)
PIN(PA11, EXTINT_CHANNEL(11), NO_ADC, TOUCH(0),
    SERCOM(0, 3),
    #ifdef SERCOM4
    SERCOM(4, 3),
    #else
    NO_SERCOM,
    #endif
    NO_TIMER,
    TCC(0, 3));
#endif

// Second page.
#if defined(PIN_PB11) && !defined(IGNORE_PIN_PB11)
PIN(PB11, EXTINT_CHANNEL(11), NO_ADC, NO_TOUCH,
    NO_SERCOM,
    SERCOM(3, 3),
    TC(1, 1),
    TCC(0, 5));
#endif
#if defined(PIN_PB12) && !defined(IGNORE_PIN_PB12)
PIN(PB12, EXTINT_CHANNEL(12), NO_ADC, NO_TOUCH,
    SERCOM(3, 0),
    NO_SERCOM,
    TC(0, 0),
    TCC(0, 6));
#endif
#if defined(PIN_PB13) && !defined(IGNORE_PIN_PB13)
PIN(PB13, EXTINT_CHANNEL(13), NO_ADC, NO_TOUCH,
    SERCOM(3, 1),
    NO_SERCOM,
    TC(0, 1),
    TCC(0, 7));
#endif
#if defined(PIN_PB14) && !defined(IGNORE_PIN_PB14)
PIN(PB14, EXTINT_CHANNEL(14), NO_ADC, NO_TOUCH,
    SERCOM(3, 2),
    NO_SERCOM,
    TC(1, 0),
    NO_TIMER);
#endif
#if defined(PIN_PB15) && !defined(IGNORE_PIN_PB15)
PIN(PB15, EXTINT_CHANNEL(15), NO_ADC, NO_TOUCH,
    SERCOM(3, 3),
    NO_SERCOM,
    TC(1, 1),
    NO_TIMER);
#endif
#if defined(PIN_PA12) && !defined(IGNORE_PIN_PA12)
PIN(PA12, EXTINT_CHANNEL(12), NO_ADC, NO_TOUCH,
    #ifdef SERCOM4
    SERCOM(4, 0),
    #else
    NO_SERCOM,
    #endif
    SERCOM(3, 0),
    NO_TIMER,
    TCC(0, 6));
#endif
#if defined(PIN_PA13) && !defined(IGNORE_PIN_PA13)
PIN(PA13, EXTINT_CHANNEL(13), NO_ADC, NO_TOUCH,
    #ifdef SERCOM4
    SERCOM(4, 1),
    #else
    NO_SERCOM,
    #endif
    SERCOM(3, 1),
    NO_TIMER,
    TCC(0, 7));
#endif
#if defined(PIN_PA14) && !defined(IGNORE_PIN_PA14)
PIN(PA14, EXTINT_CHANNEL(14), NO_ADC, NO_TOUCH,
    #ifdef SERCOM4
    SERCOM(4, 2),
    #else
    NO_SERCOM,
    #endif
    SERCOM(3, 2),
    NO_TIMER,
    TCC(0, 4));
#endif
#if defined(PIN_PA15) && !defined(IGNORE_PIN_PA15)
PIN(PA15, EXTINT_CHANNEL(15), NO_ADC, NO_TOUCH,
    #ifdef SERCOM4
    SERCOM(4, 3),
    #else
    NO_SERCOM,
    #endif
    SERCOM(3, 3),
    NO_TIMER,
    TCC(0, 5));
#endif
#if defined(PIN_PA16) && !defined(IGNORE_PIN_PA16)
PIN(PA16, EXTINT_CHANNEL(0), NO_ADC, NO_TOUCH,
    SERCOM(1, 0),
    SERCOM(2, 0),
    NO_TIMER,
    TCC(0, 6));
#endif
#if defined(PIN_PA17) && !defined(IGNORE_PIN_PA17)
PIN(PA17, EXTINT_CHANNEL(1), NO_ADC, NO_TOUCH,
    SERCOM(1, 1),
    SERCOM(2, 1),
    NO_TIMER,
    TCC(0, 7));
#endif
#if defined(PIN_PA18) && !defined(IGNORE_PIN_PA18)
PIN(PA18, EXTINT_CHANNEL(2), NO_ADC, NO_TOUCH,
    SERCOM(1, 2),
    SERCOM(2, 2),
    NO_TIMER,
    TCC(0, 2));
#endif
#if defined(PIN_PA19) && !defined(IGNORE_PIN_PA19)
PIN(PA19, EXTINT_CHANNEL(3), NO_ADC, NO_TOUCH,
    SERCOM(1, 3),
    SERCOM(2, 3),
    NO_TIMER,
    TCC(0, 3));
#endif
#if defined(PIN_PB16) && !defined(IGNORE_PIN_PB16)
PIN(PB16, EXTINT_CHANNEL(0), NO_ADC, NO_TOUCH,
    #ifdef SERCOM5
    SERCOM(5, 0),
    #else
    NO_SERCOM,
    #endif
    NO_SERCOM,
    TC(2, 0),
    TCC(0, 4));
#endif
#if defined(PIN_PB17) && !defined(IGNORE_PIN_PB17)
PIN(PB17, EXTINT_CHANNEL(1), NO_ADC, NO_TOUCH,
    #ifdef SERCOM5
    SERCOM(5, 1),
    #else
    NO_SERCOM,
    #endif
    NO_SERCOM,
    TC(2, 1),
    TCC(0, 5));
#endif
#if defined(PIN_PA20) && !defined(IGNORE_PIN_PA20)
PIN(PA20, EXTINT_CHANNEL(4), NO_ADC, TOUCH(16),
    SERCOM(0, 0),
    SERCOM(2, 2),
    TC(3, 0),
    TCC(0, 6));
#endif
#if defined(PIN_PA21) && !defined(IGNORE_PIN_PA21)
PIN(PA21, EXTINT_CHANNEL(5), NO_ADC, TOUCH(17),
    SERCOM(0, 1),
    SERCOM(2, 3),
    TC(3, 1),
    TCC(0, 7));
#endif
#if defined(PIN_PA22) && !defined(IGNORE_PIN_PA22)
PIN(PA22, EXTINT_CHANNEL(6), NO_ADC, TOUCH(18),
    SERCOM(0, 2),
    SERCOM(2, 0),
    TC(0, 0),
    TCC(0, 4));
#endif
#if defined(PIN_PA23) && !defined(IGNORE_PIN_PA23)
PIN(PA23, EXTINT_CHANNEL(7), NO_ADC, TOUCH(19),
    SERCOM(0, 3),
    SERCOM(2, 1),
    TC(0, 1),
    TCC(0, 5));
#endif
#if defined(PIN_PA24) && !defined(IGNORE_PIN_PA24)
PIN(PA24, EXTINT_CHANNEL(12), NO_ADC, NO_TOUCH,
    SERCOM(2, 2),
    #ifdef SERCOM5
    SERCOM(5, 0),
    #else
    NO_SERCOM,
    #endif
    TC(1, 0),
    TCC(0, 0));
#endif
#if defined(PIN_PA25) && !defined(IGNORE_PIN_PA25)
PIN(PA25, EXTINT_CHANNEL(13), NO_ADC, NO_TOUCH,
    SERCOM(2, 3),
    #ifdef SERCOM5
    SERCOM(5, 1),
    #else
    NO_SERCOM,
    #endif
    TC(1, 1),
    TCC(0, 1));
#endif
#if defined(PIN_PB22) && !defined(IGNORE_PIN_PB22)
PIN(PB22, EXTINT_CHANNEL(6), NO_ADC, NO_TOUCH,
    SERCOM(0, 2),
    #ifdef SERCOM5
    SERCOM(5, 2),
    #else
    NO_SERCOM,
    #endif
    TC(3, 0),
    TCC(0, 2));
#endif
#if defined(PIN_PB23) && !defined(IGNORE_PIN_PB23)
PIN(PB23, EXTINT_CHANNEL(7), NO_ADC, NO_TOUCH,
    SERCOM(0, 3),
    #ifdef SERCOM5
    SERCOM(5, 3),
    #else
    NO_SERCOM,
    #endif
    TC(3, 1),
    TCC(0, 3));
#endif

// Third page.
#if defined(PIN_PA27) && !defined(IGNORE_PIN_PA27)
PIN(PA27, EXTINT_CHANNEL(15), NO_ADC, TOUCH(21),
    NO_SERCOM,
    NO_SERCOM,
    NO_TIMER,
    TCC(0, 5));
#endif
#if defined(PIN_PA30) && !defined(IGNORE_PIN_PA30)
PIN(PA30, EXTINT_CHANNEL(10), NO_ADC, TOUCH(22),
    NO_SERCOM,
    SERCOM(1, 2),
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PA31) && !defined(IGNORE_PIN_PA31)
PIN(PA31, EXTINT_CHANNEL(11), NO_ADC, TOUCH(23),
    NO_SERCOM,
    SERCOM(1, 3),
    NO_TIMER,
    NO_TIMER);
#endif
#if defined(PIN_PB30) && !defined(IGNORE_PIN_PB30)
PIN(PB30, EXTINT_CHANNEL(14), NO_ADC, NO_TOUCH,
    SERCOM(1, 0),
    #ifdef SERCOM5
    SERCOM(5, 0),
    #else
    NO_SERCOM,
    #endif
    TCC(0, 0),
    NO_TIMER);
#endif
#if defined(PIN_PB31) && !defined(IGNORE_PIN_PB31)
PIN(PB31, EXTINT_CHANNEL(15), NO_ADC, NO_TOUCH,
    SERCOM(1, 1),
    #ifdef SERCOM5
    SERCOM(5, 1),
    #else
    NO_SERCOM,
    #endif
    TCC(0, 1),
    NO_TIMER);
#endif
#if defined(PIN_PB00) && !defined(IGNORE_PIN_PB00)
PIN(PB00, EXTINT_CHANNEL(0), ADC_INPUT(8), TOUCH(6),
    SERCOM(3, 2),
    #ifdef SERCOM5
    SERCOM(5, 2),
    #else
    NO_SERCOM,
    #endif
    TC(3, 0),
    NO_TIMER);
#endif
#if defined(PIN_PB01) && !defined(IGNORE_PIN_PB01)
PIN(PB01, EXTINT_CHANNEL(1), ADC_INPUT(9), TOUCH(7),
    SERCOM(3, 3),
    #ifdef SERCOM5
    SERCOM(5, 3),
    #else
    NO_SERCOM,
    #endif
    TC(3, 1),
    NO_TIMER);
#endif
#if defined(PIN_PB02) && !defined(IGNORE_PIN_PB02)
PIN(PB02, EXTINT_CHANNEL(2), ADC_INPUT(10), TOUCH(8),
    SERCOM(3, 0),
    #ifdef SERCOM5
    SERCOM(5, 0),
    #else
    NO_SERCOM,
    #endif
    TC(2, 0),
    NO_TIMER);
#endif
#if defined(PIN_PB03) && !defined(IGNORE_PIN_PB03)
PIN(PB03, EXTINT_CHANNEL(3), ADC_INPUT(11), TOUCH(9),
    SERCOM(3, 1),
    #ifdef SERCOM5
    SERCOM(5, 1),
    #else
    NO_SERCOM,
    #endif
    TC(2, 1),
    NO_TIMER);
#endif
