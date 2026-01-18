class TutorialCarLockPlayerDb
{
    private bool _isAdmin;
    private string _steamId;
    private string _playerName;
    private ref set<int> _passwords;
    void TutorialCarLockPlayerDb()
    {
        _passwords = new set<int>();
    }

    string GetSteamId()
    {
        return _steamId;
    }

    int GetOwnerId()
    {
        return _steamId.Substring(8,9).ToInt();
    }

    bool IsOwnerId(int id)
    {
        return GetOwnerId() == id;
    }

    bool HasPassword(int password)
    {
        return _passwords.Find(password) != -1;
    }

    bool IsAdmin()
    {
        return _isAdmin;
    }

    void SetAdmin(bool state)
    {
        _isAdmin = state;
        Save();
    }

    void AddPasswordToDb(int password)
    {
        if(!HasPassword(password)){
            _passwords.Insert(password);
            Save();
        }
    }

    static void MakeDirectoriesIfNotExist()
    {
        if( !FileExist(TUTORIAL_CARLOCK_DB_DIR))
        {
            MakeDirectory(TUTORIAL_CARLOCK_DB_DIR);
        }
    }

    void Initialize(string steamId, string playerName)
    {
        _steamId = steamId;
        _playerName = playerName;
        Save();
    }

    void Save()
    {
        JsonFileLoader<TutorialCarLockPlayerDb>.JsonSaveFile(string.Format(TUTORIAL_CARLOCK_DB_FILENAME, _steamId), this);
    }

    static ref TutorialCarLockPlayerDb Load(string steamId, string playerName)
    {
        TutorialCarLockPlayerDb carLockPlayerDb = new TutorialCarLockPlayerDb();

        MakeDirectoriesIfNotExist();

        string path = string.Format(TUTORIAL_CARLOCK_DB_FILENAME, steamId);
        if(FileExist(path))
        {
            JsonFileLoader<TutorialCarLockPlayerDb>.JsonLoadFile(path, carLockPlayerDb);
            return carLockPlayerDb;
        }

        carLockPlayerDb.Initialize(steamId, playerName);
        return carLockPlayerDb;
    }
}