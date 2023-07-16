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
 * Description: tic list control.
*/

#include "TiclistCtrl.hpp"
#include "xdre.hpp"


TiclistCtrl::TiclistCtrl(wxWindow* parent,
                         wxWindowID const id,
                         wxPoint const& pos,
                         wxSize const& size,
                         wxString const& name)
                        : wxListCtrl(parent, id, pos, size,
                                     wxLC_VIRTUAL|wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL,
                                     wxDefaultValidator, name) {
    wxListItem col;
    col.SetId(0);
    col.SetWidth(size.GetWidth() - 22);
    InsertColumn(0, col);
    SetItemCount(20);
}

wxString TiclistCtrl::OnGetItemText(long item, long column) const {return xdre::getTicString(item);}
