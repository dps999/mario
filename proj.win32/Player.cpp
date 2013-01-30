#include "Player.h"
using namespace cocos2d;

Player::Player(void)
{
	
}

void Player::create(const char *name)
{
	velocity = ccp(0.0, 0.0);
}

void Player::update1(float dt)
{
	// 2
    CCPoint gravity = ccp(0.0, -450.0);
 
    // 3
    CCPoint gravityStep = ccpMult(gravity, dt);
 
    // 4
    velocity = ccpAdd(velocity, gravityStep);
    CCPoint stepVelocity = ccpMult(velocity, dt);
 
    // 5
	this->setPosition( ccpAdd(this->getPosition(), stepVelocity));
}

//Player Player::initWithFile(CCString filename) {
//    this->initWithFile(filename);
//    return this;
//}
Player::~Player(void)
{
}
