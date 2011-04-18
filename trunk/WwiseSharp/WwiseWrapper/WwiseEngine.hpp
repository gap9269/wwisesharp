//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
//////////////////////////////////////////////////////////////////////

#ifndef WWISEWRAPPER_WWISEENGINE_HPP
#define WWISEWRAPPER_WWISEENGINE_HPP

#include <stdlib.h> //for malloc & free
#include <windows.h> //for DWORD & others.
#include <exception>

#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include "AkFilePackageLowLevelIOBlocking.h"					// Sample low-level I/O implementation (in $(WWISESDK)/samples/SoundEngine/Win32).
#include <AK/SoundEngine/Common/AkSoundEngine.h>                // Sound engine
#include <AK/MusicEngine/Common/AkMusicEngine.h>                // Music Engine
#ifdef WWISESHARP_VORBIS
	#include <AK/Plugin/AkVorbisFactory.h>						// Vorbis Plugin.
#endif
#ifdef DEBUG
	#include <AK/Comm/AkCommunication.h>						// Communication (is the component which allow Wwise editor to track Game Instance).
#endif

namespace WwiseWrapper
{
	class WwiseEngine
	{
		// We're using the default Low-Level I/O implementation that's part
		// of the SDK's sample code, with the file package extension.
		CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	public:
		bool Init();
		void ProcessAudio();
		void Term();

		void SetBasePath(const wchar_t* basePath);
		void SetLangSpecificDirName(const wchar_t* specificDirName);
		void LoadBank(const wchar_t* bankName);
		void RegisterGameObject(AkGameObjectID akId, const char* gameObjectLabel);
		void UnregisterGameObject(AkGameObjectID akId);
		void UnregisterAllGameObject();

		void PostEvent(const wchar_t* eventName, AkGameObjectID gameObject);
		void PostEvent(AkUniqueID eventId, AkGameObjectID gameObject);

		void SetRTPCValue(const wchar_t* rtpcName, AkRtpcValue value, AkGameObjectID  gameObjectId);
		void SetRTPCValue(AkRtpcID rtpcId, AkRtpcValue value, AkGameObjectID  gameObjectId);

		void SetState(const wchar_t* stateGroup, const wchar_t* state);
		void SetState(AkStateGroupID stateGroupId, AkStateID stateId);

		void SetSwitch(const wchar_t* switchGroupName, const wchar_t* switchName, AkGameObjectID gameObjectId);
		void SetSwitch(AkStateGroupID switchGroupId, AkStateID switchId, AkGameObjectID gameObjectId);

		void SetListenerPosition(AkListenerPosition& akPosition);
		void SetListenerPosition(int listenerIndex, AkListenerPosition& akPosition);

		void SetPosition(AkGameObjectID gameObjectId, AkSoundPosition& position);
		void SetPosition(AkGameObjectID gameObjectId, AkSoundPosition& position, int listenerIndex);

		void SetActiveListeners(AkGameObjectID gameObjectId, AkUInt32 listenerMask);
	};
}

#endif