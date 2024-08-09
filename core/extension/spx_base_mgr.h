#pragma once

#include "gdextension_spx_ext.h"
#include "scene/main/node.h"

class SpxBaseMgr {
protected:
	virtual GdInt get_unique_id();
	virtual Node *get_root_node();

public :
	virtual void on_start();
	virtual void on_update(float delta);
	virtual void on_destroy();
};
