class ActionDetachCarLockPickCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(60); //define the action time here in seconds
    }
}

class ActionDetachCarLockPick: ActionContinuousBase
{
    void ActionDetachCarLockPick()
    {
        m_CallbackClass = ActionDetachCarLockPickCB;
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
        return "Force CarLock";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if(GetGame().IsServer())
            return true;

        if(!CF_String.EqualsIgnoreCase(ItemBase.GetTutorialCarLockPickClassName(), item.GetType()))
            return false;

        CarScript car = CarScript.Cast(target.GetObject());
        if(car && !car.HasPlayerInside() && car.HasCarLock())
            return true;

        return false;
    }

    override void OnStartServer(ActionData action_data)
    {
        CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
        if(car && action_data.m_MainItem && action_data.m_MainItem.GetHierarchyRootPlayer() == action_data.m_Player)
        {
            car.SetSoundToPlay(ETutorialCarLockSounds.ALARM);
        }
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
        if(car && action_data.m_MainItem && action_data.m_MainItem.GetHierarchyRootPlayer() == action_data.m_Player)
        {
            TutorialCarLockModule module = TutorialCarLockModule.Cast(CF_ModuleCoreManager.Get(TutorialCarLockModule));
            if(!module)
                return;

            if(Math.RandomFloatInclusive(0,1) < module.settings.lockPickChance)
            {
                GetCarLockLogger().LogInfo("CarLock Forced by thief: " + action_data.m_Player.GetIdentity().GetName() + " steamId:"  + action_data.m_Player.GetCarLockDb().GetSteamId() + " position:" + action_data.m_Player.GetPosition().ToString() + " ownerId:" + car.GetCarLockOwner() + " isLocked:" + car.IsCarLocked());
                car.ResetCarLockData();
            }

            action_data.m_MainItem.DecreaseHealthCoef(0.34);
        }
    }
}