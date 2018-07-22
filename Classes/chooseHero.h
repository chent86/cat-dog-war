#pragma once
#include <stdio.h>
#include "cocos2d.h"

class chooseHero : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
    // callback1为近战，callback2为远程
	void chooseHeroCallback1(cocos2d::Ref* pSender);
    void chooseHeroCallback2(cocos2d::Ref* pSender);

	void backCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(chooseHero);
};
