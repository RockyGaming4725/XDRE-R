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
 * Description: none?
*/

#ifndef __KEYSDIALOG_HPP
#define __KEYSDIALOG_HPP

#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

class KeysDialog: public wxDialog {
    public:
        KeysDialog(wxWindow* parent,wxWindowID id = wxID_ANY,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize);
        virtual ~KeysDialog();

        wxPanel* p;
        wxButton *set, *close;
        wxStaticText *StaticText1, *StaticText2, *StaticText3, *StaticText4, *StaticText5, *StaticText6, *StaticText7, *StaticText8, *StaticText9, *StaticText10, *StaticText11, *StaticText12,
*StaticText13, *StaticText14, *StaticText15, *StaticText16, *StaticText17, *StaticText18, *StaticText19, *StaticText20, *StaticText21, *StaticText22, *StaticText23, *StaticText24, *StaticText25,
*StaticText26, *StaticText27, *StaticText28, *StaticText29, *StaticText30;
        wxTextCtrl *Fire, *Use, *TurnStepLeft, *TurnStepRight, *TurnStep, *PrevTic, *NextTic, *CopyTic, *DeleteTic, *NewTic, *ToggleBlood, *SetSavepoint, *UnsetSavepoint, *Load, *Save, *Exit,
*RunBwd, *RunFwd, *StrafeLeft, *StrafeRight, *TurnLeft, *TurnRight, *Weapon, *TransposeLeft, *TransposeRight, *CopyTicX, *DeleteTicX, *BackwardDemo, *ForwardDemo, *ChangePlayer;

    protected:
        static const long ID_SET, ID_CLOSE;

    private:
        void OnClose(wxCloseEvent& event);
        void OnSet(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif //__KEYSDIALOG_HPP

