modded class CarScript
{
    bool m_CarLock_IsLocked = false;
    int  m_CarLockOwner = TUTORIAL_CL_UNDEFINED;
    int  m_CarLockPassword = TUTORIAL_CL_UNDEFINED;

    int m_SoundToPlay = ETutorialCarLockSounds.NO_SOUND;

    protected EffectSound m_AlarmSound;
    protected EffectSound m_BipSound;

    void CarScript()
    {
        RegisterNetSyncVariableBool("m_CarLock_IsLocked");
        RegisterNetSyncVariableInt("m_CarLockOwner");
        RegisterNetSyncVariableInt("m_CarLockPassword");
        RegisterNetSyncVariableInt("m_SoundToPlay");
    }

    void CarLockLogInit()
    {
        GetCarLockLogger().LogInfo("CarLockLogInit: " + GetDisplayName() + " position:" + GetPosition().ToString() + " ownerId:" + m_CarLockOwner + " isLocked:" + m_CarLock_IsLocked);
    }

    void CarLockLogClaimed()
    {
        GetCarLockLogger().LogInfo("CarLockLogClaimed: " + GetDisplayName() + " position:" + GetPosition().ToString() + " ownerId:" + m_CarLockOwner + " isLocked:" + m_CarLock_IsLocked);
    }

    void CarLockLogUnClaimed()
    {
        GetCarLockLogger().LogInfo("CarLockLogUnClaimed: " + GetDisplayName() + " position:" + GetPosition().ToString() + " ownerId:" + m_CarLockOwner + " isLocked:" + m_CarLock_IsLocked);
    }

    override void EEDelete(EntityAI parent)
    {
        super.EEDelete(parent);
        if(GetGame().IsServer())
        {
            GetCarLockLogger().LogInfo("EEDelete: " + GetDisplayName() + " position:" + GetPosition().ToString() + " ownerId:" + m_CarLockOwner + " isLocked:" + m_CarLock_IsLocked);
        }
    }

    void SetCarLockOwner(int ownerId)
    {
        m_CarLockOwner = ownerId;
        SetSynchDirty();
    }

    int GetCarLockOwner()
    {
        return m_CarLockOwner;
    } 

    void SetCarLockPassword(int password)
    {
        m_CarLockPassword = password;
        SetSynchDirty();
    }

    int GetCarLockPassword()
    {
        return m_CarLockPassword;
    }

    bool HasCarLockPassword()
    {
        return m_CarLockPassword != TUTORIAL_CL_UNDEFINED;
    }

    bool IsCarLocked()
    {
        return m_CarLock_IsLocked;
    }

    bool HasCarLock()
    {
        return m_CarLockOwner != TUTORIAL_CL_UNDEFINED;
    }

    void SetCarLock(bool lock, int soundId = TUTORIAL_CL_UNDEFINED)
    {
        m_CarLock_IsLocked = lock;
        SynchronizeLocking();

        if(soundId != TUTORIAL_CL_UNDEFINED)
            return;

        if(m_CarLock_IsLocked){
            SetSoundToPlay(ETutorialCarLockSounds.LOCK);
        }
        else{
            SetSoundToPlay(ETutorialCarLockSounds.UNLOCK);
        }
    }

    void InitializeCarLockData(int ownerId, int password)
    {
        SetCarLockOwner(ownerId);
        SetCarLockPassword(password);
        SetCarLock(true, ETutorialCarLockSounds.NO_SOUND);
        CarLockLogClaimed();
    }

    void ResetCarLockData()
    {
        CarLockLogUnClaimed();
        SetCarLockOwner(TUTORIAL_CL_UNDEFINED);
        SetCarLockPassword(TUTORIAL_CL_UNDEFINED);
        SetCarLock(false, ETutorialCarLockSounds.NO_SOUND);
    }

    bool HasPlayerInside()
    {
        Transport transport = Transport.Cast(this);
        if(!transport)
            return false;

        int crewSize = transport.CrewSize();
        for(int i=0;i<crewSize;i++)
        {
            if(transport.CrewMember(i))
                return true;
        }

        return false;
    }

    void SynchronizeLocking()
    {
        if(!GetGame().IsServer())
            return;

        if(!GetInventory())
            return;
        
        if(m_CarLock_IsLocked)
        {
            GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
        }
        else
        {
            GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
        }

        SetSynchDirty();
    }
    
    void SetSoundToPlay(int soundId = ETutorialCarLockSounds.NO_SOUND)
    {
        m_SoundToPlay = soundId;
        SetSynchDirty();

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetSoundToPlay, 1000, ETutorialCarLockSounds.NO_SOUND);
    }
    
    override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();

        switch(m_SoundToPlay)
        {
            case ETutorialCarLockSounds.NO_SOUND:return;
            break;

            case ETutorialCarLockSounds.LOCK: PlayCarLockSound();
            break;

            case ETutorialCarLockSounds.UNLOCK: PlayCarUnlockSound();
            break;

            case ETutorialCarLockSounds.ALARM: PlayCarAlarmSound();
            break;
        }
    }

    void PlayCarLockSound()
    {
        StopAlarm();

        PlaySoundSet(m_BipSound, "tutorial_cl_CarLock_SoundSet", 0, 0);
    }

    void PlayCarUnlockSound()
    {
        StopAlarm();

        PlaySoundSet(m_BipSound, "tutorial_cl_CarUnlock_SoundSet", 0, 0);
    }

    void PlayCarAlarmSound()
    {
        StopAlarm();

        PlaySoundSetLoop(m_AlarmSound, "tutorial_cl_CarLock_CarAlarm_SoundSet", 0, 0);

        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StopAlarm, 300000, false);
    }

    void StopAlarm()
    {
        if(m_AlarmSound && m_AlarmSound.IsSoundPlaying())
        {
            m_AlarmSound.SoundStop();
        }
    }

    override bool CanDisplayAttachmentSlot(string slot_name)
    {
        if(!super.CanDisplayAttachmentSlot(slot_name))
            return false;

        return !m_CarLock_IsLocked;
    }

    override bool CanDisplayAttachmentCategory(string category_name)
    {
        if(!super.CanDisplayAttachmentCategory(category_name))
            return false;

        return !m_CarLock_IsLocked;
    }
    override bool IsInventoryVisible()
    {
        if(!super.IsInventoryVisible())
            return false;
        
        return !m_CarLock_IsLocked && (GetGame().GetPlayer() && (!GetGame().GetPlayer().GetCommand_Vehicle() || GetGame().GetPlayer().GetCommand_Vehicle().GetTransport() == this));
    }

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        Param3<bool, int, int> data = new Param3<bool, int, int>(m_CarLock_IsLocked, m_CarLockOwner, m_CarLockPassword);
        ctx.Write(data);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if(!super.OnStoreLoad(ctx, version))
            return false;

        Param3<bool, int, int> data = new Param3<bool, int, int>(false, TUTORIAL_CL_UNDEFINED, TUTORIAL_CL_UNDEFINED);
        if(ctx.Read(data))
        {
            m_CarLock_IsLocked = data.param1;
            m_CarLockOwner = data.param2;
            m_CarLockPassword = data.param3;
            CarLockLogInit();
        }
        SynchronizeLocking();
        return true;
    }
}