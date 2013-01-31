#include "Player.h"
using namespace cocos2d;

Player::Player(void)
{
	
}

void Player::init1()
{
	this->velocity = ccp(0.0, 0.0);
}

//Player* Player::create(const char *name)
//{
////	velocity = ccp(0.0, 0.0);
//	return (Player*)CCSprite::create(name);
//}

CCRect Player::collisionBoundingBox() {
  CCRect collisionBox = this->boundingBox();
  CCPoint diff = ccpSub(this->desiredPosition, this->getPosition());
  CCRect returnBoundingBox;
  returnBoundingBox.setRect(collisionBox.getMinX() + diff.x, collisionBox.getMinY() + diff.y, collisionBox.getMaxX()-collisionBox.getMinX(), collisionBox.getMaxY()-collisionBox.getMinY());
  return returnBoundingBox;
//  return CCRect::intersectsRect(this->boundingBox, 2, 0);
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
	CCSprite::update(dt);
}

//Player Player::initWithFile(CCString filename) {
//    this->initWithFile(filename);
//    return this;
//}
Player::~Player(void)
{
}
