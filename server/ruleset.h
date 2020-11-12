/***********************************************************************
 Freeciv - Copyright (C) 1996 - A Kjeldberg, L Gregersen, P Unold
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
***********************************************************************/
#ifndef FC__RULESET_H
#define FC__RULESET_H

#include <QLoggingCategory>

#define RULESET_CAPABILITIES "+Freeciv-ruleset-Devel-2017.Jan.02"
/*
 * Ruleset capabilities acceptable to this program:
 *
 * +Freeciv-3.1-ruleset
 *    - basic ruleset format for Freeciv versions 3.1.x; required
 *
 * +Freeciv-ruleset-Devel-YYYY.MMM.DD
 *    - ruleset of the development version at the given data
 */

Q_DECLARE_LOGGING_CATEGORY(ruleset_category)

struct conn_list;

typedef void (*rs_conversion_logger)(const char *msg);

/* functions */
bool load_rulesets(const char *restore, const char *alt, bool compat_mode,
                   rs_conversion_logger logger, bool act, bool buffer_script,
                   bool load_luadata);
bool reload_rulesets_settings(void);
void send_rulesets(struct conn_list *dest);

void rulesets_deinit(void);

char *get_script_buffer(void);
char *get_parser_buffer(void);

/* Default ruleset values that are not settings (in game.h) */

#define GAME_DEFAULT_ADDTOSIZE 9
#define GAME_DEFAULT_CHANGABLE_TAX TRUE
#define GAME_DEFAULT_VISION_REVEAL_TILES FALSE
#define GAME_DEFAULT_NATIONALITY FALSE
#define GAME_DEFAULT_CONVERT_SPEED 50
#define GAME_DEFAULT_DISASTER_FREQ 10
#define GAME_DEFAULT_ACH_UNIQUE TRUE
#define GAME_DEFAULT_ACH_VALUE 1
#define RS_DEFAULT_MUUK_FOOD_WIPE TRUE
#define RS_DEFAULT_MUUK_GOLD_WIPE TRUE
#define RS_DEFAULT_MUUK_SHIELD_WIPE FALSE
#define RS_DEFAULT_TECH_STEAL_HOLES TRUE
#define RS_DEFAULT_TECH_TRADE_HOLES TRUE
#define RS_DEFAULT_TECH_TRADE_LOSS_HOLES TRUE
#define RS_DEFAULT_TECH_PARASITE_HOLES TRUE
#define RS_DEFAULT_TECH_LOSS_HOLES TRUE
#define RS_DEFAULT_PYTHAGOREAN_DIAGONAL FALSE

#define RS_DEFAULT_GOLD_UPKEEP_STYLE "City"
#define RS_DEFAULT_TECH_COST_STYLE "Civ I|II"
#define RS_DEFAULT_TECH_LEAKAGE "None"
#define RS_DEFAULT_TECH_UPKEEP_STYLE "None"

#define RS_DEFAULT_CULTURE_VIC_POINTS 1000
#define RS_DEFAULT_CULTURE_VIC_LEAD 300
#define RS_DEFAULT_CULTURE_MIGRATION_PML 50
#define RS_DEFAULT_HISTORY_INTEREST_PML 0

#define RS_DEFAULT_EXTRA_APPEARANCE 15
#define RS_DEFAULT_EXTRA_DISAPPEARANCE 15

#endif /* FC__RULESET_H */
