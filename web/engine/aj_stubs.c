/* SPDX-License-Identifier: GPL-3.0-only WITH GPL-3.0-linking-exception
 * Copyright (c) 2026 Partho Ghosh.
 *
 * Browser and desktop only; none of this goes on the FM-1. FM1 Open's
 * simulator (Simulator.c) has entry points that reach into FM1 Open's own
 * editor, projects and voice library. AJ Box's firmware has none of those, so
 * they are empty here to let the simulator link: the workbench's editor,
 * project and voice pages do nothing with AJ Box. */
#include "instrument.h"
#include "fm_patch.h"

void groove_screen_snapshot(uint8_t state[8]) { for (unsigned i = 0; i < 8; ++i) state[i] = 0; }
int groove_screen_open(unsigned target) { (void)target; return -1; }
int groove_screen_preset(unsigned slot) { (void)slot; return -1; }
unsigned groove_screen_preset_position(void) { return 0; }
void groove_screen_tap(uint32_t now) { (void)now; }
const char *groove_screen_web_knobs(void) { return ""; }
unsigned groove_screen_web_first(void) { return 0; }
int groove_screen_web_step(unsigned step, unsigned toggle) { (void)step; (void)toggle; return -1; }
const void *groove_screen_web_targets(void) { return ""; }
unsigned groove_screen_web_target_count(void) { return 0; }
unsigned groove_screen_web_context(void) { return 0; }
unsigned groove_screen_web_mask(void) { return 0; }
int groove_screen_web_adjust(unsigned context, unsigned action, unsigned index, unsigned owner, int delta)
{ (void)context; (void)action; (void)index; (void)owner; (void)delta; return -1; }
void groove_screen_web_fine(unsigned fine) { (void)fine; }
void groove_settings_midi(unsigned track, unsigned parameter, int delta) { (void)track; (void)parameter; (void)delta; }
uint32_t groove_project_revision(void) { return 0; }
int groove_project_import(const uint8_t *bytes, unsigned length) { (void)bytes; (void)length; return -1; }
int groove_project_export(uint8_t *bytes, unsigned capacity) { (void)bytes; (void)capacity; return -1; }
const char *groove_factory_name(unsigned index) { (void)index; return ""; }
int groove_factory_build(unsigned index, struct groove_project *project) { (void)index; (void)project; return -1; }
int groove_fm_library_read(unsigned bank, uint8_t vmem[4096]) { (void)bank; (void)vmem; return -1; }
int groove_fm_library_write(unsigned bank, const uint8_t vmem[4096]) { (void)bank; (void)vmem; return -1; }
