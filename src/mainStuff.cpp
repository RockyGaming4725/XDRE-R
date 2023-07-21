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
 *
 * Description: XDRE main() stuff. Copied and pasted from main.cpp and GuiWx.*.
*/

//for mingw-w64
#include "SDL.h"

#include "xdre.hpp"
#include <wx/app.h>
#include "MainWindow.hpp"

class GuiWx : public wxApp {
public:
    virtual bool OnInit();
protected:
private:
    MainWindow* mainWindow;
};

bool GuiWx::OnInit() {
    mainWindow = new MainWindow();
    mainWindow->Show(true);
    return true;
}

int main(int argc, char** argv) {
    GuiWx* gui = new GuiWx();
    wxApp::SetInstance(gui);

    //this is really important, I have had no idea about that problem
    xdre::init(argc, argv);

    return wxEntry(argc, argv);
}

