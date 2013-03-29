#pragma once
#include "cocos2d.h"
class Player : public cocos2d::CCSprite
{
public:
//	Player initWithFile(cocos2d::CCString filename);
	void update1(float dt);
//	virtual void update(float dt);
	bool onGround;
	static Player* spriteWithFile(const char *pszFileName);
	static Player* spriteWithFile(const char *pszFileName, int columns, int rows);

	CC_SYNTHESIZE(bool, forwardMarch, ForwardMarch);
	CC_SYNTHESIZE(bool, backwardMarch, BackwardMarch);
	CC_SYNTHESIZE(bool, mightAsWellJump, MightAsWellJump);

	cocos2d::CCPoint desiredPosition;
	cocos2d::CCRect collisionBoundingBox();
//	Player* create(const char *name);
	//void create(const char *name);
	void init1();
	cocos2d::CCPoint velocity;
	CREATE_FUNC(Player);
	
};

