const string TUTORIAL_CARLOCK_VERSION = "1.0";

const string TUTORIAL_CARLOCK_CONFIG_ROOT = "$profile:TutorialCarLock\\";
const string TUTORIAL_CARLOCK_CONFIG_FILENAME = TUTORIAL_CARLOCK_CONFIG_ROOT + "TutorialCarLockSettings.json";
const string TUTORIAL_CARLOCK_DB_DIR = TUTORIAL_CARLOCK_CONFIG_ROOT + "Database\\";
const string TUTORIAL_CARLOCK_DB_FILENAME = TUTORIAL_CARLOCK_DB_DIR + "%1.json";

const int TUTORIAL_CL_UNDEFINED = -1;

enum ETutorialCarLockSounds
{
    NO_SOUND = 0,
    LOCK,
    UNLOCK,
    ALARM
}