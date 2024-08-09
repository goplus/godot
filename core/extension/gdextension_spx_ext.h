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
extern void gdextension_spx_setup_interface();
#endif

#ifndef __cplusplus
typedef uint32_t char32_t;
typedef uint16_t char16_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef GDExtensionConstStringPtr GdString;
typedef GDExtensionInt	GdInt;
typedef GDExtensionBool GdBool;
typedef real_t	GdFloat;
typedef Vector4 GdVec4;
typedef Vector3 GdVec3;
typedef Vector2 GdVec2;
typedef Color	GdColor;
typedef Rect2	GdRect;

typedef void *GDExtensionSpxCallbackInfoPtr;
typedef void (*GDExtensionSpxGlobalRegisterCallbacks)(GDExtensionSpxCallbackInfoPtr callback_ptr);

typedef GdVec2 (*GDExtensionSpxInputGetMousePos)();
typedef GdBool (*GDExtensionSpxInputGetMouseState)(GdInt id);
typedef GdInt (*GDExtensionSpxInputGetKeyState)(GdInt key);
typedef GdFloat (*GDExtensionSpxInputGetAxis)(GdString axis);
typedef GdBool (*GDExtensionSpxInputIsActionPressed)(GdString action);
typedef GdBool (*GDExtensionSpxInputIsActionJustPressed)(GdString action);
typedef GdBool (*GDExtensionSpxInputIsActionJustReleased)(GdString action);

// audio manager
typedef void (*GDExtensionSpxAudioPlayAudio)(GdString path);
typedef void (*GDExtensionSpxAudioSetAudioVolume)(GdFloat volume);
typedef GdFloat (*GDExtensionSpxAudioGetAudioVolume)();

typedef GdBool (*GDExtensionSpxAudioIsMusicPlaying)();
typedef void (*GDExtensionSpxAudioPlayMusic)(GdString path);
typedef void (*GDExtensionSpxAudioSetMusicVolume)(GdFloat volume);
typedef GdFloat (*GDExtensionSpxAudioGetMusicVolume)();
typedef void (*GDExtensionSpxAudioPauseMusic)();
typedef void (*GDExtensionSpxAudioResumeMusic)();
typedef GdFloat (*GDExtensionSpxAudioGetMusicTimer)();
typedef void (*GDExtensionSpxAudioSetMusicTimer)(GdFloat time);

// physic
typedef void (*GDExtensionSpxPhysicSetGravity)(GdFloat gravity);
typedef GdFloat (*GDExtensionSpxPhysicGetGravity)();

typedef void (*GDExtensionSpxPhysicSetVelocity)(GdInt id, GdVec2 velocity);
typedef GdVec2 (*GDExtensionSpxPhysicGetVelocity)(GdInt id);

typedef void (*GDExtensionSpxPhysicSetMass)(GdInt id, GdFloat mass);
typedef GdFloat (*GDExtensionSpxPhysicGetMass)(GdInt id);

typedef void (*GDExtensionSpxPhysicAddForce)(GdInt id, GdVec2 force);
typedef void (*GDExtensionSpxPhysicAddImpulse)(GdInt id, GdVec2 impulse);
typedef void (*GDExtensionSpxPhysicSetCollisionLayer)(GdInt id, GdInt layer);

typedef GdInt (*GDExtensionSpxPhysicGetCollisionLayer)(GdInt id);
typedef void (*GDExtensionSpxPhysicSetCollisionMask)(GdInt id, GdInt mask);
typedef GdInt (*GDExtensionSpxPhysicGetCollisionMask)(GdInt id);

typedef GdInt (*GDExtensionSpxPhysicGetColliderType)(GdInt id);
typedef void (*GDExtensionSpxPhysicAddColliderRect)(GdInt id, GdVec2 center, GdVec2 size);
typedef void (*GDExtensionSpxPhysicAddColliderCircle)(GdInt id, GdVec2 center, GdFloat radius);
typedef void (*GDExtensionSpxPhysicAddColliderCapsule)(GdInt id, GdVec2 center, GdVec2 size);

typedef void (*GDExtensionSpxPhysicSetTrigger)(GdInt id, GdBool trigger);
typedef GdBool (*GDExtensionSpxPhysicIsTrigger)(GdInt id);

typedef void (*GDExtensionSpxPhysicSetCollisionEnabled)(GdInt id, GdBool enabled);
typedef GdBool (*GDExtensionSpxPhysicIsCollisionEnabled)(GdInt id);


// sprite
typedef GdInt (*GDExtensionSpxSpriteCreateSprite)(GdString path);
typedef GdInt (*GDExtensionSpxSpriteCloneSprite)(GdInt id);
typedef GdBool (*GDExtensionSpxSpriteDestroySprite)(GdInt id);
typedef GdBool (*GDExtensionSpxSpriteIsSpriteAlive)(GdInt id);

typedef void (*GDExtensionSpxSpriteSetPosition)(GdInt id, GdVec2 pos);
typedef void (*GDExtensionSpxSpriteSetRotation)(GdInt id, GdVec2 rot);
typedef void (*GDExtensionSpxSpriteSetScale)(GdInt id, GdVec2 scale);

typedef GdVec2 (*GDExtensionSpxSpriteGetPosition)(GdInt id);
typedef GdVec2 (*GDExtensionSpxSpriteGetRotation)(GdInt id);
typedef GdVec2 (*GDExtensionSpxSpriteGetScale)(GdInt id);

typedef void (*GDExtensionSpxSpriteSetColor)(GdInt id, GdColor GdColor);
typedef GdColor (*GDExtensionSpxSpriteGetColor)(GdInt id);

typedef void (*GDExtensionSpxSpriteUpdateTexture)(GdInt id, GdString path);
typedef GdString (*GDExtensionSpxSpriteGetTexture)(GdInt id);

typedef void (*GDExtensionSpxSpriteSetVisible)(GdInt id, GdBool visible);
typedef GdBool (*GDExtensionSpxSpriteGetVisible)(GdInt id);
typedef void (*GDExtensionSpxSpriteUpdateZIndex)(GdInt id, GdInt z);


// ui
typedef GdInt (*GDExtensionSpxUICreateButton)(GdString path, GdRect rect, GdString text);
typedef GdInt (*GDExtensionSpxUICreateLabel)(GdString path, GdRect rect, GdString text);
typedef GdInt (*GDExtensionSpxUICreateImage)(GdString path, GdRect rect, GdColor GdColor);
typedef GdInt (*GDExtensionSpxUICreateSlider)(GdString path, GdRect rect, GdFloat value);
typedef GdInt (*GDExtensionSpxUICreateToggle)(GdString path, GdRect rect, GdBool value);
typedef GdInt (*GDExtensionSpxUICreateInput)(GdString path, GdRect rect, GdString text);

typedef GdInt (*GDExtensionSpxUIGetType)(GdInt id);
typedef void (*GDExtensionSpxUISetInteractable)(GdInt id, GdBool interactable);
typedef GdBool (*GDExtensionSpxUIGetInteractable)(GdInt id);

typedef void (*GDExtensionSpxUISetText)(GdInt id, GdString text);
typedef GdString (*GDExtensionSpxUIGetText)(GdInt id);

typedef void (*GDExtensionSpxUISetRect)(GdInt id, GdRect rect);
typedef GdRect (*GDExtensionSpxUIGetRect)(GdInt id);

typedef void (*GDExtensionSpxUISetColor)(GdInt id, GdColor GdColor);
typedef GdColor (*GDExtensionSpxUIGetColor)(GdInt id);

typedef void (*GDExtensionSpxUISetFontSize)(GdInt id, GdFloat size);
typedef GdFloat (*GDExtensionSpxUIGetFontSize)(GdInt id);

typedef void (*GDExtensionSpxUISetVisible)(GdInt id, GdBool visible);
typedef GdBool (*GDExtensionSpxUIGetVisible)(GdInt id);




// callback
typedef void (*GDExtensionSpxCallbackOnEngineStart) ();
typedef void (*GDExtensionSpxCallbackOnEngineUpdate) (GdFloat delta);
typedef void (*GDExtensionSpxCallbackOnEngineDestroy) ();

typedef void (*GDExtensionSpxCallbackOnSpriteReady) (GdInt id);
typedef void (*GDExtensionSpxCallbackOnSpriteUpdated) (GdInt id);
typedef void (*GDExtensionSpxCallbackOnSpriteDestroyed) (GdInt id);

typedef void (*GDExtensionSpxCallbackOnMousePressed) (GdInt keyid);
typedef void (*GDExtensionSpxCallbackOnMouseReleased) (GdInt keyid);
typedef void (*GDExtensionSpxCallbackOnKeyPressed) (GdInt keyid);
typedef void (*GDExtensionSpxCallbackOnKeyReleased) (GdInt keyid);
typedef void (*GDExtensionSpxCallbackOnActionPressed) (GdString action_name);
typedef void (*GDExtensionSpxCallbackOnActionJustPressed) (GdString action_name);
typedef void (*GDExtensionSpxCallbackOnActionJustReleased) (GdString action_name);
typedef void (*GDExtensionSpxCallbackOnAxisChanged) (GdString action_name, GdFloat value);

typedef void (*GDExtensionSpxCallbackOnCollisionEnter) (GdInt self_id, GdInt other_id);
typedef void (*GDExtensionSpxCallbackOnCollisionStay) (GdInt self_id, GdInt other_id);
typedef void (*GDExtensionSpxCallbackOnCollisionExit) (GdInt self_id, GdInt other_id);
typedef void (*GDExtensionSpxCallbackOnTriggerEnter) (GdInt self_id, GdInt other_id);
typedef void (*GDExtensionSpxCallbackOnTriggerStay) (GdInt self_id, GdInt other_id);
typedef void (*GDExtensionSpxCallbackOnTriggerExit) (GdInt self_id, GdInt other_id);

typedef void (*GDExtensionSpxCallbackOnUIPressed) (GdInt id);
typedef void (*GDExtensionSpxCallbackOnUIReleased) (GdInt id);
typedef void (*GDExtensionSpxCallbackOnUIHovered) (GdInt id);
typedef void (*GDExtensionSpxCallbackOnUIClicked) (GdInt id);
typedef void (*GDExtensionSpxCallbackOnUIToggle) (GdInt id, GdBool is_on);
typedef void (*GDExtensionSpxCallbackOnUITextChanged) (GdInt id, GdString text);

// string
typedef void (*GDExtensionSpxStringNewWithLatin1Chars)(GDExtensionUninitializedStringPtr r_dest, const char *p_contents);
typedef void (*GDExtensionSpxStringNewWithUtf8Chars)(GDExtensionUninitializedStringPtr r_dest, const char *p_contents);
typedef void (*GDExtensionSpxStringNewWithLatin1CharsAndLen)(GDExtensionUninitializedStringPtr r_dest, const char *p_contents, GdInt p_size);
typedef void (*GDExtensionSpxStringNewWithUtf8CharsAndLen)(GDExtensionUninitializedStringPtr r_dest, const char *p_contents, GdInt p_size);
typedef GdInt (*GDExtensionSpxStringToLatin1Chars)(GDExtensionConstStringPtr p_self, char *r_text, GdInt p_max_write_length);
typedef GdInt (*GDExtensionSpxStringToUtf8Chars)(GDExtensionConstStringPtr p_self, char *r_text, GdInt p_max_write_length);
// variant
typedef GDExtensionPtrConstructor (*GDExtensionSpxVariantGetPtrConstructor)(GDExtensionVariantType p_type, int32_t p_constructor);
typedef GDExtensionPtrDestructor (*GDExtensionSpxVariantGetPtrDestructor)(GDExtensionVariantType p_type);

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
