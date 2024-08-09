#include "gdextension_spx_ext.h"
#include "spx_input.h"
// input
GdVec2 SpxInput::get_mouse_pos() {
	return GdVec2();
}

GdBool SpxInput::get_mouse_state(GdInt id) {
	return false;
}

GdInt SpxInput::get_key_state(GdInt key) {
	return 0;
}

GdFloat SpxInput::get_axis(GdString axis) {
	return 0;
}

GdBool SpxInput::is_action_pressed(GdString action) {
	return false;
}

GdBool SpxInput::is_action_just_pressed(GdString action) {
	return false;
}

GdBool SpxInput::is_action_just_released(GdString action) {
	return false;
}
