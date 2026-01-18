class TutorialCarLockMenu extends UIScriptedMenu 
{
    private ButtonWidget wBtnOK;
    private ButtonWidget wBtnCancel;
    private EditBoxWidget wPasswordInput;
    private TextWidget wTextOutput;

    private CarScript _car;

    const int PASSWORD_DIGITS_LENGTH = 8;

    void TutorialCarLockMenu(CarScript car)
    {
        _car = car;
    }

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("aCarLock/datasets/gui/carLockMenu.layout");
        wBtnOK     = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnOK"));
        wBtnCancel     = ButtonWidget.Cast(layoutRoot.FindAnyWidget("BtnCancel"));
        wPasswordInput = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("PasswordInput"));
        wTextOutput = TextWidget.Cast(layoutRoot.FindAnyWidget("TextOutput"));

        layoutRoot.Show(false);

        return layoutRoot;
    }

    void InitializeMenu()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

        if(!player || !_car)
            return;

        if(player.GetCarLockDb().IsOwnerId(_car.GetCarLockOwner()) && !_car.HasCarLockPassword())
        {
            wTextOutput.SetText(string.Format("Set Code (%1)",PASSWORD_DIGITS_LENGTH));
        }
        else
        {
            wTextOutput.SetText(string.Format("Enter Code (%1)",PASSWORD_DIGITS_LENGTH));
        }
    }

    override void OnShow()
    {
        super.OnShow();
        PPEffects.SetBlurMenu(0.5);
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowUICursor(true);
        GetGame().GetMission().GetHud().Show(false);
        layoutRoot.Show(true);
        InitializeMenu();
    }

    override void OnHide()
    {
        super.OnHide();
        PPEffects.SetBlurMenu(0);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
        GetGame().GetMission().GetHud().Show(true);
        layoutRoot.Show(false);
        Close();
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        switch(w)
        {
            case wBtnOK: return OnOKBtnClicked();
            break;

            case wBtnCancel: return OnCancelBtnClicked();
            break;
        }

        return false;
    }

    bool OnCancelBtnClicked()
    {
        OnHide();
        return true;
    }

    bool OnOKBtnClicked()
    {
        string strPassword = wPasswordInput.GetText();

        if(CheckPasswordForErrors(strPassword))
            return false;

        int password = strPassword.ToInt();

        GetRPCManager().SendRPC("TutorialCarLock", "GetCarLockLockingRequest", new Param3<CarScript, int, bool>(_car, password, false), true);

        OnHide();

        return true;
    }
    
    /// <summary>
    /// Checks if the given password string contains any character except number.
    /// </summary>
    /// <param name="strPassword">The password to check.</param>
    /// <returns>True if the password contains a character, otherwise false.</returns>
    bool HasCharacterInPassword(string strPassword)
    {
        for(int i=0; i< strPassword.Length();i++)
        {
            switch(strPassword[i])
            {
                case "0": continue;
                break;
                case "1": continue;
                break;
                case "2": continue;
                break;
                case "3": continue;
                break;
                case "4": continue;
                break;
                case "5": continue;
                break;
                case "6": continue;
                break;
                case "7": continue;
                break;
                case "8": continue;
                break;
                case "9": continue;
                break;
                default:return true;
                break;
            }
        }

        return false;
    }


    /// <summary>
    /// Checks if a password has any errors.
    /// </summary>
    /// <param name="strPassword">The password to be checked.</param>
    /// <returns>True if there are errors, False otherwise.</returns>
    bool CheckPasswordForErrors(string strPassword)
    {
        if(strPassword.Length() < PASSWORD_DIGITS_LENGTH)
        {
            wTextOutput.SetText("Too short!");
            return true;
        }

        if(strPassword.Length() > PASSWORD_DIGITS_LENGTH)
        {
            wTextOutput.SetText("Too long!");
            return true;
        }

        if(HasCharacterInPassword(strPassword))
        {
            wTextOutput.SetText("Numbers only!");
            return true;
        }

        return false;
    }
}