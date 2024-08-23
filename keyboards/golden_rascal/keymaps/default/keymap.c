// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "action.h"
#include "keycodes.h"
#include QMK_KEYBOARD_H

bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.

enum custom_keycodes {
  ALT_TAB = SAFE_RANGE,
};

enum combos {
  AB_ESC,
  AS_TAB,
  QW_ALTTAB,
  ZX_LGUI,
  VB_LAYER,
  SG_LAYER
};

enum layer_names {
  _BASE,
  _GAMING,
  _PROG
};

/* typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    K_LBRC,
    SOME_OTHER_DANCE
};

td_state_t cur_dance(tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void k_finished(tap_dance_state_t *state, void *user_data);
void k_reset(tap_dance_state_t *state, void *user_data); */

enum {
  TD_K_LBRC,
  TD_L_RBRC,
  TD_123,
  TD_456,
  TD_789
};

void numDance(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case 1:
      tap_code(KC_1);
      break;
    case 2:
      tap_code(KC_2);
      break;
    case 3:
      tap_code(KC_3);
      break;
  }
}

tap_dance_action_t tap_dance_actions[] = {
  [TD_K_LBRC] = ACTION_TAP_DANCE_DOUBLE(KC_K, KC_LBRC),
  [TD_L_RBRC] = ACTION_TAP_DANCE_DOUBLE(KC_L, KC_RBRC),
  [TD_123] = ACTION_TAP_DANCE_FN(numDance),
  [TD_456] = ACTION_TAP_DANCE_FN(numDance),
  [TD_789] = ACTION_TAP_DANCE_FN(numDance)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
      * ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
      * │ Q │ W │ E │ R │ T │       │ Y │ U │ I │ O │ P │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ A │ S │ D │ F │ G │       │ H │ J │ K │ L │ ; │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ Z │ X │ C │ V │ B │       │ N │ M │ , │ . │ / │
      * └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
      *           ┌───┐                   ┌───┐
      *           │SFT├───┐           ┌───┤LYR│
      *           └───┤SPC├───┐   ┌───┤BSP├───┘
      *               └───┤CTL│   │ENT├───┘
      *                   └───┘   └───┘
      */
    [_BASE] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                               KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                            KC_LSFT, ALT_T(KC_SPC), KC_LCTL,           KC_BSPC,  KC_ENT,  TG(_GAMING)
    ),

    [_GAMING] = LAYOUT_split_3x5_3(
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,                               KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,
        KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,                               KC_G,    KC_H,    KC_J,    KC_K,    KC_L,
        KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,                               KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,
                                TD(TD_123), KC_TRNS, KC_SPC,           KC_TRNS,  KC_TRNS,  KC_TRNS
    ),

    [_PROG] = LAYOUT_split_3x5_3(
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,                               KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,
        KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,                               KC_G,    KC_H,    KC_J,    KC_K,    KC_L,
        KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,                               KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,
                                    KC_LSFT, KC_LCTL, KC_SPC,          KC_ENT,  KC_BSPC,  KC_RALT
    ),
};

const uint16_t PROGMEM ab_combo[] = {KC_A, KC_B, COMBO_END};
const uint16_t PROGMEM as_combo[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM qw_combo[] = {KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM zx_combo[] = {KC_Z, KC_X, COMBO_END};
const uint16_t PROGMEM vb_combo[] = {KC_V, KC_B, COMBO_END};
const uint16_t PROGMEM sg_combo[] = {KC_S, KC_G, COMBO_END};

combo_t key_combos[] = {
  [AB_ESC] = COMBO(ab_combo, KC_ESC),
  [AS_TAB] = COMBO(as_combo, KC_TAB),
  [QW_ALTTAB] = COMBO(qw_combo, ALT_TAB),
  [ZX_LGUI] = COMBO(zx_combo, KC_LGUI),
  [VB_LAYER] = COMBO(vb_combo, OSL(1)),
  [SG_LAYER] = COMBO(sg_combo, MO(1)),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) { // This will do most of the grunt work with the keycodes.
        
    case ALT_TAB:
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
  }
  return true;
}

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}