//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
// -------------------------------------------------------------------
//
// Info: Wwise 3D coordinates is left-handed with Y as Up.
//
//////////////////////////////////////////////////////////////////////

#ifndef WWISESHARP_WWISESHARPENGINE_HPP
#define WWISESHARP_WWISESHARPENGINE_HPP

#include "WwiseEngine.hpp"

#include "WwiseSharpGameObject.hpp"

#include "WwiseSharpListenerPosition.hpp"

namespace WwiseSharp
{
	public ref class WwiseSharpEngine
	{
	protected:
		WwiseWrapper::WwiseEngine* akengine;

	public:
		WwiseSharpEngine();
		virtual ~WwiseSharpEngine();

		bool Init();
		void SetBasePath(System::String^ basePath);
		void SetLangSpecificDirName(System::String^ langSpecificDirName);
		void LoadBank(System::String^ bankName);
		WwiseSharpGameObject^ RegisterGameObject(unsigned int objectId, System::String^ gameObjectLabel);

		void SetState(unsigned long stateGroupId, unsigned long stateId);
		void SetState(System::String^ stateGroup, System::String^ state);

		void SetListenerPosition(WwiseSharpListenerPosition^ listenerPosition);
		void SetListenerPosition(int listenerIndex, WwiseSharpListenerPosition^ listenerPosition);


		void Update();


		void UnregisterGameObject(WwiseSharpGameObject^ wgo);
		void UnregisterAllGameObject();
		void Term();
	};
}

#endif