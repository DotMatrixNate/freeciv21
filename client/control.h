/********************************************************************** 
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
#ifndef FC__CONTROL_H
#define FC__CONTROL_H

#include "packets.h"

enum cursor_hover_state {
  HOVER_NONE = 0,
  HOVER_GOTO,
  HOVER_NUKE,
  HOVER_PARADROP,
  HOVER_CONNECT,
  HOVER_PATROL
};

extern int hover_unit; /* unit hover_state applies to */
extern enum cursor_hover_state hover_state;
extern bool draw_goto_line;

void do_move_unit(struct unit *punit, struct packet_unit_info *pinfo);
void do_unit_goto(int x, int y);
void do_unit_nuke(struct unit *punit);
void do_unit_paradrop_to(struct unit *punit, int x, int y);
void do_unit_patrol_to(struct unit *punit, int x, int y);
void do_map_click(int xtile, int ytile);

void set_hover_state(struct unit *punit, enum cursor_hover_state state);
void handle_advance_focus(struct packet_generic_integer *packet);
void request_center_focus_unit(void);
void request_move_unit_direction(struct unit *punit, int dir);
void request_new_unit_activity(struct unit *punit, enum unit_activity act);
void request_new_unit_activity_targeted(struct unit *punit,
					enum unit_activity act,
					enum tile_special_type tgt);
void request_unit_auto(struct unit *punit);
void request_unit_build_city(struct unit *punit);
void request_unit_caravan_action(struct unit *punit, enum packet_type action);
void request_unit_change_homecity(struct unit *punit);
void request_unit_connect(void);
void request_unit_disband(struct unit *punit);
void request_unit_fortify(struct unit *punit);
void request_unit_goto(void);
void request_unit_move_done(void);
void request_unit_nuke(struct unit *punit);
void request_unit_paradrop(struct unit *punit);
void request_unit_patrol(void);
void request_unit_pillage(struct unit *punit);
void request_unit_selected(struct unit *punit);
void request_unit_sentry(struct unit *punit);
void request_unit_unload(struct unit *punit);
void request_unit_airlift(struct unit *punit, struct city *pcity);
void request_unit_upgrade(struct unit *punit);
void request_unit_wait(struct unit *punit);
void request_unit_wakeup(struct unit *punit);
void request_toggle_map_grid(void);
void request_toggle_city_names(void);
void request_toggle_city_productions(void);
void request_toggle_terrain(void);
void request_toggle_coastline(void);
void request_toggle_roads_rails(void);
void request_toggle_irrigation(void);
void request_toggle_mines(void);
void request_toggle_fortress_airbase(void);
void request_toggle_specials(void);
void request_toggle_pollution(void);
void request_toggle_cities(void);
void request_toggle_units(void);
void request_toggle_focus_unit(void);
void request_toggle_fog_of_war(void);

void wakeup_sentried_units(int x, int y);

void auto_center_on_focus_unit(void);
void advance_unit_focus(void);
struct unit *get_unit_in_focus(void);
void set_unit_focus(struct unit *punit);
void set_unit_focus_no_center(struct unit *punit);
void set_unit_focus_and_select(struct unit *punit);
void update_unit_focus(void);
struct unit *find_visible_unit(struct tile *ptile);
void set_units_in_combat(struct unit *pattacker, struct unit *pdefender);
void blink_active_unit(void);
void update_unit_pix_label(struct unit *punit);

void process_caravan_arrival(struct unit *punit);
void process_diplomat_arrival(struct unit *pdiplomat, int victim_id);

void key_cancel_action(void);
void key_city_names_toggle(void);
void key_city_productions_toggle(void);
void key_terrain_toggle(void);
void key_coastline_toggle(void);
void key_roads_rails_toggle(void);
void key_irrigation_toggle(void);
void key_mines_toggle(void);
void key_fortress_airbase_toggle(void);
void key_specials_toggle(void);
void key_pollution_toggle(void);
void key_cities_toggle(void);
void key_units_toggle(void);
void key_focus_unit_toggle(void);
void key_fog_of_war_toggle(void);
void key_end_turn(void);
void key_map_grid_toggle(void);
void key_move_north(void);
void key_move_north_east(void);
void key_move_east(void);
void key_move_south_east(void);
void key_move_south(void);
void key_move_south_west(void);
void key_move_west(void);
void key_move_north_west(void);
void key_unit_airbase(void);
void key_unit_auto_attack(void);
void key_unit_auto_explore(void);
void key_unit_auto_settle(void);
void key_unit_build_city(void);
void key_unit_build_wonder(void);
void key_unit_connect(void);
void key_unit_diplomat_actions(void);
void key_unit_disband(void);
void key_unit_done(void);
void key_unit_fallout(void);
void key_unit_fortify(void);
void key_unit_fortress(void);
void key_unit_goto(void);
void key_unit_homecity(void);
void key_unit_irrigate(void);
void key_unit_mine(void);
void key_unit_nuke(void);
void key_unit_patrol(void);
void key_unit_paradrop(void);
void key_unit_pillage(void);
void key_unit_pollution(void);
void key_unit_road(void);
void key_unit_sentry(void);
void key_unit_traderoute(void);
void key_unit_transform(void);
void key_unit_unload(void);
void key_unit_wait(void);
void key_unit_wakeup_others(void);

/* don't change this unless you also put more entries in data/Freeciv */
#define MAX_NUM_UNITS_BELOW 4

extern int num_units_below;


#endif  /* FC__CONTROL_H */
