/*
===========================================================================
ARX FATALIS GPL Source Code
Copyright (C) 1999-2010 Arkane Studios SA, a ZeniMax Media company.

This file is part of the Arx Fatalis GPL Source Code ('Arx Fatalis Source Code'). 

Arx Fatalis Source Code is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Arx Fatalis Source Code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Arx Fatalis Source Code.  If not, see 
<http://www.gnu.org/licenses/>.

In addition, the Arx Fatalis Source Code is also subject to certain additional terms. You should have received a copy of these 
additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Arx 
Fatalis Source Code. If not, please request a copy in writing from Arkane Studios at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing Arkane Studios, c/o 
ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
//////////////////////////////////////////////////////////////////////////////////////
//   @@        @@@        @@@                @@                           @@@@@     //
//   @@@       @@@@@@     @@@     @@        @@@@                         @@@  @@@   //
//   @@@       @@@@@@@    @@@    @@@@       @@@@      @@                @@@@        //
//   @@@       @@  @@@@   @@@  @@@@@       @@@@@@     @@@               @@@         //
//  @@@@@      @@  @@@@   @@@ @@@@@        @@@@@@@    @@@            @  @@@         //
//  @@@@@      @@  @@@@  @@@@@@@@         @@@@ @@@    @@@@@         @@ @@@@@@@      //
//  @@ @@@     @@  @@@@  @@@@@@@          @@@  @@@    @@@@@@        @@ @@@@         //
// @@@ @@@    @@@ @@@@   @@@@@            @@@@@@@@@   @@@@@@@      @@@ @@@@         //
// @@@ @@@@   @@@@@@@    @@@@@@           @@@  @@@@   @@@ @@@      @@@ @@@@         //
// @@@@@@@@   @@@@@      @@@@@@@@@@      @@@    @@@   @@@  @@@    @@@  @@@@@        //
// @@@  @@@@  @@@@       @@@  @@@@@@@    @@@    @@@   @@@@  @@@  @@@@  @@@@@        //
//@@@   @@@@  @@@@@      @@@      @@@@@@ @@     @@@   @@@@   @@@@@@@    @@@@@ @@@@@ //
//@@@   @@@@@ @@@@@     @@@@        @@@  @@      @@   @@@@   @@@@@@@    @@@@@@@@@   //
//@@@    @@@@ @@@@@@@   @@@@             @@      @@   @@@@    @@@@@      @@@@@      //
//@@@    @@@@ @@@@@@@   @@@@             @@      @@   @@@@    @@@@@       @@        //
//@@@    @@@  @@@ @@@@@                          @@            @@@                  //
//            @@@ @@@                           @@             @@        STUDIOS    //
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// ARX_Script
//////////////////////////////////////////////////////////////////////////////////////
//
// Description:
//		ARX Script Management
//
// Updates: (date) (person) (update)
//
// Code: Cyril Meynier
//
// Copyright (c) 1999-2000 ARKANE Studios SA. All rights reserved
/////////////////////////////////////////////////////////////////////////////////////

#ifndef ARX_SCRIPTING_SCRIPT_H
#define ARX_SCRIPTING_SCRIPT_H

#include <string>

#include "platform/Flags.h"

#include "Configure.h"

class PakFile;
struct INTERACTIVE_OBJ;
struct ANIM_HANDLE;

const size_t MAX_SHORTCUT = 80;
const size_t MAX_SCRIPTTIMERS = 5;

enum ValueType {
	TYPE_TEXT = 1,
	TYPE_FLOAT = 2,
	TYPE_LONG = 3
};


enum VariableType {
	TYPE_UNKNOWN = 0, // does not exist !
	TYPE_G_TEXT = 1,
	TYPE_L_TEXT = 2,
	TYPE_G_LONG = 4,
	TYPE_L_LONG = 8,
	TYPE_G_FLOAT = 16,
	TYPE_L_FLOAT = 32
};

struct SCRIPT_VAR {
	VariableType type;
	long ival;
	float fval;
	char * text;  // for a TEXT type ival equals strlen(text).
	char name[64];
};

struct LABEL_INFO {
	char * string;
	long idx;
};

enum DisabledEvent {
	DISABLE_HIT             = (1<<0),
	DISABLE_CHAT            = (1<<1),
	DISABLE_INVENTORY2_OPEN = (1<<2),
	DISABLE_HEAR            = (1<<3),
	DISABLE_DETECT          = (1<<4),
	DISABLE_AGGRESSION      = (1<<5),
	DISABLE_MAIN            = (1<<6),
	DISABLE_COLLIDE_NPC     = (1<<7),
	DISABLE_CURSORMODE      = (1<<8),
	DISABLE_EXPLORATIONMODE = (1<<9)
};
DECLARE_FLAGS(DisabledEvent, DisabledEvents)
DECLARE_FLAGS_OPERATORS(DisabledEvents)

struct EERIE_SCRIPT {
	size_t size;
	char * data;
	long nblvar;
	SCRIPT_VAR * lvar;
	unsigned long lastcall;
	unsigned long timers[MAX_SCRIPTTIMERS];
	DisabledEvents allowevents;
	EERIE_SCRIPT * master;
	long shortcut[MAX_SHORTCUT];
	long nb_labels;
	LABEL_INFO * labels;
};

struct SCR_TIMER {
	
	std::string name;
	short exist;
	short flags;
	long times;
	long msecs;
	long pos;
	long longinfo;
	unsigned long tim;
	INTERACTIVE_OBJ * io;
	EERIE_SCRIPT * es;
	
	inline SCR_TIMER() : name(), exist(0), flags(0), times(0),
	                     msecs(0), pos(0), longinfo(0), tim(0), io(NULL), es(NULL) { };
	
	inline void reset() {
		name.clear();
		exist = 0;
		flags = 0;
		times = 0;
		msecs = 0;
		pos = 0;
		longinfo = 0;
		tim = 0;
		io = NULL;
		es = NULL;
	}
	
};

enum AnimationNumber {
	
	ANIM_NONE = -1,
	
	ANIM_WAIT = 0,
	ANIM_WALK = 1,
	ANIM_WALK2 = 2,
	ANIM_WALK3 = 3,
	ANIM_ACTION = 8,
	ANIM_ACTION2 = 9,
	ANIM_ACTION3 = 10,
	ANIM_HIT1 = 11,
	ANIM_STRIKE1 = 12,
	ANIM_DIE = 13,
	ANIM_WAIT2 = 14,
	ANIM_RUN = 15,
	ANIM_RUN2 = 16,
	ANIM_RUN3 = 17,
	ANIM_ACTION4 = 18,
	ANIM_ACTION5 = 19,
	ANIM_ACTION6 = 20,
	ANIM_ACTION7 = 21,
	ANIM_ACTION8 = 22,
	ANIM_ACTION9 = 23,
	ANIM_ACTION10 = 24,
	ANIM_TALK_NEUTRAL = 30,
	ANIM_TALK_HAPPY = 31,
	ANIM_TALK_ANGRY = 32,
	ANIM_WALK_BACKWARD = 33,
	
	ANIM_BARE_READY = 34,
	ANIM_BARE_UNREADY = (ANIM_BARE_READY+1),
	ANIM_BARE_WAIT = (ANIM_BARE_READY+2),
	ANIM_BARE_STRIKE_LEFT_START = (ANIM_BARE_READY+3),
	ANIM_BARE_STRIKE_LEFT_CYCLE = (ANIM_BARE_READY+4),
	ANIM_BARE_STRIKE_LEFT = (ANIM_BARE_READY+5),
	ANIM_BARE_STRIKE_RIGHT_START = (ANIM_BARE_READY+6),
	ANIM_BARE_STRIKE_RIGHT_CYCLE = (ANIM_BARE_READY+7),
	ANIM_BARE_STRIKE_RIGHT = (ANIM_BARE_READY+8),
	ANIM_BARE_STRIKE_TOP_START = (ANIM_BARE_READY+9),
	ANIM_BARE_STRIKE_TOP_CYCLE = (ANIM_BARE_READY+10),
	ANIM_BARE_STRIKE_TOP = (ANIM_BARE_READY+11),
	ANIM_BARE_STRIKE_BOTTOM_START = (ANIM_BARE_READY+12),
	ANIM_BARE_STRIKE_BOTTOM_CYCLE = (ANIM_BARE_READY+13),
	ANIM_BARE_STRIKE_BOTTOM = (ANIM_BARE_READY+14),
	
	ANIM_1H_READY_PART_1 = (ANIM_BARE_STRIKE_BOTTOM+1),
	ANIM_1H_READY_PART_2 = (ANIM_1H_READY_PART_1+1),
	ANIM_1H_UNREADY_PART_1 = (ANIM_1H_READY_PART_1+2),
	ANIM_1H_UNREADY_PART_2 = (ANIM_1H_READY_PART_1+3),
	ANIM_1H_WAIT = (ANIM_1H_READY_PART_1+4),
	ANIM_1H_STRIKE_LEFT_START = (ANIM_1H_READY_PART_1+5),
	ANIM_1H_STRIKE_LEFT_CYCLE = (ANIM_1H_READY_PART_1+6),
	ANIM_1H_STRIKE_LEFT = (ANIM_1H_READY_PART_1+7),
	ANIM_1H_STRIKE_RIGHT_START = (ANIM_1H_READY_PART_1+8),
	ANIM_1H_STRIKE_RIGHT_CYCLE = (ANIM_1H_READY_PART_1+9),
	ANIM_1H_STRIKE_RIGHT = (ANIM_1H_READY_PART_1+10),
	ANIM_1H_STRIKE_TOP_START = (ANIM_1H_READY_PART_1+11),
	ANIM_1H_STRIKE_TOP_CYCLE = (ANIM_1H_READY_PART_1+12),
	ANIM_1H_STRIKE_TOP = (ANIM_1H_READY_PART_1+13),
	ANIM_1H_STRIKE_BOTTOM_START = (ANIM_1H_READY_PART_1+14),
	ANIM_1H_STRIKE_BOTTOM_CYCLE = (ANIM_1H_READY_PART_1+15),
	ANIM_1H_STRIKE_BOTTOM = (ANIM_1H_READY_PART_1+16),
	
	ANIM_2H_READY_PART_1 = (ANIM_1H_STRIKE_BOTTOM+1), //66
	ANIM_2H_READY_PART_2 = (ANIM_2H_READY_PART_1+1),
	ANIM_2H_UNREADY_PART_1 = (ANIM_2H_READY_PART_1+2),
	ANIM_2H_UNREADY_PART_2 = (ANIM_2H_READY_PART_1+3),
	ANIM_2H_WAIT = (ANIM_2H_READY_PART_1+4),
	ANIM_2H_STRIKE_LEFT_START = (ANIM_2H_READY_PART_1+5),
	ANIM_2H_STRIKE_LEFT_CYCLE = (ANIM_2H_READY_PART_1+6),
	ANIM_2H_STRIKE_LEFT = (ANIM_2H_READY_PART_1+7),
	ANIM_2H_STRIKE_RIGHT_START = (ANIM_2H_READY_PART_1+8),
	ANIM_2H_STRIKE_RIGHT_CYCLE = (ANIM_2H_READY_PART_1+9),
	ANIM_2H_STRIKE_RIGHT = (ANIM_2H_READY_PART_1+10),
	ANIM_2H_STRIKE_TOP_START = (ANIM_2H_READY_PART_1+11),
	ANIM_2H_STRIKE_TOP_CYCLE = (ANIM_2H_READY_PART_1+12),
	ANIM_2H_STRIKE_TOP = (ANIM_2H_READY_PART_1+13),
	ANIM_2H_STRIKE_BOTTOM_START = (ANIM_2H_READY_PART_1+14),
	ANIM_2H_STRIKE_BOTTOM_CYCLE = (ANIM_2H_READY_PART_1+15),
	ANIM_2H_STRIKE_BOTTOM = (ANIM_2H_READY_PART_1+16),
	
	ANIM_DAGGER_READY_PART_1 = (ANIM_2H_STRIKE_BOTTOM+1), //82
	ANIM_DAGGER_READY_PART_2 = (ANIM_DAGGER_READY_PART_1+1),
	ANIM_DAGGER_UNREADY_PART_1 = (ANIM_DAGGER_READY_PART_1+2),
	ANIM_DAGGER_UNREADY_PART_2 = (ANIM_DAGGER_READY_PART_1+3),
	ANIM_DAGGER_WAIT = (ANIM_DAGGER_READY_PART_1+4),
	ANIM_DAGGER_STRIKE_LEFT_START = (ANIM_DAGGER_READY_PART_1+5),
	ANIM_DAGGER_STRIKE_LEFT_CYCLE = (ANIM_DAGGER_READY_PART_1+6),
	ANIM_DAGGER_STRIKE_LEFT = (ANIM_DAGGER_READY_PART_1+7),
	ANIM_DAGGER_STRIKE_RIGHT_START = (ANIM_DAGGER_READY_PART_1+8),
	ANIM_DAGGER_STRIKE_RIGHT_CYCLE = (ANIM_DAGGER_READY_PART_1+9),
	ANIM_DAGGER_STRIKE_RIGHT = (ANIM_DAGGER_READY_PART_1+10),
	ANIM_DAGGER_STRIKE_TOP_START = (ANIM_DAGGER_READY_PART_1+11),
	ANIM_DAGGER_STRIKE_TOP_CYCLE = (ANIM_DAGGER_READY_PART_1+12),
	ANIM_DAGGER_STRIKE_TOP = (ANIM_DAGGER_READY_PART_1+13),
	ANIM_DAGGER_STRIKE_BOTTOM_START = (ANIM_DAGGER_READY_PART_1+14),
	ANIM_DAGGER_STRIKE_BOTTOM_CYCLE = (ANIM_DAGGER_READY_PART_1+15),
	ANIM_DAGGER_STRIKE_BOTTOM = (ANIM_DAGGER_READY_PART_1+16),
	
	ANIM_MISSILE_READY_PART_1 = (ANIM_DAGGER_STRIKE_BOTTOM+1), //99
	ANIM_MISSILE_READY_PART_2 = (ANIM_MISSILE_READY_PART_1+1),
	ANIM_MISSILE_UNREADY_PART_1 = (ANIM_MISSILE_READY_PART_1+2),
	ANIM_MISSILE_UNREADY_PART_2 = (ANIM_MISSILE_READY_PART_1+3),
	ANIM_MISSILE_WAIT = (ANIM_MISSILE_READY_PART_1+4),
	ANIM_MISSILE_STRIKE_PART_1 = (ANIM_MISSILE_READY_PART_1+5),
	ANIM_MISSILE_STRIKE_PART_2 = (ANIM_MISSILE_READY_PART_1+6),
	ANIM_MISSILE_STRIKE_CYCLE = (ANIM_MISSILE_READY_PART_1+7),
	ANIM_MISSILE_STRIKE = (ANIM_MISSILE_READY_PART_1+8),
	
	ANIM_SHIELD_START = (ANIM_MISSILE_STRIKE+1), //108
	ANIM_SHIELD_CYCLE = (ANIM_SHIELD_START+1),
	ANIM_SHIELD_HIT = (ANIM_SHIELD_START+2),
	ANIM_SHIELD_END = (ANIM_SHIELD_START+3),
	
	ANIM_CAST_START = (ANIM_SHIELD_END+1), //112
	ANIM_CAST_CYCLE = (ANIM_CAST_START+1),
	ANIM_CAST = (ANIM_CAST_START+2),
	ANIM_CAST_END = (ANIM_CAST_START+3),
	
	ANIM_DEATH_CRITICAL = (ANIM_CAST_END+1),
	ANIM_CROUCH = (ANIM_CAST_END+2),
	ANIM_CROUCH_WALK = (ANIM_CAST_END+3),
	ANIM_CROUCH_WALK_BACKWARD = (ANIM_CAST_END+4),
	ANIM_LEAN_RIGHT = (ANIM_CAST_END+5),
	ANIM_LEAN_LEFT = (ANIM_CAST_END+6),
	ANIM_JUMP = (ANIM_CAST_END+7),
	ANIM_HOLD_TORCH = (ANIM_CAST_END+8),
	ANIM_WALK_MINISTEP = (ANIM_CAST_END+9),
	ANIM_STRAFE_RIGHT = (ANIM_CAST_END+10),
	ANIM_STRAFE_LEFT = (ANIM_CAST_END+11),
	ANIM_MEDITATION = (ANIM_CAST_END+12),
	ANIM_WAIT_SHORT = (ANIM_CAST_END+13),
	
	ANIM_FIGHT_WALK_FORWARD = (ANIM_CAST_END+14),
	ANIM_FIGHT_WALK_BACKWARD = (ANIM_CAST_END+15),
	ANIM_FIGHT_WALK_MINISTEP = (ANIM_CAST_END+16),
	ANIM_FIGHT_STRAFE_RIGHT = (ANIM_CAST_END+17),
	ANIM_FIGHT_STRAFE_LEFT = (ANIM_CAST_END+18),
	ANIM_FIGHT_WAIT = (ANIM_CAST_END+19),
	
	ANIM_LEVITATE = (ANIM_CAST_END+20),
	ANIM_CROUCH_START = (ANIM_CAST_END+21),
	ANIM_CROUCH_WAIT = (ANIM_CAST_END+22),
	ANIM_CROUCH_END = (ANIM_CAST_END+23),
	ANIM_JUMP_ANTICIPATION = (ANIM_CAST_END+24),
	ANIM_JUMP_UP = (ANIM_CAST_END+25),
	ANIM_JUMP_CYCLE = (ANIM_CAST_END+26),
	ANIM_JUMP_END = (ANIM_CAST_END+27),
	ANIM_TALK_NEUTRAL_HEAD = (ANIM_CAST_END+28),
	ANIM_TALK_ANGRY_HEAD = (ANIM_CAST_END+29),
	ANIM_TALK_HAPPY_HEAD = (ANIM_CAST_END+30),
	ANIM_STRAFE_RUN_LEFT = (ANIM_CAST_END+31),
	ANIM_STRAFE_RUN_RIGHT = (ANIM_CAST_END+32),
	ANIM_CROUCH_STRAFE_LEFT = (ANIM_CAST_END+33),
	ANIM_CROUCH_STRAFE_RIGHT = (ANIM_CAST_END+34),
	ANIM_WALK_SNEAK = (ANIM_CAST_END+35),
	ANIM_GRUNT = (ANIM_CAST_END+36),
	ANIM_JUMP_END_PART2 = (ANIM_CAST_END+37),
	ANIM_HIT_SHORT = (ANIM_CAST_END+38),
	ANIM_U_TURN_LEFT = (ANIM_CAST_END+39),
	ANIM_U_TURN_RIGHT = (ANIM_CAST_END+40),
	ANIM_RUN_BACKWARD = (ANIM_CAST_END+41),
	ANIM_U_TURN_LEFT_FIGHT = (ANIM_CAST_END+42),
	ANIM_U_TURN_RIGHT_FIGHT = (ANIM_CAST_END+43),
	
};

const AnimationNumber ANIM_DEFAULT = ANIM_WAIT;

enum ScriptResult {
	ACCEPT = 1,
	REFUSE = -1,
	BIGERROR = -2
};

enum ScriptMessage {
	SM_NULL = 0,
	SM_INIT = 1,
	SM_INVENTORYIN = 2,
	SM_INVENTORYOUT = 3,
	SM_INVENTORYUSE = 4,
	SM_SCENEUSE = 5,
	SM_EQUIPIN = 6,
	SM_EQUIPOUT = 7,
	SM_MAIN = 8,
	SM_RESET = 9,
	SM_CHAT = 10,
	SM_ACTION = 11,
	SM_DEAD = 12,
	SM_REACHEDTARGET = 13,
	SM_FIGHT = 14,
	SM_FLEE = 15,
	SM_HIT = 16,
	SM_DIE = 17,
	SM_LOSTTARGET = 18,
	SM_TREATIN = 19,
	SM_TREATOUT = 20,
	SM_MOVE = 21,
	SM_DETECTPLAYER = 22,
	SM_UNDETECTPLAYER = 23,
	SM_COMBINE = 24,
	SM_NPC_FOLLOW = 25,
	SM_NPC_FIGHT = 26,
	SM_NPC_STAY = 27,
	SM_INVENTORY2_OPEN = 28,
	SM_INVENTORY2_CLOSE = 29,
	SM_CUSTOM = 30,
	SM_ENTER_ZONE = 31,
	SM_LEAVE_ZONE = 32,
	SM_INITEND = 33,
	SM_CLICKED = 34,
	SM_INSIDEZONE = 35,
	SM_CONTROLLEDZONE_INSIDE = 36,
	SM_LEAVEZONE = 37,
	SM_CONTROLLEDZONE_LEAVE = 38,
	SM_ENTERZONE = 39,
	SM_CONTROLLEDZONE_ENTER = 40,
	SM_LOAD = 41,
	SM_SPELLCAST = 42,
	SM_RELOAD = 43,
	SM_COLLIDE_DOOR = 44,
	SM_OUCH = 45,
	SM_HEAR = 46,
	SM_SUMMONED = 47,
	SM_SPELLEND = 48,
	SM_SPELLDECISION = 49,
	SM_STRIKE = 50,
	SM_COLLISION_ERROR = 51,
	SM_WAYPOINT = 52,
	SM_PATHEND = 53,
	SM_CRITICAL = 54,
	SM_COLLIDE_NPC = 55,
	SM_BACKSTAB = 56,
	SM_AGGRESSION = 57,
	SM_COLLISION_ERROR_DETAIL = 58,
	SM_GAME_READY = 59,
	SM_CINE_END = 60,
	SM_KEY_PRESSED = 61,
	SM_CONTROLS_ON = 62,
	SM_CONTROLS_OFF = 63,
	SM_PATHFINDER_FAILURE = 64,
	SM_PATHFINDER_SUCCESS = 65,
	SM_TRAP_DISARMED = 66,
	SM_BOOK_OPEN = 67,
	SM_BOOK_CLOSE = 68,
	SM_IDENTIFY = 69,
	SM_BREAK = 70,
	SM_STEAL = 71,
	SM_COLLIDE_FIELD = 72,
	SM_CURSORMODE = 73,
	SM_EXPLORATIONMODE = 74,
	SM_MAXCMD = 75,
	SM_EXECUTELINE = 255,
	SM_DUMMY = 256
};

extern SCRIPT_VAR * svar;
extern INTERACTIVE_OBJ * EVENT_SENDER;
extern SCR_TIMER * scr_timer;
extern std::string ShowTextWindowtext;
extern std::string ShowText;
extern std::string ShowText2;
extern long NB_GLOBALS;
extern long ActiveTimers;
extern long FORBID_SCRIPT_IO_CREATION;
extern long MAX_TIMER_SCRIPT;

void ARX_SCRIPT_Timer_Check();
void ARX_SCRIPT_Timer_FirstInit(long number);
void ARX_SCRIPT_Timer_ClearAll();
void ARX_SCRIPT_Timer_Clear_For_IO(INTERACTIVE_OBJ * io);
void ARX_SCRIPT_Timer_Clear_By_IO(INTERACTIVE_OBJ * io);
long ARX_SCRIPT_Timer_GetFree();
 
void ARX_SCRIPT_SetMainEvent(INTERACTIVE_OBJ * io, const std::string & newevent);
void ARX_SCRIPT_EventStackExecute();
void ARX_SCRIPT_EventStackExecuteAll();
void ARX_SCRIPT_EventStackInit();
void ARX_SCRIPT_EventStackClear(bool check_exist = true);
void ARX_SCRIPT_ResetObject(INTERACTIVE_OBJ * io, long flags);
void ARX_SCRIPT_Reset(INTERACTIVE_OBJ * io, long flags);
long ARX_SCRIPT_GetSystemIOScript(INTERACTIVE_OBJ * io, const std::string & name);
void ARX_SCRIPT_ComputeShortcuts(EERIE_SCRIPT & es);
void ARX_SCRIPT_AllowInterScriptExec();
long ARX_SCRIPT_CountTimers();
void ARX_SCRIPT_Timer_ClearByNum(long num);
void ARX_SCRIPT_ResetAll(long flags);
void ARX_SCRIPT_EventStackClearForIo(INTERACTIVE_OBJ * io);
INTERACTIVE_OBJ * ARX_SCRIPT_Get_IO_Max_Events();
INTERACTIVE_OBJ * ARX_SCRIPT_Get_IO_Max_Events_Sent();
bool CheckScriptSyntax(INTERACTIVE_OBJ * io);

void ManageCasseDArme(INTERACTIVE_OBJ * io);
void ReleaseScript(EERIE_SCRIPT * es);
long GetNextWord(EERIE_SCRIPT * es, long i, std::string & temp, long flags = 0);
void ARX_SCRIPT_Init_Event_Stats();
ScriptResult SendInitScriptEvent(INTERACTIVE_OBJ * io);

//used by scriptevent
void MakeSSEPARAMS(const char * params);
float GetVarValueInterpretedAsFloat(const std::string & temp1, const EERIE_SCRIPT * esss, INTERACTIVE_OBJ * io);
std::string GetVarValueInterpretedAsText(const std::string & temp1, const EERIE_SCRIPT * esss, INTERACTIVE_OBJ * io);

//! Generates a random name for an unnamed timer
std::string ARX_SCRIPT_Timer_GetDefaultName();

// Use to set the value of a script variable
SCRIPT_VAR * SETVarValueText(SCRIPT_VAR *& svf, long & nb, const std::string &  name, const std::string & val);
SCRIPT_VAR * SETVarValueLong(SCRIPT_VAR *& svf, long & nb, const std::string & name, long val);
SCRIPT_VAR * SETVarValueFloat(SCRIPT_VAR *& svf, long & nb, const std::string & name, float val);

// Use to get the value of a script variable
long GETVarValueLong(SCRIPT_VAR svf[], size_t nb, const std::string & name);
float GETVarValueFloat(SCRIPT_VAR svf[], size_t nb, const std::string & name);
std::string GETVarValueText(SCRIPT_VAR svf[], size_t nb, const std::string & name);

ValueType GetSystemVar(const EERIE_SCRIPT * es, INTERACTIVE_OBJ * io, const std::string & name, std::string & txtcontent, float * fcontent, long * lcontent);
void ARX_SCRIPT_Timer_Clear_All_Locals_For_IO(INTERACTIVE_OBJ * io);
void ARX_SCRIPT_Timer_Clear_By_Name_And_IO(const std::string & timername, INTERACTIVE_OBJ * io);

ScriptResult SendIOScriptEvent(INTERACTIVE_OBJ * io, ScriptMessage msg, const std::string & params = "", const std::string & eventname = "" );

ScriptResult SendMsgToAllIO(ScriptMessage msg, const std::string & params = "");

void Stack_SendIOScriptEvent(INTERACTIVE_OBJ * io, ScriptMessage msg, const std::string & params = "", const std::string & eventname = "");

long FindScriptPos(const EERIE_SCRIPT * es, const std::string & str);
void CloneLocalVars(INTERACTIVE_OBJ * ioo, INTERACTIVE_OBJ * io);
bool IsIOGroup(INTERACTIVE_OBJ * io, const std::string & group);
void ARX_SCRIPT_Free_All_Global_Variables();
void MakeLocalText(EERIE_SCRIPT * es, std::string & tx);
void MakeGlobalText(std::string & tx);

void loadScript(EERIE_SCRIPT & script, PakFile * file);

#endif // ARX_SCRIPTING_SCRIPT_H
