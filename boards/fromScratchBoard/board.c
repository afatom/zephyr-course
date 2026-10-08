/*
 * Copyright (c) 2018 Aapo Vienamo
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


LOG_MODULE_REGISTER(pre_main_board_init, LOG_LEVEL_INF);

static int pre_main_board_init(void)
{
	LOG_INF("===============================================");
	LOG_INF("=              Board Initialized              =");
	LOG_INF("===============================================");

	return 0;
}


SYS_INIT(pre_main_board_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
