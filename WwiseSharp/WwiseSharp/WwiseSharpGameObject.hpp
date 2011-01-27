//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
//////////////////////////////////////////////////////////////////////

#ifndef WWISESHARP_WWISESHARPGAMEOBJECT_HPP
#define WWISESHARP_WWISESHARPGAMEOBJECT_HPP

#include "WwiseEngine.hpp"

#include "WwiseSharpSoundPosition.hpp"

using namespace WwiseWrapper;

namespace WwiseSharp
{
	public ref class WwiseSharpGameObject
	{
	protected:
		WwiseEngine* engine;

		AkGameObjectID objectId;
		char* label;

	public:
		WwiseSharpGameObject(unsigned int objectId, System::String^ label, WwiseEngine* engine);
		virtual ~WwiseSharpGameObject();

		AkGameObjectID GetAkObjectId();
		char* GetLabelChar();

		void PostEvent(System::String^ eventName);
		void PostEvent(unsigned long eventId);

		//Note: SinglePosition & MultiplePosition override each other between each ProcessAudio() call on WwiseSharpEngine object.
		void SetSinglePosition(WwiseSharpSoundPosition^ position);
		void SetSinglePosition(WwiseSharpSoundPosition^ position, int listenerIndex);
		//TODO: void SetMultiplePosition(WwiseSharpSoundPosition[]^ position, bool isMultiSource); //isMultiSource == add each source volume, false means that the object is multiple directions (refer to the documentation: Integration Details - 3D Positions)
		//TODO: void SetMultiplePosition(WwiseSharpSoundPosition[]^ position, bool isMultiSource, int listener);

		void SetRTPCValue(unsigned long rtpcId, float value);
		void SetRTPCValue(System::String^ rtpcName, float value);

		void SetSwitch(unsigned long switchGroupId, unsigned long switchId);
		void SetSwitch(System::String^ switchGroupName, System::String^ switchName);
	};
}

#endif