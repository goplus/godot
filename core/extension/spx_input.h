#pragma once
#include "gdextension_spx_ext.h"
#include "spx_base_mgr.h"

class SpxInput : SpxBaseMgr {
public:
	GdVec2 get_mouse_pos();
	GdBool get_mouse_state(GdInt id);
	GdInt get_key_state(GdInt key);
	GdFloat get_axis(GdString axis);
	GdBool is_action_pressed(GdString action);
	GdBool is_action_just_pressed(GdString action);
	GdBool is_action_just_released(GdString action);
};
