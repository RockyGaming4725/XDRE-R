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
 * Description: controls the program.
*/

#ifndef __XDRE_HPP
#define __XDRE_HPP

#include <string>
#include <vector>
#include <functional>
#include <cstdio>

enum SpStyle {
    SP_NOSAVE = -1,
    SP_USER = 0,
    SP_START = 1,
    SP_AUTO = 2
};

struct BruteTic {
    unsigned int tic;
    int fire; //-1=no, 0=both, 1=yes
    int use; //-1=no, 0=both, 1=yes
    bool turnsAreAngles; // use absolute angles instead of turns
    std::vector<signed char> turnCmds;
    std::vector<signed char> runCmds;
    std::vector<signed char> strafeCmds;
};

struct BruteCheck {
    unsigned int tic;
    bool use;
    //bool linedef;
    std::function<bool(int)> rngFunc;
    std::function<bool(int)> damageFunc;
    std::function<bool(double)> xPosFunc;
    std::function<bool(double)> yPosFunc;
    std::function<bool(double)> zPosFunc;
    std::function<bool(double)> xMomFunc;
    std::function<bool(double)> yMomFunc;
    std::function<bool(double)> speedFunc;
    //std::function<bool(bool)> linedefFunc;
};

namespace xdre {
    //you better call this
    void init(int argc, char** argv);
    //
    bool bruteForce(std::vector<BruteTic> bruteTics, BruteCheck const& bruteCheck, bool const& abort);
    //does sdl events, mainly moving window and resize, maybe in future walkcam
    //currently shuts down whole program without asking anything if you close
    //xdre through sdl window
    void doSdlEvents();
    //better be in "wb"/"rb" mode
    bool save(std::FILE* file);
    bool load(std::FILE* file);
    void setSavepoint();
    void unsetSavepoint();
    //goes fwd/bwd numTics
    void seekDemo(int numTics);
    void copyTic();
    void copyTicX(int val);
    void deleteTic();
    void deleteTicX(int val);
    void newTic();
    //changes displayplayer, does NOT change consoleplayer
    void changePlayerView(unsigned int plr);
    //set palette shit on/off (blood, pickups etc)
    void toggleBlood();
    void toggleFire();
    void toggleUse();
    void changeWeapon(char wpn);
    void getOptions(int array[]);
    void changeOptions(int options[]);
    //direction 0 is TL64, 1 is TR64, mode 0 is both, mode 1 is turn only, mode 2 is run cmds
    void transpose(bool direction, char mode = 0);
    void run(signed char val);
    void strafe(signed char val);
    void turn(signed short val);
    void setTurnStep(int step);
    void turnStepLeft();
    void turnStepRight();

    void setSavepointStyle(int style);
    void setLinedefCheck(int linedef);
    void setSectorCheck(int sector);
    void setThingCheck(int thing);

    std::string getTicString(unsigned int n);
    unsigned int getTiclistSize();
    int getRngIndex();
    double getXPos();
    double getYPos();
    double getZPos();
    double getXDist();
    double getYDist();
    double getZDist();
    double getXMom();
    double getYMom();
    double getDistanceMoved();
    double getDirectionMoved();
    void setAngleType(int type);
    unsigned short getAngle(short type);
    bool getUseSuccess();
    bool getLinedefCrossed();
    double getLinedefVertex(int v, int xy);
    double getSectorInfo(int field);
    double getThingInfo(int field);
    double getDistanceFromLine();
    int getDoneDamage();
    float getTime();
    unsigned int getCurrentTic();
    unsigned int getSavepointTic();
    int getSavepointStyle();
    bool getBlood();

    int getCurrentStrafe(int i, int listLength);
    int getCurrentRun(int i, int listLength);
    int getCurrentTurn(int i, int listLength);
};


#endif // __XDRE_HPP
