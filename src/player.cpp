#include "player.h"

void Player::increaseRound()
{ 
	m_nRound++; 
	if(m_nRound < 12)
	{
		if(m_nRound % 2 == 0) m_difficulty += 0.1f;
		else m_difficulty += 0.05f;
	}
	else 
	{
		m_difficulty += (float) m_nRound / 100.0f;
	}
}