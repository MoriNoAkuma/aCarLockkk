class ActionCarLockMenu: ActionInteractBase
{
    void ActionCarLockMenu()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_HUDCursorIcon = CursorIcons.CloseHood;
    }

    override void CreateConditionComponents()
    {
        m_ConditionTarget = new CCTObject(7);
        m_ConditionItem = new CCINone;
    }

    override string GetText()
    {
        return "Open CarLock Menu";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if(GetGame().IsServer())
            return true; // Server muss diese Bedingung nicht auswerten

        CarScript car = CarScript.Cast(target.GetObject());

        // Menu nur anzeigen, wenn das Auto ein CarLock attached hat
        if (car && car.HasCarLock() && !player.CanInteractWithCarLock(car))
        {
			return true;
        }

        return false;
    }

    override void OnStartClient(ActionData action_data)
    {
        CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
        if (car)
            OpenCarLockMenu(car);
    }

    void OpenCarLockMenu(CarScript car)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player && g_Game.GetUIManager().GetMenu() == null)
        {
            TutorialCarLockModule module = TutorialCarLockModule.Cast(CF_ModuleCoreManager.Get(TutorialCarLockModule));
            if (!module)
                return;

            module.iCarLockMenu = new TutorialCarLockMenu(car);
            module.iCarLockMenu.Init();
            GetGame().GetUIManager().ShowScriptedMenu(module.iCarLockMenu, null);
        }
    }
}
