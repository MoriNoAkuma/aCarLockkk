class ActionDetachCarLockCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(15); //define the action time here in seconds
    }
}

class ActionDetachCarLock: ActionContinuousBase
{
    void ActionDetachCarLock()
    {
        m_CallbackClass = ActionDetachCarLockCB;
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
        return "Detach CarLock";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if(GetGame().IsServer())
            return true;

        if(!CF_String.EqualsIgnoreCase(ItemBase.GetTutorialCarLockToolClassName(), item.GetType()))
            return false;

        CarScript car = CarScript.Cast(target.GetObject());
        if(car && !car.HasPlayerInside() && car.HasCarLock())
            return true;

        return false;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
        if(car && action_data.m_MainItem && action_data.m_MainItem.GetHierarchyRootPlayer() == action_data.m_Player)
        {
            //delete the tool = one time use
            action_data.m_MainItem.Delete();
            car.ResetCarLockData();

            ItemBase carLock = ItemBase.Cast(GetGame().CreateObjectEx(ItemBase.GetTutorialCarLockClassName(), action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE));
        }
    }
}