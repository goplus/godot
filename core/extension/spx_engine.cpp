#include "core/config/engine.h"
#include "core/extension/gdextension.h"
#include "core/os/memory.h"
#include "scene/main/window.h"
#include "gdextension_spx_ext.h"
#include "spx_engine.h"
#include "spx_audio.h"
#include "spx_input.h"
#include "spx_physic.h"
#include "spx_sprite.h"
#include "spx_ui.h"

SpxEngine *SpxEngine::singleton;

void SpxEngine::register_callbacks(GDExtensionSpxCallbackInfoPtr callback_ptr) {
	if (singleton != nullptr) {
		print_line("SpxEngine::register_callbacks failed, already initialed! ");
		return;
	}
	singleton = new SpxEngine();
	singleton->input = memnew(SpxInput);
	singleton->mgrs.clear();
	singleton->mgrs.append((SpxBaseMgr *)singleton->input);
	singleton->singleton->audio = memnew(SpxAudio);
	singleton->mgrs.append((SpxBaseMgr *)singleton->audio);
	singleton->singleton->physic = memnew(SpxPhysic);
	singleton->mgrs.append((SpxBaseMgr *)singleton->physic);
	singleton->singleton->sprite = memnew(SpxSprite);
	singleton->mgrs.append((SpxBaseMgr *)singleton->sprite);
	singleton->singleton->ui = memnew(SpxUI);
	singleton->mgrs.append((SpxBaseMgr *)singleton->ui);
	singleton->callbacks = *(SpxCallbackInfo *)callback_ptr;
}

GdInt SpxEngine::get_unique_id() {
	return global_id++;
}

Node *SpxEngine::get_root_node() {
	return root_node;
}

void SpxEngine::on_start() {
	for (auto mgr : mgrs) {
		mgr->on_start();
	}
	if (callbacks.func_on_engine_start != nullptr) {
		callbacks.func_on_engine_start();
	}
}

void SpxEngine::on_update(float delta) {
	for (auto mgr : mgrs) {
		mgr->on_update(delta);
	}
	if (callbacks.func_on_engine_update != nullptr) {
		callbacks.func_on_engine_update(delta);
	}
}

void SpxEngine::on_destroy() {
	for (auto mgr : mgrs) {
		mgr->on_destroy();
	}
	if (callbacks.func_on_engine_destroy != nullptr) {
		callbacks.func_on_engine_destroy();
	}
	callbacks = SpxCallbackInfo();
	memdelete(input);
	memdelete(audio);
	memdelete(physic);
	memdelete(sprite);
	memdelete(ui);
	mgrs.clear();
}
