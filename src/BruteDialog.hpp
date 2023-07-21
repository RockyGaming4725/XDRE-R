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
 * Description: brute force dialog declarations.
*/

#ifndef __BRUTEDIALOG_HPP
#define __BRUTEDIALOG_HPP

#include <wx/stattext.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/thread.h>
#include "xdre.hpp"
#include "MainWindow.hpp"

//Maybe?
#include <wx/fileconf.h>

wxDECLARE_EVENT(EVT_BRUTE_DONE, wxThreadEvent);

class mainWindow;

class BruteDialog: public wxDialog, public wxThreadHelper {

//Maybe?
#ifdef _WIN32
    wxFileConfig config {"xdre", "wxEmptyString", "xdre.ini"};
#else
    wxFileConfig config {"xdre", "wxEmptyString", ".xdre/xdre"};
#endif // _WIN32

    public:
        BruteDialog(MainWindow* mainWindow, wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
        virtual ~BruteDialog();

        wxPanel* p;
        wxStaticText *StaticText1, *StaticText2, *StaticText3, *StaticText4, *StaticText5, *StaticText6, *StaticText7, *StaticText8, *StaticText9, *StaticText10, *StaticText11, *StaticText12, *StaticText13, *StaticText14, *StaticText15, *StaticText16, *workingText;
        wxCheckBox *rngCheckBox, *damageCheckBox, *xPosCheckBox, *yPosCheckBox, *zPosCheckBox, *xMomCheckBox, *yMomCheckBox, *speedCheckBox, *useCheckBox, *linedefCheckBox, *avoidTurboCheckbox;
        wxChoice *rngChoice, *damageChoice, *xPosChoice, *yPosChoice, *zPosChoice, *xMomChoice, *yMomChoice, *speedChoice, *linedefChoice, *rangeSizeChoice;
        wxTextCtrl *rngInput, *damageInput, *xPosInput, *yPosInput, *zPosInput, *xMomInput, *yMomInput, *speedInput, *runCmdInput, *strafeCmdInput, *turnCmdInput, *bruteTicInput, *checkTicInput, *linedefInput;
        wxButton *start, *cancel, *addButton, *removeButton, *turnAngleButton, *help, *autoBruteRange;
        wxRadioBox *fireRadioBox, *useRadioBox;
        wxStaticBox *StaticBox1, *StaticBox2, *StaticBox3;
        wxListBox* bruteTicsBox;

        void OnBruteDone(wxThreadEvent& event);

    protected:
        static const long ID_BRUTETICSBOX, ID_BRUTETICINPUT, ID_STRAFECMDINPUT, ID_RUNCMDINPUT, ID_TURNCMDINPUT, ID_TURNANGLEBUTTON, ID_FIRERADIOBOX, ID_USERADIOBOX, ID_CANCEL, ID_START,
ID_ADDBUTTON, ID_REMOVEBUTTON, ID_BRUTEHELP, ID_AUTOBRUTERANGE;

        virtual wxThread::ExitCode Entry();

    private:
        struct ListItem {
            wxString ticStr {"0"};
            wxString runStr {"0"};
            wxString strafeStr {"0"};
            wxString turnStr {"0"};
            int fire {0};
            int use {0};
            int turnOrAngle {0};
        };

        MainWindow* m_mainWindow;
        std::vector<ListItem> items;
        std::vector<BruteTic> bruteTics;
        BruteCheck bruteCheck;
        bool abortBrute {false};
        bool bruteSuccess {false};

        //Matt: Changed from wxString turnAngleTexts[2] {"Turn cmds", "Angles"};
        //I think this means shorttics and longtics?
        wxString turnAngleTexts[2] {"Short", "Long"};

        // "," separates numbers, ":" separates start and end of range
        std::vector<signed char> ParseCmds(wxString str, bool parseAsUnsigned = false);
        std::vector<BruteTic> CreateBruteTics();
        BruteCheck CreateBruteCheck();


        void ShowData(int n);
        void EnableStuff();
        void DisableStuff();

        void OnClose(wxCloseEvent& event);
        void OnCancelClick(wxCommandEvent& event);
        void OnTurnAngleButtonClick(wxCommandEvent& event);
        void OnAddButtonClick(wxCommandEvent& event);
        void OnRemoveButtonClick(wxCommandEvent& event);
        void OnBruteTicsBoxSelect(wxCommandEvent& event);
        void OnBruteTicInputText(wxCommandEvent& event);
        void OnRunCmdInputText(wxCommandEvent& event);
        void OnStrafeCmdInputText(wxCommandEvent& event);
        void OnTurnCmdInputText(wxCommandEvent& event);
        void OnFireRadioBoxSelect(wxCommandEvent& event);
        void OnUseRadioBoxSelect(wxCommandEvent& event);
        void OnStartClick(wxCommandEvent& event);
        void OnBruteHelp(wxCommandEvent& event);
        void OnAutoRange(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif //__BRUTEDIALOG_HPP

