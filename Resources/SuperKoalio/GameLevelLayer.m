//
//  GameLevelLayer.m
//  SuperKoalio
//
//  Created by Jacob Gundersen on 6/4/12.


#import "GameLevelLayer.h"


@implementation GameLevelLayer

+(CCScene *) scene
{
	// 'scene' is an autorelease object.
	CCScene *scene = [CCScene node];
	
	// 'layer' is an autorelease object.
	GameLevelLayer *layer = [GameLevelLayer node];
	
	// add layer as a child to scene
	[scene addChild: layer];
	
	// return the scene
	return scene;
}

-(id) init
{
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super's" return value
	if( (self=[super init]) ) {
        //init goes here
	}
	return self;
}



@end
