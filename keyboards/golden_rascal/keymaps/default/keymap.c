// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdint.h>
#include <strings.h>
#include "action.h"
#include "keycodes.h"
#include "process_tap_dance.h"
#include "quantum_keycodes.h"
#include "keymap_uk.h"
#include QMK_KEYBOARD_H

bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.

typedef struct {
  uint16_t tap;
  uint16_t hold;
  uint16_t held;
} td_tap_hold_t;

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
  _GAMING_NUMS,
  _PROG
};

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
  td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;

  if (state->pressed) {
    if (state->count == 1
#ifndef PERMISSIVE_HOLD
      && !state->interrupted
#endif
    ) {
      register_code16(tap_hold->hold);
      tap_hold->held = tap_hold->hold;
    } else {
      register_code16(tap_hold->tap);
      tap_hold->held = tap_hold->tap;
  }
  }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
  td_tap_hold_t *tap_hold = (td_tap_hold_t *)user_data;

  if (tap_hold->held) {
    unregister_code16(tap_hold->held);
    tap_hold->held = 0;
  }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((td_tap_hold_t){tap, hold, 0}), }

enum {
  TD_K_LBRC,
  TD_L_RBRC,
  TD_123,
  TD_456,
  TD_789,
  MINS_UNDS,
  EQL_PLS,
  LBRC_LCBR,
  RBRC_RCBR,
  BSLS_PIPE,
  NUHS_TILD,
  QUOT_AT
};

/* void numDance123(tap_dance_state_t *state, void *user_data) {
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

void numDance456(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case 1:
      tap_code(KC_4);
      break;
    case 2:
      tap_code(KC_5);
      break;
    case 3:
      tap_code(KC_6);
      break;
  }
}

void numDance789(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case 1:
      tap_code(KC_7);
      break;
    case 2:
      tap_code(KC_8);
      break;
    case 3:
      tap_code(KC_9);
      break;
  }
} */

tap_dance_action_t tap_dance_actions[] = {
  [TD_K_LBRC] = ACTION_TAP_DANCE_DOUBLE(KC_K, KC_LBRC),
  [TD_L_RBRC] = ACTION_TAP_DANCE_DOUBLE(KC_L, KC_RBRC),
  /*[TD _123] = ACTION_TAP_DANCE_FN(numDance123),
  [TD_456] = ACTION_TAP_DANCE_FN(numDance456),
  [TD_789] = ACTION_TAP_DANCE_FN(numDance789), */
  [MINS_UNDS] = ACTION_TAP_DANCE_DOUBLE(KC_MINS, KC_UNDS),
  [EQL_PLS] = ACTION_TAP_DANCE_DOUBLE(KC_EQL, KC_PLUS),
  [LBRC_LCBR] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_LCBR),
  [RBRC_RCBR] = ACTION_TAP_DANCE_DOUBLE(KC_RBRC, KC_RCBR),
  [BSLS_PIPE] = ACTION_TAP_DANCE_DOUBLE(UK_BSLS, UK_PIPE),
  [NUHS_TILD] = ACTION_TAP_DANCE_DOUBLE(UK_HASH, UK_TILD),
  [QUOT_AT] = ACTION_TAP_DANCE_DOUBLE(UK_QUOT, UK_AT)
  /* [MINS_UNDS] = ACTION_TAP_DANCE_TAP_HOLD(KC_MINS, KC_UNDS),
  [EQL_PLS] = ACTION_TAP_DANCE_TAP_HOLD(KC_EQL, KC_PLUS),
  [LBRC_LCBR] = ACTION_TAP_DANCE_TAP_HOLD(KC_LBRC, KC_LCBR),
  [RBRC_RCBR] = ACTION_TAP_DANCE_TAP_HOLD(KC_RBRC, KC_RCBR),
  [BSLS_PIPE] = ACTION_TAP_DANCE_TAP_HOLD(UK_BSLS, UK_PIPE),
  [NUHS_TILD] = ACTION_TAP_DANCE_TAP_HOLD(UK_HASH, UK_TILD),
  [QUOT_AT] = ACTION_TAP_DANCE_TAP_HOLD(UK_QUOT, UK_AT) */
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
        KC_Q,               KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,               KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        LT(_PROG, KC_Z),    KC_X,    KC_C,    KC_V,    KC_B,                               KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                            KC_LSFT, ALT_T(KC_SPC), KC_LCTL,           KC_BSPC,  KC_ENT,  MO(_PROG)
    ),

    [_PROG] = LAYOUT_split_3x5_3(
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                               TD(MINS_UNDS),    TD(EQL_PLS),    KC_UP,      TD(LBRC_LCBR),    TD(RBRC_RCBR),
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_DEL,                                TD(BSLS_PIPE),    KC_LEFT,        KC_DOWN,    KC_RIGHT,         TD(NUHS_TILD),
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                               KC_HOME,    KC_END,    KC_KP_2,    KC_KP_3,    ,
                                                KC_TRNS, KC_TRNS, KC_TRNS,           TD(QUOT_AT),  KC_MPLY,  KC_TRNS
    ),

    [_GAMING] = LAYOUT_split_3x5_3(
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,                               KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,
        KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,                               KC_G,    KC_H,    KC_J,    KC_K,    KC_L,
        KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,                               KC_B,    KC_N,    KC_M,    KC_DOT,  KC_SLSH,
                            MO(_GAMING_NUMS), KC_SPC, KC_TRNS,           KC_TRNS,  KC_TRNS,  KC_TRNS
    ),

    [_GAMING_NUMS] = LAYOUT_split_3x5_3(
        KC_1,        KC_2,       KC_TRNS,    KC_3,       KC_4,                               KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_5,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_6,                               KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRANS,    KC_7,       KC_8,       KC_9,       KC_0,                            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
                                                  KC_TRNS, KC_TRNS, KC_TRNS,           KC_TRNS,  KC_TRNS,  KC_TRNS
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
  //[QW_ALTTAB] = COMBO(qw_combo, ALT_TAB),
  [ZX_LGUI] = COMBO(zx_combo, KC_LGUI),
  [VB_LAYER] = COMBO(vb_combo, TG(_GAMING)),
  //[SG_LAYER] = COMBO(sg_combo, MO(1)),
};

/* void tap_hold(tap_dance_action_t *action, uint16_t keycode, keyrecord_t *record) {
  action = &td_actions_hold[QK_TAP_DANCE_GET_INDEX(keycode)];
  if (!record->event.pressed && action->state.count && !action->state.finished) {
        td_tap_hold_t *tap_hold = (td_tap_hold_t *)action->user_data;
        tap_code16(tap_hold->tap);
  }
} */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  tap_dance_action_t *action;

  /* switch (keycode) {
    case TD()
  } */

  switch (keycode) { // This will do most of the grunt work with the keycodes.
    case TD(MINS_UNDS):
    case TD(EQL_PLS):
    case TD(LBRC_LCBR):
    case TD(RBRC_RCBR):
    case TD(BSLS_PIPE):
    case TD(NUHS_TILD):
    case TD(QUOT_AT):
      action = &tap_dance_actions[QK_TAP_DANCE_GET_INDEX(keycode)];
      if (!record->event.pressed && action->state.count && !action->state.finished) {
          td_tap_hold_t *tap_hold = (td_tap_hold_t *)action->user_data;
          tap_code16(tap_hold->tap);
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