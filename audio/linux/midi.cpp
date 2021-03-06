// MIDI stuff follows.

#ifdef HAVE_CONFIG_H
#include <conf.h>
#endif

#if defined (_WIN32) || defined (USE_SDL_MIXER)

#include <stdio.h>

#include "descent.h"
#include "digi.h"
#include "cfile.h"
#include "error.h"
#include "hmpfile.h"

#if USE_SDL_MIXER
#	if defined (__APPLE__) && defined (__MACH__) && defined (USE_MAC_FRAMEWORKS)
#		include <SDL/SDL_mixer.h>
#	else
#		include <SDL_mixer.h>
#	endif

Mix_Music *mixMusic = NULL;
#endif

hmp_file *hmp = NULL;

int32_t midiVolume = 255;

//------------------------------------------------------------------------------

int32_t CMidi::SetVolume (int32_t nVolume)
{
if (nVolume < 0)
	midiVolume = 0;
else if (nVolume > 127)
	midiVolume = 127;
else
	midiVolume = nVolume;

#if USE_SDL_MIXER
if (gameOpts->sound.bUseSDLMixer)
	Mix_VolumeMusic (midiVolume);
#endif
#if defined (_WIN32)
#	if USE_SDL_MIXER
else 
#	endif
if (hmp) {
	int32_t mmVolume;

	// scale up from 0-127 to 0-0xffff
	mmVolume = (midiVolume << 1) | (midiVolume & 1);
	mmVolume |= (mmVolume << 8);
	nVolume = midiOutSetVolume((HMIDIOUT)hmp->hmidi, mmVolume | (mmVolume << 16));
	}
#endif
return midiVolume;
}

//------------------------------------------------------------------------------

void DigiStopCurrentSong ()
{
	int32_t h;

if (songManager.Playing ()) {
	DigiFadeoutMusic ();
	h = midiVolume;	// preserve it for another song being started
#if USE_SDL_MIXER
	if (!gameOpts->sound.bUseSDLMixer)
#endif
	DigiSetMidiVolume(0);
	midiVolume = h;
#if defined (_WIN32)
#	if USE_SDL_MIXER
if (!gameOpts->sound.bUseSDLMixer)
#	endif
		{
		hmp_close (hmp);
		hmp = NULL;
		gameData.songs.bPlaying = 0;
		}
#endif
	}
}

//------------------------------------------------------------------------------

int32_t DigiPlayMidiSong (const char *pszSong, char *melodic_bank, char *drum_bank, int32_t loop, int32_t bD1Song)
{
	int32_t	bCustom;
#if 0
if (!gameStates.sound.digi.bInitialized)
	return 0;
#endif
PrintLog ("DigiPlayMidiSong (%s)\n", pszSong);
DigiStopCurrentSong ();
if (!(pszSong && *pszSong))
	return 0;
if (midiVolume < 1)
	return 0;
bCustom = ((strstr (pszSong, ".ogg") != NULL) || strstr (pszSong, ".flac"));
if (!(bCustom || (hmp = hmp_open (pszSong, bD1Song))))
	return 0;
#if USE_SDL_MIXER
if (gameOpts->sound.bUseSDLMixer) {
	char	fnSong [FILENAME_LEN], *pfnSong;

	if (bCustom)
		pfnSong = pszSong;
	else {
		sprintf (fnSong, "%s/d2x-temp.mid", gameFolders.szHomeDir);
		if (!hmp_to_midi (hmp, fnSong)) {
			PrintLog ("SDL_mixer failed to load %s\n(%s)\n", fnSong, Mix_GetError ());
			return 0;
			}
		pfnSong = fnSong;
		}
	if (!(mixMusic = Mix_LoadMUS (pfnSong))) {
		PrintLog ("SDL_mixer failed to load %s\n(%s)\n", fnSong, Mix_GetError ());
		return 0;
		}
	if (-1 == Mix_PlayMusic (mixMusic, loop ? -1 : 1)) {
		PrintLog ("SDL_mixer cannot play %s\n(%s)\n", pszSong, Mix_GetError ());
		return 0;
		}
	PrintLog ("SDL_mixer playing %s\n", pszSong);
	gameData.songs.bPlaying = 1;
	DigiSetMidiVolume (midiVolume);
	return 1;
	}
#endif
#if defined (_WIN32)
if (bCustom) {
	PrintLog ("Cannot play %s - enable SDL_mixer\n", pszSong);
	return 0;
	}
hmp_play (hmp, loop);
gameData.songs.bPlaying = 1;
DigiSetMidiVolume (midiVolume);
#endif
return 1;
}

//------------------------------------------------------------------------------

int32_t sound_paused = 0;

void DigiPauseMidi()
{
#if 0
	if (!gameStates.sound.digi.bInitialized)
		return;
#endif

if (sound_paused == 0) {
#if USE_SDL_MIXER
	if (gameOpts->sound.bUseSDLMixer)
		Mix_PauseMusic ();
#endif
	}
sound_paused++;
}

//------------------------------------------------------------------------------

void DigiResumeMidi()
{
#if 0
	if (!gameStates.sound.digi.bInitialized)
		return;
#endif
	Assert(sound_paused > 0);
if (sound_paused == 1) {
#if USE_SDL_MIXER
	if (gameOpts->sound.bUseSDLMixer)
		Mix_ResumeMusic ();
#endif
	}
sound_paused--;
}

//------------------------------------------------------------------------------

#endif //defined (_WIN32) || USE_SDL_MIXER
