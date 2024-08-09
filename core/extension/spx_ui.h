#pragma once
#include "gdextension_spx_ext.h"
#include "spx_engine.h"


class SpxUI : SpxBaseMgr {
public:
	GdInt create_button(GdString path, GdRect rect, GdString text);
	GdInt create_label(GdString path, GdRect rect, GdString text);
	GdInt create_image(GdString path, GdRect rect, GdColor GdColor);
	GdInt create_slider(GdString path, GdRect rect, GdFloat value);
	GdInt create_toggle(GdString path, GdRect rect, GdBool value);
	GdInt create_input(GdString path, GdRect rect, GdString text);
	GdInt get_type(GdInt id);
	void set_interactable(GdInt id, GdBool interactable);
	GdBool get_interactable(GdInt id);
	void set_text(GdInt id, GdString text);
	GdString get_text(GdInt id);
	void set_rect(GdInt id, GdRect rect);
	GdRect get_rect(GdInt id);
	void set_color(GdInt id, GdColor GdColor);
	GdColor get_color(GdInt id);
	void set_font_size(GdInt id, GdFloat size);
	GdFloat get_font_size(GdInt id);
	void set_visible(GdInt id, GdBool visible);
	GdBool get_visible(GdInt id);
};
