modded class ActionDetach
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if(target && target.GetObject().IsInherited(CarWheel)){
            CarScript car = CarScript.Cast(target.GetParent());
            if(car && car.IsCarLocked())
                return false;
        }

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionGetInTransport: ActionBase
{
    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
        CarScript car;
        if (Class.CastTo(car, target.GetObject()) && car.IsCarLocked())
			return false;

        return super.ActionCondition(player, target, item);
	}
};

modded class ActionCarDoors: ActionInteractBase
{
    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
        CarScript car;
        if (Class.CastTo(car, target.GetParent()) && car.IsCarLocked())
			return false;

		return super.ActionCondition(player, target, item);
	}
}

modded class ActionCarDoorsOutside: ActionInteractBase
{
    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
       CarScript car;
        if (Class.CastTo(car, target.GetParent()) && car.IsCarLocked())
			return false;

		return super.ActionCondition(player, target, item);
	}
}