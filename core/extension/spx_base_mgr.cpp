#include "spx_base_mgr.h"
#include "spx_engine.h"

GdInt SpxBaseMgr::get_unique_id() {
	return SpxEngine::get_singleton()->get_unique_id();
}

Node *SpxBaseMgr::get_root_node() {
	return SpxEngine::get_singleton()->get_root_node();
}

void SpxBaseMgr::on_start() {
}

void SpxBaseMgr::on_update(float delta) {
}

void SpxBaseMgr::on_destroy() {
}
