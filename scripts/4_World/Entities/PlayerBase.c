modded class PlayerBase extends ManBase{

    private ref TutorialCarLockPlayerDb _carLockPlayerDb;
    
    override void Init()
    {
        super.Init();

        _carLockPlayerDb = new TutorialCarLockPlayerDb();
    }

    void SetTutorialCarLockPlayerDb(TutorialCarLockPlayerDb tutorialCarLockPlayerDb)
    {
        _carLockPlayerDb = tutorialCarLockPlayerDb;
    }

    TutorialCarLockPlayerDb GetCarLockDb()
    {
        return _carLockPlayerDb;
    }

    bool CanInteractWithCarLock(CarScript car)
    {
        if(!car.HasCarLock())
            return false;

        if(_carLockPlayerDb.IsAdmin())
            return true;

        if(_carLockPlayerDb.HasPassword(car.GetCarLockPassword()))
            return true;

        return false;
    }

    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionCarLock);
        AddAction(ActionCarUnlock);
        AddAction(ActionCarLockMenu);
    }
}