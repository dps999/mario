#pragma once
#include "cocos2d.h"
class Player : public cocos2d::CCSprite
{
public:
//	Player initWithFile(cocos2d::CCString filename);
	void update1(float dt);
	cocos2d::CCPoint desiredPosition;
	cocos2d::CCRect collisionBoundingBox();
//	Player* create(const char *name);
	//void create(const char *name);
	void init1();
	cocos2d::CCPoint velocity;
	Player(void);
	~Player(void);
};

