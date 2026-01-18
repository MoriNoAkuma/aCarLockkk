
static ref TutorialCarLockLoggingModule GetCarLockLogger()
{
    return TutorialCarLockLoggingModule.Cast(CF_ModuleCoreManager.Get(TutorialCarLockLoggingModule));
}

[CF_RegisterModule(TutorialCarLockLoggingModule)]
class TutorialCarLockLoggingModule : CF_ModuleGame
{
    private int networkSync_LogLevel;

    ref CarLockLoggingSettings settings;

    FileHandle fileHandle;

    float dtime = 0;

    float dtimeTemp = 0;


    override void OnInit()
    {
        super.OnInit();
        
        EnableUpdate();
        EnableMissionStart();
    }

    override void OnMissionStart(Class sender, CF_EventArgs args)
    {
        super.OnMissionStart(sender, args);

        fileHandle = CreateNewLogFile();

        if(GetGame().IsServer())
        {
            settings = CarLockLoggingSettings.Load();

            networkSync_LogLevel = settings.logLevel;
            SynchLogLevel();       
        }
        else
        {
            AddLegacyRPC("GetLogLevelResponse", SingleplayerExecutionType.Client);
        }
    }

    void GetLogLevelResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		 if (type != CallType.Client)
			 return;

		 Param1<int> data;
		 if (!ctx.Read(data))
            return;

		 networkSync_LogLevel = data.param1;
	}

    void SynchLogLevel()
    {
        GetRPCManager().SendRPC(ClassName(), "GetLogLevelResponse",  new Param1<int>(networkSync_LogLevel), true, NULL);
    }

    override void OnUpdate(Class sender, CF_EventArgs args)
    {
        auto update = CF_EventUpdateArgs.Cast(args);

        dtime += update.DeltaTime;
        dtimeTemp += update.DeltaTime;

        if(GetGame().IsServer() && settings && dtime >= settings.refreshRateInSeconds)
        {
            dtime = 0;

            settings = CarLockLoggingSettings.Load();

            networkSync_LogLevel = settings.logLevel;
            SynchLogLevel();
        }
    }

    void SendLogViaApiToDiscord(string content)
    {
        if(!settings || !settings.webhookLink)
            return;

        CarLoggerContent carLoggerContent = new CarLoggerContent();
        carLoggerContent.content = content;

        string stringContent = JsonFileLoader<CarLoggerContent>.JsonMakeData(carLoggerContent);
        
        RestContext ctx = CreateRestApi().GetRestContext(settings.webhookLink);
        string request = "";
        ctx.SetHeader("application/json");
        ctx.POST(new RestCallback(), request, stringContent);
    }


    void MakeDirectoryIfNotExists()
    {
        if(!FileExist(CARLOCK_LOGGER_ROOT))
            MakeDirectory(CARLOCK_LOGGER_ROOT);

        if(!FileExist(CARLOCK_LOGGER_LOG_DIR))
            MakeDirectory(CARLOCK_LOGGER_LOG_DIR);
    }
    
    string GenerateShortDateString()
    {
        int year, month, day;
        GetYearMonthDay(year, month, day);
        return "" + year + "_" + month + "_" + day;
    }

    string GenerateShortTimeString()
    {
        int hour, minute, second;
        GetHourMinuteSecond(hour, minute, second);
        return "" + hour + "_" + minute + "_" + second;
    }

    string GenerateFullTimestamp()
    {
        string dateStr = GenerateShortDateString();
        string timeStr = GenerateShortTimeString();

        return dateStr + "-" + timeStr;
    }

    FileHandle CreateNewLogFile()
    {
        MakeDirectoryIfNotExists();

        string filePath = string.Format(CARLOCK_LOGGER_LOG_FILE, GenerateFullTimestamp());

        fileHandle = OpenFile(filePath, FileMode.WRITE);

        if(fileHandle != 0)
        {
            FPrintln(fileHandle, "Creation Time: " + GenerateFullTimestamp());
            return fileHandle;
        }

        return null;
    }

    void Log(string content, CarLockLogLevel logLevel)
    {
        if(logLevel < networkSync_LogLevel)
            return;
        
        string timestamp = GenerateShortTimeString();
        string logContent = timestamp + " | " + GetLogLevelString(logLevel) + " | " + content;
        FPrintln(fileHandle, logContent);

        SendLogViaApiToDiscord(logContent);
    }

    void LogInfo(string content)
    {
        Log(content, CarLockLogLevel.Info);
    }

    void LogWarning(string content)
    {
        Log(content, CarLockLogLevel.Warn);
    }

    void LogError(string content)
    {
        Log(content, CarLockLogLevel.Error);
    }

    void LogDebug(string content)
    {
        Log(content, CarLockLogLevel.Debug);
    }

    string GetLogLevelString(CarLockLogLevel logLevel)
    {
        switch(logLevel)
        {
             case CarLockLogLevel.Debug:
                return "DEBUG";
            case CarLockLogLevel.Info:
                return "INFO";
            case CarLockLogLevel.Warn:
                return "WARNING";
            case CarLockLogLevel.Error:
                return "ERROR";
            default:
                return "";
        }

        return "";
    }

}