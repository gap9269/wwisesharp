//////////////////////////////////////////////////////////////////////
//
// Author: Adrien "Lucyberad" Bock.
//
// Program.cpp could be used as a "unit test" system which launch the sample soundbank.
// To activate this, change the General property of this project from DLL to EXE.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h> //for Exit_Success constant.
#include <iostream> //for cin stream.

#include "WwiseSharpEngine.hpp";
using namespace WwiseSharp;

using namespace std;

//
// EXEC TEST
//

int main(int argc, const char* argv[])
{
	//
	// INIT
	//

	WwiseSharpEngine engine;

	engine.Init();

	System::String^ basePath = gcnew System::String(getenv("WWISESDK"));
	basePath = basePath->Replace('\\', '/');
	basePath += gcnew System::String("/samples/IntegrationDemo/WwiseProject/GeneratedSoundBanks/Windows");
	engine.SetBasePath(basePath);

	engine.SetLangSpecificDirName(gcnew System::String("English(US)"));
	engine.LoadBank(gcnew System::String("Init.bnk"));
	engine.LoadBank(gcnew System::String("MusicCallbacks.bnk"));
	
	WwiseSharpGameObject^ mainGameObject = engine.RegisterGameObject(200, "MainGameObject");
	
	mainGameObject->PostEvent(gcnew System::String("PlayMusicDemo1")); //event with name;
	//mainGameObject->PostEvent(519773714U); //event with ID;
	
	//
	// LOOP
	//

	while (System::Console::KeyAvailable == false) //waiting for a key to be pressed
	{
		engine.Update();
	}

	//
	// END
	//

	engine.UnregisterGameObject(mainGameObject); //or engine.UnregisterAllGameObject();
	engine.Term();
	
	return EXIT_SUCCESS;
}