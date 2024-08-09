#pragma once
#include "gdextension_spx_ext.h"
#include "spx_base_mgr.h"


class SpxPhysic : SpxBaseMgr {
public:
	void set_gravity(GdFloat gravity);
	GdFloat get_gravity();
	void set_velocity(GdInt id, GdVec2 velocity);
	GdVec2 get_velocity(GdInt id);
	void set_mass(GdInt id, GdFloat mass);
	GdFloat get_mass(GdInt id);
	void add_force(GdInt id, GdVec2 force);
	void add_impulse(GdInt id, GdVec2 impulse);
	void set_collision_layer(GdInt id, GdInt layer);
	GdInt get_collision_layer(GdInt id);
	void set_collision_mask(GdInt id, GdInt mask);
	GdInt get_collision_mask(GdInt id);
	GdInt get_collider_type(GdInt id);
	void add_collider_rect(GdInt id, GdVec2 center, GdVec2 size);
	void add_collider_circle(GdInt id, GdVec2 center, GdFloat radius);
	void add_collider_capsule(GdInt id, GdVec2 center, GdVec2 size);
	void set_trigger(GdInt id, GdBool trigger);
	GdBool is_trigger(GdInt id);
	void set_collision_enabled(GdInt id, GdBool enabled);
	GdBool is_collision_enabled(GdInt id);
};
