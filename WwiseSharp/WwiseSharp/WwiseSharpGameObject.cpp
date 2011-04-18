//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
//////////////////////////////////////////////////////////////////////

#include "WwiseSharpGameObject.hpp"
using namespace WwiseSharp;

WwiseSharpGameObject::WwiseSharpGameObject(unsigned int wwiseObjectId, System::String^ wwiseLabel, WwiseEngine* engine)
{
	label = static_cast<char *>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(wwiseLabel).ToPointer());
	objectId = static_cast<AkGameObjectID>(wwiseObjectId);
	this->engine = engine;
}

WwiseSharpGameObject::~WwiseSharpGameObject()
{
	System::Runtime::InteropServices::Marshal::FreeHGlobal(safe_cast<System::IntPtr>(label));
}

AkGameObjectID WwiseSharpGameObject::GetAkObjectId()
{
	return objectId;
}
char* WwiseSharpGameObject::GetLabelChar()
{
	return label;
}

void WwiseSharpGameObject::SetActiveListeners(unsigned long listenerMask)
{
	engine->SetActiveListeners(objectId, listenerMask);
}

void WwiseSharpGameObject::PostEvent(System::String^ eventName)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(eventName);
	engine->PostEvent(static_cast<LPCWSTR>(static_cast<void *>(p)), objectId);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}
void WwiseSharpGameObject::PostEvent(unsigned long eventId)
{
	engine->PostEvent(static_cast<AkUniqueID>(eventId), objectId);
}

void WwiseSharpGameObject::SetSinglePosition(WwiseSharpSoundPosition^ position)
{
	AkVector akVectorPosition;
	akVectorPosition.X = position->Xposition;
	akVectorPosition.Y = position->Yposition;
	akVectorPosition.Z = position->Zposition;
	
	AkVector akVectorOrientation;
	akVectorOrientation.X = position->Xorientation;
	akVectorOrientation.Y = position->Yorientation;
	akVectorOrientation.Z = position->Zorientation;

	AkSoundPosition akSoundPosition;
	akSoundPosition.Position = akVectorPosition;
	akSoundPosition.Orientation = akVectorOrientation;

	engine->SetPosition(objectId, akSoundPosition);
}
void WwiseSharpGameObject::SetSinglePosition(WwiseSharpSoundPosition^ position, int listenerIndex)
{
	AkVector akVectorPosition;
	akVectorPosition.X = position->Xposition;
	akVectorPosition.Y = position->Yposition;
	akVectorPosition.Z = position->Zposition;
	
	AkVector akVectorOrientation;
	akVectorOrientation.X = position->Xorientation;
	akVectorOrientation.Y = position->Yorientation;
	akVectorOrientation.Z = position->Zorientation;

	AkSoundPosition akSoundPosition;
	akSoundPosition.Position = akVectorPosition;
	akSoundPosition.Orientation = akVectorOrientation;

	engine->SetPosition(objectId, akSoundPosition, listenerIndex);
}

void WwiseSharpGameObject::SetRTPCValue(unsigned long rtpcId, float value)
{
	engine->SetRTPCValue(static_cast<AkRtpcID>(rtpcId), static_cast<AkRtpcValue>(value), objectId);
}
void WwiseSharpGameObject::SetRTPCValue(System::String^ rtpcName, float value)
{
	System::IntPtr p = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(rtpcName);
	engine->SetRTPCValue(static_cast<LPCWSTR>(static_cast<void *>(p)), static_cast<AkRtpcValue>(value), objectId);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
}

void WwiseSharpGameObject::SetSwitch(unsigned long switchGroupId, unsigned long switchId)
{
	engine->SetSwitch(static_cast<AkSwitchGroupID>(switchGroupId), static_cast<AkSwitchStateID>(switchId), objectId);
}
void WwiseSharpGameObject::SetSwitch(System::String^ switchGroupName, System::String^ switchName)
{
	System::IntPtr p1 = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(switchGroupName);
	System::IntPtr p2 = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(switchName);
	engine->SetSwitch(static_cast<LPCWSTR>(static_cast<void *>(p1)), static_cast<LPCWSTR>(static_cast<void *>(p2)), objectId);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p1);
	System::Runtime::InteropServices::Marshal::FreeHGlobal(p2);
}