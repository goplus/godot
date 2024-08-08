/**************************************************************************/
/*  gdextension_spx_ext.cpp                                               */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "gdextension_interface.h"
#include "gdextension_spx_ext.h"
#include "gdextension_spx_engine.h"

#include "core/config/engine.h"
#include "core/extension/gdextension.h"
#include "core/extension/gdextension_compat_hashes.h"
#include "core/io/file_access.h"
#include "core/io/xml_parser.h"
#include "core/object/class_db.h"
#include "core/object/script_language_extension.h"
#include "core/object/worker_thread_pool.h"
#include "core/os/memory.h"
#include "core/variant/variant.h"
#include "core/version.h"
#include "scene/main/window.h"


static void gdextension_spx_global_register_callbacks(GDExtensionSpxCallbackInfoPtr callback_ptr) {
	SpxEngine::spx_callback_info = *(SpxCallbackInfo*)callback_ptr;
	print_line("register callbacks");
}

// input
static GdVec2 gdextension_spx_input_get_mouse_pos() { return GdVec2(); }
static GdBool gdextension_spx_input_get_mouse_state(GdInt id) { return false; }
static GdInt gdextension_spx_input_get_key_state(GdInt key) { return 0; }
static GdFloat gdextension_spx_input_get_axis(GdString axis) { return 0; }
static GdBool gdextension_spx_input_is_action_pressed(GdString action) { return false; }
static GdBool gdextension_spx_input_is_action_just_pressed(GdString action) { return false; }
static GdBool gdextension_spx_input_is_action_just_released(GdString action) { return false; }

// audio manager
static void gdextension_spx_audio_play_audio(GdString path) { print_line(vformat("Playing %s", path)); }
static void gdextension_spx_audio_set_audio_volume(GdFloat volume) { print_line(vformat("Setting volume to %f", volume)); }

static GdFloat gdextension_spx_audio_get_audio_volume() {
	print_line("GetAudioVolume");
	return 0;
}

static GdBool gdextension_spx_audio_is_music_playing() { return false; }
static void gdextension_spx_audio_play_music(GdString path) { print_line(vformat("Playing music %s", path)); }
static void gdextension_spx_audio_set_music_volume(GdFloat volume) { print_line(vformat("Setting music volume to %f", volume)); }

static GdFloat gdextension_spx_audio_get_music_volume() {
	print_line("GetMusicVolume");
	return 0;
}

static void gdextension_spx_audio_pause_music() { print_line("PauseMusic"); }
static void gdextension_spx_audio_resume_music() { print_line("ResumeMusic"); }
static GdFloat gdextension_spx_audio_get_music_timer() { return 0; }
static void gdextension_spx_audio_set_music_timer(GdFloat time) { print_line(vformat("Setting music timer to %f", time)); }

// physic
static void gdextension_spx_physic_set_gravity(GdFloat gravity) { print_line(vformat("Setting gravity to %f", gravity)); }
static GdFloat gdextension_spx_physic_get_gravity() { return 0; }

static void gdextension_spx_physic_set_velocity(GdInt id, GdVec2 velocity) { print_line(vformat("Setting velocity of %d to %f, %f", id, velocity.x, velocity.y)); }
static GdVec2 gdextension_spx_physic_get_velocity(GdInt id) { return GdVec2(); }

static void gdextension_spx_physic_set_mass(GdInt id, GdFloat mass) { print_line(vformat("Setting mass of %d to %f", id, mass)); }
static GdFloat gdextension_spx_physic_get_mass(GdInt id) { return 0; }

static void gdextension_spx_physic_add_force(GdInt id, GdVec2 force) { print_line(vformat("Adding force to %d %f, %f", id, force.x, force.y)); }
static void gdextension_spx_physic_add_impulse(GdInt id, GdVec2 impulse) { print_line(vformat("Adding impulse to %d %f, %f", id, impulse.x, impulse.y)); }
static void gdextension_spx_physic_set_collision_layer(GdInt id, GdInt layer) { print_line(""); }

static GdInt gdextension_spx_physic_get_collision_layer(GdInt id) { return 0; }
static void gdextension_spx_physic_set_collision_mask(GdInt id, GdInt mask) { print_line(vformat("Setting collision mask of %d to %d", id, mask)); }
static GdInt gdextension_spx_physic_get_collision_mask(GdInt id) { return 0; }

static GdInt gdextension_spx_physic_get_collider_type(GdInt id) { return 0; }
static void gdextension_spx_physic_add_collider_rect(GdInt id, GdVec2 center, GdVec2 size) { print_line(""); }
static void gdextension_spx_physic_add_collider_circle(GdInt id, GdVec2 center, GdFloat radius) { print_line(""); }
static void gdextension_spx_physic_add_collider_capsule(GdInt id, GdVec2 center, GdVec2 size) { print_line(""); }

static void gdextension_spx_physic_set_trigger(GdInt id, GdBool trigger) { print_line(vformat("Setting trigger of %d to %d", id, trigger)); }
static GdBool gdextension_spx_physic_is_trigger(GdInt id) { return false; }

static void gdextension_spx_physic_set_collision_enabled(GdInt id, GdBool enabled) { print_line(""); }
static GdBool gdextension_spx_physic_is_collision_enabled(GdInt id) { return false; }


// sprite
static GdInt gdextension_spx_sprite_instantiate_sprite(GdString path) { return 0; }
static GdInt gdextension_spx_sprite_clone_sprite(GdInt id) { return 0; }
static GdBool gdextension_spx_sprite_destroy_sprite(GdInt id) { return false; }
static GdBool gdextension_spx_sprite_is_sprite_alive(GdInt id) { return false; }

static void gdextension_spx_sprite_set_position(GdInt id, GdVec2 pos) { print_line(vformat("Updating position of %d to %f, %f", id, pos.x, pos.y)); }
static void gdextension_spx_sprite_set_rotation(GdInt id, GdVec2 rot) { print_line(vformat("Updating rotation of %d to %f, %f", id, rot.x, rot.y)); }
static void gdextension_spx_sprite_set_scale(GdInt id, GdVec2 scale) { print_line(vformat("Updating scale of %d to %f, %f", id, scale.x, scale.y)); }

static GdVec2 gdextension_spx_sprite_get_position(GdInt id) { return GdVec2(); }
static GdVec2 gdextension_spx_sprite_get_rotation(GdInt id) { return GdVec2(); }
static GdVec2 gdextension_spx_sprite_get_scale(GdInt id) { return GdVec2(); }

static void gdextension_spx_sprite_set_color(GdInt id, GdColor GdColor) { print_line(vformat("Updating GdColor of %d to %f, %f, %f, %f", id, GdColor.r, GdColor.g, GdColor.b, GdColor.a)); }
static GdColor gdextension_spx_sprite_get_color(GdInt id) { return GdColor(); }

static void gdextension_spx_sprite_update_texture(GdInt id, GdString path) { print_line(vformat("Updating texture of %d to %s", id, path)); }
static GdString gdextension_spx_sprite_get_texture(GdInt id) { return nullptr; }

static void gdextension_spx_sprite_set_visible(GdInt id, GdBool visible) { print_line(vformat("Updating visibility of %d to %s", id, visible ? "true" : "false")); }
static GdBool gdextension_spx_sprite_get_visible(GdInt id) { return false; }
static void gdextension_spx_sprite_update_z_index(GdInt id, GdInt z) { print_line(vformat("Updating z index of %d to %d", id, z)); }
// TODO add child

// ui
static GdInt gdextension_spx_ui_create_button(GdString path, GdRect rect, GdString text) { return 0; }
static GdInt gdextension_spx_ui_create_label(GdString path, GdRect rect, GdString text) { return 0; }
static GdInt gdextension_spx_ui_create_image(GdString path, GdRect rect, GdColor GdColor) { return 0; }
static GdInt gdextension_spx_ui_create_slider(GdString path, GdRect rect, GdFloat value) { return 0; }
static GdInt gdextension_spx_ui_create_toggle(GdString path, GdRect rect, GdBool value) { return 0; }
static GdInt gdextension_spx_ui_create_input(GdString path, GdRect rect, GdString text) { return 0; }

static GdInt gdextension_spx_ui_get_type(GdInt id) { return 0; }
static void gdextension_spx_ui_set_interactable(GdInt id, GdBool interactable) { print_line(vformat("Updating interactable of %d to %s", id, interactable ? "true" : "false")); }
static GdBool gdextension_spx_ui_get_interactable(GdInt id) { return false; }

static void gdextension_spx_ui_set_text(GdInt id, GdString text) { print_line(vformat("Updating text of %d to %s", id, text)); }
static GdString gdextension_spx_ui_get_text(GdInt id) { return nullptr; }

static void gdextension_spx_ui_set_rect(GdInt id, GdRect rect) { print_line(vformat("Updating rect of %d to %f, %f, %f, %f", id, rect.position.x, rect.position.y, rect.size.x, rect.size.y)); }
static GdRect gdextension_spx_ui_get_rect(GdInt id) { return GdRect(); }

static void gdextension_spx_ui_set_color(GdInt id, GdColor GdColor) { print_line(vformat("Updating GdColor of %d to %f, %f, %f, %f", id, GdColor.r, GdColor.g, GdColor.b, GdColor.a)); }
static GdColor gdextension_spx_ui_get_color(GdInt id) { return GdColor(); }

static void gdextension_spx_ui_set_font_size(GdInt id, GdFloat size) { print_line(vformat("Updating font size of %d to %f", id, size)); }
static GdFloat gdextension_spx_ui_get_font_size(GdInt id) { return 0; }

static void gdextension_spx_ui_set_visible(GdInt id, GdBool visible) { print_line(vformat("Updating visibility of %d to %s", id, visible ? "true" : "false")); }
static GdBool gdextension_spx_ui_get_visible(GdInt id) { return false; }


#define REGISTER_SPX_INTERFACE_FUNC(m_name) GDExtension::register_interface_function("spx_"#m_name, (GDExtensionInterfaceFunctionPtr)&gdextension_spx_##m_name)

static void gdextension_spx_ext() {
	REGISTER_SPX_INTERFACE_FUNC(global_register_callbacks);
	REGISTER_SPX_INTERFACE_FUNC(input_get_mouse_pos);
	REGISTER_SPX_INTERFACE_FUNC(input_get_mouse_state);
	REGISTER_SPX_INTERFACE_FUNC(input_get_key_state);
	REGISTER_SPX_INTERFACE_FUNC(input_get_axis);
	REGISTER_SPX_INTERFACE_FUNC(input_is_action_pressed);
	REGISTER_SPX_INTERFACE_FUNC(input_is_action_just_pressed);
	REGISTER_SPX_INTERFACE_FUNC(input_is_action_just_released);
	REGISTER_SPX_INTERFACE_FUNC(audio_play_audio);
	REGISTER_SPX_INTERFACE_FUNC(audio_set_audio_volume);
	REGISTER_SPX_INTERFACE_FUNC(audio_get_audio_volume);
	REGISTER_SPX_INTERFACE_FUNC(audio_is_music_playing);
	REGISTER_SPX_INTERFACE_FUNC(audio_play_music);
	REGISTER_SPX_INTERFACE_FUNC(audio_set_music_volume);
	REGISTER_SPX_INTERFACE_FUNC(audio_get_music_volume);
	REGISTER_SPX_INTERFACE_FUNC(audio_pause_music);
	REGISTER_SPX_INTERFACE_FUNC(audio_resume_music);
	REGISTER_SPX_INTERFACE_FUNC(audio_get_music_timer);
	REGISTER_SPX_INTERFACE_FUNC(audio_set_music_timer);
	REGISTER_SPX_INTERFACE_FUNC(physic_set_gravity);
	REGISTER_SPX_INTERFACE_FUNC(physic_get_gravity);
	REGISTER_SPX_INTERFACE_FUNC(physic_set_velocity);
	REGISTER_SPX_INTERFACE_FUNC(physic_get_velocity);
	REGISTER_SPX_INTERFACE_FUNC(physic_set_mass);
	REGISTER_SPX_INTERFACE_FUNC(physic_get_mass);
	REGISTER_SPX_INTERFACE_FUNC(physic_add_force);
	REGISTER_SPX_INTERFACE_FUNC(physic_add_impulse);
	REGISTER_SPX_INTERFACE_FUNC(physic_set_collision_layer);
	REGISTER_SPX_INTERFACE_FUNC(physic_get_collision_layer);
	REGISTER_SPX_INTERFACE_FUNC(physic_set_collision_mask);
	REGISTER_SPX_INTERFACE_FUNC(physic_get_collision_mask);
	REGISTER_SPX_INTERFACE_FUNC(physic_get_collider_type);
	REGISTER_SPX_INTERFACE_FUNC(physic_add_collider_rect);
	REGISTER_SPX_INTERFACE_FUNC(physic_add_collider_circle);
	REGISTER_SPX_INTERFACE_FUNC(physic_add_collider_capsule);
	REGISTER_SPX_INTERFACE_FUNC(physic_set_trigger);
	REGISTER_SPX_INTERFACE_FUNC(physic_is_trigger);
	REGISTER_SPX_INTERFACE_FUNC(physic_set_collision_enabled);
	REGISTER_SPX_INTERFACE_FUNC(physic_is_collision_enabled);
	REGISTER_SPX_INTERFACE_FUNC(sprite_instantiate_sprite);
	REGISTER_SPX_INTERFACE_FUNC(sprite_clone_sprite);
	REGISTER_SPX_INTERFACE_FUNC(sprite_destroy_sprite);
	REGISTER_SPX_INTERFACE_FUNC(sprite_is_sprite_alive);
	REGISTER_SPX_INTERFACE_FUNC(sprite_set_position);
	REGISTER_SPX_INTERFACE_FUNC(sprite_set_rotation);
	REGISTER_SPX_INTERFACE_FUNC(sprite_set_scale);
	REGISTER_SPX_INTERFACE_FUNC(sprite_get_position);
	REGISTER_SPX_INTERFACE_FUNC(sprite_get_rotation);
	REGISTER_SPX_INTERFACE_FUNC(sprite_get_scale);
	REGISTER_SPX_INTERFACE_FUNC(sprite_set_color);
	REGISTER_SPX_INTERFACE_FUNC(sprite_get_color);
	REGISTER_SPX_INTERFACE_FUNC(sprite_update_texture);
	REGISTER_SPX_INTERFACE_FUNC(sprite_get_texture);
	REGISTER_SPX_INTERFACE_FUNC(sprite_set_visible);
	REGISTER_SPX_INTERFACE_FUNC(sprite_get_visible);
	REGISTER_SPX_INTERFACE_FUNC(sprite_update_z_index);
	REGISTER_SPX_INTERFACE_FUNC(ui_create_button);
	REGISTER_SPX_INTERFACE_FUNC(ui_create_label);
	REGISTER_SPX_INTERFACE_FUNC(ui_create_image);
	REGISTER_SPX_INTERFACE_FUNC(ui_create_slider);
	REGISTER_SPX_INTERFACE_FUNC(ui_create_toggle);
	REGISTER_SPX_INTERFACE_FUNC(ui_create_input);
	REGISTER_SPX_INTERFACE_FUNC(ui_get_type);
	REGISTER_SPX_INTERFACE_FUNC(ui_set_interactable);
	REGISTER_SPX_INTERFACE_FUNC(ui_get_interactable);
	REGISTER_SPX_INTERFACE_FUNC(ui_set_text);
	REGISTER_SPX_INTERFACE_FUNC(ui_get_text);
	REGISTER_SPX_INTERFACE_FUNC(ui_set_rect);
	REGISTER_SPX_INTERFACE_FUNC(ui_get_rect);
	REGISTER_SPX_INTERFACE_FUNC(ui_set_color);
	REGISTER_SPX_INTERFACE_FUNC(ui_get_color);
	REGISTER_SPX_INTERFACE_FUNC(ui_set_font_size);
	REGISTER_SPX_INTERFACE_FUNC(ui_get_font_size);
	REGISTER_SPX_INTERFACE_FUNC(ui_set_visible);
	REGISTER_SPX_INTERFACE_FUNC(ui_get_visible);
}
