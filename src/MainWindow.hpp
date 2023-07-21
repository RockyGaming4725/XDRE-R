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
 * Description: main window declarations.
*/

#ifndef __MAINWINDOW_HPP
#define __MAINWINDOW_HPP

#include <functional>

#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/frame.h>
#include "TiclistCtrl.hpp"
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/fileconf.h>

enum class XCmd {
    //commands without value, let exit be last of these
    Fire,
    Use,
    TurnStepLeft,
    TurnStepRight,
    PrevTic,
    NextTic,
    CopyTic,
    DeleteTic,
    NewTic,
    ToggleBlood,
    SetSavepoint,
    UnsetSavepoint,
    Load,
    Save,
    Exit,
    //commands with one digit value, let weapon be last of these
    ChangePlayer,
    TransposeLeft,
    TransposeRight,
    Weapon,
    //commands with char
    RunBwd,
    RunFwd,
    StrafeLeft,
    StrafeRight,
    TurnStep,
    //commands with bigger value
    CopyTicX,
    DeleteTicX,
    TurnLeft,
    TurnRight,
    BackwardDemo,
    ForwardDemo
};

WX_DECLARE_STRING_HASH_MAP(XCmd, StringToCommandMap);

class MainWindow : public wxFrame {
public:
    MainWindow(wxWindow* parent = NULL, wxWindowID id = wxID_ANY, wxPoint const& pos = wxDefaultPosition, wxSize const& size = wxDefaultSize);
    virtual ~MainWindow();
    void SetKeys();
#ifdef _WIN32
    wxFileConfig config {"xdre", "wxEmptyString", "xdre.ini"};
#else
    wxFileConfig config {"xdre", "wxEmptyString", ".xdre/xdre"};
#endif // _WIN32

    wxPanel* p;
    wxStaticText *StaticText1, *StaticText2, *StaticText3, *StaticText4, *StaticText5, *StaticText6, *StaticText7, *StaticText8, *StaticText9, *StaticText10, *StaticText11, *StaticText12,
*StaticText13, *StaticText14, *StaticText15, *StaticText16, *StaticText17, *StaticText18, *StaticText19, *StaticText20, *StaticText21, *StaticText22, *StaticText23, *StaticText24, *StaticText25,
*StaticText26, *StaticText27, *StaticText28, *StaticText29, *StaticText30, *StaticText31, *StaticText32, *StaticText33, *StaticText34, *StaticText35, *StaticText36, *StaticText37, *StaticText38;
    wxTextCtrl *xPos, *yPos, *zPos, *xDist, *yDist, *zDist, *xMom, *yMom, *rngIndex, *useSuccess, *inputField, *playerAngle, *savepointTic, *distanceMoved, *directionMoved, *linedefXV1,
*linedefYV1, *linedefXV2, *linedefYV2, *crossed, *distanceFromLine, *floorheight, *ceilingheight, *special, *damageDone, *time, *currentTic, *thingX, *thingY, *thingZ, *thingRadius, *thingTics,
*thingHealth, *thingReactionTime, *thingThreshold;
    wxMenu *menuFile, *menuDemo, *menuTools, *menuOptions, *menuTracking;
    wxChoice *AngleType, *styleChoice;
    wxButton *linedefAdd, *sectorAdd, *thingAdd;
    wxComboBox *linedefInputField, *sectorInputField, *thingInputField;
    TiclistCtrl *ticlist;
    wxMenuItem *MenuItem1, *MenuItem2, *MenuItem3, *MenuItem4, *MenuItem5, *MenuItem6, *MenuItem7, *MenuItem8, *MenuItem9, *MenuItem10, *MenuItem11, *MenuItem12;
    wxMenuBar *menuBar;
protected:
    static const long ID_INPUTFIELD, ID_LINEDEFCONTROL, ID_LINEADD, ID_SECTORCONTROL, ID_SECTORADD, ID_THINGCONTROL, ID_THINGADD, ID_STYLECHOICE, ID_ANGLETYPE, IDM_LOAD, IDM_SAVE, IDM_EXIT,
IDM_BRUTE, IDM_PALSTUFF, IDM_SETKEYS, IDM_HEADER, IDM_CLEARTRACES, IDM_PUSHBACKTICS, IDM_THINGTRACKING, IDM_LINETRACKING,IDM_SECTORTRACKING;
private:
    void DoInput(XCmd cmd, unsigned int val = 0);
    void RefreshStuff();
    void RS();
    void WriteConfig();
    void Save();
    void Load();
    void Quit(bool canVeto = true);

    wxStringToStringHashMap configToStringMap {}; // config -> textCommand
    StringToCommandMap stringToCommandMap {}; // textCommand -> XCmd

    std::vector<wxWindow*> thingTracks;
    std::vector<wxWindow*> lineTracks;
    std::vector<wxWindow*> sectorTracks;
    bool pushBackTics;
    //variable for "STRING" in returnTrackVal("STRING")
    std::string check_type;

    void OnIdle(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnInputFieldText(wxCommandEvent& event);
    void OnAngleType(wxCommandEvent& event);
    void OnLinedefInput(wxCommandEvent& event);
    void OnLineAdd(wxCommandEvent& event);
    void OnSectorInput(wxCommandEvent& event);
    void OnSectorAdd(wxCommandEvent& event);
    void OnThingInput(wxCommandEvent& event);
    void OnThingAdd(wxCommandEvent& event);
    void OnMenuExit(wxCommandEvent& event);
    void OnMenuLoad(wxCommandEvent& event);
    void OnMenuSave(wxCommandEvent& event);
    void OnMenuBrute(wxCommandEvent& event);
    void OnMenuSetKeys(wxCommandEvent& event);
    void OnMenuPalStuff(wxCommandEvent& event);
    void OnMenuHeader(wxCommandEvent& event);
    void OnMenuClearTraces(wxCommandEvent& event);
    void OnStyleChoiceSelect(wxCommandEvent& event);
    void OnMenuPushTics(wxCommandEvent& event);
    void OnMenuThingTracking(wxCommandEvent& event);
    void OnMenuLineTracking(wxCommandEvent& event);
    void OnMenuSectorTracking(wxCommandEvent& event);
    //must be defined here, too lazy
    bool returnTrackVal(const std::string& check_type, wxFileConfig& config) {
    wxString value = config.Read((check_type), "true");
    if (value == "true") {return true;}
    if (value == "false") {return false;}
    else {return true;}
}

    DECLARE_EVENT_TABLE()
};

#endif // __MAINWINDOW_HPP

