#pragma once
#include "cocos2d.h"
class Player : public cocos2d::CCSprite
{
public:
//	Player initWithFile(cocos2d::CCString filename);
	void update1(float dt);
	void create(const char *name);
	cocos2d::CCPoint velocity;
	Player(void);
	~Player(void);
};

