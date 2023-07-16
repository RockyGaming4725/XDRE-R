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
 * Description: C stuff.
*/

#include "cstuff.h"
#include "doomtype.h"
#include "doomstat.h"
#include "m_random.h"
#include "d_player.h"
#include "g_game.h"
#include "lprintf.h"
#include "p_saveg.h"
#include "p_map.h"
#include "p_mobj.h"
#include "r_demo.h"
#include "r_draw.h"
#include "r_fps.h"
#include "r_main.h"
#include "s_sound.h"
#include "s_advsound.h"

#include "stdio.h"
#include "p_tick.h"

#include "p_setup.h"

extern void RecalculateDrawnSubsectors(void);
extern int totalleveltimes;
extern dboolean setsizeneeded;
extern byte* savebuffer;
extern size_t savegamesize;

extern int numsectors;
extern sector_t* sectors;
int secID;
int xSectorInfo[3];

extern int numthings;
extern mobj_t **mobj_ptrs;
int thingID;
int xThingInfo[8];

extern int numlines;
extern line_t* lines;
extern int xLineId;
extern int xLineCrossed;
extern int xLineVertexes[2][2];

float angles = 16384.0;

typedef struct {
    int tic;
    byte* save;
} xSavepoint_t;

#define MAXSSP 100
static xSavepoint_t savepoints[MAXSSP]; //autosavepoint at 0

int playeringameGet(int plr) {
    if(plr < 0 || plr > 3) return 0;
    return playeringame[plr];
}

void playeringameSet(int plr, int val) {
    if(plr < 0 || plr > 3) return;
    playeringame[plr] = val == 0 ? 0 : 1;
}

void x_setLinedefCheck(int linedef) {
    int i;
    if(linedef == -1) linedef = xLineId;
    xLineId = -1;
    for(int i = 0; i < numlines; ++i) {
        if(linedef == lines[i].iLineID) {
            xLineId = lines[i].iLineID;
            xLineVertexes[0][0] = lines[i].v1->x;
            xLineVertexes[0][1] = lines[i].v1->y;
            xLineVertexes[1][0] = lines[i].v2->x;
            xLineVertexes[1][1] = lines[i].v2->y;
            break;
        }
    }
    if(xLineId == -1) {
        xLineVertexes[0][0] = 0;
        xLineVertexes[0][1] = 0;
        xLineVertexes[1][0] = 0;
        xLineVertexes[1][1] = 0;
    }
}

void x_setSectorCheck(int sector) {
    int i;
    if(sector == -1) sector = secID;
    secID = -1;

    for(i = 0; i < numsectors; ++i) {
        if(sector == sectors[i].iSectorID) {
            secID = sectors[i].iSectorID;
            xSectorInfo[0] = sectors[i].floorheight;
            xSectorInfo[1] = sectors[i].ceilingheight;
            xSectorInfo[2] = sectors[i].special;
            break;
        }
    }
    if(secID == -1)
        for(i = 0; i < 3; ++i)
            xSectorInfo[i] = 0;
}

//mobj_ptrs is in src/prboom-plus/src/p_setup.c
void x_setThingCheck(int index) {
    if(index == -1) index = thingID;
    if(index < numthings && mobj_ptrs[index] != NULL) {
        thingID = index;
        xThingInfo[0] = mobj_ptrs[index]->x;
        xThingInfo[1] = mobj_ptrs[index]->y;
        xThingInfo[2] = mobj_ptrs[index]->z;
        xThingInfo[3] = mobj_ptrs[index]->radius;
        xThingInfo[4] = mobj_ptrs[index]->tics; //state tic counter
        xThingInfo[5] = mobj_ptrs[index]->health;
        xThingInfo[6] = mobj_ptrs[index]->reactiontime;
        xThingInfo[7] = mobj_ptrs[index]->threshold;
    }
    else
        for(index = 0; index < 8; ++index)
            xThingInfo[index] = 0;
}

int x_getRngIndex(void) {return rng.rndindex;}

double x_getXPos(void) {
    if(gamestate != GS_LEVEL) return 0;
    return players[displayplayer].mo->x / (double)FRACUNIT;
}

double x_getYPos(void) {
    if(gamestate != GS_LEVEL) return 0;
    return players[displayplayer].mo->y / (double)FRACUNIT;
}

double x_getZPos(void) {
    if(gamestate != GS_LEVEL) return 0;
    return players[displayplayer].mo->z / (double)FRACUNIT;
}

double x_getPrevXPos(void) {
    if(gamestate != GS_LEVEL) return 0;
    return players[displayplayer].mo->PrevX / (double)FRACUNIT;
}

double x_getPrevYPos(void) {
    if(gamestate != GS_LEVEL) return 0;
    return players[displayplayer].mo->PrevY / (double)FRACUNIT;
}

double x_getPrevZPos(void) {
    if(gamestate != GS_LEVEL) return 0;
    return players[displayplayer].mo->PrevZ / (double)FRACUNIT;
}

double x_getXMom(void) {
    if(gamestate != GS_LEVEL) return 0;
    return players[displayplayer].mo->momx / (double)FRACUNIT;
}

double x_getYMom(void) {
    if(gamestate != GS_LEVEL) return 0;
    return players[displayplayer].mo->momy / (double)FRACUNIT;
}

void x_setAngleType(int type) { //moved from xdre.cpp because of accuracy
    switch(type) {
        case 0: angles = 16384.0; break; //longtics
        case 1: angles = 2048.0; break; //fine angles
        case 2: angles = 90.0; break; //degrees
        case 3: angles = 64.0; break; //byte angles
    }
}

unsigned int x_getAngle(short type) {
    if(gamestate != GS_LEVEL) return 0;
//zero is east, the values increase anti-clockwise. Also changed to unsigned
    return players[displayplayer].mo->angle * (angles / ANG90);
}

/*
1.  comp_telefrag     = 1
2.  comp_dropoff      = 2
3.  comp_vile         = 4
4.  comp_pain         = 8
5.  comp_skull        = 16
6.  comp_blazing      = 32
7.  comp_doorlight    = 64
8.  comp_model        = 128
9.  comp_god          = 256
10. comp_falloff      = 512
11. comp_floors       = 1024
12. comp_skymap       = 2048
13. comp_pursuit      = 4096
14. comp_doorstuck    = 8192
15. comp_staylift     = 16384
16. comp_zombie       = 32768
17. comp_stairs       = 65536
18. comp_infcheat     = 131072
19. comp_zerotags     = 262144
20. comp_moveblock    = 524288
21. comp_respawn      = 1048576
22. comp_sound        = 2097152
23. comp_666          = 4194304
24. comp_soul         = 8388608
25. comp_maskedanim   = 16777216
26. comp_ouchface     = 33554432
27. comp_maxhealth    = 67108864
28. comp_translucency = 134217728
I don't think this will be useful, but you can always add flags below.
1.  monsters_remember     = 1
2.  variable_friction     = 2
3.  weapon_recoil         = 4
4.  allow_pushers         = 8
5.  player_bobbing        = 16
6.  demo_insurance        = 32
7.  monster_infighting    = 64
8.  monster_backing       = 128
9.  monster_avoid_hazards = 256
10. monster_friction      = 512
11. help_friends          = 1024
12. dog_jumping           = 2048
13. monkeys               = 4096
*/
int x_GetCompatibility(void) {
    int compatflags = 0;
    for(int i = 0; i < COMP_NUM; i++) compatflags += comp[i] << i; //no need for 4 extra values yet
    return compatflags;
}

void x_SetCompatibility(int compatflags) {
    for(int i = 0; i < COMP_NUM; i++)
        comp[i] = compatflags & (1 << i);
}

dboolean x_longtics = 0; //avoid demo errors
dboolean usedHeader = false;

//get options, for HeaderDialog
void x_getOptions(int options[]) {
    options[0] = compatibility_level;
    options[1] = gameskill + 1;
    options[2] = gameepisode;
    options[3] = gamemap;
    options[4] = deathmatch;
    options[5] = respawnparm;
    options[6] = fastparm;
    options[7] = nomonsters;
    options[8] = consoleplayer;
    options[9] = 0;
    for(int i = 0; i < 4; i++)
        options[9] += playeringameGet(i) << i;
    options[10] = monsters_remember; //didn't really want bit flags here
    options[11] = variable_friction;
    options[12] = weapon_recoil;
    options[13] = allow_pushers;
    options[14] = player_bobbing;
    options[15] = demo_insurance;
    options[16] = rngseed;
    options[17] = monster_infighting;
    options[18] = dogs;
    options[19] = distfriend;
    options[20] = monster_backing;
    options[21] = monster_avoid_hazards;
    options[22] = monster_friction;
    options[23] = help_friends;
    options[24] = dog_jumping;
    options[25] = monkeys;
    options[26] = x_GetCompatibility();
    options[27] = longtics;
}

//set
void x_changeOptions(int options[]) {
    compatibility_level = options[0];
    gameskill = (skill_t)options[1] - 1;
    gameepisode = options[2];
    gamemap = options[3];
    deathmatch = options[4];
    respawnparm = options[5];
    fastparm = options[6];
    nomonsters = options[7];
    displayplayer = consoleplayer = options[8];
    dboolean players[4];
    int lastplayer;
    for(int i = 0; i < 4; i++) {
        players[i] = options[9] & (1 << i);
        playeringameSet(i, players[i]);
        if(players[i])
            lastplayer = i;
    }
    //avoid segfaults
    for(int i = 0; i < 4; i++) {
        if(!players[i] && consoleplayer == i) {
            displayplayer = consoleplayer = lastplayer;
        }
    }
    monsters_remember = options[10];
    variable_friction = options[11];
    weapon_recoil = options[12];
    allow_pushers = options[13];
    player_bobbing = options[14];
    demo_insurance = options[15];
    rngseed = options[16];
    monster_infighting = options[17];
    dogs = options[18];
    distfriend = options[19];
    monster_backing = options[20];
    monster_avoid_hazards = options[21];
    monster_friction = options[22];
    help_friends = options[23];
    dog_jumping = options[24];
    monkeys = options[25];
    x_SetCompatibility(options[26]);
    x_longtics = options[27];
    usedHeader = true;
}

#define MIN_MAXPLAYERS 32
void x_initHeader(unsigned char* demo_p) //modified from G_BeginRecording
{
  int i;

  /* cph - 3 demo record formats supported: MBF+, BOOM, and Doom v1.9 */
  if (mbf_features) {
    { /* Write version code into demo */
      unsigned char v = 0;
      switch(compatibility_level) {
        case mbf_compatibility: v = 203; break; // e6y: Bug in MBF compatibility mode fixed
        case prboom_2_compatibility: v = 210; break;
        case prboom_3_compatibility: v = 211; break;
        case prboom_4_compatibility: v = 212; break;
        case prboom_5_compatibility: v = 213; break;
        case prboom_6_compatibility: v = 214; longtics = 1; break;
        default: I_Error("x_initHeader: PrBoom compatibility level unrecognised?");
      }
      *demo_p++ = v;
    }

    // signature
    *demo_p++ = 0x1d;
    *demo_p++ = 'M';
    *demo_p++ = 'B';
    *demo_p++ = 'F';
    *demo_p++ = 0xe6;
    *demo_p++ = '\0';

    /* killough 2/22/98: save compatibility flag in new demos
     * cph - FIXME? MBF demos will always be not in compat. mode */
    *demo_p++ = 0;

    *demo_p++ = gameskill;
    *demo_p++ = gameepisode;
    *demo_p++ = gamemap;
    *demo_p++ = deathmatch;
    *demo_p++ = consoleplayer;

    demo_p = G_WriteOptions(demo_p); // killough 3/1/98: Save game options

    for (i=0 ; i<MAXPLAYERS ; i++)
      *demo_p++ = playeringame[i];

    // killough 2/28/98:
    // We always store at least MIN_MAXPLAYERS bytes in demo, to
    // support enhancements later w/o losing demo compatibility

    for (; i<32; i++)
      *demo_p++ = 0;

  } else if (compatibility_level >= boom_compatibility_compatibility) { //they still desync, though
    byte v = 0, c = 0; /* Nominally, version and compatibility bits */
    switch (compatibility_level) {
    case boom_compatibility_compatibility: v = 202, c = 1; break;
    case boom_201_compatibility: v = 201; c = 0; break;
    case boom_202_compatibility: v = 202, c = 0; break;
    default: I_Error("x_initHeader: Boom compatibility level unrecognised?");
    }
    *demo_p++ = v;

    // signature
    *demo_p++ = 0x1d;
    *demo_p++ = 'B';
    *demo_p++ = 'o';
    *demo_p++ = 'o';
    *demo_p++ = 'm';
    *demo_p++ = 0xe6;

    /* CPhipps - save compatibility level in demos */
    *demo_p++ = c;

    *demo_p++ = gameskill;
    *demo_p++ = gameepisode;
    *demo_p++ = gamemap;
    *demo_p++ = deathmatch;
    *demo_p++ = consoleplayer;

    demo_p = G_WriteOptions(demo_p); // killough 3/1/98: Save game options

    for (i=0; i<MAXPLAYERS; i++)
      *demo_p++ = playeringame[i];

    // killough 2/28/98:
    // We always store at least MIN_MAXPLAYERS bytes in demo, to
    // support enhancements later w/o losing demo compatibility

    for (; i<32; i++)
      *demo_p++ = 0;
  } else { // cph - write old v1.9 demos (might even sync)
    unsigned char v = 109;
    if(!usedHeader) longtics = M_CheckParm("-longtics");
    else longtics = x_longtics;
    if (longtics)
    {
      v = 111;
    }
    else
    {
      switch (compatibility_level)
      {
      case doom_1666_compatibility:
        v = 106;
        break;
      case tasdoom_compatibility:
        v = 110;
        break;
      }
    }
    *demo_p++ = v;
    *demo_p++ = gameskill;
    *demo_p++ = gameepisode;
    *demo_p++ = gamemap;
    *demo_p++ = deathmatch;
    *demo_p++ = respawnparm;
    *demo_p++ = fastparm;
    *demo_p++ = nomonsters;
    *demo_p++ = consoleplayer;
    for (i=0; i<4; i++)  // intentionally hard-coded 4 -- killough
      *demo_p++ = playeringame[i];
  }
}

void x_clearMapSavepoints(void) {
    int i;
    for(i = 1; i < MAXSSP; ++i) {
        free(savepoints[i].save);
        savepoints[i].save = NULL;
        savepoints[i].tic = 0;
    }
}

void x_clearUserSavepoint() {
    free(savepoints[0].save);
    savepoints[0].save = NULL;
    savepoints[0].tic = 0;
}

static int x_getSavepointIndex(int dest) {
    int index = -1;
    int savetic = 0;
    int map = gamemap;

    if(map > MAXSSP-1) map = MAXSSP-1;

    if(savepoints[0].save && savepoints[0].tic < dest) {
        index = 0;
        savetic = savepoints[0].tic;
    }

    for(; map > 0; --map) {
        if(savetic < savepoints[map].tic && savepoints[map].tic < dest) {
            index = map;
            break;
        }
    }

    return index;
}

int x_getSavepointTic(int dest) {
    int index = x_getSavepointIndex(dest);
    if(index < 0) return 0;
    return savepoints[index].tic;
}

int x_setSavepoint(int isUserSave) {
    int map = gamemap;

    if(map < 0 || map > MAXSSP-1 || gamestate != GS_LEVEL || gameaction != ga_nothing) return 1;

    if(leveltime != 1 || isUserSave != 0) map = 0;

    if(savepoints[map].save) {
        free(savepoints[map].save);
        savepoints[map].save = NULL;
    }

    savepoints[map].save = malloc(savegamesize);
    savepoints[map].tic = gametic;
    save_p = savebuffer = savepoints[map].save;


    // do save
    {
        int i;
        *save_p++ = compatibility_level;

        *save_p++ = gameskill;
        *save_p++ = gameepisode;
        *save_p++ = gamemap;

        for(i = 0; i < MAXPLAYERS; ++i) *save_p++ = playeringame[i];

        for(; i < MIN_MAXPLAYERS; ++i) *save_p++ = 0;

        *save_p++ = idmusnum;

        save_p = G_WriteOptions(save_p);

        memcpy(save_p, &leveltime, sizeof leveltime);
        save_p += sizeof leveltime;

        memcpy(save_p, &totalleveltimes, sizeof totalleveltimes);
        save_p += sizeof totalleveltimes;

        *save_p++ = (gametic - basetic) & 255;

        Z_CheckHeap();
        P_ArchivePlayers();
        Z_CheckHeap();

        P_ThinkerToIndex();

        P_ArchiveWorld();
        Z_CheckHeap();
        P_ArchiveThinkers();
        Z_CheckHeap();
        P_ArchiveSpecials();

        P_IndexToThinker();

        P_ArchiveRNG();
        Z_CheckHeap();
        P_ArchiveMap();

        *save_p++ = 0xe6;

        Z_CheckHeap();
    }
    // done


    savepoints[map].save = savebuffer; //incase checksavegame reallocated savebuffer
    save_p = savebuffer = NULL;
    return 0;
}

//modified G_DoLoadGame
int x_loadSavepoint(int dest) {
    int i;
    save_p = NULL;
    int map = x_getSavepointIndex(dest);

    if(map < 0) return 1;

    if(map != 0) x_clearUserSavepoint();

    save_p = savepoints[map].save;
    gametic = savepoints[map].tic;

    gameaction = ga_nothing;

    compatibility_level = *save_p++;

    gameskill = *save_p++;
    gameepisode = *save_p++;
    gamemap = *save_p++;

    for(i = 0; i < MAXPLAYERS; ++i) playeringame[i] = *save_p++;

    save_p += 28; //28 = MIN_MAXPLAYERS - MAXPLAYERS in g_game.c

    idmusnum = *save_p++;
    if(idmusnum == 255) idmusnum = -1;

    save_p += (G_ReadOptions(save_p) - save_p);


    G_InitNew(gameskill, gameepisode, gamemap);

    memcpy(&leveltime, save_p, sizeof leveltime);
    save_p += sizeof leveltime;

    memcpy(&totalleveltimes, save_p, sizeof totalleveltimes);
    save_p += sizeof totalleveltimes;

    basetic = gametic - *save_p++;

    P_MapStart();
    P_UnArchivePlayers();
    P_UnArchiveWorld();
    P_UnarchiveThinkersAndSpecials();
    P_UnArchiveRNG();
    P_UnArchiveMap();
    P_MapEnd();
    R_ActivateSectorInterpolations();
    R_SmoothPlaying_Reset(NULL);

    if(musinfo.current_item != -1) S_ChangeMusInfoMusic(musinfo.current_item, true);

    RecalculateDrawnSubsectors();

    if(*save_p != 0xe6) I_Error("x_loadSavepoint: Bad savepoint on map %i", map);

    if(setsizeneeded) R_ExecuteSetViewSize();

    R_FillBackScreen();

    save_p = NULL;

    return 0;
}

