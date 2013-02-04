#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
	cocos2d::CCTMXTiledMap *map;
	Player *player;
	cocos2d::CCSprite *player1;
	cocos2d::CCTMXLayer *walls;
	cocos2d::CCTMXLayer *hazards;
	cocos2d::CCPoint tileCoordForPosition(cocos2d::CCPoint position) ;
	cocos2d::CCRect tileRectFromTileCoords(cocos2d::CCPoint tileCoords);
	void update1(float dt);
	void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event) ;
	void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event) ;
	void gameOver(bool won);
	void checkForWin();
	bool _gameOver;
	void handleHazardCollisions(Player *p);
	void menuSettings(cocos2d::CCObject* pSender);
	//void ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event) ;
	void setViewpointCenter(cocos2d::CCPoint position );
	cocos2d::CCArray* getSurroundingTilesAtPosition(cocos2d::CCPoint position, cocos2d::CCTMXLayer* layer);
	void checkForAndResolveCollisions(Player *p );
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif  // __HELLOWORLD_SCENE_H__