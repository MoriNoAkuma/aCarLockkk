modded class MissionGameplay extends MissionBase
{
	const int CARLOCK_DISTANCE_RADIUS = 20;

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if(!GetUIManager().GetMenu() && GetUApi().GetInputByName("UACarLockToggle").LocalPress())
		{
			TryToLockCarNearby();
		}
	}

	void TryToLockCarNearby()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		array<Object> objs = new array<Object>();

		GetGame().GetObjectsAtPosition3D(player.GetPosition(), CARLOCK_DISTANCE_RADIUS, objs, null);
		if(objs.Count() < 1)
			return;

		foreach(Object obj: objs)
		{
			CarScript car = CarScript.Cast(obj);
			if(car && car.HasCarLock() && player.CanInteractWithCarLock(car))
				GetRPCManager().SendRPC("TutorialCarLock", "GetCarLockLockingRequest", new Param3<CarScript, int, bool>(car, car.GetCarLockPassword(), !car.IsCarLocked()), true);
		}
	}
};
