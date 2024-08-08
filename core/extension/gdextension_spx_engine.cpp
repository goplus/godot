/**************************************************************************/
/*  gdextension_spx_engine.h                                              */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/


#include "scene/main/node.h"
#include "gdextension_spx_engine.h"
#include "gdextension_spx_ext.h"

SpxCallbackInfo SpxEngine::spx_callback_info;
void SpxEngine::on_start(void* root) {
	Node* tree = (Node *)root;
	if (tree != nullptr) {
		Node *new_node = memnew(Node);
		new_node->set_name("SpxEngineNode");
		tree->add_child(new_node);
	}
	if(spx_callback_info.func_on_engine_start != nullptr) {
		spx_callback_info.func_on_engine_start();
	}
}

void SpxEngine::on_update(float delta) {
	if(spx_callback_info.func_on_engine_update != nullptr) {
		spx_callback_info.func_on_engine_update(delta);
	}
}

void SpxEngine::on_destroy() {
	if(spx_callback_info.func_on_engine_destroy != nullptr) {
		spx_callback_info.func_on_engine_destroy();
	}
	spx_callback_info = SpxCallbackInfo();
}

