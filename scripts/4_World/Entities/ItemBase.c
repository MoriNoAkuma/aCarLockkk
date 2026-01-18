modded class ItemBase{

    private static string _tutorialCarLockClassName;
    private static string _tutorialCarLockToolClassName;
    private static string _tutorialCarLockPickClassName;

    static string GetTutorialCarLockClassName()
    {
        return _tutorialCarLockClassName;
    }

    static string GetTutorialCarLockToolClassName()
    {
        return _tutorialCarLockToolClassName;
    }

    static string GetTutorialCarLockPickClassName()
    {
        return _tutorialCarLockPickClassName;
    }

    static void SetTutorialCarLockClassNames(string carlock, string tool, string lockpick)
    {
        _tutorialCarLockClassName = carlock;
        _tutorialCarLockToolClassName = tool;
        _tutorialCarLockPickClassName = lockpick;
    }

    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionAttachCarLock);
        AddAction(ActionDetachCarLock);
        AddAction(ActionDetachCarLockPick);
    }
}