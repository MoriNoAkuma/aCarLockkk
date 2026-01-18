class TutorialCarLockSettings
{
    string version = TUTORIAL_CARLOCK_VERSION;
    string lockPickClassName = "lockpick";
    float  lockPickChance = 0.25;
    string carLockDetachClassName = "Pliers";
    string carLockClassName = "CarLock";

    ref array<string> admins;

    void TutorialCarLockSettings()
    {
        admins = new array<string>();
    }

    static void MakeDirectoriesIfNotExist()
    {
        if( !FileExist(TUTORIAL_CARLOCK_CONFIG_ROOT))
        {
            MakeDirectory(TUTORIAL_CARLOCK_CONFIG_ROOT);
        }
    }

    void TutorialCarLockDefaultSettings()
    {
        admins.Insert("99999999999999999");
        Save();
    }

    void Save()
    {
        JsonFileLoader<TutorialCarLockSettings>.JsonSaveFile(TUTORIAL_CARLOCK_CONFIG_FILENAME, this);
    }

    static ref TutorialCarLockSettings Load()
    {
        TutorialCarLockSettings settings = new TutorialCarLockSettings();

        MakeDirectoriesIfNotExist();

        if(FileExist(TUTORIAL_CARLOCK_CONFIG_FILENAME))
        {
            JsonFileLoader<TutorialCarLockSettings>.JsonLoadFile(TUTORIAL_CARLOCK_CONFIG_FILENAME, settings);
            return settings;
        }

        settings.TutorialCarLockDefaultSettings();
        return settings;
    }
}