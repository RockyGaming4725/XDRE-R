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
 * Description: avoiding macro minefield.
*/

#ifndef __CSTUFF_H
#define __CSTUFF_H

#define FRACBITS 16 //screw m_fixed.h
#define FRACUNIT (1<<FRACBITS)

extern int M_CheckParm(const char *check);

int playeringameGet(int plr); //get
void playeringameSet(int plr, int val); //set
//-1 refreshes current line/sector/thing
void x_setLinedefCheck(int linedef);
void x_setSectorCheck(int sector);
void x_setThingCheck(int index);
int x_getRngIndex(void);
void x_setAngleType(int type);
unsigned int x_getAngle(short type);
double x_getXPos(void);
double x_getYPos(void);
double x_getZPos(void);
double x_getPrevXPos(void);
double x_getPrevYPos(void);
double x_getPrevZPos(void);
double x_getXMom(void);
double x_getYMom(void);
int x_GetCompatibility(void);
void x_SetCompatibility(int compatflags);
void x_getOptions(int options[]);
void x_changeOptions(int options[]);
void x_initHeader(unsigned char* demo_p);
int x_initSize(void);
void x_clearMapSavepoints(void);
void x_clearUserSavepoint(void);
int x_getSavepointTic(int dest);
//try if saveing is feasible
//return 0 if good
int x_setSavepoint(int isUserSave);
//loads closest savepoint around gametic = tic
//return 0 if good
int x_loadSavepoint(int dest);

#endif // __CSTUFF_H

