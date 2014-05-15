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

#include "graphics/particle/ParticleSystem.h"

#include <cstdio>
#include <cstring>

#include <boost/foreach.hpp>

#include "core/GameTime.h"

#include "graphics/Draw.h"
#include "graphics/Math.h"
#include "graphics/GraphicsTypes.h"
#include "graphics/data/TextureContainer.h"
#include "graphics/effects/SpellEffects.h"
#include "graphics/particle/ParticleParams.h"
#include "graphics/particle/Particle.h"

#include "scene/Light.h"

using std::list;

void ParticleSystem::RecomputeDirection() {
	Vec3f eVect = m_parameters.m_direction;
	eVect.y = -eVect.y;
	GenerateMatrixUsingVector(eMat, eVect, 0);
}

ParticleSystem::ParticleSystem() {
	
	int i;
	for(i = 0; i < 20; i++) {
		tex_tab[i] = NULL;
	}
	
	lLightId = -1;
	
	m_parameters.m_nbMax = 50;
	
	ulNbParticleGen = 10;
	iParticleNbAlive = 0;
	iNbTex = 0;
	iTexTime = 500;
	bTexLoop = true;
	fParticleRotation = 0;
	
	fParticleFreq = -1;
	ulParticleSpawn = 0;
	
	// default settings for EDITOR MODE only
	Vec3f eVect = m_parameters.m_direction = -Vec3f_Y_AXIS;
	eVect.y = -eVect.y;
	GenerateMatrixUsingVector(eMat, eVect, 0);
	
	fParticleStartSize = 1;
	fParticleEndSize = 1;
	fParticleStartColor[0] = 0.1f;
	fParticleStartColor[1] = 0.1f;
	fParticleStartColor[2] = 0.1f;
	fParticleStartColor[3] = 0.1f;
	fParticleEndColor[0] = 0.1f;
	fParticleEndColor[1] = 0.1f;
	fParticleEndColor[2] = 0.1f;
	fParticleEndColor[3] = 0.1f;
	m_parameters.m_speed = 10;
	m_parameters.m_life = 1000;
	m_parameters.m_pos = Vec3f_ZERO;
	bParticleFollow = true;
	fParticleFlash = 0;
	fParticleRotation = 0;
	bParticleRotationRandomDirection = false;
	bParticleRotationRandomStart = false;
	m_parameters.m_gravity = Vec3f_ZERO;
	m_parameters.m_lifeRandom = 1000;
	m_parameters.m_angle = 0;
	m_parameters.m_speedRandom = 10;

	bParticleStartColorRandomLock = false;
	fParticleStartSizeRandom = 1;
	fParticleStartColorRandom[0] = 0.1f;
	fParticleStartColorRandom[1] = 0.1f;
	fParticleStartColorRandom[2] = 0.1f;
	fParticleStartColorRandom[3] = 0.1f;

	bParticleEndColorRandomLock = false;
	fParticleEndSizeRandom = 1;
	fParticleEndColorRandom[0] = 0.1f;
	fParticleEndColorRandom[1] = 0.1f;
	fParticleEndColorRandom[2] = 0.1f;
	fParticleEndColorRandom[3] = 0.1f;

	blendMode = RenderMaterial::Additive;
}

ParticleSystem::~ParticleSystem() {
	
	BOOST_FOREACH(Particle * p, listParticle) {
		delete p;
	}
	
	listParticle.clear();
}

void ParticleSystem::SetPos(const Vec3f & _p3) {
	
	p3Pos = _p3;
	if(lightHandleIsValid(lLightId)) {
		EERIE_LIGHT * light = lightHandleGet(lLightId);
		
		light->pos = p3Pos;
	}
}

void ParticleSystem::SetColor(float _fR, float _fG, float _fB) {
	if(lightHandleIsValid(lLightId)) {
		EERIE_LIGHT * light = lightHandleGet(lLightId);
		
		light->rgb = Color3f(_fR, _fG, _fB);
	}
}

void ParticleSystem::SetParams(const ParticleParams & _pp) {
	
	m_parameters.m_pos = _pp.m_pos;
	m_parameters.m_direction = _pp.m_direction * 0.1f;
	m_parameters.m_gravity = _pp.m_gravity;
	
	m_parameters.m_nbMax		= _pp.m_nbMax;
	m_parameters.m_life		= _pp.m_life;
	m_parameters.m_lifeRandom = _pp.m_lifeRandom;
	
	m_parameters.m_angle = _pp.m_angle;
	m_parameters.m_speed = _pp.m_speed;
	m_parameters.m_speedRandom = _pp.m_speedRandom;
	
	fParticleFlash = _pp.m_flash * ( 1.0f / 100 );
	
	if(_pp.m_rotation >= 2) {
		fParticleRotation = 1.0f / (101 - _pp.m_rotation);
	} else {
		fParticleRotation = 0.0f;
	}

	bParticleRotationRandomDirection = _pp.m_rotationRandomDirection;
	bParticleRotationRandomStart = _pp.m_rotationRandomStart;

	fParticleStartSize = _pp.m_startSize;
	fParticleStartSizeRandom = _pp.m_startSizeRandom;

	for(int i = 0; i < 4; i++) {
		fParticleStartColor[i] = _pp.m_startColor[i] / 255.0f;
		fParticleStartColorRandom[i] = _pp.m_startColorRandom[i] / 255.0f;
	}

	bParticleStartColorRandomLock = _pp.m_startLock;

	fParticleEndSize = _pp.m_endSize;
	fParticleEndSizeRandom = _pp.m_endSizeRandom;

	for(int i = 0; i < 4; i++) {
		fParticleEndColor[i] = _pp.m_endColor[i] / 255.0f;
		fParticleEndColorRandom[i] = _pp.m_endColorRandom[i] / 255.0f;
	}

	bParticleEndColorRandomLock = _pp.m_endLock;

	m_parameters.m_direction = glm::normalize(m_parameters.m_direction);
	Vec3f eVect(m_parameters.m_direction.x, -m_parameters.m_direction.y, m_parameters.m_direction.z);
	GenerateMatrixUsingVector(eMat, eVect, 0);

	float r = (fParticleStartColor[0]  + fParticleEndColor[0] ) * 0.5f;
	float g = (fParticleStartColor[1]  + fParticleEndColor[1] ) * 0.5f;
	float b = (fParticleStartColor[2]  + fParticleEndColor[2] ) * 0.5f;
	SetColor(r, g, b);

	blendMode = _pp.m_blendMode;

	if(_pp.m_texInfo) {
		SetTexture(_pp.m_texName, _pp.m_texNb, _pp.m_texTime, _pp.m_texLoop);
	}
}

void ParticleSystem::SetTexture(const char * _pszTex, int _iNbTex, int _iTime, bool _bLoop) {

	if(_iNbTex == 0) {
		tex_tab[0] = TextureContainer::Load(_pszTex);
		iNbTex = 0;
	} else {
		_iNbTex = min(_iNbTex, 20);
		char cBuf[256];

		for(int i = 0; i < _iNbTex; i++) {
			memset(cBuf, 0, 256);
			sprintf(cBuf, "%s_%04d", _pszTex, i + 1);
			tex_tab[i] = TextureContainer::Load(cBuf);
		}

		iNbTex = _iNbTex;
		iTexTime = _iTime;
		bTexLoop = _bLoop;
	}
}

void ParticleSystem::SpawnParticle(Particle * pP) {
	
	pP->p3Pos = Vec3f_ZERO;
	
	if((ulParticleSpawn & PARTICLE_CIRCULAR) == PARTICLE_CIRCULAR
	   && (ulParticleSpawn & PARTICLE_BORDER) == PARTICLE_BORDER) {
		float randd = rnd() * 360.f;
		pP->p3Pos.x = std::sin(randd) * m_parameters.m_pos.x;
		pP->p3Pos.y = rnd() * m_parameters.m_pos.y;
		pP->p3Pos.z = std::cos(randd) * m_parameters.m_pos.z;
	} else if((ulParticleSpawn & PARTICLE_CIRCULAR) == PARTICLE_CIRCULAR) {
		float randd = rnd() * 360.f;
		pP->p3Pos.x = std::sin(randd) * rnd() * m_parameters.m_pos.x;
		pP->p3Pos.y = rnd() * m_parameters.m_pos.y;
		pP->p3Pos.z = std::cos(randd) * rnd() * m_parameters.m_pos.z;
	} else {
		pP->p3Pos = m_parameters.m_pos * randomVec(-1.f, 1.f);
	}
	
	if(bParticleFollow == false) {
		pP->p3Pos = p3Pos;
	}
}

void VectorRotateY(Vec3f & _eIn, Vec3f & _eOut, float _fAngle) {
	float c = std::cos(_fAngle);
	float s = std::sin(_fAngle);
	_eOut.x = (_eIn.x * c) + (_eIn.z * s);
	_eOut.y =  _eIn.y;
	_eOut.z = (_eIn.z * c) - (_eIn.x * s);
}

void VectorRotateZ(Vec3f & _eIn, Vec3f & _eOut, float _fAngle) {
	float c = std::cos(_fAngle);
	float s = std::sin(_fAngle);
	_eOut.x = (_eIn.x * c) + (_eIn.y * s);
	_eOut.y = (_eIn.y * c) - (_eIn.x * s);
	_eOut.z =  _eIn.z;
}

void ParticleSystem::SetParticleParams(Particle * pP) {

	SpawnParticle(pP);

	float fTTL = m_parameters.m_life + rnd() * m_parameters.m_lifeRandom;
	pP->ulTTL = checked_range_cast<long>(fTTL);
	pP->fOneOnTTL = 1.0f / (float)pP->ulTTL;

	float fAngleX = rnd() * m_parameters.m_angle; //*0.5f;
 
	Vec3f vv1, vvz;
	
	// ici modifs ----------------------------------
	
	vv1 = -Vec3f_Y_AXIS;
	
	VectorRotateZ(vv1, vvz, fAngleX); 
	VectorRotateY(vvz, vv1, radians(rnd() * 360.0f));
	
	vvz = Vec3f(eMat * Vec4f(vv1, 1.f));

	float fSpeed = m_parameters.m_speed + rnd() * m_parameters.m_speedRandom;

	pP->p3Velocity = vvz * fSpeed;
	pP->fSizeStart = fParticleStartSize + rnd() * fParticleStartSizeRandom;

	if(bParticleStartColorRandomLock) {
		float t = rnd() * fParticleStartColorRandom[0];
		pP->fColorStart[0] = fParticleStartColor[0] + t;
		pP->fColorStart[1] = fParticleStartColor[1] + t;
		pP->fColorStart[2] = fParticleStartColor[2] + t;
	} else {
		pP->fColorStart[0] = fParticleStartColor[0] + rnd() * fParticleStartColorRandom[0];
		pP->fColorStart[1] = fParticleStartColor[1] + rnd() * fParticleStartColorRandom[1];
		pP->fColorStart[2] = fParticleStartColor[2] + rnd() * fParticleStartColorRandom[2];
	}

	pP->fColorStart[3] = fParticleStartColor[3] + rnd() * fParticleStartColorRandom[3];

	pP->fSizeEnd = fParticleEndSize + rnd() * fParticleEndSizeRandom;

	if(bParticleEndColorRandomLock) {
		float t = rnd() * fParticleEndColorRandom[0];
		pP->fColorEnd[0] = fParticleEndColor[0] + t;
		pP->fColorEnd[1] = fParticleEndColor[1] + t;
		pP->fColorEnd[2] = fParticleEndColor[2] + t;
	} else {
		pP->fColorEnd[0] = fParticleEndColor[0] + rnd() * fParticleEndColorRandom[0];
		pP->fColorEnd[1] = fParticleEndColor[1] + rnd() * fParticleEndColorRandom[1];
		pP->fColorEnd[2] = fParticleEndColor[2] + rnd() * fParticleEndColorRandom[2];
	}

	pP->fColorEnd[3] = fParticleEndColor[3] + rnd() * fParticleEndColorRandom[3];

	if(bParticleRotationRandomDirection) {
		float fRandom	= frand2();

		pP->iRot = checked_range_cast<int>(fRandom);

		if(pP->iRot < 0)
			pP->iRot = -1;

		if(pP->iRot >= 0)
			pP->iRot = 1;
	} else {
		pP->iRot = 1;
	}

	if(bParticleRotationRandomStart) {
		pP->fRotStart = rnd() * 360.0f;
	} else {
		pP->fRotStart = 0;
	}
}

bool ParticleSystem::IsAlive() {

	if((iParticleNbAlive == 0) && (m_parameters.m_nbMax == 0))
		return false;

	return true;
}

void ParticleSystem::Update(long _lTime) {

	if(arxtime.is_paused())
		return;

	int nbtotal = 0;
	int iNb;
	float fTimeSec = _lTime * ( 1.0f / 1000 );
	Particle * pP;

	list<Particle *>::iterator i;
	iParticleNbAlive = 0;

	i = listParticle.begin();

	while(i != listParticle.end()) {
		pP = *i;
		++i;
		nbtotal++;

		if(pP->isAlive()) {
			pP->Update(_lTime);
			pP->p3Velocity += m_parameters.m_gravity * fTimeSec;
			iParticleNbAlive ++;
		} else {
			if(iParticleNbAlive >= m_parameters.m_nbMax) {
				delete pP;
				listParticle.remove(pP);
			} else {
				pP->Regen();
				SetParticleParams(pP);
				pP->Validate();
				pP->Update(0);
				ulNbParticleGen++;
				iParticleNbAlive++;
			}
		}
	}

	// création de particules en fct de la fréquence
	if(iParticleNbAlive < m_parameters.m_nbMax) {
		long t = m_parameters.m_nbMax - iParticleNbAlive;
		
		if(fParticleFreq != -1.f) {
			t = std::min(long(m_storedTime.update(fTimeSec * fParticleFreq)), t);
		}
		
		for(iNb = 0; iNb < t; iNb++) {
			Particle * pP  = new Particle();
			SetParticleParams(pP);
			pP->Validate();
			pP->Update(0);
			listParticle.insert(listParticle.end(), pP);
			ulNbParticleGen ++;
			iParticleNbAlive++;
		}
	}
}

void ParticleSystem::Render() {
	
	RenderMaterial mat;
	mat.setBlendType(blendMode);
	mat.setDepthTest(true);

	int inumtex = 0;

	list<Particle *>::iterator i;

	for(i = listParticle.begin(); i != listParticle.end(); ++i) {
		Particle * p = *i;

		if(p->isAlive()) {
			if(fParticleFlash > 0) {
				if(rnd() < fParticleFlash)
					continue;
			}

			if(iNbTex > 0) {
				inumtex = p->iTexNum;

				if(iTexTime == 0) {
					float fNbTex	= (p->ulTime * p->fOneOnTTL) * (iNbTex);

					inumtex = checked_range_cast<int>(fNbTex);
					if(inumtex >= iNbTex) {
						inumtex = iNbTex - 1;
					}
				} else {
					if(p->iTexTime > iTexTime) {
						p->iTexTime -= iTexTime;
						p->iTexNum++;

						if(p->iTexNum > iNbTex - 1) {
							if(bTexLoop) {
								p->iTexNum = 0;
							} else {
								p->iTexNum = iNbTex - 1;
							}
						}

						inumtex = p->iTexNum;
					}
				}
			}
			
			TexturedVertex p3pos;
			p3pos.p = p->p3Pos;
			if(bParticleFollow) {
				p3pos.p += p3Pos;
			}
            
			mat.setTexture(tex_tab[inumtex]);
			
			if(fParticleRotation != 0) {
				float fRot;
				if(p->iRot == 1)
					fRot = (fParticleRotation) * p->ulTime + p->fRotStart;
				else
					fRot = (-fParticleRotation) * p->ulTime + p->fRotStart;

				float size = std::max(p->fSize, 0.f);
				
				if(tex_tab[inumtex])
					EERIEAddSprite(mat, p3pos, size, p->ulColor, 2, fRot);
			} else {
				if(tex_tab[inumtex])
					EERIEAddSprite(mat, p3pos, p->fSize, p->ulColor, 2);
			}
		}
	}
}
