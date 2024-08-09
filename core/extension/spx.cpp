#include "scene/main/node.h"
#include "spx.h"
#include "gdextension_spx_ext.h"
#include "spx_engine.h"

#define SPX_ENGINE SpxEngine::get_singleton()

void Spx::on_start(void *root_node) {
	if(!SpxEngine::has_initialed()) {
		return;
	}
	Node *tree = (Node *)root_node;
	if (tree == nullptr) {
		return;
	}
	Node *new_node = memnew(Node);
	new_node->set_name("SpxEngineNode");
	tree->add_child(new_node);
	SPX_ENGINE->set_root_node(new_node);
	SPX_ENGINE->on_start();
}

void Spx::on_update(float delta) {
	if(!SpxEngine::has_initialed()) {
		return;
	}
	SPX_ENGINE->on_update(delta);
}

void Spx::on_destroy() {
	if(!SpxEngine::has_initialed()) {
		return;
	}
	SPX_ENGINE->on_destroy();
}
