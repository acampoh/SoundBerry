/*
 * Copyright 2015 Adrián del Campo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Player.h"
#include <fmod.hpp>
#include <fmod_common.h>

Player* Player::sharedInstance()
{
	static Player sharedInstance;
	return &sharedInstance;
}

Player::Player()
: m_currentSound(nullptr)
, m_mainChannel(nullptr)
{
	FMOD::System_Create(&m_system);
	m_system->init(2, FMOD_INIT_NORMAL, 0);
}

Player::~Player()
{
	m_system->release();

	if (m_currentSound)
	{
		m_currentSound->release();
	}
}

void Player::play(const std::string& uri)
{
	m_system->createStream(uri.c_str(), FMOD_DEFAULT, 0, &m_currentSound);
	m_system->playSound(m_currentSound, nullptr, false, &m_mainChannel);
}

void Player::update(float dt)
{
	m_system->update();
}

