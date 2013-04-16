// Vibrator Pebble App
// Copyright (C) 2013  Fazli Sapuan
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// You may contact the author at fazli@sapuan.org
//
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xC4, 0xFD, 0x11, 0x74, 0x59, 0x7A, 0x43, 0x9D, 0xA5, 0xC4, 0x78, 0x6D, 0x02, 0x57, 0x32, 0x64 }
PBL_APP_INFO(MY_UUID,
			 "Vibe", "Fuzzie",
			 1, 0, /* App version */
			 RESOURCE_ID_IMAGE_MENU_ICON,
			 APP_INFO_STANDARD_APP);

static AppContextRef app_ctx;

Window window;

AppTimerHandle timer_handle;

Layer controls_layer;
Layer bars_layer;

BmpContainer controls;
BmpContainer bar_on;
BmpContainer bar_off;

int power;
bool on;

#define COOKIE 1

void controls_update_callback(Layer *me, GContext* ctx) {
	(void)me;
	(void)ctx;
	
	GRect destination = layer_get_frame(&controls.layer.layer);
	
	destination.origin.x = 0;
	destination.origin.y = 0;
	
	graphics_draw_bitmap_in_rect(ctx, &controls.bmp, destination);
}

void bars_update_callback(Layer *me, GContext* ctx) {
	(void)me;
	(void)ctx;
	
	for (int i=0; i<=5; i++) {
		GRect destination = layer_get_frame(&bar_off.layer.layer);
	
		destination.origin.x = 0;
		destination.origin.y = 115 - i*23;
	
		if (i <= power && on) {
			graphics_draw_bitmap_in_rect(ctx, &bar_on.bmp, destination);
		} else {
			graphics_draw_bitmap_in_rect(ctx, &bar_off.bmp, destination);
		}
	}
}

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;
	
	if (power >= 5) return;
	
	power++;
	layer_mark_dirty(&bars_layer);
}

void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;  
	
	if (power <= 1) return;
	
	power--;
	layer_mark_dirty(&bars_layer);
}

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
	(void)recognizer;
	(void)window;
	
	on = !on;
	
	if (on) {
		timer_handle = app_timer_send_event(app_ctx, 100 /* milliseconds */, COOKIE);
	} else {
		app_timer_cancel_event(app_ctx, timer_handle);
	}
	
	layer_mark_dirty(&bars_layer);
}

void click_config_provider(ClickConfig **config, Window *window) {
	(void)window;

	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

	config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
	config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;

	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
	config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
}

void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {
    (void)ctx;
    (void)handle;

    if (cookie == COOKIE) {
		vibes_short_pulse();
		if (on && power > 0) {
        	timer_handle = app_timer_send_event(ctx, 200 * (6 - power) /* milliseconds */, COOKIE);
		}
    }
}

void handle_init(AppContextRef ctx) {
	(void)ctx;
	
	app_ctx = ctx;

	window_init(&window, "Vibe");
	window_stack_push(&window, true /* Animated */);
    
	power = 3;
	on = false;
	
	resource_init_current_app(&APP_RESOURCES);
	
	layer_init(&controls_layer, GRect(124, 3, 20, 146));
	controls_layer.update_proc = &controls_update_callback;
	layer_add_child(&window.layer, &controls_layer);
	
	layer_init(&bars_layer, GRect(22, 22, 85, 115));
	bars_layer.update_proc = &bars_update_callback;
	layer_add_child(&window.layer, &bars_layer);
	
	bmp_init_container(RESOURCE_ID_IMAGE_CONTROLS, &controls);
	bmp_init_container(RESOURCE_ID_IMAGE_BAR_ON,   &bar_on);
	bmp_init_container(RESOURCE_ID_IMAGE_BAR_OFF,  &bar_off);
	
	window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
}

void handle_deinit(AppContextRef ctx) {
	(void)ctx;
	
	bmp_deinit_container(&controls);
	bmp_deinit_container(&bar_on);
	bmp_deinit_container(&bar_off);
}

void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init,
		.deinit_handler = &handle_deinit,
		.timer_handler = &handle_timer
	};
	app_event_loop(params, &handlers);
}
