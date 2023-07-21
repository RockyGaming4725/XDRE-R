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
 * Description: main XDRE window (command line, traces etc.).
*/

#include "MainWindow.hpp"

#include <wx/intl.h>
#include <wx/string.h>
#include <wx/valnum.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/ffile.h>
#include <wx/tokenzr.h>
#include <vector>
#include "BruteDialog.hpp"
#include "HeaderDialog.hpp"
#include "KeysDialog.hpp"
#include "xdre.hpp"
#include "config.h"


//for wxSscanf()
#include <wx/crt.h>

//for SDL_SetWindowPosition()
#include "SDL.h"
extern SDL_Window *sdl_window;


const long MainWindow::ID_INPUTFIELD = wxNewId();
const long MainWindow::ID_LINEDEFCONTROL = wxNewId();
const long MainWindow::ID_LINEADD = wxNewId();
const long MainWindow::ID_SECTORCONTROL = wxNewId();
const long MainWindow::ID_SECTORADD = wxNewId();
const long MainWindow::ID_THINGCONTROL = wxNewId();
const long MainWindow::ID_THINGADD = wxNewId();
const long MainWindow::ID_STYLECHOICE = wxNewId();
const long MainWindow::ID_ANGLETYPE = wxNewId();

const long MainWindow::IDM_LOAD = wxNewId();
const long MainWindow::IDM_SAVE = wxNewId();
const long MainWindow::IDM_EXIT = wxNewId();
const long MainWindow::IDM_BRUTE = wxNewId();
const long MainWindow::IDM_PALSTUFF = wxNewId();
const long MainWindow::IDM_SETKEYS = wxNewId();
const long MainWindow::IDM_HEADER = wxNewId();
const long MainWindow::IDM_CLEARTRACES = wxNewId();
const long MainWindow::IDM_PUSHBACKTICS = wxNewId();
const long MainWindow::IDM_THINGTRACKING = wxNewId();
const long MainWindow::IDM_LINETRACKING = wxNewId();
const long MainWindow::IDM_SECTORTRACKING = wxNewId();

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
END_EVENT_TABLE()

MainWindow::MainWindow(wxWindow* parent, wxWindowID id, wxPoint const& pos, wxSize const& size) {
    SetKeys();

    Create(parent, id, PACKAGE_STRING, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
    SetClientSize(wxSize(640,-1));
    int x, y;
    //"-1,-1" is wxDefaultPosition
    wxSscanf(config.Read("WindowPos", "-1,-1"), "%d,%d", &x, &y);
    Move(x, y);

    wxString str(config.Read("DoomWindowPos", "center"));
    if(str == "centre" || str == "center")
        SDL_SetWindowPosition(sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    else {
        wxSscanf(str, "%d,%d", &x, &y);
        SDL_SetWindowPosition(sdl_window, x, y);
    }
    str = config.Read("SavepointMode", "1");
    str.ToLong((long*)&x);
    if(x < SP_NOSAVE && x > SP_AUTO) x = SP_START;
    xdre::setSavepointStyle(x);
    str.Clear();

    SetMinSize(wxSize(640,520));
    SetMaxSize(wxSize(640,-1));
    p = new wxPanel(this);
    StaticText1  = new wxStaticText(p, wxID_ANY, _("(P)RNG index"), wxPoint(476,44));
    StaticText2  = new wxStaticText(p, wxID_ANY, _("Input"), wxPoint(526,12));

    StaticText3  = new wxStaticText(p, wxID_ANY, _("X"), wxPoint(236,76));
    StaticText4  = new wxStaticText(p, wxID_ANY, _("Y"), wxPoint(380,76));
    StaticText24 = new wxStaticText(p, wxID_ANY,_("Z"), wxPoint(524,76));

    StaticText22 = new wxStaticText(p, wxID_ANY, _("X dist."), wxPoint(236,108));
    StaticText23 = new wxStaticText(p, wxID_ANY, _("Y dist."), wxPoint(380,108));
    StaticText25 = new wxStaticText(p, wxID_ANY, _("Z dist."), wxPoint(524,108));

    StaticText5  = new wxStaticText(p, wxID_ANY, _("X mom."), wxPoint(236,140));
    StaticText6  = new wxStaticText(p, wxID_ANY, _("Y mom."), wxPoint(380,140));

    StaticText7  = new wxStaticText(p, wxID_ANY, _("Distance moved"), wxPoint(444,172));
    StaticText8  = new wxStaticText(p, wxID_ANY, _("Direction moved"),wxPoint(444,204));
    StaticText9  = new wxStaticText(p, wxID_ANY, _("Angle"), wxPoint(496,236));

    StaticText10 = new wxStaticText(p, wxID_ANY, _("Linedef"), wxPoint(472,300));
    StaticText11 = new wxStaticText(p, wxID_ANY, _("Hit special"), wxPoint(502,332));
    StaticText12 = new wxStaticText(p, wxID_ANY, _("X"), wxPoint(528,352));
    StaticText13 = new wxStaticText(p, wxID_ANY, _("Y"), wxPoint(584,352));
    StaticText14 = new wxStaticText(p, wxID_ANY, _("V. 1"), wxPoint(480,372));
    StaticText15 = new wxStaticText(p, wxID_ANY, _("V. 2"), wxPoint(480,396));
    StaticText16 = new wxStaticText(p, wxID_ANY, _("Dist. from line"), wxPoint(432,428));

    StaticText17 = new wxStaticText(p, wxID_ANY, _("Current tic"), wxPoint(64,428));
    StaticText18 = new wxStaticText(p, wxID_ANY, _("Used a line"), wxPoint(196,364));
    StaticText19 = new wxStaticText(p, wxID_ANY, _("Damage done"), wxPoint(184,396));
    StaticText20 = new wxStaticText(p, wxID_ANY, _("Time"), wxPoint(224,428));
    StaticText21 = new wxStaticText(p, wxID_ANY, _("Savepoint"), wxPoint(68,396));

    StaticText26 = new wxStaticText(p, wxID_ANY, _("Sector"), wxPoint(372,324));
    StaticText27 = new wxStaticText(p, wxID_ANY, _("Floor"), wxPoint(372,348));
    StaticText28 = new wxStaticText(p, wxID_ANY, _("Ceiling"), wxPoint(372,372));
    StaticText29 = new wxStaticText(p, wxID_ANY, _("Effect"), wxPoint(372,396));

    StaticText30 = new wxStaticText(p, wxID_ANY, _("Thing"), wxPoint(188,168));
    StaticText31 = new wxStaticText(p, wxID_ANY, _("X"), wxPoint(188,192));
    StaticText32 = new wxStaticText(p, wxID_ANY, _("Y"), wxPoint(188,216));
    StaticText33 = new wxStaticText(p, wxID_ANY, _("Z"), wxPoint(188,240));
    StaticText34 = new wxStaticText(p, wxID_ANY, _("Radius"), wxPoint(188,264));
    StaticText35 = new wxStaticText(p, wxID_ANY, _("Tics"), wxPoint(320,192));
    StaticText36 = new wxStaticText(p, wxID_ANY, _("Health"), wxPoint(320,216));
    StaticText37 = new wxStaticText(p, wxID_ANY, _("Reaction time"), wxPoint(320,240));
    StaticText38 = new wxStaticText(p, wxID_ANY, _("Threshold"), wxPoint(320,264));

    inputField = new wxTextCtrl(p, ID_INPUTFIELD, wxEmptyString, wxPoint(568,8), wxSize(52,21), wxTE_RIGHT);
    inputField->SetMaxLength(10);
    rngIndex = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(568,40), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);

    xPos = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(288,72), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);
    yPos = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(432,72), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);
    zPos = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(568,72), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);

    xMom = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(288,136), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);
    yMom = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(432,136), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);

    xDist = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(288,104), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);
    yDist = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(432,104), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);
    zDist = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(568,104), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);

    distanceMoved = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(536,168), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);
    directionMoved = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(536,200), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);
    playerAngle = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(536,232), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);

    AngleType = new wxChoice(p, ID_ANGLETYPE, wxPoint(508,264), wxSize(108,28), 0, 0, 0, wxDefaultValidator);
    AngleType->Append(_("longtics"));
    AngleType->Append(_("fine angles"));
    AngleType->Append(_("degrees"));
    AngleType->Append(_("byte angles"));
    AngleType->SetSelection(config.ReadLong("AngleType", 0));

    linedefInputField = new wxComboBox(p, ID_LINEDEFCONTROL, wxEmptyString, wxPoint(524,296), wxSize(60,28), 0, 0, 0, wxIntegerValidator<unsigned int>());
    linedefInputField->SetMaxLength(10);

    wxStringTokenizer tok(config.Read("LinedefNums", "0"), ",");
    while(tok.HasMoreTokens()) linedefInputField->Append(tok.GetNextToken());

    linedefInputField->SetSelection(0);
    linedefAdd       = new wxButton(p, ID_LINEADD, _("Add"), wxPoint(585,296), wxSize(26,21), 0);
    crossed          = new wxTextCtrl(p, wxID_ANY, _("false"), wxPoint(560,328), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);
    linedefXV1       = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(504,368), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);
    linedefYV1       = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(560,368), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);
    linedefXV2       = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(504,392), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);
    linedefYV2       = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(560,392), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);
    distanceFromLine = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(528,424), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);

    sectorInputField = new wxComboBox(p, ID_SECTORCONTROL, wxEmptyString, wxPoint(410,320), wxSize(53,21), 0, 0, 0, wxIntegerValidator<unsigned int>());
    sectorInputField->SetMaxLength(10);

    tok.SetString(config.Read("SectorNums", "0"), ",");
    while(tok.HasMoreTokens()) sectorInputField->Append(tok.GetNextToken());

    sectorInputField->SetSelection(0);
    sectorAdd     = new wxButton(p, ID_SECTORADD, _("Add"), wxPoint(465,318), wxSize(26,21), 0);
    floorheight   = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(410,344), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);
    ceilingheight = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(410,368), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);
    special       = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(410,392), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);

    thingInputField = new wxComboBox(p, ID_THINGCONTROL, wxEmptyString, wxPoint(232,164), wxSize(60,21), 0, 0, 0, wxIntegerValidator<unsigned int>());
    thingInputField->SetMaxLength(10);

    tok.SetString(config.Read("ThingNums", "0"), ",");
    while(tok.HasMoreTokens()) thingInputField->Append(tok.GetNextToken());

    thingInputField->SetSelection(0);
    thingAdd          = new wxButton(p, ID_THINGADD, _("Add"), wxPoint(296,164), wxSize(26,21), 0);
    thingX            = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(232,188), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);
    thingY            = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(232,212), wxSize(84,21), wxTE_READONLY|wxTE_RIGHT);
    thingZ            = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(232,236), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);
    thingRadius       = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(232,260), wxSize(52,21), wxTE_READONLY|wxTE_RIGHT);
    thingTics         = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(388,188), wxSize(40,21), wxTE_READONLY|wxTE_RIGHT);
    thingHealth       = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(388,212), wxSize(40,21), wxTE_READONLY|wxTE_RIGHT);
    thingReactionTime = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(388,236), wxSize(40,21), wxTE_READONLY|wxTE_RIGHT);
    thingThreshold    = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(388,260), wxSize(40,21), wxTE_READONLY|wxTE_RIGHT);

    useSuccess = new wxTextCtrl(p, wxID_ANY, _("false"), wxPoint(260,360), wxSize(60,21), wxTE_READONLY|wxTE_RIGHT);
    damageDone = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(260,392), wxSize(60,21), wxTE_READONLY|wxTE_RIGHT);
    time = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(260,424), wxSize(60,21), wxTE_READONLY|wxTE_RIGHT);
    currentTic = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(120,424), wxSize(60,21), wxTE_READONLY|wxTE_RIGHT);
    ticlist = new TiclistCtrl(p, wxID_ANY, wxPoint(16,16), wxSize(170,368));
    savepointTic = new wxTextCtrl(p, wxID_ANY, _("0"), wxPoint(120,392), wxSize(60,21), wxTE_READONLY|wxTE_RIGHT);
    styleChoice = new wxChoice(p, ID_STYLECHOICE, wxPoint(16,392), wxSize(48,21), 0, 0, 0);
    styleChoice->Append(_("User"));
    styleChoice->Append(_("Start"));
    styleChoice->Append(_("Auto"));
    styleChoice->SetSelection(x);

    menuBar   = new wxMenuBar();
    menuFile  = new wxMenu();
    MenuItem1 = new wxMenuItem(menuFile, IDM_LOAD, _("Load"), wxEmptyString, wxITEM_NORMAL);
    MenuItem2 = new wxMenuItem(menuFile, IDM_SAVE, _("Save"), wxEmptyString, wxITEM_NORMAL);
    MenuItem3 = new wxMenuItem(menuFile, IDM_EXIT, _("Exit"), wxEmptyString, wxITEM_NORMAL);
    MenuItem4 = new wxMenuItem(menuTools, IDM_BRUTE, _("Brute force"), wxEmptyString, wxITEM_NORMAL);
    MenuItem5 = new wxMenuItem(menuOptions, IDM_SETKEYS, _("Set keys"), wxEmptyString, wxITEM_NORMAL);
    MenuItem6 = new wxMenuItem(menuTools, IDM_HEADER, _("Edit header"), wxEmptyString, wxITEM_NORMAL);
    menuTools = new wxMenu();
    menuOptions = new wxMenu();
    MenuItem7 = new wxMenuItem(menuOptions, IDM_PALSTUFF, _("Palette stuff"), wxEmptyString, wxITEM_CHECK);
    MenuItem8 = new wxMenuItem(menuOptions, IDM_CLEARTRACES, _("Clear all traces"), wxEmptyString, wxITEM_NORMAL);
    MenuItem12 = new wxMenuItem(menuOptions, IDM_PUSHBACKTICS, _("Push back brute force tics"), wxEmptyString, wxITEM_CHECK);
    menuTracking = new wxMenu();
    MenuItem9 = new wxMenuItem(menuTracking, IDM_THINGTRACKING, _("Show thing tracking"), wxEmptyString, wxITEM_CHECK);
    MenuItem10 = new wxMenuItem(menuTracking, IDM_LINETRACKING, _("Show line tracking"), wxEmptyString, wxITEM_CHECK);
    MenuItem11 = new wxMenuItem(menuTracking, IDM_SECTORTRACKING, _("Show sector tracking"), wxEmptyString, wxITEM_CHECK);
    menuDemo  = new wxMenu();
    SetMenuBar(menuBar);

    menuBar->Append(menuFile, _("File"));
    menuBar->Append(menuTools, _("Tools"));
    menuBar->Append(menuOptions, _("Options"));
    menuBar->Append(menuTracking, _("Tracking"));

    menuFile->Append(MenuItem1);
    menuFile->Append(MenuItem2);
    menuFile->Append(MenuItem3);
    menuTools->Append(MenuItem4);
    menuTools->Append(MenuItem6);
    menuOptions->Append(MenuItem5);
    menuOptions->Append(MenuItem8);
    menuOptions->Append(MenuItem7);
    menuOptions->Append(MenuItem12);
    menuTracking->Append(MenuItem9);
    menuTracking->Append(MenuItem10);
    menuTracking->Append(MenuItem11);

    MenuItem7->Check(returnTrackVal("EnablePalette", config));
    MenuItem9->Check(returnTrackVal("ThingTracking", config));
    MenuItem10->Check(returnTrackVal("LineTracking", config));
    MenuItem11->Check(returnTrackVal("SectorTracking", config));
    MenuItem12->Check(returnTrackVal("PushBackTics", config));

    Connect(ID_INPUTFIELD, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(MainWindow::OnInputFieldText));

    Connect(ID_ANGLETYPE, wxEVT_CHOICE, wxCommandEventHandler(MainWindow::OnAngleType));

    Connect(ID_LINEDEFCONTROL, wxEVT_TEXT, wxCommandEventHandler(MainWindow::OnLinedefInput));
    Connect(ID_LINEDEFCONTROL, wxEVT_COMBOBOX, wxCommandEventHandler(MainWindow::OnLinedefInput));
    Connect(ID_LINEADD, wxEVT_BUTTON, wxCommandEventHandler(MainWindow::OnLineAdd));

    Connect(ID_SECTORCONTROL, wxEVT_TEXT, wxCommandEventHandler(MainWindow::OnSectorInput));
    Connect(ID_SECTORCONTROL, wxEVT_COMBOBOX, wxCommandEventHandler(MainWindow::OnSectorInput));
    Connect(ID_SECTORADD, wxEVT_BUTTON, wxCommandEventHandler(MainWindow::OnSectorAdd));

    Connect(ID_THINGCONTROL, wxEVT_TEXT, wxCommandEventHandler(MainWindow::OnThingInput));
    Connect(ID_THINGCONTROL, wxEVT_COMBOBOX, wxCommandEventHandler(MainWindow::OnThingInput));
    Connect(ID_THINGADD, wxEVT_BUTTON, wxCommandEventHandler(MainWindow::OnThingAdd));

    Connect(ID_STYLECHOICE, wxEVT_CHOICE, wxCommandEventHandler(MainWindow::OnStyleChoiceSelect));
    Connect(IDM_LOAD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuLoad));
    Connect(IDM_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuSave));
    Connect(IDM_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuExit));
    Connect(IDM_BRUTE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuBrute));
    Connect(IDM_SETKEYS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuSetKeys));
    Connect(IDM_PALSTUFF, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuPalStuff));
    Connect(IDM_HEADER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuHeader));
    Connect(IDM_CLEARTRACES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuClearTraces));
    Connect(IDM_PUSHBACKTICS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuPushTics));
    Connect(IDM_THINGTRACKING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuThingTracking));
    Connect(IDM_LINETRACKING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuLineTracking));
    Connect(IDM_SECTORTRACKING, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnMenuSectorTracking));
    Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainWindow::OnClose));
    Connect(wxID_ANY, wxEVT_IDLE, wxCommandEventHandler(MainWindow::OnIdle));

    xdre::setAngleType(AngleType->GetSelection());
    linedefXV1->ChangeValue(wxString::FromCDouble(xdre::getLinedefVertex(0, 0), 0));
    linedefYV1->ChangeValue(wxString::FromCDouble(xdre::getLinedefVertex(0, 1), 0));
    linedefXV2->ChangeValue(wxString::FromCDouble(xdre::getLinedefVertex(1, 0), 0));
    linedefYV2->ChangeValue(wxString::FromCDouble(xdre::getLinedefVertex(1, 1), 0));

//Create vectors of stuff for hiding/showing more efficiently
    thingTracks.push_back(StaticText30);
    thingTracks.push_back(StaticText31);
    thingTracks.push_back(StaticText32);
    thingTracks.push_back(StaticText33);
    thingTracks.push_back(StaticText34);
    thingTracks.push_back(StaticText35);
    thingTracks.push_back(StaticText36);
    thingTracks.push_back(StaticText37);
    thingTracks.push_back(StaticText38);
    thingTracks.push_back(thingInputField);
    thingTracks.push_back(thingAdd);
    thingTracks.push_back(thingX);
    thingTracks.push_back(thingY);
    thingTracks.push_back(thingZ);
    thingTracks.push_back(thingRadius);
    thingTracks.push_back(thingTics);
    thingTracks.push_back(thingHealth);
    thingTracks.push_back(thingReactionTime);
    thingTracks.push_back(thingThreshold);

    lineTracks.push_back(StaticText10);
    lineTracks.push_back(StaticText11);
    lineTracks.push_back(StaticText12);
    lineTracks.push_back(StaticText13);
    lineTracks.push_back(StaticText14);
    lineTracks.push_back(StaticText15);
    lineTracks.push_back(StaticText16);
    lineTracks.push_back(linedefInputField);
    lineTracks.push_back(linedefAdd);
    lineTracks.push_back(crossed);
    lineTracks.push_back(linedefXV1);
    lineTracks.push_back(linedefXV2);
    lineTracks.push_back(linedefYV1);
    lineTracks.push_back(linedefYV2);
    lineTracks.push_back(distanceFromLine);

    sectorTracks.push_back(StaticText26);
    sectorTracks.push_back(StaticText27);
    sectorTracks.push_back(StaticText28);
    sectorTracks.push_back(StaticText29);
    sectorTracks.push_back(sectorInputField);
    sectorTracks.push_back(sectorAdd);
    sectorTracks.push_back(floorheight);
    sectorTracks.push_back(ceilingheight);
    sectorTracks.push_back(special);

    for (wxWindow* widget : thingTracks) {widget->Show(returnTrackVal("ThingTracking", config));}
    for (wxWindow* widget : lineTracks) {widget->Show(returnTrackVal("LineTracking", config));}
    for (wxWindow* widget : sectorTracks) {widget->Show(returnTrackVal("SectorTracking", config));}
//we know if the menu is checked or not. XDRE always launches with palstuff on,
//so turn it off if the menu isn't checked. Terrible approach, but it works.
    if (!MenuItem7->IsChecked()) {xdre::toggleBlood();}


    RS();
}

MainWindow::~MainWindow() {}

void MainWindow::DoInput(XCmd cmd, unsigned int val) {
    switch(cmd) {
        case XCmd::Fire:          xdre::toggleFire();     RS(); break;
        case XCmd::Use:           xdre::toggleUse();      RS(); break;
        case XCmd::TurnStepLeft:  xdre::turnStepLeft();   RS(); break;
        case XCmd::TurnStepRight: xdre::turnStepRight();  RS(); break;
        case XCmd::TurnStep:      xdre::setTurnStep(val);       break;
        case XCmd::PrevTic:       xdre::seekDemo(-1);     RS(); break;
        case XCmd::NextTic:       xdre::seekDemo(1);      RS(); break;
        case XCmd::CopyTic:       xdre::copyTic();        RS(); break;
        case XCmd::CopyTicX:      xdre::copyTicX(val);    RS(); break;
        case XCmd::DeleteTic:     xdre::deleteTic();      RS(); break;
        case XCmd::DeleteTicX:    xdre::deleteTicX(val);  RS(); break;
        case XCmd::NewTic:        xdre::newTic();         RS(); break;
        case XCmd::ToggleBlood:   xdre::toggleBlood();MenuItem7->Check(xdre::getBlood());break;
        case XCmd::SetSavepoint:  xdre::setSavepoint();   RS(); break;
        case XCmd::UnsetSavepoint:xdre::unsetSavepoint(); RS(); break;
        case XCmd::Load:          Load();                       break;
        case XCmd::Save:          Save();                       break;
        case XCmd::Exit:          Quit();                       break;
        case XCmd::RunBwd:        xdre::run(-val);        RS(); break;
        case XCmd::RunFwd:        xdre::run(val);         RS(); break;
        case XCmd::StrafeLeft:    xdre::strafe(-val);     RS(); break;
        case XCmd::StrafeRight:   xdre::strafe(val);      RS(); break;
        case XCmd::TurnLeft:      xdre::turn(val);        RS(); break;
        case XCmd::TurnRight:     xdre::turn(-val);       RS(); break;
        case XCmd::Weapon:        xdre::changeWeapon(val);RS(); break;
        case XCmd::TransposeLeft: xdre::transpose(0, val);RS(); break;
        case XCmd::TransposeRight:xdre::transpose(1, val);RS(); break;
        case XCmd::BackwardDemo:  xdre::seekDemo(-val);   RS(); break;
        case XCmd::ForwardDemo:   xdre::seekDemo(val);    RS(); break;
        case XCmd::ChangePlayer:  xdre::changePlayerView(val);RS();break;
        default: break;
    }
}

void MainWindow::SetKeys() {
    configToStringMap.clear();
    configToStringMap["Fire"] = config.Read("Fire", "f");
    configToStringMap["Use"] = config.Read("Use", "u");
    configToStringMap["TurnStepLeft"] = config.Read("TurnStepLeft", "-");
    configToStringMap["TurnStepRight"] = config.Read("TurnStepRight", "+");
    configToStringMap["TurnStep"] = config.Read("TurnStep", "ts");
    configToStringMap["PrevTic"] = config.Read("PrevTic", "j");
    configToStringMap["NextTic"] = config.Read("NextTic", "i");
    configToStringMap["CopyTic"] = config.Read("CopyTic", "c");
    configToStringMap["DeleteTic"] = config.Read("DeleteTic", "x");;
    configToStringMap["NewTic"] = config.Read("NewTic", "n");
    configToStringMap["ToggleBlood"] = config.Read("ToggleBlood", "z");
    configToStringMap["SetSavepoint"] = config.Read("SetSavepoint", "ssp");
    configToStringMap["UnsetSavepoint"] = config.Read("UnsetSavepoint", "pss");
    configToStringMap["Load"] = config.Read("Load", "load");
    configToStringMap["Save"] = config.Read("Save", "save");
    configToStringMap["Exit"] = config.Read("Exit", "exit");

    configToStringMap["RunBwd"] = config.Read("RunBwd", "wq");
    configToStringMap["RunFwd"] = config.Read("RunFwd", "we");
    configToStringMap["StrafeLeft"] = config.Read("StrafeLeft", "sa");
    configToStringMap["StrafeRight"] = config.Read("StrafeRight", "sd");
    configToStringMap["TurnLeft"] = config.Read("TurnLeft", "re");
    configToStringMap["TurnRight"] = config.Read("TurnRight", "rt");
    configToStringMap["Weapon"] = config.Read("Weapon", "g");

    configToStringMap["TransposeLeft"] = config.Read("TransposeLeft", "tr");
    configToStringMap["TransposeRight"] = config.Read("TransposeRight", "ty");
    configToStringMap["CopyTicX"] = config.Read("CopyTicX", "tc");
    configToStringMap["DeleteTicX"] = config.Read("DeleteTicX", "tx");
    configToStringMap["BackwardDemo"] = config.Read("BackwardDemo", ",");
    configToStringMap["ForwardDemo"] = config.Read("ForwardDemo", ".");
    configToStringMap["ChangePlayer"] = config.Read("ChangePlayer", "v");

    stringToCommandMap.clear();
    stringToCommandMap[configToStringMap["Fire"]] = XCmd::Fire;
    stringToCommandMap[configToStringMap["Use"]] = XCmd::Use;
    stringToCommandMap[configToStringMap["TurnStepLeft"]] = XCmd::TurnStepLeft;
    stringToCommandMap[configToStringMap["TurnStepRight"]] = XCmd::TurnStepRight;
    stringToCommandMap[configToStringMap["TurnStep"]] = XCmd::TurnStep;
    stringToCommandMap[configToStringMap["PrevTic"]] = XCmd::PrevTic;
    stringToCommandMap[configToStringMap["NextTic"]] = XCmd::NextTic;
    stringToCommandMap[configToStringMap["CopyTic"]] = XCmd::CopyTic;
    stringToCommandMap[configToStringMap["DeleteTic"]] = XCmd::DeleteTic;
    stringToCommandMap[configToStringMap["NewTic"]] = XCmd::NewTic;
    stringToCommandMap[configToStringMap["ToggleBlood"]] = XCmd::ToggleBlood;
    stringToCommandMap[configToStringMap["SetSavepoint"]] = XCmd::SetSavepoint;
    stringToCommandMap[configToStringMap["UnsetSavepoint"]] = XCmd::UnsetSavepoint;
    stringToCommandMap[configToStringMap["Load"]] = XCmd::Load;
    stringToCommandMap[configToStringMap["Save"]] = XCmd::Save;
    stringToCommandMap[configToStringMap["Exit"]] = XCmd::Exit;

    stringToCommandMap[configToStringMap["RunBwd"]] = XCmd::RunBwd;
    stringToCommandMap[configToStringMap["RunFwd"]] = XCmd::RunFwd;
    stringToCommandMap[configToStringMap["StrafeLeft"]] = XCmd::StrafeLeft;
    stringToCommandMap[configToStringMap["StrafeRight"]] = XCmd::StrafeRight;
    stringToCommandMap[configToStringMap["TurnLeft"]] = XCmd::TurnLeft;
    stringToCommandMap[configToStringMap["TurnRight"]] = XCmd::TurnRight;
    stringToCommandMap[configToStringMap["Weapon"]] = XCmd::Weapon;

    stringToCommandMap[configToStringMap["TransposeLeft"]] = XCmd::TransposeLeft;
    stringToCommandMap[configToStringMap["TransposeRight"]] = XCmd::TransposeRight;
    stringToCommandMap[configToStringMap["CopyTicX"]] = XCmd::CopyTicX;
    stringToCommandMap[configToStringMap["DeleteTicX"]] = XCmd::DeleteTicX;
    stringToCommandMap[configToStringMap["BackwardDemo"]] = XCmd::BackwardDemo;
    stringToCommandMap[configToStringMap["ForwardDemo"]] = XCmd::ForwardDemo;
    stringToCommandMap[configToStringMap["ChangePlayer"]] = XCmd::ChangePlayer;
}

void MainWindow::RS() { //RefreshStuff
    rngIndex->ChangeValue(wxString::FromCDouble(xdre::getRngIndex()));
    xPos->ChangeValue(wxString::FromCDouble(xdre::getXPos(), 6));
    yPos->ChangeValue(wxString::FromCDouble(xdre::getYPos(), 6));
    zPos->ChangeValue(wxString::FromCDouble(xdre::getZPos(), 2)); //2 for slow stairs
    xMom->ChangeValue(wxString::FromCDouble(xdre::getXMom(), 6));
    yMom->ChangeValue(wxString::FromCDouble(xdre::getYMom(), 6));
    xDist->ChangeValue(wxString::FromCDouble(xdre::getXDist(), 6));
    yDist->ChangeValue(wxString::FromCDouble(xdre::getYDist(), 6));
    zDist->ChangeValue(wxString::FromCDouble(xdre::getZDist(), 2));
    distanceMoved->ChangeValue(wxString::FromCDouble(xdre::getDistanceMoved(), 6));
    directionMoved->ChangeValue(wxString::FromCDouble(xdre::getDirectionMoved(), 6));
    playerAngle->ChangeValue(wxString::FromCDouble(xdre::getAngle(0)));
    time->ChangeValue(wxString::FromCDouble(xdre::getTime(), 2));
    currentTic->ChangeValue(wxString::FromCDouble(xdre::getCurrentTic()));
    damageDone->ChangeValue(wxString::FromCDouble(xdre::getDoneDamage()));
    useSuccess->ChangeValue(xdre::getUseSuccess() ? "true" : "false");

    crossed->ChangeValue(xdre::getLinedefCrossed() ? "true" : "false");
    distanceFromLine->ChangeValue(wxString::FromCDouble(xdre::getDistanceFromLine(), 6));

    floorheight->ChangeValue(wxString::FromCDouble(xdre::getSectorInfo(0), 2));
    ceilingheight->ChangeValue(wxString::FromCDouble(xdre::getSectorInfo(1), 2));
    special->ChangeValue(wxString::FromCDouble(xdre::getSectorInfo(2)));

    thingX->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(0), 6));
    thingY->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(1), 6));
    thingZ->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(2), 2));
    thingRadius->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(3), 3));
    thingTics->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(4), 0));
    thingHealth->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(5), 0));
    thingReactionTime->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(6), 0));
    thingThreshold->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(7), 0));

    savepointTic->ChangeValue(wxString::FromCDouble(xdre::getSavepointTic()));
    styleChoice->SetSelection(xdre::getSavepointStyle());
    ticlist->SetItemCount(xdre::getTiclistSize());
    ticlist->SetItemState(xdre::getCurrentTic(), wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    ticlist->EnsureVisible(xdre::getCurrentTic());
    ticlist->Refresh();
}


void MainWindow::Save() {
    wxString filename = wxFileSelector(wxFileSelectorPromptStr,
                                       wxEmptyString,
                                       wxEmptyString,
                                       ".lmp",
                                       "LMP files (*.lmp)|*.lmp",
                                       wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if(!filename.empty()) {
        wxFFile file;
        if(file.Open(filename, "wb")) {
            if(!xdre::save(file.fp())) {
                wxMessageBox("Couldn't write demo. That's really weird and shouldn't happen.",
                             "Couldn't write demo",
                             wxOK | wxCENTRE | wxICON_ERROR,
                             this);
            }
        } else {
            wxMessageBox("Couldn't open file for write: " + filename,
                         "Couldn't open file",
                         wxOK | wxCENTRE | wxICON_ERROR,
                         this);
        }
    }
}

void MainWindow::Load() {
    wxString filename = wxFileSelector(wxFileSelectorPromptStr,
                                       wxEmptyString,
                                       wxEmptyString,
                                       ".lmp",
                                       "LMP files (*.lmp)|*.lmp",
                                       wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(!filename.empty()) {
        wxFFile file;
        if(file.Open(filename, "rb")) {
            if(!xdre::load(file.fp())) {
                wxMessageBox("Couldn't load demo. Debug it yourself to see what went wrong.",
                             "Couldn't load demo",
                             wxOK | wxCENTRE | wxICON_ERROR,
                             this);
            } else RS();
        } else {
            wxMessageBox("Couldn't open file for read: " + filename,
                         "Couldn't open file",
                         wxOK | wxCENTRE | wxICON_ERROR,
                         this);
        }
    }
}

void MainWindow::Quit(bool canVeto) {
    if(canVeto) {
        int ync = wxMessageBox("Do you want to save before quit?", "Save before quit", wxYES_NO | wxCANCEL, this);
        if(ync == wxCANCEL) return;
        else if(ync == wxYES) Save();
    }
    WriteConfig();
    Destroy();
}

void MainWindow::WriteConfig() {
    for(auto& key : configToStringMap) config.Write(key.first, key.second);

    unsigned int i, j;
    wxString s(wxEmptyString);
    int x, y;
    MainWindow::GetScreenPosition(&x, &y);
    s << x; s << ","; s << y;
    config.Write("WindowPos", s);
    s.Clear();
    SDL_GetWindowPosition(sdl_window, &x, &y);
    s << x; s << ","; s << y;
    config.Write("DoomWindowPos", s);
    s.Clear();
    config.Write("AngleType", AngleType->GetSelection());
    config.Write("SavepointMode", styleChoice->GetSelection());

    j = linedefInputField->GetCount();
    for(i = 0; i != j; i++) {
        if(linedefInputField->GetString(i) == wxEmptyString) continue;
        s << linedefInputField->GetString(i);
        if(j-1 > i) s << ",";
    }
    if(s.IsEmpty()) s = "0";
    config.Write("LinedefNums", s);
    s.Clear();

    j = sectorInputField->GetCount();
    for(i = 0; i != j; i++) {
        if(sectorInputField->GetString(i) == wxEmptyString) continue;
        s << sectorInputField->GetString(i);
        if(j-1 > i) s << ",";
    }
    if(s.IsEmpty()) s = "0";
    config.Write("SectorNums", s);
    s.Clear();

    j = thingInputField->GetCount();
    for(i = 0; i != j; i++) {
        if(thingInputField->GetString(i) == wxEmptyString) continue;
        s << thingInputField->GetString(i);
        if(j-1 > i) s << ",";
    }
    if(s.IsEmpty()) s = "0";
    config.Write("ThingNums", s);
    s.Clear();

    config.Write("ThingTracking", wxString::Format("%s", MenuItem9->IsChecked() ? "true" : "false"));
    config.Write("LineTracking", wxString::Format("%s", MenuItem10->IsChecked() ? "true" : "false"));
    config.Write("SectorTracking", wxString::Format("%s", MenuItem11->IsChecked() ? "true" : "false"));
    config.Write("PushBackTics", wxString::Format("%s", MenuItem12->IsChecked() ? "true" : "false"));
    config.Write("EnablePalette", wxString::Format("%s", MenuItem7->IsChecked() ? "true" : "false"));
}

void MainWindow::OnIdle(wxCommandEvent& event) {xdre::doSdlEvents();}

void MainWindow::OnClose(wxCloseEvent& event) {Quit(event.CanVeto());}

void MainWindow::OnInputFieldText(wxCommandEvent& event) {
    wxString input = inputField->GetValue();

    if(input.length() < 1) return;

    wxString strCmd = input;
    unsigned long val = 0;
    int numDigits = 0;
    bool space = false;
    //plain cmd or with value?
    for(unsigned int i = 1; i < input.length(); ++i) {
        if(wxIsdigit(input[i])) {
            numDigits = input.Mid(i).First(" ");
            if(numDigits == wxNOT_FOUND) numDigits = 1;
            else space = true;

            //there should be digits only, is there?
            if(input.Mid(i, numDigits).ToULong(&val)) {
                strCmd = input.Mid(0, i);
                break; //yes
            } else return; //no
        }
    }

    auto keyvalue = stringToCommandMap.find(strCmd);
    if(keyvalue == stringToCommandMap.end()) return;

    // final checks D:
    if(keyvalue->second > XCmd::Exit && numDigits < 1) return;
    else if(keyvalue->second > XCmd::Weapon && !space) return;

    inputField->ChangeValue("");
    DoInput(keyvalue->second, val);
}

void MainWindow::OnAngleType(wxCommandEvent& event) {
    xdre::setAngleType(AngleType->GetSelection());
    playerAngle->ChangeValue(wxString::FromCDouble(xdre::getAngle(0)));
}

void MainWindow::OnLinedefInput(wxCommandEvent& event) {
    unsigned long val;
    if(linedefInputField->GetValue().ToULong(&val)) {
        xdre::setLinedefCheck(val);
        linedefXV1->ChangeValue(wxString::FromCDouble(xdre::getLinedefVertex(0, 0), 0));
        linedefYV1->ChangeValue(wxString::FromCDouble(xdre::getLinedefVertex(0, 1), 0));
        linedefXV2->ChangeValue(wxString::FromCDouble(xdre::getLinedefVertex(1, 0), 0));
        linedefYV2->ChangeValue(wxString::FromCDouble(xdre::getLinedefVertex(1, 1), 0));
        crossed->ChangeValue(xdre::getLinedefCrossed() ? "true" : "false");
        distanceFromLine->ChangeValue(wxString::FromCDouble(xdre::getDistanceFromLine(), 6));
    }
}

void MainWindow::OnLineAdd(wxCommandEvent& event) {
    if(linedefInputField->GetValue() != wxEmptyString)
        linedefInputField->Append(linedefInputField->GetValue());
}

void MainWindow::OnSectorInput(wxCommandEvent& event) {
    unsigned long val;
    if(sectorInputField->GetValue().ToULong(&val)) {
        xdre::setSectorCheck(val);
        floorheight->ChangeValue(wxString::FromCDouble(xdre::getSectorInfo(0), 2));
        ceilingheight->ChangeValue(wxString::FromCDouble(xdre::getSectorInfo(1), 2));
        special->ChangeValue(wxString::FromCDouble(xdre::getSectorInfo(2)));
    }
}

void MainWindow::OnSectorAdd(wxCommandEvent& event) {
    if(sectorInputField->GetValue() != wxEmptyString) sectorInputField->Append(sectorInputField->GetValue());
}

void MainWindow::OnThingInput(wxCommandEvent& event) {
    unsigned long val;
    if(thingInputField->GetValue().ToULong(&val)) {
        xdre::setThingCheck(val);
        thingX->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(0), 6));
        thingY->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(1), 6));
        thingZ->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(2), 2));
        thingRadius->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(3), 3));
        thingTics->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(4), 0));
        thingHealth->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(5), 0));
        thingReactionTime->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(6), 0));
        thingThreshold->ChangeValue(wxString::FromCDouble(xdre::getThingInfo(7), 0));
    }
}

void MainWindow::OnThingAdd(wxCommandEvent& event) {
    if(thingInputField->GetValue() != wxEmptyString)
        thingInputField->Append(thingInputField->GetValue());
}

void MainWindow::OnMenuPalStuff(wxCommandEvent& event) {
    xdre::toggleBlood();
    MenuItem7->Check(xdre::getBlood());
}

void MainWindow::OnMenuExit(wxCommandEvent& event) {Quit();}

void MainWindow::OnMenuLoad(wxCommandEvent& event) {Load();}

void MainWindow::OnMenuSave(wxCommandEvent& event) {Save();}

void MainWindow::OnMenuBrute(wxCommandEvent& event) {
    BruteDialog bruteDialog(this, NULL);
    bruteDialog.ShowModal();
    RS();
}

void MainWindow::OnMenuSetKeys(wxCommandEvent& event) {
    KeysDialog *keysDialog = new KeysDialog(this);
    keysDialog->Show(true);
}

void MainWindow::OnMenuHeader(wxCommandEvent& event) {
    HeaderDialog *headerDialog = new HeaderDialog(this);
    headerDialog->Show(true);
}

void MainWindow::OnMenuClearTraces(wxCommandEvent& event) {
    linedefInputField->Clear();
    sectorInputField->Clear();
    thingInputField->Clear();
    linedefInputField->Append("0");
    sectorInputField->Append("0");
    thingInputField->Append("0");

//Once cleared, add defaults
    linedefInputField->SetSelection(0);
    sectorInputField->SetSelection(0);
    thingInputField->SetSelection(0);
}

void MainWindow::OnStyleChoiceSelect(wxCommandEvent& event) {
    xdre::setSavepointStyle(styleChoice->GetSelection());
    RS();
}

void MainWindow::OnMenuThingTracking(wxCommandEvent& event) {
    if (event.IsChecked()) {
        ShowTrackWidgets(thingTracks);
    }

    else {
        HideTrackWidgets(thingTracks);
    }   
}

void MainWindow::OnMenuLineTracking(wxCommandEvent& event) {
    if (event.IsChecked()) {
        ShowTrackWidgets(lineTracks);
    }

    else {
        HideTrackWidgets(lineTracks);
    }

}

void MainWindow::OnMenuSectorTracking(wxCommandEvent& event) {
    if (event.IsChecked()) {
            ShowTrackWidgets(sectorTracks);
    }

    else {
        HideTrackWidgets(sectorTracks);
    }

}

void MainWindow::OnMenuPushTics(wxCommandEvent& event) {
    pushBackTics = MenuItem12->IsChecked();
}