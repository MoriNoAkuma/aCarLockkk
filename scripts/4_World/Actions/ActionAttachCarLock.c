class ActionAttachCarLockCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(15); //define the action time here in seconds
    }
}

class ActionAttachCarLock: ActionContinuousBase
{
    void ActionAttachCarLock()
    {
        m_CallbackClass = ActionAttachCarLockCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
    }

    override void CreateConditionComponents()
    {
        m_ConditionTarget = new CCTNonRuined(10);
        m_ConditionItem = new CCINonRuined;
    }

    override string GetText()
    {
        return "Attach CarLock";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if(GetGame().IsServer())
            return true;

        if(!CF_String.EqualsIgnoreCase(ItemBase.GetTutorialCarLockClassName(), item.GetType()))
            return false;

        CarScript car = CarScript.Cast(target.GetObject());
        if(car && !car.HasPlayerInside() && !car.HasCarLock())
            return true;

        return false;
    }

    override void OnFinishProgressClient(ActionData action_data)
    {
        CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
        if(car && action_data.m_MainItem && action_data.m_MainItem.GetHierarchyRootPlayer() == action_data.m_Player)
        {
            OpenCarLockMenu(car);
        }
    }

    void OpenCarLockMenu(CarScript car)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if(player && g_Game.GetUIManager().GetMenu() == null)
        {
            TutorialCarLockModule module = TutorialCarLockModule.Cast(CF_ModuleCoreManager.Get(TutorialCarLockModule));
            if(!module)
                return;

            module.iCarLockMenu = new TutorialCarLockMenu(car);
            module.iCarLockMenu.Init();
            GetGame().GetUIManager().ShowScriptedMenu(module.iCarLockMenu, null);
        }
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
        if(car && action_data.m_MainItem && action_data.m_MainItem.GetHierarchyRootPlayer() == action_data.m_Player)
        {
            action_data.m_MainItem.Delete();
            car.InitializeCarLockData(action_data.m_Player.GetCarLockDb().GetOwnerId(), TUTORIAL_CL_UNDEFINED);
        }
    }
}