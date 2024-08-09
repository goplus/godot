#include "spx_physic.h"


// physic
void SpxPhysic::set_gravity(GdFloat gravity) {
	print_line(vformat("Setting gravity to %f", gravity));
}

GdFloat SpxPhysic::get_gravity() {
	return 0;
}

void SpxPhysic::set_velocity(GdInt id, GdVec2 velocity) {
	print_line(vformat("Setting velocity of %d to %f, %f", id, velocity.x, velocity.y));
}

GdVec2 SpxPhysic::get_velocity(GdInt id) {
	return GdVec2();
}

void SpxPhysic::set_mass(GdInt id, GdFloat mass) {
	print_line(vformat("Setting mass of %d to %f", id, mass));
}

GdFloat SpxPhysic::get_mass(GdInt id) {
	return 0;
}

void SpxPhysic::add_force(GdInt id, GdVec2 force) {
	print_line(vformat("Adding force to %d %f, %f", id, force.x, force.y));
}

void SpxPhysic::add_impulse(GdInt id, GdVec2 impulse) {
	print_line(vformat("Adding impulse to %d %f, %f", id, impulse.x, impulse.y));
}

void SpxPhysic::set_collision_layer(GdInt id, GdInt layer) {
	print_line("");
}

GdInt SpxPhysic::get_collision_layer(GdInt id) {
	return 0;
}

void SpxPhysic::set_collision_mask(GdInt id, GdInt mask) {
	print_line(vformat("Setting collision mask of %d to %d", id, mask));
}

GdInt SpxPhysic::get_collision_mask(GdInt id) {
	return 0;
}

GdInt SpxPhysic::get_collider_type(GdInt id) {
	return 0;
}

void SpxPhysic::add_collider_rect(GdInt id, GdVec2 center, GdVec2 size) {
	print_line("");
}

void SpxPhysic::add_collider_circle(GdInt id, GdVec2 center, GdFloat radius) {
	print_line("");
}

void SpxPhysic::add_collider_capsule(GdInt id, GdVec2 center, GdVec2 size) {
	print_line("");
}

void SpxPhysic::set_trigger(GdInt id, GdBool trigger) {
	print_line(vformat("Setting trigger of %d to %d", id, trigger));
}

GdBool SpxPhysic::is_trigger(GdInt id) {
	return false;
}

void SpxPhysic::set_collision_enabled(GdInt id, GdBool enabled) {
	print_line("");
}

GdBool SpxPhysic::is_collision_enabled(GdInt id) {
	return false;
}
