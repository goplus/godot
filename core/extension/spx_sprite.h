#pragma once
#include "gdextension_spx_ext.h"
#include "spx_base_mgr.h"


class SpxSprite : SpxBaseMgr {
public:
	GdInt create_sprite(GdString path);
	GdInt clone_sprite(GdInt id);
	GdBool destroy_sprite(GdInt id);
	GdBool is_sprite_alive(GdInt id);
	void set_position(GdInt id, GdVec2 pos);
	void set_rotation(GdInt id, GdVec2 rot);
	void set_scale(GdInt id, GdVec2 scale);
	GdVec2 get_position(GdInt id);
	GdVec2 get_rotation(GdInt id);
	GdVec2 get_scale(GdInt id);
	void set_color(GdInt id, GdColor GdColor);
	GdColor get_color(GdInt id);
	void update_texture(GdInt id, GdString path);
	GdString get_texture(GdInt id);
	void set_visible(GdInt id, GdBool visible);
	GdBool get_visible(GdInt id);
	void update_z_index(GdInt id, GdInt z);
};
