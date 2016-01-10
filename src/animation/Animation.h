/*
 * Copyright 2011-2012 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Based on:
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
// Code: Cyril Meynier
//
// Copyright (c) 1999 ARKANE Studios SA. All rights reserved

#ifndef ARX_ANIMATION_ANIMATION_H
#define ARX_ANIMATION_ANIMATION_H

#include <stddef.h>
#include <string>

#include "math/Types.h"
#include "graphics/BaseGraphicsTypes.h"
#include "graphics/GraphicsTypes.h"

class Entity;

struct EERIE_FRAME
{
	long		num_frame;
	long		flag;
	int			master_key_frame;
	short		f_translate; //int
	short		f_rotate; //int
	float		time;
	Vec3f	translate;
	glm::quat	quat;
	audio::SampleId	sample;
};

struct EERIE_GROUP
{
	int		key;
	Vec3f	translate;
	glm::quat	quat;
	Vec3f	zoom;
};

struct EERIE_ANIM
{
	long		anim_time;
	unsigned long	flag;
	long		nb_groups;
	long		nb_key_frames;
	EERIE_FRAME *	frames;
	EERIE_GROUP  *  groups;
	unsigned char *	voidgroups;
};

struct ANIM_HANDLE {

	ANIM_HANDLE();

	res::path path; // empty path means an unallocated slot
	EERIE_ANIM ** anims;
	short alt_nb;
	long locks;
};

// Animation playing flags
enum AnimUseTypeFlag {
	EA_LOOP			= 1,	// Must be looped at end (indefinitely...)
	EA_REVERSE		= 2,	// Is played reversed (from end to start)
	EA_PAUSED		= 4,	// Is paused
	EA_ANIMEND		= 8,	// Has just finished
	EA_STATICANIM	= 16,	// Is a static Anim (no movement offset returned).
	EA_STOPEND		= 32,	// Must Be Stopped at end.
	EA_FORCEPLAY	= 64,	// User controlled... MUST be played...
	EA_EXCONTROL	= 128	// ctime externally set, no update.
};
DECLARE_FLAGS(AnimUseTypeFlag, AnimUseType)
DECLARE_FLAGS_OPERATORS(AnimUseType)

struct AnimLayer {

	AnimLayer()
		: next_anim(NULL)
		, cur_anim(NULL)
		, altidx_next(0)
		, altidx_cur(0)
		, ctime(0)
		, flags(0)
		, nextflags(0)
		, lastframe(-1)
		, pour(0.f)
		, fr(0)
	{}

	ANIM_HANDLE * next_anim;
	ANIM_HANDLE * cur_anim;
	short altidx_next; // idx to alternate anims...
	short altidx_cur; // idx to alternate anims...
	long ctime;
	AnimUseType flags;
	AnimUseType nextflags;
	long lastframe;
	float pour;
	long fr;
};

/*!
 * Cancel any existing animation and then start a new one.
 * This will reset the current annimation even if it is the same as the given animation.
 * \param entity           the entity being animated
 * \param layer            the animation layer to change
 * \param animation        the new animation to set
 * \param flags            animation use flags to set after changing the animation
 * \param startAtBeginning force the animation to start at the first keyframe
 *                         - otherwise it may start at a randomly chosen one
 */
void changeAnimation(Entity * entity, size_t layer, ANIM_HANDLE * animation,
                          AnimUseType flags = 0, bool startAtBeginning = false);
/*!
 * Cancel any existing primary animation and then start a new one.
 * This will reset the current annimation even if it is the same as the given animation.
 * \param entity           the entity being animated
 * \param animation        the new animation to set
 * \param flags            animation use flags to set after changing the animation
 * \param startAtBeginning force the animation to start at the first keyframe
 *                         - otherwise it may start at a randomly chosen one
 */
void changeAnimation(Entity * entity, ANIM_HANDLE * animation,
                     AnimUseType flags = 0, bool startAtBeginning = false);

/*!
 * Change the animation of an entity if it isn't already set to the same one.
 * Unlike \ref changeAnimation(), this will not reset the current animation unless it
 * differs from the current one.
 * \param entity    the entity being animated
 * \param animation the new animation to set
 * \param flags     animation use flags to set if the animation has been changed
 */
void setAnimation(Entity * entity, ANIM_HANDLE * animation,
                  AnimUseType flags = 0, bool startAtBeginning = false);

/*!
 * Stop the current animation.
 * \param entity           the entity being animated
 * \param layer            the animation layer to change
 */
void stopAnimation(Entity * entity, size_t layer = 0);

short ANIM_GetAltIdx(ANIM_HANDLE * ah,long old);
void ANIM_Set(AnimLayer & layer, ANIM_HANDLE * anim);

Vec3f GetAnimTotalTranslate(ANIM_HANDLE * eanim, long alt_idx);

void EERIE_ANIMMANAGER_ClearAll();
void EERIE_ANIMMANAGER_PurgeUnused();
void EERIE_ANIMMANAGER_ReleaseHandle(ANIM_HANDLE * anim);
ANIM_HANDLE * EERIE_ANIMMANAGER_Load(const res::path & path);
ANIM_HANDLE * EERIE_ANIMMANAGER_Load_NoWarning(const res::path & path);

void PrepareAnim(AnimLayer & layer, unsigned long time, Entity *io);
void ResetAnim(AnimLayer & layer);

void AcquireLastAnim(Entity * io);
void FinishAnim(Entity * io,ANIM_HANDLE * eanim);

void ARX_SOUND_PushAnimSamples();
void ARX_SOUND_PopAnimSamples();

void ReleaseAnimFromIO(Entity * io,long num);

#endif // ARX_ANIMATION_ANIMATION_H
