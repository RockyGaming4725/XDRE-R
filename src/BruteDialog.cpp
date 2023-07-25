/*
 * This file is part of XDRE.
 *
 * XDRE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * XDRE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XDRE.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Этот файл — часть XDRE.
 *
 * XDRE — свободная программа: вы можете перераспространять её и/или
 * изменять её на условиях Стандартной общественной лицензии GNU в том виде,
 * в каком она была опубликована Фондом свободного программного обеспечения;
 * либо версии 2 лицензии, либо (по вашему выбору) любой более поздней
 * версии.
 *
 * XDRE распространяется в надежде, что она будет полезной,
 * но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА
 * или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной
 * общественной лицензии GNU.
 *
 * Вы должны были получить копию Стандартной общественной лицензии GNU
 * вместе с этой программой. Если это не так, см.
 * <http://www.gnu.org/licenses/>.
 *
 * Description: brute force dialog.
*/

#include "BruteDialog.hpp"

#include <wx/intl.h>
#include <wx/string.h>
#include <wx/valnum.h>
#include <wx/valtext.h>
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include <wx/app.h>

//prevent more than 6 digits past decimal so coordinat brute forcing works
class CustomFloatingPointValidator : public wxFloatingPointValidator<double>
{
public:
    CustomFloatingPointValidator(int precision, double* valuePtr)
        : wxFloatingPointValidator<double>(precision, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES), m_valuePtr(valuePtr), m_precision(precision)
    {
    }

    wxObject* Clone() const override
    {
        return new CustomFloatingPointValidator(*this);
    }

    double* GetValuePtr() const { return m_valuePtr; }
    void SetValuePtr(double* valuePtr) { m_valuePtr = valuePtr; }

private:
    double* m_valuePtr;
    int m_precision;
};


class BruteException : public std::exception {
public:
    BruteException(const char* str) : str{str} {}
    virtual const char* what() const noexcept {return str;}
    const char* str;
};


const long BruteDialog::ID_BRUTETICSBOX = wxNewId();
const long BruteDialog::ID_BRUTETICINPUT = wxNewId();
const long BruteDialog::ID_STRAFECMDINPUT = wxNewId();
const long BruteDialog::ID_RUNCMDINPUT = wxNewId();
const long BruteDialog::ID_TURNCMDINPUT = wxNewId();
const long BruteDialog::ID_TURNANGLEBUTTON = wxNewId();
const long BruteDialog::ID_FIRERADIOBOX = wxNewId();
const long BruteDialog::ID_USERADIOBOX = wxNewId();
const long BruteDialog::ID_CANCEL = wxNewId();
const long BruteDialog::ID_START = wxNewId();
const long BruteDialog::ID_ADDBUTTON = wxNewId();
const long BruteDialog::ID_REMOVEBUTTON = wxNewId();
const long BruteDialog::ID_BRUTEHELP = wxNewId();
const long BruteDialog::ID_AUTOBRUTERANGE = wxNewId();

wxDEFINE_EVENT(EVT_BRUTE_DONE, wxThreadEvent);
BEGIN_EVENT_TABLE(BruteDialog,wxDialog)
END_EVENT_TABLE()


BruteDialog::BruteDialog(MainWindow* mainWindow, wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
    : wxDialog(parent, id, _("Brute Force"), pos, size), m_mainWindow(mainWindow) 

{
    //wxDialog::Create(parent, wxID_ANY, _("Brute Force")), m_mainWindow(mainWindow);
    SetClientSize(wxSize(426,400));
    SetMinSize(wxSize(426,396));
    SetMaxSize(wxSize(426,-1));
    p = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(460,440), wxTAB_TRAVERSAL);

    double value = 0.0;
    double* valuePtr = &value;

    checkTicInput = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(236,23), wxSize(96,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    rngInput      = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(330,48), wxSize(80,21), wxTE_RIGHT);
    damageInput   = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(330,74), wxSize(80,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    xPosInput     = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(330,100), wxSize(80,21), wxTE_RIGHT, CustomFloatingPointValidator(6, valuePtr));
    yPosInput     = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(330,126), wxSize(80,21), wxTE_RIGHT, CustomFloatingPointValidator(6, valuePtr));
    zPosInput     = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(330,152), wxSize(80,21), wxTE_RIGHT, wxIntegerValidator<int>());
    xMomInput     = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(330,178), wxSize(80,21), wxTE_RIGHT, CustomFloatingPointValidator(6, valuePtr));
    yMomInput     = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(330,204), wxSize(80,21), wxTE_RIGHT, CustomFloatingPointValidator(6, valuePtr));
    speedInput    = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(330,230), wxSize(80,21), wxTE_RIGHT, CustomFloatingPointValidator(6, valuePtr));
    //For Linedef Skip later:
    //wxTextCtrl* linedefInput;
    //linedefInput  = new wxTextCtrl(p, wxID_ANY, wxEmptyString, wxPoint(330,256), wxSize(80,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());

    StaticText1 = new wxStaticText(p, wxID_ANY, _("RNG"), wxPoint(180,50));
    rngCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,50), wxDefaultSize, wxALIGN_RIGHT);
    rngCheckBox->SetValue(false);
    rngChoice = new wxChoice(p, wxID_ANY, wxPoint(258,48), wxSize(64,21), 0, 0, 0, wxDefaultValidator);
    rngChoice->SetSelection( rngChoice->Append(_("=")) );
    rngChoice->Append(_("!="));

    StaticText2 = new wxStaticText(p, wxID_ANY, _("Damage"), wxPoint(180,76));
    damageCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,76), wxDefaultSize, wxALIGN_RIGHT);
    damageCheckBox->SetValue(false);
    damageChoice = new wxChoice(p, wxID_ANY, wxPoint(258,74), wxSize(64,21), 0, 0, 0);
    damageChoice->SetSelection( damageChoice->Append(_("=")) );
    damageChoice->Append(_(">="));
    damageChoice->Append(_(">"));
    damageChoice->Append(_("<="));
    damageChoice->Append(_("<"));

    StaticText3 = new wxStaticText(p, wxID_ANY, _("X"), wxPoint(180,102));
    xPosCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,102), wxDefaultSize, wxALIGN_RIGHT);
    xPosCheckBox->SetValue(false);
    xPosChoice = new wxChoice(p, wxID_ANY, wxPoint(258,100), wxSize(64,21), 0, 0, 0);
    xPosChoice->SetSelection(xPosChoice->Append(_("=")));
    xPosChoice->Append(_(">="));
    xPosChoice->Append(_(">"));
    xPosChoice->Append(_("<="));
    xPosChoice->Append(_("<"));

    StaticText4 = new wxStaticText(p, wxID_ANY, _("Y"), wxPoint(180,128));
    yPosCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,128), wxDefaultSize, wxALIGN_RIGHT);
    yPosCheckBox->SetValue(false);
    yPosChoice = new wxChoice(p, wxID_ANY, wxPoint(258,126), wxSize(64,21), 0, 0, 0);
    yPosChoice->SetSelection(yPosChoice->Append(_("=")));
    yPosChoice->Append(_(">="));
    yPosChoice->Append(_(">"));
    yPosChoice->Append(_("<="));
    yPosChoice->Append(_("<"));

    StaticText5 = new wxStaticText(p, wxID_ANY, _("Z"), wxPoint(180,154));
    zPosCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,154), wxDefaultSize, wxALIGN_RIGHT);
    zPosCheckBox->SetValue(false);
    zPosChoice = new wxChoice(p, wxID_ANY, wxPoint(258,152), wxSize(64,21), 0, 0, 0);
    zPosChoice->SetSelection(zPosChoice->Append(_("=")));
    zPosChoice->Append(_(">="));
    zPosChoice->Append(_(">"));
    zPosChoice->Append(_("<="));
    zPosChoice->Append(_("<"));

    StaticText6 = new wxStaticText(p, wxID_ANY, _("X mom."), wxPoint(180,180));
    xMomCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,180), wxDefaultSize, wxALIGN_RIGHT);
    xMomCheckBox->SetValue(false);
    xMomChoice = new wxChoice(p, wxID_ANY, wxPoint(258,178), wxSize(64,21), 0, 0, 0);
    xMomChoice->SetSelection(xMomChoice->Append(_("=")));
    xMomChoice->Append(_(">="));
    xMomChoice->Append(_(">"));
    xMomChoice->Append(_("<="));
    xMomChoice->Append(_("<"));

    StaticText7 = new wxStaticText(p, wxID_ANY, _("Y mom."), wxPoint(180,206));
    yMomCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,206), wxDefaultSize, wxALIGN_RIGHT);
    yMomCheckBox->SetValue(false);
    yMomChoice = new wxChoice(p, wxID_ANY, wxPoint(258,204), wxSize(64,21), 0, 0, 0);
    yMomChoice->SetSelection(yMomChoice->Append(_("=")));
    yMomChoice->Append(_(">="));
    yMomChoice->Append(_(">"));
    yMomChoice->Append(_("<="));
    yMomChoice->Append(_("<"));

    StaticText8 = new wxStaticText(p, wxID_ANY, _("Speed"), wxPoint(180,232));
    speedCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,232), wxDefaultSize, wxALIGN_RIGHT);
    speedCheckBox->SetValue(false);
    speedChoice = new wxChoice(p, wxID_ANY, wxPoint(258,230), wxSize(64,21), 0, 0, 0);
    speedChoice->SetSelection(speedChoice->Append(_("=")));
    speedChoice->Append(_(">="));
    speedChoice->Append(_(">"));
    speedChoice->Append(_("<="));
    speedChoice->Append(_("<"));

    /*StaticText9 = new wxStaticText(p, wxID_ANY, _("Linedef"), wxPoint(180,258));
    //wxCheckBox* linedefCheckBox;
    linedefCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,258), wxDefaultSize, wxALIGN_RIGHT);
    linedefCheckBox->SetValue(false);
    linedefChoice = new wxChoice(p, wxID_ANY, wxPoint(258,256), wxSize(64,21), 0, 0, 0);
    linedefChoice->SetSelection(linedefChoice->Append(_("No")));
    linedefChoice->Append(_("Yes"));*/

    StaticText10 = new wxStaticText(p, wxID_ANY, _("Use"), wxPoint(180,258));
    useCheckBox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(232,258), wxDefaultSize, wxALIGN_RIGHT);
    useCheckBox->SetValue(false);

    bruteTicsBox    = new wxListBox(p, ID_BRUTETICSBOX, wxPoint(16,16), wxSize(140,100), 0, 0, 0);
    addButton       = new wxButton(p, ID_ADDBUTTON, _("Add"), wxPoint(16,120), wxSize(68,-1), 0);
    removeButton    = new wxButton(p, ID_REMOVEBUTTON, _("Remove"), wxPoint(88,120), wxSize(68,-1), 0);
    bruteTicInput   = new wxTextCtrl(p, ID_BRUTETICINPUT, wxEmptyString, wxPoint(64,152), wxSize(92,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    strafeCmdInput  = new wxTextCtrl(p, ID_STRAFECMDINPUT, wxEmptyString, wxPoint(64,176), wxSize(92,21), wxTE_RIGHT, wxTextValidator());
    runCmdInput     = new wxTextCtrl(p, ID_RUNCMDINPUT, wxEmptyString, wxPoint(64,200), wxSize(92,21), wxTE_RIGHT, wxTextValidator());
    turnCmdInput    = new wxTextCtrl(p, ID_TURNCMDINPUT, wxEmptyString, wxPoint(64,224), wxSize(92,21), wxTE_RIGHT, wxTextValidator());
    turnAngleButton = new wxButton(p, ID_TURNANGLEBUTTON, _("Turn"), wxPoint(16,224), wxSize(40,21), 0);
    removeButton->Disable();
    bruteTicInput->Disable();
    strafeCmdInput->Disable();
    runCmdInput->Disable();
    turnCmdInput->Disable();
    turnAngleButton->Disable();


    wxString __wxRadioBoxChoices_1[3] = {_("No"), _("Both"), _("Yes")};
    fireRadioBox = new wxRadioBox(p, ID_FIRERADIOBOX, _("Fire"), wxPoint(16,246), wxSize(56,100), 3, __wxRadioBoxChoices_1, 3, wxRA_VERTICAL);
    wxString __wxRadioBoxChoices_2[3] = {_("No"), _("Both"), _("Yes")};
    useRadioBox = new wxRadioBox(p, ID_USERADIOBOX, _("Use"), wxPoint(80,246), wxSize(56,100), 3, __wxRadioBoxChoices_2, 3, wxRA_VERTICAL);
    fireRadioBox->SetSelection(0);
    useRadioBox->SetSelection(0);
    fireRadioBox->Disable();
    useRadioBox->Disable();

    cancel = new wxButton(p, ID_CANCEL, _("Cancel"), wxPoint(100,360), wxDefaultSize, 0, wxDefaultValidator);
    start  = new wxButton(p, ID_START, _("Start"), wxPoint(190,360), wxDefaultSize, 0, wxDefaultValidator);
    help   = new wxButton(p, ID_BRUTEHELP, _("Help"), wxPoint(8,360), wxDefaultSize, 0, wxDefaultValidator);
    autoBruteRange = new wxButton(p, ID_AUTOBRUTERANGE, _("Auto Range"), wxPoint(280,360), wxDefaultSize, 0, wxDefaultValidator);
    workingText = new wxStaticText(p, wxID_ANY, _("Working..."), wxPoint(263,258), wxDefaultSize, 0);
    workingText->Hide();
    StaticBox1 = new wxStaticBox(p, wxID_ANY, _("Tics to brute force"), wxPoint(8,0), wxSize(156,354), 0);
    StaticBox2 = new wxStaticBox(p, wxID_ANY, _("Checks"), wxPoint(172,6), wxSize(246,275), 0);
    StaticBox3 = new wxStaticBox(p, wxID_ANY, _("Auto range"), wxPoint(172,280), wxSize(246,74), 0);
    StaticText11 = new wxStaticText(p, wxID_ANY, _("On tic"), wxPoint(180,26), wxDefaultSize, 0);
    StaticText12 = new wxStaticText(p, wxID_ANY, _("Tic"), wxPoint(16,156), wxDefaultSize, 0);
    StaticText13 = new wxStaticText(p, wxID_ANY, _("Run"), wxPoint(16,204), wxDefaultSize, 0);
    StaticText14 = new wxStaticText(p, wxID_ANY, _("Strafe"), wxPoint(16,180), wxDefaultSize, 0);

    StaticText15 = new wxStaticText(p, wxID_ANY, _("Range size:"), wxPoint(180,300), wxDefaultSize, 0);
    rangeSizeChoice = new wxChoice(p, wxID_ANY, wxPoint(245,298), wxSize(50,21), 0, 0, 0);
    rangeSizeChoice->SetSelection(rangeSizeChoice->Append(_("1")));
    rangeSizeChoice->Append(_("2"));
    rangeSizeChoice->Append(_("3"));
    rangeSizeChoice->Append(_("4"));
    rangeSizeChoice->Append(_("5"));
    rangeSizeChoice->Append(_("Max"));
    StaticText16 = new wxStaticText(p, wxID_ANY, _("Avoid Turbo:"), wxPoint(180,328), wxDefaultSize, 0);
    avoidTurboCheckbox = new wxCheckBox(p, wxID_ANY, _(""), wxPoint(256,328), wxDefaultSize, wxALIGN_RIGHT);
    avoidTurboCheckbox->SetValue(1);

    Connect(ID_BRUTETICSBOX, wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(BruteDialog::OnBruteTicsBoxSelect));
    Connect(ID_BRUTETICINPUT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(BruteDialog::OnBruteTicInputText));

    Connect(ID_STRAFECMDINPUT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(BruteDialog::OnStrafeCmdInputText));
    Connect(ID_RUNCMDINPUT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(BruteDialog::OnRunCmdInputText));
    Connect(ID_TURNCMDINPUT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(BruteDialog::OnTurnCmdInputText));
    Connect(ID_FIRERADIOBOX, wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(BruteDialog::OnFireRadioBoxSelect));
    Connect(ID_USERADIOBOX, wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(BruteDialog::OnUseRadioBoxSelect));
    Connect(ID_TURNANGLEBUTTON, wxEVT_BUTTON, wxCommandEventHandler(BruteDialog::OnTurnAngleButtonClick));

    Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(BruteDialog::OnClose));
    Connect(ID_CANCEL, wxEVT_BUTTON, wxCommandEventHandler(BruteDialog::OnCancelClick));
    Connect(ID_START, wxEVT_BUTTON, wxCommandEventHandler(BruteDialog::OnStartClick));
    Connect(ID_ADDBUTTON, wxEVT_BUTTON, wxCommandEventHandler(BruteDialog::OnAddButtonClick));
    Connect(ID_REMOVEBUTTON, wxEVT_BUTTON, wxCommandEventHandler(BruteDialog::OnRemoveButtonClick));
    Connect(ID_BRUTEHELP, wxEVT_BUTTON, wxCommandEventHandler(BruteDialog::OnBruteHelp));
    Connect(ID_AUTOBRUTERANGE, wxEVT_BUTTON, wxCommandEventHandler(BruteDialog::OnAutoRange));


    (*checkTicInput) << static_cast<long>(xdre::getCurrentTic());
    rngInput->ChangeValue("0");
    damageInput->ChangeValue("0");
    xPosInput->ChangeValue("0");
    yPosInput->ChangeValue("0");
    zPosInput->ChangeValue("0");
    xMomInput->ChangeValue("0");
    yMomInput->ChangeValue("0");
    speedInput->ChangeValue("0");
    //linedefInput->ChangeValue("0");
}

BruteDialog::~BruteDialog() {}

std::vector<signed char> BruteDialog::ParseCmds(wxString str, bool parseAsUnsigned) {
    long limitMax = 127;
    long limitMin = -128;
    if(parseAsUnsigned) {
        limitMax = 255;
        limitMin = 0;
    }

    std::vector<signed char> ret;

    auto cmds = wxStringTokenize(str, ",", wxTOKEN_STRTOK);

    for(auto& cmd : cmds) {
        auto range = wxStringTokenize(cmd, ":", wxTOKEN_STRTOK);

        if(range.GetCount() == 1) {
            long num;
            if(cmd.ToLong(&num)) {
                if(num > limitMax || num < limitMin) throw BruteException("Too big input value given.");
                ret.push_back(num);
            } else {
                throw BruteException("Invalid value.");
            }
        } else if(range.GetCount() == 2) {
            long start;
            long end;
            if(!range[0].ToLong(&start) || !range[1].ToLong(&end)) throw BruteException("Invalid range given.");

            if(start > limitMax || start < limitMin || end > limitMax || end < limitMin)
                throw BruteException("Too big input value given.");

            if(start <= end) for(; start <= end; ++start) ret.push_back(start);
            else for(; start >= end; --start) ret.push_back(start);
        } else throw BruteException("Invalid range given.");
    }
    return ret;
}

std::vector<BruteTic> BruteDialog::CreateBruteTics() {
    std::vector<BruteTic> ret;
    for(auto& item : items) {
        BruteTic tic;
        long num;

        if(!item.ticStr.ToLong(&num)) {
            throw BruteException("Tic value not a number.");
        } else if(num < 0) {throw BruteException("Tic value < 0.");}

        tic.tic = num;
        tic.fire = item.fire - 1;
        tic.use = item.use - 1;

        if(tic.fire < -1 || tic.fire > 1 || tic.use < -1 || tic.use > 1) throw BruteException("Invalid fire/use choices.");

        bool turnsUnsigned;
        if(item.turnOrAngle == 0) {
            tic.turnsAreAngles = false;
            turnsUnsigned = false;
        } else {
            tic.turnsAreAngles = true;
            turnsUnsigned = true;
        }

        tic.runCmds = ParseCmds(item.runStr);
        tic.strafeCmds = ParseCmds(item.strafeStr);
        tic.turnCmds = ParseCmds(item.turnStr, turnsUnsigned);

        if(tic.runCmds.empty() || tic.strafeCmds.empty() || tic.turnCmds.empty()) throw BruteException("Empty field.");

        ret.push_back(tic);
    }

    return ret;
}

BruteCheck BruteDialog::CreateBruteCheck() {
    BruteCheck check;

    long tic;
    if(!checkTicInput->GetValue().ToLong(&tic)) {
        throw BruteException("Checktic not a number.");
    } else if(tic < 0) {throw BruteException("Checktic < 0.");}

    check.tic = tic;

//TODO: refactor this shit
    if(rngCheckBox->GetValue()) {
        auto indexes = ParseCmds(rngInput->GetValue(), true);
        switch(rngChoice->GetSelection()) {
        case 0:
            check.rngFunc = [indexes](int val) {
                for(auto index : indexes) {
                    if(val == index) return true;
                }
                return false;
            };
            break;

        case 1:
            check.rngFunc = [indexes](int val) {
                for(auto index : indexes) {
                    if(val == index) return false;
                }
                return true;
            };
            break;
        }
    } else {check.rngFunc = [](int val){return true;};}

    if(damageCheckBox->GetValue()) {
        long num;
        if(!damageInput->GetValue().ToLong(&num)) throw BruteException("Invalid damage.");
        switch(damageChoice->GetSelection()) {
        case 0: check.damageFunc = [num](int val){return val == num;}; break;
        case 1: check.damageFunc = [num](int val){return val >= num;}; break;
        case 2: check.damageFunc = [num](int val){return val > num;}; break;
        case 3: check.damageFunc = [num](int val){return val <= num;}; break;
        case 4: check.damageFunc = [num](int val){return val < num;}; break;
        }
    } else {check.damageFunc = [](int val){return true;};}
    
    if(xPosCheckBox->GetValue()) {
        double dbl;
        if(!xPosInput->GetValue().ToDouble(&dbl)) throw BruteException("Invalid X position.");
        switch(xPosChoice->GetSelection()) {
        case 0: check.xPosFunc = [dbl](double val){return val == dbl;}; break;
        case 1: check.xPosFunc = [dbl](double val){return val >= dbl;}; break;
        case 2: check.xPosFunc = [dbl](double val){return val > dbl;}; break;
        case 3: check.xPosFunc = [dbl](double val){return val <= dbl;}; break;
        case 4: check.xPosFunc = [dbl](double val){return val < dbl;}; break;
//        case 5: break;
//        case 6: break;
        }
    } else {check.xPosFunc = [](double val){return true;};}
    if(yPosCheckBox->GetValue()) {
        double dbl;
        if(!yPosInput->GetValue().ToDouble(&dbl)) throw BruteException("Invalid Y position.");
        switch(yPosChoice->GetSelection()) {
        case 0: check.yPosFunc = [dbl](double val){return val == dbl;}; break;
        case 1: check.yPosFunc = [dbl](double val){return val >= dbl;}; break;
        case 2: check.yPosFunc = [dbl](double val){return val > dbl;}; break;
        case 3: check.yPosFunc = [dbl](double val){return val <= dbl;}; break;
        case 4: check.yPosFunc = [dbl](double val){return val < dbl;}; break;
//        case 5: break;
//        case 6: break;
        }
    } else {check.yPosFunc = [](double val){return true;};}
    if(zPosCheckBox->GetValue()) {
        double dbl;
        if(!zPosInput->GetValue().ToDouble(&dbl)) throw BruteException("Invalid Z position.");
        switch(zPosChoice->GetSelection()) {
        case 0: check.zPosFunc = [dbl](double val){return val == dbl;}; break;
        case 1: check.zPosFunc = [dbl](double val){return val >= dbl;}; break;
        case 2: check.zPosFunc = [dbl](double val){return val > dbl;}; break;
        case 3: check.zPosFunc = [dbl](double val){return val <= dbl;}; break;
        case 4: check.zPosFunc = [dbl](double val){return val < dbl;}; break;
        }
    } else {check.zPosFunc = [](double val){return true;};}
    if(xMomCheckBox->GetValue()) {
        double dbl;
        if(!xMomInput->GetValue().ToDouble(&dbl)) throw BruteException("Invalid X momentum.");
        switch(xMomChoice->GetSelection()) {
        case 0: check.xMomFunc = [dbl](double val){return val == dbl;}; break;
        case 1: check.xMomFunc = [dbl](double val){return val >= dbl;}; break;
        case 2: check.xMomFunc = [dbl](double val){return val > dbl;}; break;
        case 3: check.xMomFunc = [dbl](double val){return val <= dbl;}; break;
        case 4: check.xMomFunc = [dbl](double val){return val < dbl;}; break;
//        case 5: break;
//        case 6: break;
        }
    } else {check.xMomFunc = [](double val){return true;};}
    if(yMomCheckBox->GetValue()) {
        double dbl;
        if(!yMomInput->GetValue().ToDouble(&dbl)) throw BruteException("Invalid Y momentum.");
        switch(yMomChoice->GetSelection()) {
        case 0: check.yMomFunc = [dbl](double val){return val == dbl;}; break;
        case 1: check.yMomFunc = [dbl](double val){return val >= dbl;}; break;
        case 2: check.yMomFunc = [dbl](double val){return val > dbl;}; break;
        case 3: check.yMomFunc = [dbl](double val){return val <= dbl;}; break;
        case 4: check.yMomFunc = [dbl](double val){return val < dbl;}; break;
//        case 5: break;
//        case 6: break;
        }
    } else {check.yMomFunc = [](double val){return true;};}

    if(speedCheckBox->GetValue()) {
        double dbl;
        if(!speedInput->GetValue().ToDouble(&dbl)) throw BruteException("Invalid speed.");
        switch(speedChoice->GetSelection()) {
        case 0: check.speedFunc = [dbl](double val){return val == dbl;}; break;
        case 1: check.speedFunc = [dbl](double val){return val >= dbl;}; break;
        case 2: check.speedFunc = [dbl](double val){return val > dbl;}; break;
        case 3: check.speedFunc = [dbl](double val){return val <= dbl;}; break;
        case 4: check.speedFunc = [dbl](double val){return val < dbl;}; break;
        }
    } else {check.speedFunc = [](double val){return true;};}

    /*if (linedefCheckBox->GetValue()) {
        unsigned long num;
        if (!linedefInput->GetValue().ToULong(&num)) throw BruteException("Invalid Linedef");
        xdre::setLinedefCheck(num);
        switch(linedefChoice->GetSelection()) {
        case 0: check.linedefFunc = [](bool val){return val;}; break;
        case 1: check.linedefFunc = [num](bool val){return xdre::getLinedefCrossed();}; break;
        }
    } else {check.linedefFunc = [](bool val){return true;};}*/

    //check.linedef = linedefCheckBox->GetValue();
    //return check;


    check.use = useCheckBox->GetValue();
    return check;
}

void BruteDialog::ShowData(int n) {
    ListItem const& data = items.at(n);
    bruteTicInput->ChangeValue(data.ticStr);
    runCmdInput->ChangeValue(data.runStr);
    strafeCmdInput->ChangeValue(data.strafeStr);
    turnCmdInput->ChangeValue(data.turnStr);
    fireRadioBox->SetSelection(data.fire);
    useRadioBox->SetSelection(data.use);
    turnAngleButton->SetLabel(turnAngleTexts[data.turnOrAngle]);
}

void BruteDialog::EnableStuff() {
    bruteTicInput->Enable();
    runCmdInput->Enable();
    strafeCmdInput->Enable();
    turnCmdInput->Enable();
    fireRadioBox->Enable();
    useRadioBox->Enable();
    turnAngleButton->Enable();
    removeButton->Enable();
}

void BruteDialog::DisableStuff() {
    bruteTicInput->Disable();
    runCmdInput->Disable();
    strafeCmdInput->Disable();
    turnCmdInput->Disable();
    fireRadioBox->Disable();
    useRadioBox->Disable();
    turnAngleButton->Disable();
    removeButton->Disable();
}

void BruteDialog::OnClose(wxCloseEvent& event) {
    if(GetThread() && GetThread()->IsRunning()) return;
    Destroy();
}

void BruteDialog::OnCancelClick(wxCommandEvent& event)
{
    if(GetThread() && GetThread()->IsRunning()) {
        abortBrute = true;
        GetThread()->Wait();
        return;
    }
    Close();
}

void BruteDialog::OnTurnAngleButtonClick(wxCommandEvent& event) {
    auto item = bruteTicsBox->GetSelection();
    if(items[item].turnOrAngle) {
        items[item].turnOrAngle = 0;
    } else {items[item].turnOrAngle = 1;}
    turnAngleButton->SetLabel(turnAngleTexts[items[item].turnOrAngle]);
}

void BruteDialog::OnAddButtonClick(wxCommandEvent& event) {
    //m_mainWindow = mainWindow;
            if (m_mainWindow->MenuItem12->IsChecked()) {
                items.push_back({});
                bruteTicsBox->Append(items.at(items.size()-1).ticStr);
                bruteTicsBox->SetSelection(items.size()-1);

                /* This creates a list with higher values at the top. Code below creates a list with higher values at bottom.
                for(int i=0; i<(bruteTicsBox->GetCount()); i++) {
                    items.at(i).ticStr = wxString::Format(wxT("%i"), xdre::getCurrentTic()-i);
                    bruteTicsBox->SetString(i, items.at(i).ticStr);
                } */ 

                for(int i=bruteTicsBox->GetCount()-1; i>=0; i--) {
                    items.at(i).ticStr = wxString::Format(wxT("%i"), xdre::getCurrentTic()-bruteTicsBox->GetCount()+i+1);
                    bruteTicsBox->SetString(i, items.at(i).ticStr);
                }   
                EnableStuff();
                ShowData(items.size()-1);
            }

            else {
                //ORIGINAL CODE FROM XDRE 2.22
                items.push_back({});
                int last = items.size() - 1;
                bruteTicsBox->Append(items.at(last).ticStr);
                bruteTicsBox->SetSelection(last);
                items.at(last).ticStr = wxString::Format(wxT("%i"), xdre::getCurrentTic());
                bruteTicsBox->SetString(last, items.at(last).ticStr);
                EnableStuff();
                ShowData(last);
            }
}
    

void BruteDialog::OnAutoRange(wxCommandEvent& event) {
    if(bruteTicsBox->GetCount() < 1) return;

    int autoRangeSize = 0;
    bool maxRange = 0;
    switch(rangeSizeChoice->GetSelection()) {
    case 0: autoRangeSize = 1; break;    
    case 1: autoRangeSize = 2; break;
    case 2: autoRangeSize = 3; break;
    case 3: autoRangeSize = 4; break;
    case 4: autoRangeSize = 5; break;
    case 5: maxRange = 1; break;        
    }

    if(autoRangeSize != 0){
        for(int i=bruteTicsBox->GetCount()-1; i>=0; i--) {
            if(avoidTurboCheckbox->GetValue()){
                if (xdre::getCurrentStrafe(i, bruteTicsBox->GetCount()) + autoRangeSize > 50) {
                    items.at(i).strafeStr = wxString::Format(wxT("%i" ":" "50"), 50-(2*autoRangeSize));
                } else if (xdre::getCurrentStrafe(i, bruteTicsBox->GetCount()) - autoRangeSize < -50) {
                    items.at(i).strafeStr = wxString::Format(wxT("%i" ":" "-50"), -50+(2*autoRangeSize));
                } else {
                    items.at(i).strafeStr = wxString::Format(wxT("%i" ":" "%i"), xdre::getCurrentStrafe(i, bruteTicsBox->GetCount())-autoRangeSize, xdre::getCurrentStrafe(i, bruteTicsBox->GetCount())+autoRangeSize);
                }
            } else {
                items.at(i).strafeStr = wxString::Format(wxT("%i" ":" "%i"), xdre::getCurrentStrafe(i, bruteTicsBox->GetCount())-autoRangeSize, xdre::getCurrentStrafe(i, bruteTicsBox->GetCount())+autoRangeSize);
            }

            if(avoidTurboCheckbox->GetValue()){
                if (xdre::getCurrentRun(i, bruteTicsBox->GetCount()) + autoRangeSize > 50) {
                    items.at(i).runStr = wxString::Format(wxT("%i" ":" "50"), 50-(2*autoRangeSize));
                } else if (xdre::getCurrentRun(i, bruteTicsBox->GetCount()) - autoRangeSize < -50) {
                    items.at(i).runStr = wxString::Format(wxT("%i" ":" "-50"), -50+(2*autoRangeSize));
                } else {
                    items.at(i).runStr = wxString::Format(wxT("%i" ":" "%i"), xdre::getCurrentRun(i, bruteTicsBox->GetCount())-autoRangeSize, xdre::getCurrentRun(i, bruteTicsBox->GetCount())+autoRangeSize);
                }
            } else {
                items.at(i).runStr = wxString::Format(wxT("%i" ":" "%i"), xdre::getCurrentRun(i, bruteTicsBox->GetCount())-autoRangeSize, xdre::getCurrentRun(i, bruteTicsBox->GetCount())+autoRangeSize);
            }
            // Automatic turn range produced is longtic value, %255 converts it to shorttic equivalent. Turn range deliberately 1 value wider each side than movement values
            items.at(i).turnStr = wxString::Format(wxT("%i" ":" "%i"), (xdre::getCurrentTurn(i, bruteTicsBox->GetCount()))%255-autoRangeSize-1, (xdre::getCurrentTurn(i, bruteTicsBox->GetCount()))%255+autoRangeSize+1);
        }
    }

    //I think this covers the same range as 50:-50, 50:-50, 127:-127. That range would have several instances of the same 
    //position being checked because of angle transposition. I THINK this covers everything but takes about 1/4 as long to run.
    if(maxRange == 1){
        for(int i=bruteTicsBox->GetCount()-1; i>=0; i--) {
            items.at(i).strafeStr = wxString::Format(wxT("50" ":" "-50"));
            items.at(i).runStr = wxString::Format(wxT("50" ":" "0"));
            items.at(i).turnStr = wxString::Format(wxT("127" ":" "0"));
        }
    }

    ShowData(bruteTicsBox->GetSelection());
}

void BruteDialog::OnRemoveButtonClick(wxCommandEvent& event) {
    int item = bruteTicsBox->GetSelection();
    if(item == wxNOT_FOUND) return;
    bruteTicsBox->Delete(item);
    items.erase(std::begin(items) + item);
    int sel = item < items.size() ? item : items.size() - 1;
    bruteTicsBox->SetSelection(sel);
    if(sel >= 0) ShowData(sel);
    if(bruteTicsBox->GetCount() < 1) DisableStuff();
}

void BruteDialog::OnBruteTicsBoxSelect(wxCommandEvent& event) {ShowData(bruteTicsBox->GetSelection());}

void BruteDialog::OnBruteTicInputText(wxCommandEvent& event) {
    int item = bruteTicsBox->GetSelection();
    if(item == wxNOT_FOUND) return;
    items.at(item).ticStr = bruteTicInput->GetValue();
    if(items.at(item).ticStr.IsSameAs("")) items.at(item).ticStr = "_";
    bruteTicsBox->SetString(item, items.at(item).ticStr);
}

void BruteDialog::OnRunCmdInputText(wxCommandEvent& event) {
    int item = bruteTicsBox->GetSelection();
    if(item == wxNOT_FOUND) return;
    items.at(item).runStr = runCmdInput->GetValue();
}

void BruteDialog::OnStrafeCmdInputText(wxCommandEvent& event) {
    int item = bruteTicsBox->GetSelection();
    if(item == wxNOT_FOUND) return;
    items.at(item).strafeStr = strafeCmdInput->GetValue();
}

void BruteDialog::OnTurnCmdInputText(wxCommandEvent& event) {
    int item = bruteTicsBox->GetSelection();
    if(item == wxNOT_FOUND)return;
    items.at(item).turnStr = turnCmdInput->GetValue();
}

void BruteDialog::OnFireRadioBoxSelect(wxCommandEvent& event) {
    int item = bruteTicsBox->GetSelection();
    if(item == wxNOT_FOUND) return;
    items.at(item).fire = fireRadioBox->GetSelection();
}

void BruteDialog::OnUseRadioBoxSelect(wxCommandEvent& event) {
    int item = bruteTicsBox->GetSelection();
    if(item == wxNOT_FOUND) return;
    items.at(item).use = useRadioBox->GetSelection();
}

wxThread::ExitCode BruteDialog::Entry() {
    bruteSuccess = xdre::bruteForce(bruteTics, bruteCheck, abortBrute);
    wxQueueEvent(this, new wxThreadEvent(EVT_BRUTE_DONE));
    return static_cast<wxThread::ExitCode>(0);
}

void BruteDialog::OnBruteDone(wxThreadEvent& event) {
    Disconnect(wxID_ANY, EVT_BRUTE_DONE, wxThreadEventHandler(BruteDialog::OnBruteDone));
    start->Enable();
    workingText->Hide();

    if(bruteSuccess) {
        wxMessageBox("Found it!",
                     "Success",
                     wxOK | wxCENTRE,
                     this);
        if(GetThread() && GetThread()->IsRunning()) {
            GetThread()->Wait();
        }
//        Close();
    } else if(!bruteSuccess && !abortBrute) {
        wxMessageBox("Whole range examined. No cigar.",
                     "No success",
                     wxOK | wxCENTRE,
                     this);
    }
}

void BruteDialog::OnStartClick(wxCommandEvent& event)
{
    if(GetThread() && GetThread()->IsRunning()) return;

//This sucks, don't really know how to deal with conflicting min/max yet, will be fixed/improved later
    /*int multipleMinMax = 0;
    if(xPosCheckBox->GetValue()){
        if(xPosChoice->GetSelection() == 5 || xPosChoice->GetSelection() == 6){multipleMinMax += 1;}}
    if(yPosCheckBox->GetValue()){
        if(yPosChoice->GetSelection() == 5 || yPosChoice->GetSelection() == 6){multipleMinMax += 1;}}
    if(xMomCheckBox->GetValue()){
        if(xMomChoice->GetSelection() == 5 || xMomChoice->GetSelection() == 6){multipleMinMax += 1;}}
    if(yMomCheckBox->GetValue()){
        if(yMomChoice->GetSelection() == 5 || yMomChoice->GetSelection() == 6){multipleMinMax += 1;}}
    
    if(multipleMinMax > 1) {
        wxMessageBox("Cannot simultaneously check Min/Max for multiple fields.\n\n"
        "Min/Max checks in multiple fields will typically result in conflicting results; for example, Max X Pos will always conflict with Max Y Pos.\n\n"
        "An exception is Min/Maxing momentum AND position on one axis, for which there is no point in searching for both anyway - "
        "however, diagonal wallrunning can potentially result in a higher momentum value conflicting with a higher coordinate value, "
        "in which case one should be prioritised (probably position).",
        "Cannot set multiple Min/Max checks",
        wxOK | wxCENTRE | wxICON_ERROR,
        this);
        return;
    }
    multipleMinMax = 0;*/

    try {
        bruteTics = CreateBruteTics();
        if(bruteTics.empty()) return;
        bruteCheck = CreateBruteCheck();

        if(CreateThread(wxTHREAD_JOINABLE) == wxTHREAD_NO_ERROR) {
            Connect(wxID_ANY, EVT_BRUTE_DONE, wxThreadEventHandler(BruteDialog::OnBruteDone));
            start->Disable();
            abortBrute = false;
            bruteSuccess = false;
            if(GetThread()->Run() == wxTHREAD_NO_ERROR) {
                workingText->Show();
            } else {
                wxMessageBox("Couldn't start brute force thread.",
                             "Brute force error",
                             wxOK | wxCENTRE | wxICON_ERROR,
                             this);
                Disconnect(wxID_ANY, EVT_BRUTE_DONE, wxThreadEventHandler(BruteDialog::OnBruteDone));
                start->Enable();
                workingText->Hide();
            }
        } else {
            wxMessageBox("Couldn't create brute force thread.",
                         "Brute force error",
                         wxOK | wxCENTRE | wxICON_ERROR,
                         this);
        }
    } catch (BruteException& e) {
        wxMessageBox(e.what(),
                     "Brute force error",
                     wxOK | wxCENTRE | wxICON_ERROR,
                     this);
    }
}

void BruteDialog::OnBruteHelp(wxCommandEvent& event) {
    wxMessageBox(
        "\'Brute force\' tests a range of movement inputs & finds the ones necessary "
        "for a desired outcome, if that outcome is possible.\n"
        "On the left, add the tic numbers you would like to brute force and enter the "
        "movement value range. The range must be entered for each tic individually.\n\n"

        "Select each tic in the list and alter its values individually.\n"
        "For strafing, negative values mean SL and positive values mean SR.\n"
        "For running, negative values are MB and positive values mean MF.\n"
        "For turning, negative values mean TR and positive values mean TL.\n\n"
        
        "x:y will test every value between, and including, x and y.\n"
        "   - Run 10:15 will test MF10, MF11, MF12, MF13, MF14 & MF15.\n"
        "x,y,z will test every value separated by the comma, and not values between.\n"
        "   - Run 10,15 will test MF10 & MF15.\n\n"

        "On the right, choose the tic you would like to search for an outcome on, and then determine "
        "that outcome. This part is as simple as it looks; 'On tic 100' searches for the outcome on "
        "that tic, and searching for Y = 100 will test every combination of values searching for "
        "Y = 100 on tic 100.\n\n"

        "'Auto Range' considers the inputs already present on a given tic before opening the brute "
        "force window and automatically generates strafing, running & turning values in the range to "
        "brute force. The number selected in 'Automatic range size' defines the size of the brute force "
        "range.\n"
        "Wider ranges have greater success chances, but will take longer to calculate.\n"
        "Turning angles are deliberately 1 value wider, as angles affect the chance of success "
        "enough for it to be worth it.\n"
        "'Avoid Turbo' affects only the auto range button, and will ensure the running and strafing range "
        "does not exceed values of 50, while keeping the range the same size.\n"
        "   - If movement before entering brute force is MF49 SR49, and an auto range of size 3 is "
        "automatically generated, the range will be 46:52, 46:52, -4:4.\n"
        "   - If the 'Avoid Turbo' button is checked, the range will be 44:50, 44:50, -4:4.\n\n"
        "Currently a little bugged - bottom tic in list must be selected when clicking the Auto Range button, "
        "& it wont properly work if the tic list isn't in order. For example, trying to set an auto range for 2, "
        "4 & 5 will create an auto range based on existing inputs on 3, 4 & 5. (almostmatt1)",
        "Brute Force Help",
        wxOK|wxICON_INFORMATION, this);
}

