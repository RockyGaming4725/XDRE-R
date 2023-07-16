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
 * Description: demo header editor.
*/

#include "HeaderDialog.hpp"

#include <wx/intl.h>
#include <wx/string.h>
#include <wx/valnum.h>
#include <wx/valtext.h>
#include <wx/msgdlg.h>

const long HeaderDialog::ID_CHANGE = wxNewId();
const long HeaderDialog::ID_CLOSE = wxNewId();
const long HeaderDialog::ID_HELP = wxNewId();

BEGIN_EVENT_TABLE(HeaderDialog,wxDialog)
END_EVENT_TABLE()

class HeaderException : public std::exception {
public:
    HeaderException(const char* str) : str{str} {}
    virtual const char* what() const noexcept {return str;}
    const char* str;
};

int options[28];
HeaderDialog::HeaderDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) {
    wxDialog::Create(parent, wxID_ANY, _("Header editor"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
    SetClientSize(wxSize(384,352));
    SetMinSize(wxSize(384,352));
    SetMaxSize(wxSize(384,-1));

    p = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400,440), wxTAB_TRAVERSAL);
    change = new wxButton(p, ID_CHANGE, _("Apply"), wxPoint(32,320), wxDefaultSize, 0, wxDefaultValidator);
    close = new wxButton(p, ID_CLOSE, _("Close"), wxPoint(292,320), wxDefaultSize, 0, wxDefaultValidator);
    help = new wxButton(p, ID_HELP, _("Help"), wxPoint(160,320), wxDefaultSize, 0, wxDefaultValidator);

    StaticText1  = new wxStaticText(p, wxID_ANY, _("Complevel"), wxPoint(8,8), wxDefaultSize, 0);
    StaticText2  = new wxStaticText(p, wxID_ANY, _("Skill"), wxPoint(8,40), wxDefaultSize, 0);
    StaticText3  = new wxStaticText(p, wxID_ANY, _("Episode"), wxPoint(8,72), wxDefaultSize, 0);
    StaticText4  = new wxStaticText(p, wxID_ANY, _("Map"), wxPoint(8,104), wxDefaultSize, 0);
    StaticText5  = new wxStaticText(p, wxID_ANY, _("Deathmatch"), wxPoint(8,136), wxDefaultSize, 0);
    StaticText6  = new wxStaticText(p, wxID_ANY, _("-respawn"), wxPoint(8,168), wxDefaultSize, 0);
    StaticText7  = new wxStaticText(p, wxID_ANY, _("-fast"), wxPoint(8,200), wxDefaultSize, 0);
    StaticText8  = new wxStaticText(p, wxID_ANY, _("-nomonsters"), wxPoint(8,232), wxDefaultSize, 0);
    StaticText9  = new wxStaticText(p, wxID_ANY, _("Consoleplayer"), wxPoint(8,264), wxDefaultSize, 0);
    StaticText10 = new wxStaticText(p, wxID_ANY, _("Players"), wxPoint(8,296), wxDefaultSize, 0);
    StaticText11 = new wxStaticText(p, wxID_ANY, _("Monsters \nremember targets"), wxPoint(114,4), wxDefaultSize, 0);
    StaticText12 = new wxStaticText(p, wxID_ANY, _("Variable friction"), wxPoint(114,40), wxDefaultSize, 0);
    StaticText13 = new wxStaticText(p, wxID_ANY, _("Weapon recoil"), wxPoint(114,72), wxDefaultSize, 0);
    StaticText14 = new wxStaticText(p, wxID_ANY, _("Allow pushers"), wxPoint(114,104), wxDefaultSize, 0);
    StaticText15 = new wxStaticText(p, wxID_ANY, _("Player bobbing"), wxPoint(114,136), wxDefaultSize, 0);
    StaticText16 = new wxStaticText(p, wxID_ANY, _("Demo insurance"), wxPoint(114,168), wxDefaultSize, 0);
    StaticText17 = new wxStaticText(p, wxID_ANY, _("(P)RNG seed"), wxPoint(114,200), wxDefaultSize, 0);
    StaticText18 = new wxStaticText(p, wxID_ANY, _("Monster infighting"), wxPoint(114,232), wxDefaultSize, 0);
    StaticText19 = new wxStaticText(p, wxID_ANY, _("Dogs (0-3)"), wxPoint(240,8), wxDefaultSize, 0);
    StaticText20 = new wxStaticText(p, wxID_ANY, _("Friend distance"), wxPoint(240,40), wxDefaultSize, 0);
    StaticText21 = new wxStaticText(p, wxID_ANY, _("Monsters can \ngo backwards"), wxPoint(240,68), wxDefaultSize, 0);
    StaticText22 = new wxStaticText(p, wxID_ANY, _("Monsters avoid\n hazards"), wxPoint(240,100), wxDefaultSize, 0);
    StaticText23 = new wxStaticText(p, wxID_ANY, _("Friction applies\n to monsters"), wxPoint(240,132), wxDefaultSize, 0);
    StaticText24 = new wxStaticText(p, wxID_ANY, _("Monsters help friends"), wxPoint(240,168), wxDefaultSize, 0);
    StaticText25 = new wxStaticText(p, wxID_ANY, _("Dogs can jump down"), wxPoint(240,200), wxDefaultSize, 0);
    StaticText26 = new wxStaticText(p, wxID_ANY, _("Climb steep stairs"), wxPoint(240,232), wxDefaultSize, 0);
    StaticText27 = new wxStaticText(p, wxID_ANY, _("Compatflags"), wxPoint(176,264), wxDefaultSize, 0);
    StaticText28 = new wxStaticText(p, wxID_ANY, _("Longtics"), wxPoint(176,296), wxDefaultSize, 0);

    complevel         = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,4),    wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<signed int>());
    skill             = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,36),   wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    episode           = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,68),   wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    map               = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,100),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    deathmatch        = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,132),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    respawn           = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,164),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    fast              = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,196),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    nomonsters        = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,228),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    consoleplayer     = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,260),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    players           = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(80,292),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    monsters_remember = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(204,4),   wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    variable_friction = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(204,36),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    weapon_recoil     = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(204,68),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    allow_pushers     = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(204,100), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    player_bobbing    = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(204,132), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    demo_insurance    = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(204,164), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    rngseed           = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(196,196), wxSize(40,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    monster_infighting= new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(204,228), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    dogs              = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(346,4),   wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    distfriend        = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(338,36),  wxSize(40,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    monster_backing   = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(346,68),  wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    monster_avoid_hazards=new wxTextCtrl(p,wxID_ANY,_(""), wxPoint(346,100), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    monster_friction  = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(346,132), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    help_friends      = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(346,164), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    dog_jumping       = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(346,196), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    monkeys           = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(346,228), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    compatflags       = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(240,260), wxSize(64,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());
    longtics          = new wxTextCtrl(p, wxID_ANY, _(""), wxPoint(240,292), wxSize(32,21), wxTE_RIGHT, wxIntegerValidator<unsigned int>());

    xdre::getOptions(options);
    complevel->ChangeValue(wxString::FromCDouble(options[0], 0));
    skill->ChangeValue(wxString::FromCDouble(options[1], 0));
    episode->ChangeValue(wxString::FromCDouble(options[2], 0));
    map->ChangeValue(wxString::FromCDouble(options[3], 0));
    deathmatch->ChangeValue(wxString::FromCDouble(options[4], 0));
    respawn->ChangeValue(wxString::FromCDouble(options[5], 0));
    fast->ChangeValue(wxString::FromCDouble(options[6], 0));
    nomonsters->ChangeValue(wxString::FromCDouble(options[7], 0));
    consoleplayer->ChangeValue(wxString::FromCDouble(options[8], 0));
    players->ChangeValue(wxString::FromCDouble(options[9], 0));
    monsters_remember->ChangeValue(wxString::FromCDouble(options[10], 0));
    variable_friction->ChangeValue(wxString::FromCDouble(options[11], 0));
    weapon_recoil->ChangeValue(wxString::FromCDouble(options[12], 0));
    allow_pushers->ChangeValue(wxString::FromCDouble(options[13], 0));
    player_bobbing->ChangeValue(wxString::FromCDouble(options[14], 0));
    demo_insurance->ChangeValue(wxString::FromCDouble(options[15], 0));
    rngseed->ChangeValue(wxString::FromCDouble(options[16], 0));
    monster_infighting->ChangeValue(wxString::FromCDouble(options[17], 0));
    dogs->ChangeValue(wxString::FromCDouble(options[18], 0));
    distfriend->ChangeValue(wxString::FromCDouble(options[19], 0));
    monster_backing->ChangeValue(wxString::FromCDouble(options[20], 0));
    monster_avoid_hazards->ChangeValue(wxString::FromCDouble(options[21], 0));
    monster_friction->ChangeValue(wxString::FromCDouble(options[22], 0));
    help_friends->ChangeValue(wxString::FromCDouble(options[23], 0));
    dog_jumping->ChangeValue(wxString::FromCDouble(options[24], 0));
    monkeys->ChangeValue(wxString::FromCDouble(options[25], 0));
    compatflags->ChangeValue(wxString::FromCDouble(options[26], 0));
    longtics->ChangeValue(wxString::FromCDouble(options[27], 0));

    Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(HeaderDialog::OnClose));
    Connect(ID_CLOSE, wxEVT_BUTTON, wxCloseEventHandler(HeaderDialog::OnClose));
    Connect(ID_CHANGE, wxEVT_BUTTON, wxCommandEventHandler(HeaderDialog::OnApply));
    Connect(ID_HELP, wxEVT_BUTTON, wxCommandEventHandler(HeaderDialog::OnHelp));
}

HeaderDialog::~HeaderDialog() {}

void HeaderDialog::OnClose(wxCloseEvent& event) {Destroy();}

void HeaderDialog::OnApply(wxCommandEvent& event) {
    try {
        signed long i;
        if(complevel->GetValue().ToCLong(&i))         options[0] = i;
        if(options[0] < -1 || options[0] > 17 || options[0] == 10 || options[0] == 12)
            throw HeaderException("Invalid complevel.");
        if(skill->GetValue().ToCLong(&i))             options[1] = i;
        if(episode->GetValue().ToCLong(&i))           options[2] = i;
        if(map->GetValue().ToCLong(&i))               options[3] = i;
        if(deathmatch->GetValue().ToCLong(&i))        options[4] = i;
        if(respawn->GetValue().ToCLong(&i))           options[5] = i;
        if(fast->GetValue().ToCLong(&i))              options[6] = i;
        if(nomonsters->GetValue().ToCLong(&i))        options[7] = i;
        //consoleplayer will change in x_changeOptions() if it's invalid
        if(consoleplayer->GetValue().ToCLong(&i))     options[8] = i;
        if(options[8] > 3) throw HeaderException("Invalid consoleplayer.");
        if(players->GetValue().ToCLong(&i))           options[9] = i;
        if(options[9] > 15 || !options[9])
            throw HeaderException("Invalid players.");
        if(monsters_remember->GetValue().ToCLong(&i)) options[10] = i;
        if(variable_friction->GetValue().ToCLong(&i)) options[11] = i;
        if(weapon_recoil->GetValue().ToCLong(&i))     options[12] = i;
        if(allow_pushers->GetValue().ToCLong(&i))     options[13] = i;
        if(player_bobbing->GetValue().ToCLong(&i))    options[14] = i;
        if(demo_insurance->GetValue().ToCLong(&i))    options[15] = i;
        if(rngseed->GetValue().ToCLong(&i))           options[16] = i;
        if(monster_infighting->GetValue().ToCLong(&i))options[17] = i;
        if(dogs->GetValue().ToCLong(&i))              options[18] = i;
        if(distfriend->GetValue().ToCLong(&i))        options[19] = i;
        if(monster_backing->GetValue().ToCLong(&i))   options[20] = i;
        if(monster_avoid_hazards->GetValue().ToCLong(&i))options[21] = i;
        if(monster_friction->GetValue().ToCLong(&i))  options[22] = i;
        if(help_friends->GetValue().ToCLong(&i))      options[23] = i;
        if(dog_jumping->GetValue().ToCLong(&i))       options[24] = i;
        if(monkeys->GetValue().ToCLong(&i))           options[25] = i;
        if(compatflags->GetValue().ToCLong(&i))       options[26] = i;
        if(longtics->GetValue().ToCLong(&i))          options[27] = i;
        xdre::changeOptions(options);
    }
    catch(HeaderException& e) {
        wxMessageBox(e.what(), "Invalid value(s)",
                     wxOK|wxCENTRE|wxICON_ERROR, this);
    }
}

void HeaderDialog::OnHelp(wxCommandEvent& event) {
        wxMessageBox(
        "Complevel is compatibility level (from -1 to 17, but 10 and 12 don't work).\n"
        "Skill level goes from 1 to 5 (like -skill parameter).\n"
        "Episode matters only in Doom 1.\n"
        "Deathmatch is multiplayer mode: 1 means -deathmatch, 2 means -altdeath, 0 means singleplayer or co-op.\n"
        "RNG seed only exists in Boom, MBF and PrBoom+ demo headers. Changing it with complevels 0-7 does nothing.\n"
        "Point of view is zero-indexed (0 is player 1).\n"
        "Players field is a bit more complicated, it uses bit flags. 1 is player 1, 2 is player 2, 4 is player 3, "
        "8 is player 4. For example, 15 would be all 4 players.",
        "Help",
        wxOK|wxICON_INFORMATION, this);
}

