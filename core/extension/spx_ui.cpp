#include "spx_ui.h"


// ui
GdInt SpxUI::create_button(GdString path, GdRect rect, GdString text) {
	return 0;
}

GdInt SpxUI::create_label(GdString path, GdRect rect, GdString text) {
	return 0;
}

GdInt SpxUI::create_image(GdString path, GdRect rect, GdColor GdColor) {
	return 0;
}

GdInt SpxUI::create_slider(GdString path, GdRect rect, GdFloat value) {
	return 0;
}

GdInt SpxUI::create_toggle(GdString path, GdRect rect, GdBool value) {
	return 0;
}

GdInt SpxUI::create_input(GdString path, GdRect rect, GdString text) {
	return 0;
}

GdInt SpxUI::get_type(GdInt id) {
	return 0;
}

void SpxUI::set_interactable(GdInt id, GdBool interactable) {
	print_line(vformat("Updating interactable of %d to %s", id, interactable ? "true" : "false"));
}

GdBool SpxUI::get_interactable(GdInt id) {
	return false;
}

void SpxUI::set_text(GdInt id, GdString text) {
	print_line(vformat("Updating text of %d to %s", id, text));
}

GdString SpxUI::get_text(GdInt id) {
	return nullptr;
}

void SpxUI::set_rect(GdInt id, GdRect rect) {
	print_line(vformat("Updating rect of %d to %f, %f, %f, %f", id, rect.position.x, rect.position.y, rect.size.x, rect.size.y));
}

GdRect SpxUI::get_rect(GdInt id) {
	return GdRect();
}

void SpxUI::set_color(GdInt id, GdColor GdColor) {
	print_line(vformat("Updating GdColor of %d to %f, %f, %f, %f", id, GdColor.r, GdColor.g, GdColor.b, GdColor.a));
}

GdColor SpxUI::get_color(GdInt id) {
	return GdColor();
}

void SpxUI::set_font_size(GdInt id, GdFloat size) {
	print_line(vformat("Updating font size of %d to %f", id, size));
}

GdFloat SpxUI::get_font_size(GdInt id) {
	return 0;
}

void SpxUI::set_visible(GdInt id, GdBool visible) {
	print_line(vformat("Updating visibility of %d to %s", id, visible ? "true" : "false"));
}

GdBool SpxUI::get_visible(GdInt id) {
	return false;
}
