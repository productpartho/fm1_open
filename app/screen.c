/* SPDX-License-Identifier: GPL-3.0-only WITH GPL-3.0-linking-exception
 * Copyright (c) 2026 Partho Ghosh.
 *
 * AJ Box's front on FM1 Open's instrument. This file takes the place of FM1
 * Open's screen.c (its editor) and project.c (its project storage): it gives
 * M-VAVE's library the entry points those files gave it, and hands the FM-1's
 * controls and screen to AJ Box (app/ajbox, copied in by tools/sync_firmware.sh
 * of the AJ Box repository). The file keeps the name screen.c because
 * sdk/Build.ps1 builds the library's entry points by name. */
#include "instrument.h"
#include "fm_patch.h"
#include "tuning_hook.h"
#include "fm1.h"
#include "fm1_engine_control.h"
#include "ajbox/ajapp.h"
#include "ajbox/ajpanel.h"
#include "ajbox/ajscreen.h"

/* The library borrows this while it copies a project; AJ Box keeps no projects. */
static struct groove_project scratch;
void groove_project_init(void) {}
void groove_project_poll(void) {}
void groove_project_name(char name[13]) { name[0] = 0; }
void groove_project_metadata(const struct groove_project *p) { (void)p; }
struct groove_project *groove_project_scratch(void) { return &scratch; }
/* The checksum the stock sample bank code (samples.c) uses; from FM1 Open V14's
 * project.c (M-VAVE, same licence). */
uint32_t groove_crc32(uint32_t crc, const void *data, unsigned bytes)
{
    static const uint32_t table[16] = {
        0x00000000U, 0x1db71064U, 0x3b6e20c8U, 0x26d930acU,
        0x76dc4190U, 0x6b6b51f4U, 0x4db26158U, 0x5005713cU,
        0xedb88320U, 0xf00f9344U, 0xd6d6a3e8U, 0xcb61b38cU,
        0x9b64c2b0U, 0x86d3d2d4U, 0xa00ae278U, 0xbdbdf21cU
    };
    const uint8_t *p = data;
    while (bytes--) {
        crc ^= *p++;
        crc = (crc >> 4) ^ table[crc & 15U];
        crc = (crc >> 4) ^ table[crc & 15U];
    }
    return crc;
}

/* sdk/Build.ps1 checks that these parts of M-VAVE's library are in the firmware
 * (Bluetooth MIDI, tunings, project edits). AJ Box does not call them, so they
 * are named here to keep them in. Nothing runs through this table. */
static void *volatile kept[11];
static void keep_library_parts(void)
{
    kept[0] = (void *)groove_tuning_capture; kept[1] = (void *)groove_tuning_apply;
    kept[2] = (void *)groove_project_apply_edit;
    kept[3] = (void *)fm1_ble_set_role; kept[4] = (void *)fm1_ble_scan; kept[5] = (void *)fm1_ble_connect;
    kept[6] = (void *)fm1_ble_disconnect; kept[7] = (void *)fm1_ble_get_status; kept[8] = (void *)fm1_ble_get_device;
    kept[9] = (void *)fm1_bluetooth_get_status; kept[10] = (void *)fm1_ble_forget_bonds;
}

static uint32_t readings_at;

void groove_screen_init(void)
{
    keep_library_parts();
    fm1_controls_init();
    fm1_screen_init();
    ajapp_init(&ajb_factory_library);
    ajpanel_init();
    const uint32_t now = fm1_millis();
    ajscreen_init(now);
    /* Which battery readings mean full and low is not known yet: leave the
     * battery picture out and show the reading in parent mode instead. */
    ajscreen_battery(AJSCREEN_BATTERY_UNKNOWN, 0, now);
    readings_at = now;
    ajapp_chime();
    (void)ajscreen_draw(now, 1);
}

void groove_screen_control(void)
{
    struct fm1_controls controls;
    fm1_controls_read(&controls);
    const uint32_t now = fm1_millis();
    (void)groove_master_update(controls.knob);          /* the MASTER volume knob */
    ajpanel_step(&controls, now);
    ajapp_tick();
    if (now - readings_at >= 500U) {
        struct fm1_engine_stats engine;
        struct fm1_audio_stats output;
        struct fm1_render_stats render;
        fm1_engine_stats_read(&engine);
        fm1_audio_stats_read(&output);
        groove_audio_stats(&render);
        ajscreen_readings(controls.battery, engine.underruns + output.overruns + render.overloads);
        readings_at = now;
    }
    (void)ajscreen_draw(now, 1);
}
