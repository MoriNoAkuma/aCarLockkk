modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(ActionAttachCarLock);
        actions.Insert(ActionDetachCarLock);
        actions.Insert(ActionDetachCarLockPick);

        actions.Insert(ActionCarLock);
        actions.Insert(ActionCarUnlock);
        actions.Insert(ActionCarLockMenu);
    }
}