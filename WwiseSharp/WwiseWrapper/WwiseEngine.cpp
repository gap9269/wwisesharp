//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
//////////////////////////////////////////////////////////////////////

#include "WwiseEngine.hpp"
using namespace WwiseWrapper;

// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
#ifdef WIN32
	void * AllocHook( size_t in_size )
	{
		return malloc( in_size );
	}
	void FreeHook( void * in_ptr )
	{
		free( in_ptr );
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
		)
	{
		return VirtualAlloc( in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect );
	}
	void VirtualFreeHook( 
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
		)
	{
		VirtualFree( in_pMemAddress, in_size, in_dwFreeType );
	}
#endif
}

bool WwiseEngine::Init()
{
	//
	// Create and initialize an instance of the default memory manager. Note
	// that you can override the default memory manager with your own. Refer
	// to the SDK documentation for more information.
	//

	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if ( AK::MemoryMgr::Init( &memSettings ) != AK_Success )
	{
		throw std::exception( "Could not create the memory manager." );
		return false;
	}

	//
	// Create and initialize an instance of the default streaming manager. Note
	// that you can override the default streaming manager with your own. Refer
	// to the SDK documentation for more information.
	//

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings( stmSettings );
	
	// Customize the Stream Manager settings here.
	
	if ( !AK::StreamMgr::Create( stmSettings ) )
	{
		throw std::exception( "Could not create the Streaming Manager" );
		return false;
	}
	
	//
	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. Refer
	// to the SDK documentation for more information.        
	//
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );
	
	// Customize the streaming device settings here.
	
	// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
	// in the Stream Manager, and registers itself as the File Location Resolver.
	if ( g_lowLevelIO.Init( deviceSettings ) != AK_Success )
	{
		throw std::exception( "Could not create the streaming device and Low-Level I/O system" );
		return false;
	}

	//
	// Create the Sound Engine
	// Using default initialization parameters
	//
	
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings( initSettings );
	AK::SoundEngine::GetDefaultPlatformInitSettings( platformInitSettings );

	if ( AK::SoundEngine::Init( &initSettings, &platformInitSettings ) != AK_Success )
	{
		assert( ! "Could not initialize the Sound Engine." );
		return false;
	}

	//
	// Initialize the music engine
	// Using default initialization parameters
	//
	
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings( musicInit );
		
	if ( AK::MusicEngine::Init( &musicInit ) != AK_Success )
	{
		assert( ! "Could not initialize the Music Engine." );
		return false;
	}

#ifdef WWISESHARP_VORBIS
	AK::SoundEngine::RegisterCodec( AKCOMPANYID_AUDIOKINETIC, 
									AKCODECID_VORBIS, 
									CreateVorbisFilePlugin, 
									CreateVorbisBankPlugin );
#endif

#ifdef DEBUG
	//
	// Initialize communications (not in release build!)
	//
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings( commSettings );
	if ( AK::Comm::Init( commSettings ) != AK_Success )
	{
		assert( ! "Could not initialize communication." );
		return false;
	}
#endif

	return false;
}

void WwiseEngine::ProcessAudio()
{
	// Process bank requests, events, positions, RTPC, etc.
	AK::SoundEngine::RenderAudio();
}

void WwiseEngine::Term()
{
#ifdef DEBUG
	//
	// Terminate Communication Services
	//
	AK::Comm::Term();
#endif

	//
	// Terminate the music engine
	//
	
	AK::MusicEngine::Term();

	//
	// Terminate the sound engine
	//
	
	AK::SoundEngine::Term();

	// Terminate the streaming device and streaming manager
	
	// CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
	// that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	g_lowLevelIO.Term();
	
	if ( AK::IAkStreamMgr::Get() )
		AK::IAkStreamMgr::Get()->Destroy();

	// Terminate the Memory Manager
	AK::MemoryMgr::Term();
}

void WwiseEngine::SetBasePath(const wchar_t* basePath)
{
	g_lowLevelIO.SetBasePath(basePath);
}

void WwiseEngine::SetLangSpecificDirName(const wchar_t* specificDirName)
{
	g_lowLevelIO.SetLangSpecificDirName(specificDirName);
}

void WwiseEngine::LoadBank(const wchar_t *bankName)
{
	AkBankID bankID; // Not used. These banks can be unloaded with their file name.
	if (AK::SoundEngine::LoadBank( bankName, AK_DEFAULT_POOL_ID, bankID ) != AK_Success)
		throw std::exception("Failed at loading soundbank.");
}

void WwiseEngine::RegisterGameObject(AkGameObjectID akId, const char* gameObjectLabel)
{
	AK::SoundEngine::RegisterGameObj(akId, gameObjectLabel);
}

void WwiseEngine::UnregisterGameObject(AkGameObjectID akId)
{
	AK::SoundEngine::UnregisterGameObj(akId);
}

void WwiseEngine::UnregisterAllGameObject()
{
	AK::SoundEngine::UnregisterAllGameObj();
}

void WwiseEngine::PostEvent(const wchar_t* eventName, AkGameObjectID gameObject)
{
	AK::SoundEngine::PostEvent(eventName, gameObject);
}
void WwiseEngine::PostEvent(AkUniqueID eventId, AkGameObjectID gameObject)
{
	AK::SoundEngine::PostEvent(eventId, gameObject);
}

void WwiseEngine::SetRTPCValue(const wchar_t* rtpcName, AkRtpcValue value, AkGameObjectID gameObjectId)
{
	AK::SoundEngine::SetRTPCValue(rtpcName, value, gameObjectId);
}
void WwiseEngine::SetRTPCValue(AkRtpcID rtpcId, AkRtpcValue value, AkGameObjectID gameObjectId)
{
	AK::SoundEngine::SetRTPCValue(rtpcId, value, gameObjectId);
}

void WwiseEngine::SetState(const wchar_t* stateGroup, const wchar_t* state)
{
	AK::SoundEngine::SetState(stateGroup, state);
}
void WwiseEngine::SetState(AkStateGroupID stateGroupId, AkStateID stateId)
{
	AK::SoundEngine::SetState(stateGroupId, stateId);
}

void WwiseEngine::SetSwitch(const wchar_t* switchGroupName, const wchar_t* switchName, AkGameObjectID gameObjectId)
{
	AK::SoundEngine::SetSwitch(switchGroupName, switchName, gameObjectId);
}
void WwiseEngine::SetSwitch(AkSwitchGroupID switchGroupId, AkSwitchStateID switchId, AkGameObjectID gameObjectId)
{
	AK::SoundEngine::SetSwitch(switchGroupId, switchId, gameObjectId);
}

void WwiseEngine::SetListenerPosition(AkListenerPosition& akPosition)
{
	AK::SoundEngine::SetListenerPosition(akPosition);
}
void WwiseEngine::SetListenerPosition(int listenerIndex, AkListenerPosition& akPosition)
{
	AK::SoundEngine::SetListenerPosition(akPosition, listenerIndex);
}

void WwiseEngine::SetPosition(AkGameObjectID gameObjectId, AkSoundPosition& position)
{
	AK::SoundEngine::SetPosition(gameObjectId, position);
}
void WwiseEngine::SetPosition(AkGameObjectID gameObjectId, AkSoundPosition& position, int listenerIndex)
{
	AK::SoundEngine::SetPosition(gameObjectId, position, listenerIndex);
}

void WwiseEngine::SetActiveListeners(AkGameObjectID gameObjectId, AkUInt32 listenerMask)
{
	AK::SoundEngine::SetActiveListeners(gameObjectId, listenerMask);
}