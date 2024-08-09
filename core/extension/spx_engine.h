#pragma once
#include <cstdint>
#include "gdextension_spx_ext.h"
#include "spx_base_mgr.h"

class Node;
class SpxInput;
class SpxAudio;
class SpxPhysic;
class SpxSprite;
class SpxUI;

class SpxEngine : SpxBaseMgr {
	static SpxEngine *singleton;

public :
	static SpxEngine *get_singleton() { return singleton; }
	static bool has_initialed() { return singleton != nullptr; }
	static void register_callbacks(GDExtensionSpxCallbackInfoPtr callback_ptr);

private:
	SpxInput *input;
	SpxAudio *audio;
	SpxPhysic *physic;
	SpxSprite *sprite;
	SpxUI *ui;
	Vector<SpxBaseMgr *> mgrs;

public :
	SpxInput *get_input() { return input; }
	SpxAudio *get_audio() { return audio; }
	SpxPhysic *get_physic() { return physic; }
	SpxSprite *get_sprite() { return sprite; }
	SpxUI *get_ui() { return ui; }

private:
	Node *root_node;
	GdInt global_id;
	SpxCallbackInfo callbacks;

public :
	GdInt get_unique_id() override;
	Node *get_root_node() override;
	void set_root_node(Node *node) { root_node = node; }

	void on_start() override;
	void on_update(float delta) override;
	void on_destroy() override;
};
