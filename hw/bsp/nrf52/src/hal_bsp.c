/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "os/mynewt.h"
#include "nrfx.h"
#include "flash_map/flash_map.h"
#include "hal/hal_bsp.h"
#include "hal/hal_flash.h"
#include "hal/hal_system.h"
#include "mcu/nrf52_hal.h"
#include "mcu/nrf52_periph.h"
#include "bsp/bsp.h"
#include "defs/sections.h"

#if MYNEWT_VAL(SPIFLASH)  //  If External SPI Flash exists...
#include <spiflash/spiflash.h>
#endif  //  MYNEWT_VAL(SPIFLASH)

/*
 * What memory to include in coredump.
 */
static const struct hal_bsp_mem_dump dump_cfg[] = {
    [0] = {
        .hbmd_start = &_ram_start,
        .hbmd_size = RAM_SIZE
    }
};

/// Array of Flash Devices
static const struct hal_flash *flash_devs[] = {
    [0] = &nrf52k_flash_dev,  //  Internal Flash ROM
#if MYNEWT_VAL(SPIFLASH)      //  If External SPI Flash exists...
    [1] = &spiflash_dev.hal,  //  External SPI Flash
#endif                        //  MYNEWT_VAL(SPIFLASH)
};

/// Return the Flash Device for the ID. 0 for Internal Flash ROM, 1 for External SPI Flash
const struct hal_flash *
hal_bsp_flash_dev(uint8_t id)
{
    if (id >= ARRAY_SIZE(flash_devs)) {
        return NULL;
    }
    return flash_devs[id];
}

const struct hal_bsp_mem_dump *
hal_bsp_core_dump(int *area_cnt)
{
    *area_cnt = sizeof(dump_cfg) / sizeof(dump_cfg[0]);
    return dump_cfg;
}

int
hal_bsp_power_state(int state)
{
    return (0);
}

/**
 * Returns the configured priority for the given interrupt. If no priority
 * configured, return the priority passed in
 *
 * @param irq_num
 * @param pri
 *
 * @return uint32_t
 */
uint32_t
hal_bsp_get_nvic_priority(int irq_num, uint32_t pri)
{
    uint32_t cfg_pri;

    switch (irq_num) {
    /* Radio gets highest priority */
    case RADIO_IRQn:
        cfg_pri = 0;
        break;
    default:
        cfg_pri = pri;
    }
    return cfg_pri;
}

void
hal_bsp_init(void)
{
    /* Make sure system clocks have started */
    hal_system_clock_start();

    /* Create all available nRF52840 peripherals */
    nrf52_periph_create();
}
