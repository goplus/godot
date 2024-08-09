#include "spx_sprite.h"

#include "spx_engine.h"
#include "scene/main/node.h"


// sprite
GdInt SpxSprite::create_sprite(GdString path) {
	const String path_str = String(*(const String *)path);
	Node *new_node = memnew(Node);
	new_node->set_name(path_str);
	get_root_node()->add_child(new_node);
	print_line("cpp SpxSprite::create_sprite " + path_str);
	return get_unique_id();
}

GdInt SpxSprite::clone_sprite(GdInt id) {
	return 0;
}

GdBool SpxSprite::destroy_sprite(GdInt id) {
	return false;
}

GdBool SpxSprite::is_sprite_alive(GdInt id) {
	return false;
}

void SpxSprite::set_position(GdInt id, GdVec2 pos) {
	print_line(vformat("Updating position of %d to %f, %f", id, pos.x, pos.y));
}

void SpxSprite::set_rotation(GdInt id, GdVec2 rot) {
	print_line(vformat("Updating rotation of %d to %f, %f", id, rot.x, rot.y));
}

void SpxSprite::set_scale(GdInt id, GdVec2 scale) {
	print_line(vformat("Updating scale of %d to %f, %f", id, scale.x, scale.y));
}

GdVec2 SpxSprite::get_position(GdInt id) {
	return GdVec2();
}

GdVec2 SpxSprite::get_rotation(GdInt id) {
	return GdVec2();
}

GdVec2 SpxSprite::get_scale(GdInt id) {
	return GdVec2();
}

void SpxSprite::set_color(GdInt id, GdColor GdColor) {
	print_line(vformat("Updating GdColor of %d to %f, %f, %f, %f", id, GdColor.r, GdColor.g, GdColor.b, GdColor.a));
}

GdColor SpxSprite::get_color(GdInt id) {
	return GdColor();
}

void SpxSprite::update_texture(GdInt id, GdString path) {
	print_line(vformat("Updating texture of %d to %s", id, path));
}

GdString SpxSprite::get_texture(GdInt id) {
	return nullptr;
}

void SpxSprite::set_visible(GdInt id, GdBool visible) {
	print_line(vformat("Updating visibility of %d to %s", id, visible ? "true" : "false"));
}

GdBool SpxSprite::get_visible(GdInt id) {
	return false;
}

void SpxSprite::update_z_index(GdInt id, GdInt z) {
	print_line(vformat("Updating z index of %d to %d", id, z));
}
