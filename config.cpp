class CfgPatches {
	class TutorialCarLock_Script {
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {};
	};
};

class CfgMods {
	class TutorialCarLock {
		dir = "Test";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = ARMADA;
		credits = ARMADA;
		author = ARMADA;
		authorID = "0";
		version = 1.1;
		extra = 0;
		type = "mod";
		inputs = "aCarLock/Inputs.xml";
		dependencies[] = {"Game", "World", "Mission"};
		
		class defs {
			class gameScriptModule {
				value = "";
				files[] = {"aCarLock/scripts/Common", "aCarLock/scripts/3_Game"};
			};
			
			class worldScriptModule {
				value = "";
				files[] = {"aCarLock/scripts/Common", "aCarLock/scripts/4_World"};
			};
			
			class missionScriptModule {
				value = "";
				files[] = {"aCarLock/scripts/Common", "aCarLock/scripts/5_Mission"};
			};
		};
	};
};

class CfgSoundShaders {
	class tutorial_cl_CarLock_CarAlarm_SoundShader {
		samples[] = {{"aCarLock\datasets\sounds\carAlarm", 1}};
		radius = 500;
		range = 500;
		volume = 1;
		rangeCurve[] = {{0, 1}, {50, 1}, {500, 1}};
	};
	
	class tutorial_cl_CarLock_SoundShader {
		samples[] = {{"aCarLock\datasets\sounds\carLock", 1}};
		range = 15;
		volume = 1;
	};
	
	class tutorial_cl_CarUnlock_SoundShader {
		samples[] = {{"aCarLock\datasets\sounds\carUnlock", 1}};
		range = 15;
		volume = 1;
	};
};

class CfgSoundSets {
	class tutorial_cl_CarLock_CarAlarm_SoundSet {
		soundShaders[] = {"tutorial_cl_CarLock_CarAlarm_SoundShader"};
	};
	
	class tutorial_cl_CarLock_SoundSet {
		soundShaders[] = {"tutorial_cl_CarLock_SoundShader"};
	};
	
	class tutorial_cl_CarUnlock_SoundSet {
		soundShaders[] = {"tutorial_cl_CarUnlock_SoundShader"};
	};
};
