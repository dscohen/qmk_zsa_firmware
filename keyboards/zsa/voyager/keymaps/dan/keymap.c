// Copyright 2023 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "version.h"
#define MOON_LED_LEVEL LED_LEVEL
#ifndef ZSA_SAFE_RANGE
#define ZSA_SAFE_RANGE SAFE_RANGE
#endif

// Combo typing detection - prevent combos during fast typing
#define QUICK_TAP_TERM 30  // Time window to consider keys as "typing"
#define IS_TYPING(k) ( \
    ((uint8_t)(k) <= KC_Z || (uint8_t)(k) == KC_SPC) && \
    (last_input_activity_elapsed() < QUICK_TAP_TERM)    )

enum {
    _ALPHA,   // default
    _NAV,     // special characters / navigation
    _NUMS,    // numbers/function/motion
    _HELPER,  // helper layer
    _MOUSE,   // mouse layer (not used)
    _TMUX     // tmux layer
};

enum custom_keycodes {
    // Navigator pointing device keycodes
    DRAG_SCROLL = ZSA_SAFE_RANGE,
    TOGGLE_SCROLL,
    NAVIGATOR_INC_CPI,
    NAVIGATOR_DEC_CPI,
    NAVIGATOR_TURBO,
    NAVIGATOR_AIM,
    // Tmux keycodes
    _T_NEW_,               // tmux new
    _T_PREV,               // tmux prev-window
    _T_NEXT,               // tmux next-window
    _T_LAST,               // tmux last-window
    _T_BEGI,               // tmux go to front of line
    _T_CMD_,               // tmux command
    _T_COPY,               // tmux copy-mode
    _T_PAST,               // tmux paste-buffer
    _T_SCRL,               // tmux scroll
    _T_MOVE,               // tmux move-window
    _T_RNAM,               // tmux rename-window
    _T_LEFT,               // tmux left panel
    _T_DOWN,               // tmux down panel
    _T_RGHT,               // tmux right panel
    _T_VSPT,               // tmux vert split panel
    _T_HSPT,               // tmux hori split panel
    _T_KPAN,               // tmux kill panel
    _T_UP__,               // tmux up panel
    _T_1___,               // tmux select-window -t 1
    _T_2___,               // tmux select-window -t 2
    _T_3___,               // tmux select-window -t 3
    _T_4___,               // tmux select-window -t 4
    _T_5___,               // tmux select-window -t 5
    _T_6___,               // tmux select-window -t 6
    _T_7___,               // tmux select-window -t 7
    _T_8___,               // tmux select-window -t 8
    _T_9___,               // tmux select-window -t 9
    _T_0___,               // tmux select-window -t 0
    KC_SEAL,
};

// Navigator pointing device variables
extern bool set_scrolling;
extern bool navigator_turbo;
extern bool navigator_aim;

void pointing_device_init_user(void) {
    set_auto_mouse_enable(true);
}

bool is_mouse_record_kb(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case NAVIGATOR_INC_CPI ... NAVIGATOR_AIM:
    case DRAG_SCROLL:
    case TOGGLE_SCROLL:
      return true;
  }
  return is_mouse_record_user(keycode, record);
}

enum combos {
    COMBO_TAB_FORWARD,
    COMBO_TAB_BACKWARD,
    COMBO_ESC,
    COMBO_SK_ALT,
    COMBO_ITERM,
    COMBO_SK_CTRL,
    COMBO_NORM_CTRL,
    COMBO_BACKSLASH,
    COMBO_BACKSPACE,
    COMBO_SCOOT,
    COMBO_SEAL,
    COMBO_REPEAT,
    COMBO_SK_LGUI,
    COMBO_SK_RGUI,
    COMBO_TAB,
    COMBO_COLON,
    COMBO_MEH_LAYER,
    COMBO_COPY,
    COMBO_ALFRED,
    COMBO_PASTE,
    COMBO_LBRC,
    COMBO_RBRC,
    COMBO_RPRN,
    COMBO_LBRN,
    COMBO_QSTN,
    COMBO_EXCLM,
    COMBO_RBRK,
    COMBO_EMAILAT,
    COMBO_CARROT,
    COMBO_LBRK,
    COMBO_UNDERSCORE,
    COMBO_EQUAL,
    COMBO_AST,
    COMBO_HASH,
    COMBO_FSLASH,
    COMBO_PLUS,
    COMBO_PERCENT,
    COMBO_SEMICOLON,
    COMBO_TILDA,
    COMBO_DASH,
    COMBO_PIPE,
    COMBO_DOLLAR,
    COMBO_MEH,
    COMBO_LMOUSE,
    COMBO_RMOUSE,
    COMBO_MMOUSE,
    COMBO_TMUX,
    COMBO_NUM_BS,
    COMBO_NUM_LMOUSE,
};

// Combos - adapted for Voyager layout
const uint16_t PROGMEM combo_tab_forward[] = {KC_S, KC_W, COMBO_END};
const uint16_t PROGMEM combo_tab_backward[] = {KC_T, KC_M, COMBO_END};
const uint16_t PROGMEM combo_esc[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_sk_alt[] = {KC_L, KC_D, COMBO_END};
const uint16_t PROGMEM combo_iterm[] = {LT(_NUMS, KC_P), KC_H, COMBO_END};
const uint16_t PROGMEM combo_sk_ctrl[] = {KC_X, KC_M, COMBO_END};
const uint16_t PROGMEM combo_norm_ctrl[] = {KC_F, KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_backslash[] = {KC_Z, KC_Y, COMBO_END};
const uint16_t PROGMEM combo_backspace[] = {KC_R, KC_T, COMBO_END};
const uint16_t PROGMEM combo_scoot[] = {KC_A, KC_E, COMBO_END};
const uint16_t PROGMEM combo_seal[] = {KC_B, KC_L, KC_D, KC_C, COMBO_END};
const uint16_t PROGMEM combo_repeat[] = {KC_M, KC_W, COMBO_END};
const uint16_t PROGMEM combo_sk_lgui[] = {KC_D, KC_T, COMBO_END};
const uint16_t PROGMEM combo_sk_rgui[] = {KC_O, KC_A, COMBO_END};
const uint16_t PROGMEM combo_tab[] = {KC_S, LT(_NAV, KC_G), COMBO_END};
const uint16_t PROGMEM combo_colon[] = {KC_A, KC_COMM, COMBO_END};
const uint16_t PROGMEM combo_meh_layer[] = {KC_C, KC_S, COMBO_END};
const uint16_t PROGMEM combo_copy[] = {KC_X, KC_W, COMBO_END};
const uint16_t PROGMEM combo_alfred[] = {KC_Y, KC_H, COMBO_END};
const uint16_t PROGMEM combo_paste[] = {KC_X, KC_M, KC_W, COMBO_END};
const uint16_t PROGMEM combo_lbrc[] = {KC_D, KC_C, COMBO_END};
const uint16_t PROGMEM combo_rbrc[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM combo_rprn[] = {KC_Y, KC_O, COMBO_END};
const uint16_t PROGMEM combo_lbrn[] = {KC_O, KC_U, COMBO_END};
const uint16_t PROGMEM combo_qstn[] = {KC_L, KC_R, COMBO_END};
const uint16_t PROGMEM combo_exclm[] = {KC_R, KC_X, COMBO_END};
const uint16_t PROGMEM combo_rbrk[] = {KC_T, KC_X, COMBO_END};
const uint16_t PROGMEM combo_emailat[] = {LT(_NUMS, KC_P), KC_Y, COMBO_END};
const uint16_t PROGMEM combo_carrot[] = {KC_D, KC_R, COMBO_END};
const uint16_t PROGMEM combo_lbrk[] = {KC_T, KC_W, COMBO_END};
const uint16_t PROGMEM combo_underscore[] = {KC_W, KC_J, COMBO_END};
const uint16_t PROGMEM combo_equal[] = {KC_F, KC_COMM, COMBO_END};
const uint16_t PROGMEM combo_ast[] = {KC_K, KC_H, COMBO_END};
const uint16_t PROGMEM combo_hash[] = {KC_U, KC_E, COMBO_END};
const uint16_t PROGMEM combo_fslash[] = {KC_H, KC_F, COMBO_END};
const uint16_t PROGMEM combo_plus[] = {KC_K, KC_F, COMBO_END};
const uint16_t PROGMEM combo_percent[] = {KC_E, KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_semicolon[] = {KC_V, LT(_NAV, KC_G), COMBO_END};
const uint16_t PROGMEM combo_tilda[] = {KC_D, KC_S, COMBO_END};
const uint16_t PROGMEM combo_dash[] = {KC_H, KC_A, KC_E, COMBO_END};
const uint16_t PROGMEM combo_pipe[] = {LT(_NAV, KC_G), KC_J, COMBO_END};
const uint16_t PROGMEM combo_dollar[] = {KC_C, LT(_NAV, KC_G), COMBO_END};
const uint16_t PROGMEM combo_meh[] = {KC_C, KC_S, COMBO_END};
const uint16_t PROGMEM combo_lmouse[] = {KC_R, KC_T, KC_S, COMBO_END};
const uint16_t PROGMEM combo_rmouse[] = {KC_T, KC_S, COMBO_END};
const uint16_t PROGMEM combo_mmouse[] = {KC_N, KC_R, KC_T, KC_S, COMBO_END};
const uint16_t PROGMEM combo_tmux[] = {KC_L, KC_D, KC_C, COMBO_END};
const uint16_t PROGMEM combo_num_bs[] = {KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_num_lmouse[] = {KC_2, KC_3, COMBO_END};

combo_t key_combos[] = {
    [COMBO_TAB_FORWARD] = COMBO(combo_tab_forward, LCTL(KC_TAB)),
    [COMBO_TAB_BACKWARD] = COMBO(combo_tab_backward, LCTL(LSFT(KC_TAB))),
    [COMBO_ESC] = COMBO(combo_esc, KC_ESC),
    [COMBO_SK_ALT] = COMBO(combo_sk_alt, OSM(MOD_LALT)),
    [COMBO_ITERM] = COMBO(combo_iterm, LCTL(KC_BSLS)),
    [COMBO_SK_CTRL] = COMBO(combo_sk_ctrl, OSM(MOD_LCTL)),
    [COMBO_NORM_CTRL] = COMBO(combo_norm_ctrl, MOD_LCTL),
    [COMBO_BACKSLASH] = COMBO(combo_backslash, KC_BSLS),
    [COMBO_BACKSPACE] = COMBO(combo_backspace, KC_BSPC),
    [COMBO_SCOOT] = COMBO(combo_scoot, LALT(KC_D)),
    [COMBO_SEAL] = COMBO(combo_seal, KC_S),
    [COMBO_REPEAT] = COMBO(combo_repeat, KC_ENT),
    [COMBO_SK_LGUI] = COMBO(combo_sk_lgui, OSM(MOD_LGUI)),
    [COMBO_SK_RGUI] = COMBO(combo_sk_rgui, OSM(MOD_RGUI)),
    [COMBO_TAB] = COMBO(combo_tab, KC_TAB),
    [COMBO_COLON] = COMBO(combo_colon, KC_COLN),
    [COMBO_MEH_LAYER] = COMBO(combo_meh_layer, OSM(MOD_MEH)),
    [COMBO_COPY] = COMBO(combo_copy, LGUI(KC_C)),
    [COMBO_ALFRED] = COMBO(combo_alfred, LALT(KC_SPACE)),
    [COMBO_PASTE] = COMBO(combo_paste, LGUI(KC_V)),
    [COMBO_LBRC] = COMBO(combo_lbrc, KC_LCBR),
    [COMBO_RBRC] = COMBO(combo_rbrc, KC_RCBR),
    [COMBO_RPRN] = COMBO(combo_rprn, KC_LPRN),
    [COMBO_LBRN] = COMBO(combo_lbrn, KC_RPRN),
    [COMBO_QSTN] = COMBO(combo_qstn, KC_QUES),
    [COMBO_EXCLM] = COMBO(combo_exclm, KC_EXLM),
    [COMBO_RBRK] = COMBO(combo_rbrk, KC_LBRC),
    [COMBO_EMAILAT] = COMBO(combo_emailat, KC_AT),
    [COMBO_CARROT] = COMBO(combo_carrot, KC_CIRC),
    [COMBO_LBRK] = COMBO(combo_lbrk, KC_RBRC),
    [COMBO_UNDERSCORE] = COMBO(combo_underscore, KC_UNDS),
    [COMBO_EQUAL] = COMBO(combo_equal, KC_EQL),
    [COMBO_AST] = COMBO(combo_ast, KC_ASTR),
    [COMBO_HASH] = COMBO(combo_hash, KC_HASH),
    [COMBO_FSLASH] = COMBO(combo_fslash, KC_SLSH),
    [COMBO_PLUS] = COMBO(combo_plus, KC_PLUS),
    [COMBO_PERCENT] = COMBO(combo_percent, KC_PERC),
    [COMBO_SEMICOLON] = COMBO(combo_semicolon, KC_SCLN),
    [COMBO_TILDA] = COMBO(combo_tilda, KC_TILD),
    [COMBO_DASH] = COMBO(combo_dash, KC_MINS),
    [COMBO_PIPE] = COMBO(combo_pipe, KC_PIPE),
    [COMBO_DOLLAR] = COMBO(combo_dollar, KC_DLR),
    [COMBO_MEH] = COMBO(combo_meh, OSM(MOD_MEH)),
    [COMBO_LMOUSE] = COMBO(combo_lmouse, KC_BTN3),
    [COMBO_RMOUSE] = COMBO(combo_rmouse, KC_BTN1),
    [COMBO_MMOUSE] = COMBO(combo_mmouse, KC_BTN2),
    [COMBO_TMUX] = COMBO(combo_tmux, OSL(_TMUX)),
    [COMBO_NUM_BS] = COMBO(combo_num_bs, KC_BSPC),
    [COMBO_NUM_LMOUSE] = COMBO(combo_num_lmouse, KC_BTN1),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ALPHA layer - main typing layer
     *
     * ╭──────────────────────────────────────╮ ╭──────────────────────────────────────╮
     * │esc  │  b  │  l  │  d  │  c  │  v   │ │  z  │  y  │  o  │  u  │ lctl │  '   │
     * ├──────────────────────────────────────┤ ├──────────────────────────────────────┤
     * │del  │  n  │  r  │  t  │  s  │g(nav)│ │p(num)│ h  │  a  │  e  │  i   │ enter│
     * ├──────────────────────────────────────┤ ├──────────────────────────────────────┤
     * │lalt │  q  │  x  │  m  │  w  │  j   │ │  k  │  f  │  ,  │  .  │ ralt │ lctl │
     * │ (f) │     │     │     │     │      │ │     │     │     │     │      │      │
     * ├──────────────────────────────────────┤ ├──────────────────────────────────────┤
     * │                  shift│ bspc│       │ │space│ helper                       │
     * ╰──────────────────────────────────────╯ ╰──────────────────────────────────────╯
     */
    [_ALPHA] = LAYOUT_voyager(
        KC_ESC,       KC_B,     KC_L,       KC_D,       KC_C,      KC_V,                      KC_Z,        KC_Y,     KC_O,       KC_U,     MT(MOD_LCTL, KC_P), KC_QUOT,
        KC_DEL,       KC_N,     KC_R,       KC_T,       KC_S,      LT(_NAV,KC_G),             LT(_NUMS,KC_P), KC_H, KC_A,       KC_E,     KC_I,   KC_ENT,
        LALT(KC_F),   KC_Q,     KC_X,       KC_M,       KC_W,      KC_J,                      KC_K,        KC_F,     KC_COMM,    KC_DOT,   MT(MOD_RALT,KC_QUOT), KC_LCTL,
        KC_TRNS,      KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,   KC_TRNS,                   KC_TRNS,     KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS,
                                            QK_REP,     OSM(MOD_LSFT),                       KC_SPC,       MO(_HELPER)
    ),

    /*
     * NAV layer - navigation and special chars
     */
    [_NAV] = LAYOUT_voyager(
        KC_NO,        QK_BOOT,  QK_RBT,     KC_NO,      KC_NO,     KC_NO,                     LGUI(KC_LEFT), LALT(KC_LEFT), LALT(KC_RGHT), LGUI(KC_RGHT), KC_PGUP, KC_NO,
        KC_NO,        KC_NO,    KC_NO,      KC_NO,      KC_LSFT,   KC_TRNS,                   KC_LEFT,       KC_DOWN,  KC_UP,      KC_RGHT,  KC_TRNS, KC_NO,
        KC_NO,        KC_NO,    KC_NO,      KC_NO,      KC_LALT,   KC_NO,                     KC_BSLS,       KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_PGDN, KC_NO,
        KC_TRNS,      KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,   KC_TRNS,                   KC_TRNS,       KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS,
                                            KC_TRNS,    KC_TRNS,                             KC_TRNS,       KC_1
    ),

    /*
     * NUMS layer - numbers and function keys
     */
    [_NUMS] = LAYOUT_voyager(
        KC_TRNS,      KC_5,     KC_6,       KC_7,       KC_8,      KC_9,                      KC_0,         KC_1,     KC_2,       KC_3,     KC_4,    KC_TRNS,
        KC_TRNS,      KC_0,     KC_1,       KC_2,       KC_3,      LT(_NAV,KC_4),             KC_TRNS,      KC_LGUI,  KC_LCTL,    KC_LSFT,  KC_9,    KC_TRNS,
        KC_TRNS,      KC_COMM,  KC_DOT,     KC_LBRC,    KC_RBRC,   KC_BTN1,                   KC_AMPR,      KC_LCTL,  KC_COMM,    KC_DOT,   KC_MINS, KC_TRNS,
        KC_TRNS,      KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,   KC_TRNS,                   KC_TRNS,      KC_TRNS,  KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS,
                                            KC_TRNS,    KC_TRNS,                             KC_TRNS,       KC_TRNS
    ),

    /*
     * HELPER layer - macros and system
     */
    [_HELPER] = LAYOUT_voyager(
        KC_TRNS,      LCTL(LSFT(KC_F14)), LCTL(LSFT(KC_F15)), LCTL(LSFT(KC_F16)), LCTL(LSFT(KC_F17)), LCTL(LSFT(KC_F18)),   KC_TRNS, KC_TRNS, KC_TRNS, QK_RBT, QK_BOOT, KC_TRNS,
        KC_TRNS,      LCTL(LSFT(KC_0)), LCTL(LSFT(KC_6)), LCTL(LSFT(KC_7)), LCTL(LSFT(KC_8)), LCTL(LSFT(KC_9)),          KC_LEFT, KC_TRNS, KC_TRNS, KC_RGHT, KC_AUDIO_MUTE, KC_TRNS,
        KC_TRNS,      LCTL(LSFT(KC_1)), LCTL(LSFT(KC_2)), LCTL(LSFT(KC_3)), LCTL(LSFT(KC_4)), LCTL(LSFT(KC_5)),          KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE, KC_MEDIA_NEXT_TRACK, KC_AUDIO_VOL_UP, KC_AUDIO_VOL_DOWN, KC_TRNS,
        KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS,    KC_TRNS,                             KC_TRNS,       KC_TRNS
    ),

    /*
     * TMUX layer - tmux commands
     */
    [_TMUX] = LAYOUT_voyager(
        KC_TRNS,      _T_5___, _T_6___, _T_7___, _T_8___, _T_9___,                   KC_SEAL, _T_1___, _T_2___, _T_3___, _T_SCRL, KC_TRNS,
        KC_TRNS,      _T_0___, _T_1___, _T_2___, _T_3___, _T_4___,                    _T_LEFT, _T_DOWN, _T_UP__, _T_RGHT, KC_TRNS, KC_TRNS,
        KC_TRNS,      _T_HSPT, KC_TRNS, KC_TRNS, KC_TRNS, _T_VSPT,                    _T_KPAN, _T_NEW_, _T_RNAM, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                            KC_TRNS,    KC_TRNS,                     KC_TRNS, KC_TRNS
    ),
};

#define TMUX_PREFIX SS_DOWN(X_LCTL) "a" SS_UP(X_LCTL)

/// Handle events when on the TMUX layer
static bool process_record_tmux(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;
    switch (keycode) {
        case KC_SEAL:
            SEND_STRING("The seal loves running to the store");
            return false;
        case _T_NEW_:
            SEND_STRING(TMUX_PREFIX "c");
            return false;
        case _T_PREV:
            SEND_STRING(TMUX_PREFIX "p");
            return false;
        case _T_LEFT:
            SEND_STRING(TMUX_PREFIX SS_TAP(X_LEFT));
            return false;
        case _T_DOWN:
            SEND_STRING(TMUX_PREFIX SS_TAP(X_DOWN));
            return false;
        case _T_UP__:
            SEND_STRING(TMUX_PREFIX SS_TAP(X_UP));
            return false;
        case _T_RGHT:
            SEND_STRING(TMUX_PREFIX SS_TAP(X_RIGHT));
            return false;
        case _T_NEXT:
            SEND_STRING(TMUX_PREFIX "n");
            return false;
        case _T_KPAN:
            SEND_STRING(TMUX_PREFIX "x");
            return false;
        case _T_VSPT:
            SEND_STRING(TMUX_PREFIX "|");
            return false;
        case _T_HSPT:
            SEND_STRING(TMUX_PREFIX "-");
            return false;
        case _T_LAST:
            SEND_STRING(TMUX_PREFIX "l");
            return false;
        case _T_CMD_:
            SEND_STRING(TMUX_PREFIX ":");
            return false;
        case _T_COPY:
            SEND_STRING(TMUX_PREFIX "[");
            return false;
        case _T_PAST:
            SEND_STRING(TMUX_PREFIX "]");
            return false;
        case _T_SCRL:
            SEND_STRING(TMUX_PREFIX SS_TAP(X_PGUP));
            return false;
        case _T_MOVE:
            SEND_STRING(TMUX_PREFIX ".");
            return false;
        case _T_RNAM:
            SEND_STRING(TMUX_PREFIX ",");
            return false;
        case _T_BEGI:
            SEND_STRING(TMUX_PREFIX TMUX_PREFIX);
            return false;
        case _T_1___:
            SEND_STRING(TMUX_PREFIX "1");
            return false;
        case _T_2___:
            SEND_STRING(TMUX_PREFIX "2");
            return false;
        case _T_3___:
            SEND_STRING(TMUX_PREFIX "3");
            return false;
        case _T_4___:
            SEND_STRING(TMUX_PREFIX "4");
            return false;
        case _T_5___:
            SEND_STRING(TMUX_PREFIX "5");
            return false;
        case _T_6___:
            SEND_STRING(TMUX_PREFIX "6");
            return false;
        case _T_7___:
            SEND_STRING(TMUX_PREFIX "7");
            return false;
        case _T_8___:
            SEND_STRING(TMUX_PREFIX "8");
            return false;
        case _T_9___:
            SEND_STRING(TMUX_PREFIX "9");
            return false;
        case _T_0___:
            SEND_STRING(TMUX_PREFIX "0");
            return false;
    }
    return true;
}

// Require prior idle time before combos can fire during typing
// This prevents accidental combos while typing quickly
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t last_combo_keycode) {
    // Allow combo if user hasn't been actively typing (idle period)
    // IS_TYPING macro checks if last input was within QUICK_TAP_TERM (100ms)
    if (IS_TYPING(last_combo_keycode)) {
        // Actively typing - don't trigger combo
        return false;
    }
    // Not actively typing - allow combo to trigger
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (get_highest_layer(layer_state) == _TMUX) {
        return process_record_tmux(keycode, record);
    }

    // Navigator pointing device controls
    switch (keycode) {
        case DRAG_SCROLL:
            if (record->event.pressed) {
                set_scrolling = true;
            } else {
                set_scrolling = false;
            }
            return false;
        case TOGGLE_SCROLL:
            if (record->event.pressed) {
                set_scrolling = !set_scrolling;
            }
            return false;
        case NAVIGATOR_TURBO:
            if (record->event.pressed) {
                navigator_turbo = true;
            } else {
                navigator_turbo = false;
            }
            return false;
        case NAVIGATOR_AIM:
            if (record->event.pressed) {
                navigator_aim = true;
            } else {
                navigator_aim = false;
            }
            return false;
        case NAVIGATOR_INC_CPI:
            if (record->event.pressed) {
                pointing_device_set_cpi(1);
            }
            return false;
        case NAVIGATOR_DEC_CPI:
            if (record->event.pressed) {
                pointing_device_set_cpi(0);
            }
            return false;
    }

    return true;
}
