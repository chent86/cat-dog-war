#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "chooseHero.h"

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

	auto border1 = Sprite::create("border.png");
	border1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + origin.y - 70));
	this->addChild(border1, 0);

	auto border2 = Sprite::create("border.png");
	border2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + origin.y - 160));
	this->addChild(border2, 0);

	auto play = Label::createWithTTF("PLAY", "fonts/Marker Felt.ttf", 30);
	play->setColor(Color3B(0, 0, 0));
	auto play_item = MenuItemLabel::create(play, CC_CALLBACK_1(MenuScene::startMenuCallback, this));
	play_item->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + origin.y - 70));

	auto con = Label::createWithTTF("CONTINUE", "fonts/Marker Felt.ttf", 30);
	con->setColor(Color3B(0, 0, 0));
	auto continue_item = MenuItemLabel::create(con, CC_CALLBACK_1(MenuScene::startMenuCallback, this));
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
	auto scene = chooseHero::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
}

