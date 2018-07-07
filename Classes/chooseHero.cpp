#include "chooseHero.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "MenuScene.h"

USING_NS_CC;

Scene* chooseHero::createScene()
{
	return chooseHero::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in chooseHero.cpp\n");
}

// on "init" you need to initialize your instance
bool chooseHero::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("bg_selectstage_map_01-hd.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	auto title = Sprite::create("choose_title.png");
	title->setPosition(Vec2(visibleSize.width / 2 + origin.x + 180, visibleSize.height / 2 + origin.y + 250));
	this->addChild(title, 0);

	auto back = MenuItemImage::create(
											"back.png",
											"back.png",
											CC_CALLBACK_1(chooseHero::backCallback, this));
	back->setPosition(Vec2(origin.x + 50, visibleSize.height - 50));

	//近程攻击
	auto hero1 = MenuItemImage::create(
											"hero_1.png",
											"hero_11.png",
											CC_CALLBACK_1(chooseHero::chooseHeroCallback, this));

	hero1->setPosition(Vec2(visibleSize.width / 2 + origin.x + 200, visibleSize.height / 2 + origin.y));

	//auto border1 = Sprite::create("border.png");
	//border1->setPosition(Vec2(visibleSize.width / 2 + origin.x + 200, visibleSize.height / 2 + origin.y - 250));
	//this->addChild(border1, 0);

	auto border1 = MenuItemImage::create(
		"border.png",
		"border.png",
		CC_CALLBACK_1(chooseHero::chooseHeroCallback, this)       //点击英雄图片和英雄名字都会开始游戏
	);
	auto first_button = Menu::create(border1, NULL);
	border1->setPosition(Vec2(border1->getPositionX()+200, border1->getPositionY() - 250));
	this->addChild(first_button, 0);

	auto hero_1_name = Sprite::create("hero_1_name.png");
	hero_1_name->setPosition(Vec2(visibleSize.width / 2 + origin.x + 200, visibleSize.height / 2 + origin.y - 250));
	this->addChild(hero_1_name, 0);

	//远程攻击
	auto hero2 = MenuItemImage::create(
											"hero_2.png",
											"hero_21.png",
											CC_CALLBACK_1(chooseHero::chooseHeroCallback, this));

	hero2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 200, visibleSize.height / 2 + origin.y));



	//auto border2 = Sprite::create("border.png");
	//border2->setPosition(Vec2(visibleSize.width / 2 + origin.x - 200, visibleSize.height / 2 + origin.y - 250));
	//this->addChild(border2, 0);

	auto border2 = MenuItemImage::create(
		"border.png",
		"border.png",
		CC_CALLBACK_1(chooseHero::chooseHeroCallback, this)       //点击英雄图片和英雄名字都会开始游戏
	);
	auto second_button = Menu::create(border2, NULL);
	border2->setPosition(Vec2(border2->getPositionX() - 200, border2->getPositionY() - 250));
	this->addChild(second_button, 0);

	auto hero_2_name = Sprite::create("hero_2_name.png");
	hero_2_name->setPosition(Vec2(visibleSize.width / 2 + origin.x - 200, visibleSize.height / 2 + origin.y - 250));
	this->addChild(hero_2_name, 0);

	auto menu = Menu::create(hero1, hero2, back, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void chooseHero::chooseHeroCallback(cocos2d::Ref* pSender) {
	auto scene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
}

void chooseHero::backCallback(cocos2d::Ref* pSender) {
	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
}

