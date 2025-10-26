// Copyright 2023 Dan Cohen
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include QMK_KEYBOARD_H

// Voyager key position indexing (5 rows × 6 cols per side + 2 thumb keys per side = 40 total)
// Indexed left-to-right across both sides, then down
// Row 0: L: 0  1  2  3  4  5     R: 6  7  8  9 10 11
// Row 1: L: 12 13 14 15 16 17    R: 18 19 20 21 22 23
// Row 2: L: 24 25 26 27 28 29    R: 30 31 32 33 34 35
// Row 3: L: 36 37 38 39 40 41    R: 42 43 44 45 46 47
// Row 4 (thumbs): L: 48 49       R: 50 51

typedef struct {
    uint16_t keycode;           // What the combo produces
    uint16_t key_positions[8];  // Key positions involved (up to 8)
    uint8_t position_count;     // Number of positions
    uint16_t timeout_ms;        // Custom timeout for this combo
    uint32_t layer_mask;        // Bitmask of active layers (1 << layer)
} combo_config_t;

// Helper function to check if combo should be active on current layer
static inline bool is_combo_active_on_layer(uint32_t layer_mask, uint8_t current_layer) {
    return (layer_mask & (1 << current_layer)) != 0;
}

#endif // COMBOS_H
