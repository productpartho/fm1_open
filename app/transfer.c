/* SPDX-License-Identifier: GPL-3.0-only WITH GPL-3.0-linking-exception
 * Copyright (c) 2026 Partho Ghosh.
 *
 * AJ Box has no workbench transfers: projects, voices and samples are not
 * sent to or read from it over USB. M-VAVE's library still calls these three,
 * so they are here and do nothing. Firmware updates do not come through this
 * file; M-VAVE's library handles them itself. */
#include "instrument.h"
#include "fm1.h"

void fm1_app_sysex(const uint8_t *message, size_t bytes) { (void)message; (void)bytes; }
void groove_transfer_init(void) {}
void groove_transfer_poll(void) {}
