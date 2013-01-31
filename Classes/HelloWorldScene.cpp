#include "HelloWorldScene.h"
#include "Player.h"

using namespace cocos2d;
CCTMXTiledMap *map;
Player *player;
CCTMXLayer *walls;
CCPoint velocity;

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


		
		map = CCTMXTiledMap::create("SuperKoalio/level1.tmx");
		this->addChild(map);
		walls = map->layerNamed("walls");
		velocity = ccp(0.0, 0.0);
		player =(Player*) CCSprite::create("SuperKoalio/koalio_stand.png");
		//player->init1();

		player->setPosition(ccp(100,50));
		map->addChild(player, 15);

		this->schedule( schedule_selector(HelloWorld::update), 1.0 );

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

void HelloWorld::update(float dt)
{
		// 2
    CCPoint gravity = ccp(0.0, -450.0);
 
    // 3
    CCPoint gravityStep = ccpMult(gravity, dt);
 
    // 4
    velocity = ccpAdd(velocity, gravityStep);
    CCPoint stepVelocity = ccpMult(velocity, dt);
 
    // 5
	player->setPosition( ccpAdd(this->getPosition(), stepVelocity));
	getSurroundingTilesAtPosition(player->getPosition(), walls);

//	player->update1(dt);
}

CCPoint tileCoordForPosition(CCPoint position) 
{
  float x = floor(position.x / map->getTileSize().width);
  float levelHeightInPixels = map->getMapSize().height * map->getTileSize().height;
  float y = floor((levelHeightInPixels - position.y) / map->getTileSize().height);
  return ccp(x, y);
}

CCRect tileRectFromTileCoords(CCPoint tileCoords)
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
 
    int tgid = layer->tileGIDAt(tilePos); //4
 
    CCRect tileRect = tileRectFromTileCoords(tilePos); //5
 
	CCDictionary *tileDict = CCDictionary::create();
 /*                [NSNumber numberWithFloat:tileRect.origin.x], @"x",
                 [NSNumber numberWithFloat:tileRect.origin.y], @"y",
                 [NSValue valueWithCGPoint:tilePos],@"tilePos",
                 nil];*/

	tileDict->setObject(CCString::createWithFormat("%d",tgid), "gid");
	tileDict->setObject(CCString::createWithFormat("%d",tileRect.origin.x), "x");
	tileDict->setObject(CCString::createWithFormat("%d",tileRect.origin.y), "y");
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
		CCLog("%@", d);
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
        } else if (tileIndx == 1) {
          //Ячейка прямо над Коалой
          p->desiredPosition = ccp(p->desiredPosition.x, p->desiredPosition.y - intersection.size.height);
        } else if (tileIndx == 2) {
          //Ячейка слева от Коалы
          p->desiredPosition = ccp(p->desiredPosition.x + intersection.size.width, p->desiredPosition.y);
        } else if (tileIndx == 3) {
          //Ячейка справа от Коалы
          p->desiredPosition = ccp(p->desiredPosition.x - intersection.size.width, p->desiredPosition.y);
        } else {
          if (intersection.size.width > intersection.size.height) { //7
            //Ячейка диагональна, но решаем проблему вертикально
            float intersectionHeight;
            if (tileIndx > 5) {
              intersectionHeight = intersection.size.height;
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

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

