/**************************************************************************/
/*  gdextension_interface.cpp                                             */
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

// input
static Vector2 gdextension_spx_input_get_mouse_pos() { return Vector2(); }
static bool gdextension_spx_input_get_mouse_state(int id) { return false; }
static int gdextension_spx_input_get_key_state(int key) { return 0; }
static float gdextension_spx_input_get_axis(String axis) { return 0; }
static bool gdextension_spx_input_is_action_pressed(String action) { return false; }
static bool gdextension_spx_input_is_action_just_pressed(String action) { return false; }
static bool gdextension_spx_input_is_action_just_released(String action) { return false; }

// audio manager
static void gdextension_spx_audio_play_audio(String path) { print_line(vformat("Playing %s", path)); }
static void gdextension_spx_audio_set_audio_volume(float volume) { print_line(vformat("Setting volume to %f", volume)); }
static float gdextension_spx_audio_get_audio_volume() { print_line("GetAudioVolume"); return 0; }

static bool gdextension_spx_audio_is_music_playing() { return false; }
static void gdextension_spx_audio_play_music(String path) { print_line(vformat("Playing music %s", path)); }
static void gdextension_spx_audio_set_music_volume(float volume) { print_line(vformat("Setting music volume to %f", volume)); }
static float gdextension_spx_audio_get_music_volume() { print_line("GetMusicVolume"); return 0; }
static void gdextension_spx_audio_pause_music() { print_line("PauseMusic"); }
static void gdextension_spx_audio_resume_music() { print_line("ResumeMusic"); }
static float gdextension_spx_audio_get_music_timer() { return 0; }
static void gdextension_spx_audio_set_music_timer(float time) { print_line(vformat("Setting music timer to %f", time)); }

// physic
static void gdextension_spx_physic_set_gravity(float gravity) { print_line(vformat("Setting gravity to %f", gravity)); }
static float gdextension_spx_physic_get_gravity() { return 0; }

static void gdextension_spx_physic_set_velocity(int id, Vector3 velocity) { print_line(vformat("Setting velocity of %d to %f, %f", id, velocity.x, velocity.y)); }
static Vector3 gdextension_spx_physic_get_velocity(int id) { return Vector3(); }

static void gdextension_spx_physic_set_mass(int id, float mass) { print_line(vformat("Setting mass of %d to %f", id, mass)); }
static float gdextension_spx_physic_get_mass(int id) { return 0; }

static void gdextension_spx_physic_add_force(int id, Vector3 force) { print_line(vformat("Adding force to %d %f, %f", id, force.x, force.y)); }
static void gdextension_spx_physic_add_impulse(int id, Vector3 impulse) { print_line(vformat("Adding impulse to %d %f, %f", id, impulse.x, impulse.y)); }
static void gdextension_spx_physic_set_collision_layer(int id, int layer) { print_line(""); }

static int gdextension_spx_physic_get_collision_layer(int id) { return 0; }
static void gdextension_spx_physic_set_collision_mask(int id, int mask) { print_line(vformat("Setting collision mask of %d to %d", id, mask)); }
static int gdextension_spx_physic_get_collision_mask(int id) { return 0; }

static int gdextension_spx_physic_get_collider_type(int id) { return 0; }
static void gdextension_spx_physic_add_collider_2D_rect(int id, Vector3 center, Vector3 size) { print_line(""); }
static void gdextension_spx_physic_add_collider_2D_circle(int id, Vector3 center, float radius) { print_line(""); }
static void gdextension_spx_physic_add_collider_2D_capsule(int id, Vector3 center, Vector3 size) { print_line(""); }

static void gdextension_spx_physic_set_trigger(int id, bool trigger) { print_line(vformat("Setting trigger of %d to %d", id, trigger)); }
static bool gdextension_spx_physic_is_trigger(int id) { return false; }

static void gdextension_spx_physic_set_collision_enabled(int id, bool enabled) { print_line(""); }
static bool gdextension_spx_physic_is_collision_enabled(int id) { return false; }


// sprite
static int gdextension_spx_sprite_instantiate_sprite(String path) { return 0; }
static int gdextension_spx_sprite_clone_sprite(int id) { return 0; }
static bool gdextension_spx_sprite_destroy_sprite(int id) { return false; }
static bool gdextension_spx_sprite_is_sprite_alive(int id) { return false; }

static void gdextension_spx_sprite_set_position(int id, Vector3 pos) { print_line(vformat("Updating position of %d to %f, %f, %f", id, pos.x, pos.y, pos.z)); }
static void gdextension_spx_sprite_set_rotation(int id, Vector3 rot) { print_line(vformat("Updating rotation of %d to %f, %f, %f", id, rot.x, rot.y, rot.z)); }
static void gdextension_spx_sprite_set_scale(int id, Vector3 scale) { print_line(vformat("Updating scale of %d to %f, %f, %f", id, scale.x, scale.y, scale.z)); }

static Vector3 gdextension_spx_sprite_get_position(int id) { return Vector3(); }
static Vector3 gdextension_spx_sprite_get_rotation(int id) { return Vector3(); }
static Vector3 gdextension_spx_sprite_get_scale(int id) { return Vector3(); }

static void gdextension_spx_sprite_set_color(int id, Color color) { print_line(vformat("Updating color of %d to %f, %f, %f, %f", id, color.r, color.g, color.b, color.a)); }
static Color gdextension_spx_sprite_get_color(int id) { return Color(); }

static void gdextension_spx_sprite_update_texture(int id, String path) { print_line(vformat("Updating texture of %d to %s", id, path)); }
static String gdextension_spx_sprite_get_texture(int id) { return String(); }

static void gdextension_spx_sprite_set_visible(int id, bool visible) { print_line(vformat("Updating visibility of %d to %s", id, visible ? "true" : "false")); }
static bool gdextension_spx_sprite_get_visible(int id) { return false; }
static void gdextension_spx_sprite_update_z_index(int id, int z) { print_line(vformat("Updating z index of %d to %d", id, z)); }
// TODO add child


// ui
static int gdextension_spx_ui_create_button(String path, Rect2 rect, String text) { return 0; }
static int gdextension_spx_ui_create_label(String path, Rect2 rect, String text) { return 0; }
static int gdextension_spx_ui_create_image(String path, Rect2 rect, Color color) { return 0; }
static int gdextension_spx_ui_create_slider(String path, Rect2 rect, float value) { return 0; }
static int gdextension_spx_ui_create_toggle(String path, Rect2 rect, bool value) { return 0; }
static int gdextension_spx_ui_create_input(String path, Rect2 rect, String text) { return 0; }

static int gdextension_spx_ui_get_type(int id) { return 0; }
static void gdextension_spx_ui_set_interactable(int id, bool interactable) { print_line(vformat("Updating interactable of %d to %s", id, interactable ? "true" : "false")); }
static bool gdextension_spx_ui_get_interactable(int id) { return false; }

static void gdextension_spx_ui_set_text(int id, String text) { print_line(vformat("Updating text of %d to %s", id, text)); }
static String gdextension_spx_ui_get_text(int id) { return String(); }

static void gdextension_spx_ui_set_rect(int id, Rect2 rect) { print_line(vformat("Updating rect of %d to %f, %f, %f, %f", id, rect.position.x, rect.position.y, rect.size.x, rect.size.y)); }
static Rect2 gdextension_spx_ui_get_rect(int id) { return Rect2(); }

static void gdextension_spx_ui_set_color(int id, Color color) { print_line(vformat("Updating color of %d to %f, %f, %f, %f", id, color.r, color.g, color.b, color.a)); }
static Color gdextension_spx_ui_get_color(int id) { return Color(); }

static void gdextension_spx_ui_set_font_size(int id, float size) { print_line(vformat("Updating font size of %d to %f", id, size)); }
static float gdextension_spx_ui_get_font_size(int id) { return 0; }

static void gdextension_spx_ui_set_visible(int id, bool visible) { print_line(vformat("Updating visibility of %d to %s", id, visible ? "true" : "false")); }
static bool gdextension_spx_ui_get_visible(int id) { return false; }



#define REGISTER_SPX_INTERFACE_FUNC(m_name) GDExtension::register_interface_function(#m_name, (GDExtensionInterfaceFunctionPtr)&gdextension_spx_##m_name)
static void gdextension_spx_ext() {
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
	REGISTER_SPX_INTERFACE_FUNC(physic_add_collider_2D_rect);
	REGISTER_SPX_INTERFACE_FUNC(physic_add_collider_2D_circle);
	REGISTER_SPX_INTERFACE_FUNC(physic_add_collider_2D_capsule);
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
