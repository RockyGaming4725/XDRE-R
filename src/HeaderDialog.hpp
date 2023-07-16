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
 * Description: demo header editor declarations.
*/

#ifndef __HEADERDIALOG_HPP
#define __HEADERDIALOG_HPP

#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include "xdre.hpp"

class HeaderDialog: public wxDialog {
    public:
        HeaderDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
        virtual ~HeaderDialog();

        wxPanel* p;
        wxStaticText *StaticText1, *StaticText2, *StaticText3, *StaticText4, *StaticText5, *StaticText6, *StaticText7, *StaticText8, *StaticText9, *StaticText10, *StaticText11, *StaticText12,
*StaticText13, *StaticText14, *StaticText15, *StaticText16, *StaticText17, *StaticText18, *StaticText19, *StaticText20, *StaticText21, *StaticText22, *StaticText23, *StaticText24, *StaticText25,
*StaticText26, *StaticText27, *StaticText28;
        wxTextCtrl *complevel, *skill, *episode, *map, *deathmatch, *respawn, *fast, *nomonsters, *consoleplayer, *players, *monsters_remember, *variable_friction, *weapon_recoil,
*allow_pushers, *player_bobbing, *demo_insurance, *rngseed, *monster_infighting, *dogs, *distfriend, *monster_backing, *monster_avoid_hazards, *monster_friction, *help_friends, *dog_jumping,
*monkeys, *compatflags, *longtics;
        wxButton *change, *close, *help;

    protected:
        static const long ID_CHANGE, ID_CLOSE, ID_HELP;

    private:
        void OnClose(wxCloseEvent& event);
        void OnApply(wxCommandEvent& event);
        void OnHelp(wxCommandEvent& event);

        DECLARE_EVENT_TABLE()
};

#endif //__HEADERDIALOG_HPP
