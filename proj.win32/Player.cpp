#include "Player.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

Player* Player::spriteWithFile(const char *pszFileName)
{
	Player *pobSprite = new Player();
	if (pobSprite && pobSprite->initWithFile(pszFileName))
	{
		pobSprite->scheduleUpdate();
		pobSprite->autorelease();
		pobSprite->setForwardMarch(false);
		pobSprite->setMightAsWellJump(false);
		pobSprite->setBackwardMarch(false);

		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

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

	CCPoint forwardMove = ccp(800.0, 0.0);
	CCPoint forwardStep = ccpMult(forwardMove, dt); //1

	velocity = ccpAdd(velocity, gravityStep);
	velocity = ccp(velocity.x * 0.90, velocity.y); //2

	CCPoint jumpForce = ccp(0.0, 2310.0);
	float jumpCutoff = 150.0;

	if (mightAsWellJump && onGround) {
		velocity = ccpAdd(velocity, jumpForce);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SuperKoalio/jump.wav");
	} else if (!mightAsWellJump && velocity.y > jumpCutoff) {
		velocity = ccp(velocity.x, jumpCutoff);
	}

	if (forwardMarch) {
		velocity = ccpAdd(velocity, forwardStep);
	} //3

	if (backwardMarch) {
		velocity = ccpAdd(velocity, forwardStep);

	} //3

	CCPoint minMovement = ccp(0.0, -450.0);
	CCPoint maxMovement = ccp(120.0, 250.0);
	CCPoint stepVelocity = ccpMult(velocity, dt);
	velocity = ccpClamp(velocity, minMovement, maxMovement); //4
	if (backwardMarch) {

		stepVelocity = ccp(-stepVelocity.x, velocity.y);
	} //3

	// 5
	desiredPosition = ( ccpAdd(getPosition(), stepVelocity));
}

//Player Player::initWithFile(CCString filename) {
//    this->initWithFile(filename);
//    return this;
//}

Player* Player::spriteWithFile(const char *pszFileName, int columns, int rows)
{
//	CCSpriteFrame *frame = cacher->spriteFrameByName( pszFileName);
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(pszFileName);
	CCArray* aFrames = CCArray::createWithCapacity(columns * rows);


	float textureWidth =  texture->getPixelsWide();
	float textureHeight = texture->getPixelsHigh();
	float frameWidth = textureWidth / columns;
	float frameHeight = textureHeight / rows;

	for (int c = 0; c < columns; c++)
	{
		for (int r = 0; r < rows; r++)
		{
			CCSpriteFrame *frame0 = CCSpriteFrame::createWithTexture(texture, CCRectMake((c*frameWidth), (r*frameHeight), 100, 65));
			aFrames->addObject(frame0);
		}

	}

	CCAnimation *animation = CCAnimation::createWithSpriteFrames(aFrames, 0.2f);
	CCAnimate *animate = CCAnimate::create(animation);
	CCActionInterval* seq = (CCActionInterval*)(CCSequence::create( animate,   animate->copy()->autorelease(),   CCFlipX::create(false),   NULL) );

	Player *pobSprite = new Player();
	if (pobSprite && pobSprite->initWithSpriteFrame((CCSpriteFrame*) aFrames->objectAtIndex(0)))
	{
		pobSprite->scheduleUpdate();
		pobSprite->autorelease();
		pobSprite->setForwardMarch(false);
		pobSprite->setMightAsWellJump(false);
		pobSprite->setBackwardMarch(false);
		pobSprite->runAction(CCRepeatForever::create( seq ) );
	//	pobSprite->setAnimFrames( aFrames);
	//	pobSprite->setCurrentFrame(0);

		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}