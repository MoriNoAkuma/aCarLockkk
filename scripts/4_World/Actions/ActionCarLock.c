class ActionCarLockCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(1); //define the action time here in seconds
    }
}

class ActionCarLock: ActionContinuousBase
{
    void ActionCarLock()
    {
        m_CallbackClass = ActionCarLockCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
    }

    override void CreateConditionComponents()
    {
        m_ConditionTarget = new CCTNonRuined(7);
        m_ConditionItem = new CCINone;
    }

    override string GetText()
    {
        return "Lock Car";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if(GetGame().IsServer())
            return true;

        CarScript car = CarScript.Cast(target.GetObject());
        if(car && player.CanInteractWithCarLock(car) && !car.IsCarLocked())
            return true;

        return false;
    }

   override void OnStartServer(ActionData action_data)
    {
        CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
        if(car)
        {
            car.SetCarLock(true);
        }
    }
}