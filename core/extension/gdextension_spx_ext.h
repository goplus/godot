/**************************************************************************/
/*  gdextension_spx_ext.h                                                 */
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
#ifndef GDEXTENSION_SPX_EXT_H
#define GDEXTENSION_SPX_EXT_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "gdextension_interface.h"
#ifndef NOT_GODOT_ENGINE
#include "core/variant/variant.h"
#endif

#ifndef __cplusplus
typedef uint32_t char32_t;
typedef uint16_t char16_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef GDExtensionStringPtr gdstring;
typedef GDExtensionInt	gdint;
typedef GDExtensionBool gdbool;
typedef real_t	gdfloat;
typedef Vector4 gdvec4;
typedef Vector3 gdvec3;
typedef Vector2 gdvec2;
typedef Color	gdcolor;
typedef Rect2	gdrect;

typedef void *GDExtensionSpxCallbackInfoPtr;
typedef void (*GDExtensionSpxGlobalRegisterCallbacks)(GDExtensionSpxCallbackInfoPtr callback_ptr);

typedef gdvec2 (*GDExtensionSpxInputGetMousePos)();
typedef gdbool (*GDExtensionSpxInputGetMouseState)(gdint id);
typedef gdint (*GDExtensionSpxInputGetKeyState)(gdint key);
typedef gdfloat (*GDExtensionSpxInputGetAxis)(gdstring axis);
typedef gdbool (*GDExtensionSpxInputIsActionPressed)(gdstring action);
typedef gdbool (*GDExtensionSpxInputIsActionJustPressed)(gdstring action);
typedef gdbool (*GDExtensionSpxInputIsActionJustReleased)(gdstring action);

// audio manager
typedef void (*GDExtensionSpxAudioPlayAudio)(gdstring path);
typedef void (*GDExtensionSpxAudioSetAudioVolume)(gdfloat volume);
typedef gdfloat (*GDExtensionSpxAudioGetAudioVolume)();

typedef gdbool (*GDExtensionSpxAudioIsMusicPlaying)();
typedef void (*GDExtensionSpxAudioPlayMusic)(gdstring path);
typedef void (*GDExtensionSpxAudioSetMusicVolume)(gdfloat volume);
typedef gdfloat (*GDExtensionSpxAudioGetMusicVolume)();
typedef void (*GDExtensionSpxAudioPauseMusic)();
typedef void (*GDExtensionSpxAudioResumeMusic)();
typedef gdfloat (*GDExtensionSpxAudioGetMusicTimer)();
typedef void (*GDExtensionSpxAudioSetMusicTimer)(gdfloat time);

// physic
typedef void (*GDExtensionSpxPhysicSetGravity)(gdfloat gravity);
typedef gdfloat (*GDExtensionSpxPhysicGetGravity)();

typedef void (*GDExtensionSpxPhysicSetVelocity)(gdint id, gdvec2 velocity);
typedef gdvec2 (*GDExtensionSpxPhysicGetVelocity)(gdint id);

typedef void (*GDExtensionSpxPhysicSetMass)(gdint id, gdfloat mass);
typedef gdfloat (*GDExtensionSpxPhysicGetMass)(gdint id);

typedef void (*GDExtensionSpxPhysicAddForce)(gdint id, gdvec2 force);
typedef void (*GDExtensionSpxPhysicAddImpulse)(gdint id, gdvec2 impulse);
typedef void (*GDExtensionSpxPhysicSetCollisionLayer)(gdint id, gdint layer);

typedef gdint (*GDExtensionSpxPhysicGetCollisionLayer)(gdint id);
typedef void (*GDExtensionSpxPhysicSetCollisionMask)(gdint id, gdint mask);
typedef gdint (*GDExtensionSpxPhysicGetCollisionMask)(gdint id);

typedef gdint (*GDExtensionSpxPhysicGetColliderType)(gdint id);
typedef void (*GDExtensionSpxPhysicAddColliderRect)(gdint id, gdvec2 center, gdvec2 size);
typedef void (*GDExtensionSpxPhysicAddColliderCircle)(gdint id, gdvec2 center, gdfloat radius);
typedef void (*GDExtensionSpxPhysicAddColliderCapsule)(gdint id, gdvec2 center, gdvec2 size);

typedef void (*GDExtensionSpxPhysicSetTrigger)(gdint id, gdbool trigger);
typedef gdbool (*GDExtensionSpxPhysicIsTrigger)(gdint id);

typedef void (*GDExtensionSpxPhysicSetCollisionEnabled)(gdint id, gdbool enabled);
typedef gdbool (*GDExtensionSpxPhysicIsCollisionEnabled)(gdint id);


// sprite
typedef gdint (*GDExtensionSpxSpriteCreateSprite)(gdstring path);
typedef gdint (*GDExtensionSpxSpriteCloneSprite)(gdint id);
typedef gdbool (*GDExtensionSpxSpriteDestroySprite)(gdint id);
typedef gdbool (*GDExtensionSpxSpriteIsSpriteAlive)(gdint id);

typedef void (*GDExtensionSpxSpriteSetPosition)(gdint id, gdvec2 pos);
typedef void (*GDExtensionSpxSpriteSetRotation)(gdint id, gdvec2 rot);
typedef void (*GDExtensionSpxSpriteSetScale)(gdint id, gdvec2 scale);

typedef gdvec2 (*GDExtensionSpxSpriteGetPosition)(gdint id);
typedef gdvec2 (*GDExtensionSpxSpriteGetRotation)(gdint id);
typedef gdvec2 (*GDExtensionSpxSpriteGetScale)(gdint id);

typedef void (*GDExtensionSpxSpriteSetColor)(gdint id, gdcolor gdcolor);
typedef gdcolor (*GDExtensionSpxSpriteGetColor)(gdint id);

typedef void (*GDExtensionSpxSpriteUpdateTexture)(gdint id, gdstring path);
typedef gdstring (*GDExtensionSpxSpriteGetTexture)(gdint id);

typedef void (*GDExtensionSpxSpriteSetVisible)(gdint id, gdbool visible);
typedef gdbool (*GDExtensionSpxSpriteGetVisible)(gdint id);
typedef void (*GDExtensionSpxSpriteUpdateZIndex)(gdint id, gdint z);


// ui
typedef gdint (*GDExtensionSpxUICreateButton)(gdstring path, gdrect rect, gdstring text);
typedef gdint (*GDExtensionSpxUICreateLabel)(gdstring path, gdrect rect, gdstring text);
typedef gdint (*GDExtensionSpxUICreateImage)(gdstring path, gdrect rect, gdcolor gdcolor);
typedef gdint (*GDExtensionSpxUICreateSlider)(gdstring path, gdrect rect, gdfloat value);
typedef gdint (*GDExtensionSpxUICreateToggle)(gdstring path, gdrect rect, gdbool value);
typedef gdint (*GDExtensionSpxUICreateInput)(gdstring path, gdrect rect, gdstring text);

typedef gdint (*GDExtensionSpxUIGetType)(gdint id);
typedef void (*GDExtensionSpxUISetInteractable)(gdint id, gdbool interactable);
typedef gdbool (*GDExtensionSpxUIGetInteractable)(gdint id);

typedef void (*GDExtensionSpxUISetText)(gdint id, gdstring text);
typedef gdstring (*GDExtensionSpxUIGetText)(gdint id);

typedef void (*GDExtensionSpxUISetRect)(gdint id, gdrect rect);
typedef gdrect (*GDExtensionSpxUIGetRect)(gdint id);

typedef void (*GDExtensionSpxUISetColor)(gdint id, gdcolor gdcolor);
typedef gdcolor (*GDExtensionSpxUIGetColor)(gdint id);

typedef void (*GDExtensionSpxUISetFontSize)(gdint id, gdfloat size);
typedef gdfloat (*GDExtensionSpxUIGetFontSize)(gdint id);

typedef void (*GDExtensionSpxUISetVisible)(gdint id, gdbool visible);
typedef gdbool (*GDExtensionSpxUIGetVisible)(gdint id);




// callback
typedef void (*GDExtensionSpxCallbackOnEngineStart) ();
typedef void (*GDExtensionSpxCallbackOnEngineUpdate) (gdfloat delta);
typedef void (*GDExtensionSpxCallbackOnEngineDestroy) ();

typedef void (*GDExtensionSpxCallbackOnSpriteReady) (gdint id);
typedef void (*GDExtensionSpxCallbackOnSpriteUpdated) (gdint id);
typedef void (*GDExtensionSpxCallbackOnSpriteDestroyed) (gdint id);

typedef void (*GDExtensionSpxCallbackOnMousePressed) (gdint keyid);
typedef void (*GDExtensionSpxCallbackOnMouseReleased) (gdint keyid);
typedef void (*GDExtensionSpxCallbackOnKeyPressed) (gdint keyid);
typedef void (*GDExtensionSpxCallbackOnKeyReleased) (gdint keyid);
typedef void (*GDExtensionSpxCallbackOnActionPressed) (gdstring action_name);
typedef void (*GDExtensionSpxCallbackOnActionJustPressed) (gdstring action_name);
typedef void (*GDExtensionSpxCallbackOnActionJustReleased) (gdstring action_name);
typedef void (*GDExtensionSpxCallbackOnAxisChanged) (gdstring action_name, gdfloat value);

typedef void (*GDExtensionSpxCallbackOnCollisionEnter) (gdint self_id, gdint other_id);
typedef void (*GDExtensionSpxCallbackOnCollisionStay) (gdint self_id, gdint other_id);
typedef void (*GDExtensionSpxCallbackOnCollisionExit) (gdint self_id, gdint other_id);
typedef void (*GDExtensionSpxCallbackOnTriggerEnter) (gdint self_id, gdint other_id);
typedef void (*GDExtensionSpxCallbackOnTriggerStay) (gdint self_id, gdint other_id);
typedef void (*GDExtensionSpxCallbackOnTriggerExit) (gdint self_id, gdint other_id);

typedef void (*GDExtensionSpxCallbackOnUIPressed) (gdint id);
typedef void (*GDExtensionSpxCallbackOnUIReleased) (gdint id);
typedef void (*GDExtensionSpxCallbackOnUIHovered) (gdint id);
typedef void (*GDExtensionSpxCallbackOnUIClicked) (gdint id);
typedef void (*GDExtensionSpxCallbackOnUIToggle) (gdint id, gdbool is_on);
typedef void (*GDExtensionSpxCallbackOnUITextChanged) (gdint id, gdstring text);



typedef struct {
	// engine
	GDExtensionSpxCallbackOnEngineStart func_on_engine_start;
	GDExtensionSpxCallbackOnEngineUpdate func_on_engine_update;
	GDExtensionSpxCallbackOnEngineDestroy func_on_engine_destroy;

	// sprite
	GDExtensionSpxCallbackOnSpriteReady func_on_sprite_ready;
	GDExtensionSpxCallbackOnSpriteUpdated func_on_sprite_updated;
	GDExtensionSpxCallbackOnSpriteDestroyed func_on_sprite_destroyed;

	// input
	GDExtensionSpxCallbackOnMousePressed func_on_mouse_pressed;
	GDExtensionSpxCallbackOnMouseReleased func_on_mouse_released;
	GDExtensionSpxCallbackOnKeyPressed func_on_key_pressed;
	GDExtensionSpxCallbackOnKeyReleased func_on_key_released;
	GDExtensionSpxCallbackOnActionPressed func_on_action_pressed;
	GDExtensionSpxCallbackOnActionJustPressed func_on_action_just_pressed;
	GDExtensionSpxCallbackOnActionJustReleased func_on_action_just_released;
	GDExtensionSpxCallbackOnAxisChanged func_on_axis_changed;

	// physic
	GDExtensionSpxCallbackOnCollisionEnter func_on_collision_enter;
	GDExtensionSpxCallbackOnCollisionStay func_on_collision_stay;
	GDExtensionSpxCallbackOnCollisionExit func_on_collision_exit;
	GDExtensionSpxCallbackOnTriggerEnter func_on_trigger_enter;
	GDExtensionSpxCallbackOnTriggerStay func_on_trigger_stay;
	GDExtensionSpxCallbackOnTriggerExit func_on_trigger_exit;

	// ui
	GDExtensionSpxCallbackOnUIPressed func_on_ui_pressed;
	GDExtensionSpxCallbackOnUIReleased func_on_ui_released;
	GDExtensionSpxCallbackOnUIHovered func_on_ui_hovered;
	GDExtensionSpxCallbackOnUIClicked func_on_ui_clicked;
	GDExtensionSpxCallbackOnUIToggle func_on_ui_toggle;
	GDExtensionSpxCallbackOnUITextChanged func_on_ui_text_changed;

} SpxCallbackInfo;


#ifdef __cplusplus
}
#endif

#endif // GDEXTENSION_SPX_EXT_H
