#pragma once
#include "gdextension_spx_ext.h"
#include "spx_base_mgr.h"


class SpxAudio : SpxBaseMgr {
public:
	void play_audio(GdString path);
	void set_audio_volume(GdFloat volume);
	GdFloat get_audio_volume();
	GdBool is_music_playing();
	void play_music(GdString path);
	void set_music_volume(GdFloat volume);
	GdFloat get_music_volume();
	void pause_music();
	void resume_music();
	GdFloat get_music_timer();
	void set_music_timer(GdFloat time);
};
