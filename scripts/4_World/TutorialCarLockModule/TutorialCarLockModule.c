[CF_RegisterModule(TutorialCarLockModule)]
class TutorialCarLockModule: CF_ModuleWorld
{
    ref TutorialCarLockSettings settings;

    ref TutorialCarLockMenu iCarLockMenu;

    override void OnInit()
    {
        super.OnInit();
        EnableInvokeConnect();
        EnableMissionStart();
    }

    void InitializeModule()
    {
        if(GetGame().IsServer())
        {
            settings = TutorialCarLockSettings.Load();
            SetCarLockClassNames();
        }
        else
        {
            settings = new TutorialCarLockSettings();
        }

        InitRPCs();
    }

    void SetCarLockClassNames()
    {
        ItemBase.SetTutorialCarLockClassNames(settings.carLockClassName, settings.carLockDetachClassName, settings.lockPickClassName);
    }

    void InitRPCs()
    {
        if(GetGame().IsServer())
        {
            GetRPCManager().AddRPC("TutorialCarLock", "GetCarLockLockingRequest", this, SingeplayerExecutionType.Server);
        }
        else
        {
            GetRPCManager().AddRPC("TutorialCarLock", "GetCarLockConfigResponse", this, SingeplayerExecutionType.Client);
            GetRPCManager().AddRPC("TutorialCarLock", "GetCarLockPlayerDbResponse", this, SingeplayerExecutionType.Client);
        }
    }

    void OnPlayerConnect(PlayerBase player, PlayerIdentity identity)
    {
        TutorialCarLockPlayerDb playerDb = TutorialCarLockPlayerDb.Load(identity.GetPlainId(), identity.GetName());
        playerDb.SetAdmin(IsCarLockAdmin(playerDb.GetOwnerId()));
        player.SetTutorialCarLockPlayerDb(playerDb);

        GetRPCManager().SendRPC("TutorialCarLock", "GetCarLockConfigResponse", new Param1<TutorialCarLockSettings>(settings), true, identity);
        GetRPCManager().SendRPC("TutorialCarLock", "GetCarLockPlayerDbResponse", new Param1<TutorialCarLockPlayerDb>(playerDb), true, identity);
    }

    bool IsCarLockAdmin(int id)
    {
        foreach(string steamId: settings.admins)
        {
            if(steamId.Contains(id.ToString()))
                return true;
        }
        return false;
    }

    // RPCs methods
    void GetCarLockLockingRequest(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type != CallType.Server)
            return;

        Param3<CarScript, int, bool> data;
        if(!ctx.Read(data))
            return;

        PlayerBase player = GetPlayerByIdentity(sender);
        if(!player)
            return;

        CarScript car = data.param1;
        if(!car)
            return;

        int password = data.param2;
        bool lock = data.param3;

        // KORREKTUR: Füge das Passwort IMMER zur PlayerDb hinzu
        if(!car.HasCarLockPassword())
        {
            // Erstes Mal - Setze das Passwort im Auto UND in der PlayerDb
            car.InitializeCarLockData(player.GetCarLockDb().GetOwnerId(), password);
            player.GetCarLockDb().AddPasswordToDb(password);  // DIESE ZEILE WAR DER FEHLER - SIE FEHLTE!
        }
        else
        {
            // Nachfolgende Male - Lock/Unlock und füge Passwort zur Db hinzu
            car.SetCarLock(lock);
            player.GetCarLockDb().AddPasswordToDb(password);
        }

        GetRPCManager().SendRPC("TutorialCarLock", "GetCarLockPlayerDbResponse", new Param1<TutorialCarLockPlayerDb>(player.GetCarLockDb()), true, sender);
    }

    void GetCarLockConfigResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type != CallType.Client)
            return;

        Param1<TutorialCarLockSettings> data;
        if(!ctx.Read(data))
            return;

        settings = data.param1;
        SetCarLockClassNames();
    }

    void GetCarLockPlayerDbResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if(type != CallType.Client)
            return;

        Param1<TutorialCarLockPlayerDb> data;
        if(!ctx.Read(data))
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if(!player)
            return;

        player.SetTutorialCarLockPlayerDb(data.param1);
    }

    // Module methods
    override void OnInvokeConnect(Class sender, CF_EventArgs args)
    {
        super.OnInvokeConnect(sender, args);

        auto cArgs = CF_EventPlayerArgs.Cast(args);
        if (!IsMissionHost())
            return;

        if (!cArgs.Player || !cArgs.Identity)
            return;

        OnPlayerConnect(cArgs.Player, cArgs.Identity);
    }

    override void OnMissionStart(Class sender, CF_EventArgs args)
    {
        super.OnMissionStart(sender, args);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InitializeModule,1000);
    }

    static PlayerBase GetPlayerByIdentity(PlayerIdentity identity)
    {
        int low = 0;
        int high = 0;
        GetGame().GetPlayerNetworkIDByIdentityID(identity.GetPlayerId(), low, high);
        return PlayerBase.Cast(GetGame().GetObjectByNetworkId(low, high));
    }
}