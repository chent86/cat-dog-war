#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "chooseHero.h"
#include "HelloWorldScene.h"
#include "HelloWorldScene2.h"
#define database UserDefault::getInstance()
USING_NS_CC;

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	if(database->getIntegerForKey("continue") == NULL)
		database->setIntegerForKey("continue", 0);
	if (database->getIntegerForKey("role") == NULL)
		database->setIntegerForKey("role", 0);
	if (database->getIntegerForKey("killNum") == NULL)
		database->setIntegerForKey("killNum", 0);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("bg_selectstage_map_02-hd.png");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	auto title = Sprite::create("bg_main_title-hd.png");
	title->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 110));
	this->addChild(title, 0);

	auto cat = Sprite::create("bg_main_cats.png");
	cat->setPosition(Vec2(visibleSize.width + origin.x - 180, visibleSize.height / 2 + origin.y - 100));
	this->addChild(cat, 1);

	auto border1 = MenuItemImage::create(
		"border.png",
		"border.png",
		CC_CALLBACK_1(MenuScene::startMenuCallback, this)       //点击边框和点击文字都会进行跳转
	);
	auto start_button = Menu::create(border1, NULL);
	border1->setPosition(Vec2(border1->getPositionX(), border1->getPositionY()- 60));
	this->addChild(start_button, 0);

	auto border2 = MenuItemImage::create(
		"border.png",
		"border.png",
		CC_CALLBACK_1(MenuScene::startMenuCallback, this)
	);
	auto continue_button = Menu::create(border2, NULL);
	this->addChild(continue_button, 0);
	continue_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + origin.y - 160));

	auto play = Label::createWithTTF("PLAY", "fonts/Marker Felt.ttf", 30);
	play->setColor(Color3B(0, 0, 0));
	auto play_item = MenuItemLabel::create(play, CC_CALLBACK_1(MenuScene::startMenuCallback, this));
	play_item->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + origin.y-60));

	auto con = Label::createWithTTF("CONTINUE", "fonts/Marker Felt.ttf", 30);
	con->setColor(Color3B(0, 0, 0));
	auto continue_item = MenuItemLabel::create(con, CC_CALLBACK_1(MenuScene::continueMenuCallback, this));
	continue_item->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + origin.y - 160));


	//auto startItem = MenuItemImage::create(
	//										"start.png",
	//										"continue.png",
	//										CC_CALLBACK_1(MenuScene::startMenuCallback, this));

	//startItem->setPosition(Vec2(visibleSize.width / 2 + origin.x + 300, visibleSize.height / 2 + origin.y - 130));
	//auto menu = Menu::create(startItem, NULL);
	//menu->setPosition(Vec2::ZERO);
	//this->addChild(menu, 1);

	auto menu = Menu::create(play_item, continue_item, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

    return true;
}

void MenuScene::startMenuCallback(cocos2d::Ref* pSender) {
	database->setIntegerForKey("continue", 0);
	auto scene = chooseHero::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
}

void MenuScene::continueMenuCallback(cocos2d::Ref* pSender) {
	database->setIntegerForKey("continue", 1);
	if (database->getIntegerForKey("role") == 0 || database->getIntegerForKey("role") == NULL) {
		auto scene = HelloWorld::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
	}
	else if (database->getIntegerForKey("role") == 1) {
		auto scene = HelloWorld2::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
	}
}

