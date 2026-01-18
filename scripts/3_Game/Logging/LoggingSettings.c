class CarLockLoggingSettings 
{
    int logLevel = 0;
    int refreshRateInSeconds = 60;
    string webhookLink = string.Empty;

    void MakeDirectoryIfNotExists()
    {
        if(!FileExist(CARLOCK_LOGGER_ROOT))
            MakeDirectory(CARLOCK_LOGGER_ROOT);

        if(!FileExist(CARLOCK_LOGGER_CONFIG_DIR))
            MakeDirectory(CARLOCK_LOGGER_CONFIG_DIR);
    }

    void Save()
    {
        JsonFileLoader<CarLockLoggingSettings>.JsonSaveFile(CARLOCK_LOGGER_CONFIG_FILE, this);
    }

    static ref CarLockLoggingSettings Load()
    {
        CarLockLoggingSettings settings = new CarLockLoggingSettings();

        settings.MakeDirectoryIfNotExists();

        if(FileExist(CARLOCK_LOGGER_CONFIG_FILE))
        {
            JsonFileLoader<CarLockLoggingSettings>.JsonLoadFile(CARLOCK_LOGGER_CONFIG_FILE, settings);
            return settings;
        }

        settings.Save();
        return settings;
    }
}