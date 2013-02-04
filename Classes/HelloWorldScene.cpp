#include "HelloWorldScene.h"
#include "Player.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
USING_NS_CC;



CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());
		_gameOver=false;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("SuperKoalio/level1.mp3");    
		this->setKeypadEnabled( true);
		this->setTouchEnabled( true);
		CCLayerColor *blueSky = CCLayerColor::create( ccc4(100, 100, 250, 255));
		this->addChild(blueSky);
		map = CCTMXTiledMap::create("SuperKoalio/level1.tmx");
		this->addChild(map);
		walls = map->layerNamed("walls");
		hazards = map->layerNamed("hazards");


		player = Player::spriteWithFile("SuperKoalio/koalio_stand.png");

		player->setPosition(ccp(100,150));
		map->addChild(player, 15);

		this->schedule( schedule_selector(HelloWorld::update1) );

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
   //     this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // Add the label to HelloWorld layer as a child layer.
   //     this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
   //     this->addChild(pSprite, 0);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::update1(float dt)
{
	 if (_gameOver) {
	  return;
	 }
	 player->update1(dt);
	 if (GetKeyState(VK_RIGHT) & 0x8000)
     {
            player->setForwardMarch( true);
			player->setBackwardMarch( false);
     }
	 else if (GetKeyState(VK_LEFT) & 0x8000)
     {
            player->setBackwardMarch( true);
			player->setForwardMarch( false);
     }
	 else
	 {
		 player->setBackwardMarch( false);
		 player->setForwardMarch( false);
	 }


	 if (GetKeyState(VK_SPACE) & 0x8000)
     {
            player->setMightAsWellJump( true);
     }
	 else
	 {
		 player->setMightAsWellJump( false);
	 }



//	getSurroundingTilesAtPosition(player->getPosition(), walls);
	
	this->handleHazardCollisions(player);
	this->checkForWin();
	this->checkForAndResolveCollisions(player);
	this->setViewpointCenter(player->getPosition());

}

void HelloWorld::handleHazardCollisions(Player *p)
{
	CCArray *tiles = this->getSurroundingTilesAtPosition(p->getPosition(), hazards );
//  for (CCDictionary *dic in tiles) {
	CCObject *d = NULL;
	  CCARRAY_FOREACH(tiles, d)
	  {
		CCDictionary *dic = (CCDictionary *)d;
		CCRect tileRect = CCRectMake(((CCString*)dic->objectForKey("x"))->floatValue(), ((CCString*)dic->objectForKey("y"))->floatValue(), map->getTileSize().width, map->getTileSize().height);
		CCRect pRect = p->collisionBoundingBox();
 
		if (((CCString*)dic->objectForKey("gid"))->intValue() && pRect.intersectsRect( tileRect)) {
		  this->gameOver(0);
		}
	  }
}

void HelloWorld::setViewpointCenter(CCPoint position )
{
 
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
 
    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (map->getMapSize().width * map->getMapSize().width) 
        - winSize.width / 2);
    y = MIN(y, (map->getMapSize().height * map->getMapSize().height) 
        - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);
 
    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	map->setPosition(viewPoint);
 }

CCPoint HelloWorld::tileCoordForPosition(CCPoint position) 
{
  float x = floor(position.x / map->getTileSize().width);
  float levelHeightInPixels = map->getMapSize().height * map->getTileSize().height;
  float y = floor((levelHeightInPixels - position.y) / map->getTileSize().height);
  return ccp(x, y);
}

CCRect HelloWorld::tileRectFromTileCoords(CCPoint tileCoords)
{
  float levelHeightInPixels = map->getMapSize().height * map->getTileSize().height;
  CCPoint origin = ccp(tileCoords.x * map->getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1) * map->getTileSize().height));
  return CCRectMake(origin.x, origin.y, map->getTileSize().width, map->getTileSize().height);
}

CCArray* HelloWorld::getSurroundingTilesAtPosition(CCPoint position, CCTMXLayer* layer) {
 
  CCPoint plPos = tileCoordForPosition(position); //1
 
  CCArray *gids = CCArray::array(); //2
 
  for (int i = 0; i < 9; i++) { //3
    int c = i % 3;
    int r = (int)(i / 3);
    CCPoint tilePos = ccp(plPos.x + (c - 1), plPos.y + (r - 1));
	if (tilePos.y > (map->getMapSize().height - 1))
	{
		//fallen in a hole
		this->gameOver(0);
		return NULL;
	}
    int tgid = layer->tileGIDAt(tilePos); //4
 
    CCRect tileRect = tileRectFromTileCoords(tilePos); //5
   int x=0;
	CCDictionary *tileDict = CCDictionary::create();
 /*                [NSNumber numberWithFloat:tileRect.origin.x], @"x",
                 [NSNumber numberWithFloat:tileRect.origin.y], @"y",
                 [NSValue valueWithCGPoint:tilePos],@"tilePos",
                 nil];*/


	tileDict->setObject(CCString::createWithFormat("%f",tileRect.origin.x), "x");
	x = ((CCString *)tileDict->valueForKey("x"))->intValue();
	tileDict->setObject(CCString::createWithFormat("%f",tileRect.origin.y), "y");
		tileDict->setObject(CCString::createWithFormat("%d",tgid), "gid");
//	tileDict->setObject( tilePos, "tilePos");

    gids->addObject(tileDict);
 
  }
 
  gids->removeObjectAtIndex(4);
  gids->insertObject(gids->objectAtIndex(2), 6);
  gids->removeObjectAtIndex(2);
  gids->exchangeObjectAtIndex(4, 6);
  gids->exchangeObjectAtIndex(0, 4); //6

  CCObject *obj = NULL;

  CCARRAY_FOREACH(gids, obj)
  {
	  CCDictionary *d = (CCDictionary *)obj;
	//  x = ((CCString *)d->objectForKey("x"))->intValue();
		//CCLog("%@", d);
  } //7
 
  return (CCArray *)gids;
}

CCRect CCRectIntersection( CCRect r1,  CCRect r2)
{
	CCRect intersection = CCRectMake(MAX(r1.getMinX(),r2.getMinX()), MAX(r1.getMinY(),r2.getMinY()),0,0);
	intersection.size.width = MIN(r1.getMaxX(), r2.getMaxX()) - intersection.getMinX();
	intersection.size.height = MIN(r1.getMaxY(), r2.getMaxY()) - intersection.getMinY();
	return intersection;
}

void HelloWorld::checkForAndResolveCollisions(Player *p )
{  

  CCArray *tiles = this->getSurroundingTilesAtPosition(p->getPosition(),walls); //1
  if (_gameOver) {
    return;
  }
   p->onGround = false;
 
 // for (CCDictionary *dic in tiles) {
  CCObject *d = NULL;
  CCARRAY_FOREACH(tiles, d)
  {
	CCDictionary *dic = (CCDictionary *)d;
    CCRect pRect = p->collisionBoundingBox(); //2
 
    int gid = ((CCString*)dic->objectForKey("gid"))->intValue(); //3
 
    if (gid) {
		CCRect tileRect = CCRectMake(((CCString*)dic->objectForKey("x"))->floatValue(), ((CCString*)dic->objectForKey("y"))->floatValue(), map->getTileSize().width, map->getTileSize().height); //4
      if (pRect.intersectsRect( tileRect)) {
        CCRect intersection = CCRectIntersection(pRect, tileRect); //5
 
        int tileIndx = tiles->indexOfObject(dic); //6
 
        if (tileIndx == 0) {
          //Ячейка прямо под Коалой
          p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y + intersection.size.height);
          p->velocity = ccp(p->velocity.x, 0.0); //////Здесь
          p->onGround = true; //////Здесь

        } else if (tileIndx == 1) {
          //Ячейка прямо над Коалой
          p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y - intersection.size.height);
		  p->velocity = ccp(p->velocity.x, 0.0); //////Здесь
        } else if (tileIndx == 2) {
          //Ячейка слева от Коалы
          p->desiredPosition = ccp(p->desiredPosition.x + intersection.size.width, p->desiredPosition.y);
        } else if (tileIndx == 3) {
          //Ячейка справа от Коалы
          p->desiredPosition = ccp(p->desiredPosition.x - intersection.size.width, p->desiredPosition.y);
        } else {
          if (intersection.size.width > intersection.size.height) { //7
            //Ячейка диагональна, но решаем проблему вертикально
			p->velocity = ccp(p->velocity.x, 0.0); //////Здесь
            float intersectionHeight;
            if (tileIndx > 5) {
              intersectionHeight = intersection.size.height;
			  p->onGround = true; //////Здесь
            } else {
              intersectionHeight = -intersection.size.height;
            }
            p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y + intersection.size.height );
          } else {
          	//Ячейка диагональна, но решаем проблему горизонтально
            float resolutionWidth;
            if (tileIndx == 6 || tileIndx == 4) {
              resolutionWidth = intersection.size.width;
            } else {
              resolutionWidth = -intersection.size.width;
            }
            p->desiredPosition = ccp(p->desiredPosition.x , p->desiredPosition.y + resolutionWidth);
          } 
        } 
      }
    } 
  }
  p->setPosition( p->desiredPosition); //7
}

void HelloWorld::ccTouchesBegan(CCSet *touches, CCEvent *event) 
{
  // Choose one of the touches to work with
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    CCPoint touchLocation = touch->locationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
//	touchLocation = this->convertToNodeSpace(touchLocation);
    if (touchLocation.x < 240) {
      player->setForwardMarch( true);
    } else {
      player->setMightAsWellJump( true);
    }
}

//void HelloWorld::ccTouchesMoved(CCSet *touches, CCEvent *event) 
//{
//  // Choose one of the touches to work with
//    CCTouch* touch = (CCTouch*)( touches->anyObject() );
//    CCPoint location = touch->locationInView();
//	location = CCDirector::sharedDirector()->convertToGL(location);
//	CCPoint touchLocation = this->convertToNodeSpace(touchLocation);
//    if (touchLocation.x < 240) {
//      player->forwardMarch = false;
//    } else {
//      player->mightAsWellJump = false;
//    }
//}

void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event) 
{
  // Choose one of the touches to work with
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    CCPoint touchLocation = touch->locationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
//	touchLocation = this->convertToNodeSpace(touchLocation);
    if (touchLocation.x < 240) {
      player->setForwardMarch( false);
    } else {
      player->setMightAsWellJump( false);
    }
}

void HelloWorld::checkForWin()
{
  if (player->getPosition().x > 3130.0) {
    this->gameOver(1);
  }
}

void HelloWorld::gameOver(bool won)
{
    _gameOver = true;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SuperKoalio/hurt.wav"); 
    CCString *gameText;
 
    if (won) {
		gameText = CCString::create("You Won!");
    } else {
        gameText = CCString::create("You have Died!");
    }
 
 // CCLabelTTF *diedLabel = [[CCLabelTTF alloc] initWithString:gameText fontName:@"Marker Felt" fontSize:40];
  CCLabelTTF *diedLabel = CCLabelTTF::create(gameText->getCString(), "Marker Felt", 40);
  diedLabel->setPosition(ccp(240, 200));
  CCMoveBy *slideIn = CCMoveBy::create(1.0 ,ccp(0, 250));
  CCMenuItemImage *replay = CCMenuItemImage::create("SuperKoalio/replay.png" ,"SuperKoalio/replay.png" ,"SuperKoalio/replay.png", this, menu_selector(HelloWorld::menuSettings));

 
  CCArray *menuItems = CCArray::arrayWithObject(replay);
  CCMenu *menu = CCMenu::createWithArray (menuItems);
  menu->setPosition( ccp(240, -100));
 
  this->addChild(menu);
  this->addChild(diedLabel);
 
  menu->runAction(slideIn);
}

void HelloWorld::menuSettings(CCObject* pSender)
{
     CCDirector::sharedDirector()->replaceScene(CCTransitionFade::transitionWithDuration(1.0, scene()));
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

