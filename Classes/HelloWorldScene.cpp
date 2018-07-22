#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Monster.h"
#include "MenuScene.h"
#pragma execution_character_set("utf-8")
#define database UserDefault::getInstance()
#define fac Factory::getInstance()
USING_NS_CC;

boolean bool_num = 1;
int attacknum = 0;

void HelloWorld::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setAutoStep(true);

	// Debug 模式
	// scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
	auto layer = HelloWorld::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//mp次数
	auto magic = Sprite::create("magic.png");
	magic->setPosition(Vec2(30, this->visibleSize.height-70));
	this->addChild(magic,1);
	magic_label = Label::createWithTTF("1", "fonts/arial.ttf", 36);
	magic_label->setPosition(Vec2(70, this->visibleSize.height - 70));
	this->addChild(magic_label);

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x + 14 * pT->getContentSize().width, origin.y + visibleSize.height - 2 * pT->getContentSize().height));
	addChild(pT, 1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0, 0);

	auto frame0 = SpriteFrame::create("hero_03_move_01.png", Rect(0, 0, 213, 170));

	player = Sprite::create("hero_03_move_01.png");
	player->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + 100));
	this->addChild(player, 2);

	// 攻击动画
	attack.reserve(4);
	for (int i = 1; i <= 4; i++) {
		char attack_pic[25];
		sprintf(attack_pic, "hero_03_attack_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(attack_pic, Rect(0, 0, 213, 170));
		attack.pushBack(frame);
		if (i == 4) attack.pushBack(frame0);
	}
	
	//死亡动画
	dead.reserve(4);
	for (int i = 1; i < 4; i++) {
		char dead_pic[25];
		sprintf(dead_pic, "explo_01_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(dead_pic, Rect(0, 0, 120, 120));
		dead.pushBack(frame);
	}
	SpriteFrame* dead_frame = SpriteFrame::create("ghost_dog.png", Rect(0, 0, 70, 100));
	dead.pushBack(dead_frame);

	//移动动画
	run.reserve(2);
	for (int i = 1; i <= 2; i++) {
		char run_pic[25];
		sprintf(run_pic, "hero_03_move_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(run_pic, Rect(0, 0, 213, 170));
		run.pushBack(frame);
	}

	//被攻击动画
	hurt.reserve(2);
	SpriteFrame* hurt_frame = SpriteFrame::create("hero_03_damage_01.png", Rect(0, 0, 213, 170));
	hurt.pushBack(hurt_frame);
	hurt.pushBack(frame0);

	//闪电动画
	thunder.reserve(4);
	for (int i = 1; i <= 4; i++) {
		char run_pic[25];
		sprintf(run_pic, "thunder_beam_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(run_pic, Rect(0, 0, 213, 400));
		thunder.pushBack(frame);
	}

	//地雷动画
	mine.reserve(3);
	for (int i = 1; i <= 4; i++) {
		char run_pic[25];
		sprintf(run_pic, "explo_03_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(run_pic, Rect(0, 0, 213, 400));
		mine.pushBack(frame);
	}

	//计时器
	time = Label::createWithTTF("0", "fonts/arial.ttf", 36);
	time->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 70));
	this->addChild(time);
	schedule(schedule_selector(HelloWorld::updateTime), 1);
	dtime = 0;

	//击杀数量
	database->setIntegerForKey("killNum", 0);
	attacknum = database->getIntegerForKey("killNum");
	char str[10];
	sprintf(str, "%d", attacknum);
	killnum = Label::createWithTTF(str, "fonts/arial.ttf", 36);
	killnum->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 30));
	this->addChild(killnum);

	//调度器
	schedule(schedule_selector(HelloWorld::Movetoplayer), 3);
	schedule(schedule_selector(HelloWorld::hitByMonster), 0.5);
	schedule(schedule_selector(HelloWorld::getDrug), 0.5);
	schedule(schedule_selector(HelloWorld::getBomb), 0.5);
	schedule(schedule_selector(HelloWorld::update), 0.05);

	addKeyboardListener();

	//背景图片
	bg = Sprite::create("timg.jpg");
	bg->setPosition(Vec2(origin.x + visibleSize.width * 1.4, visibleSize.height / 2 + origin.y));

	bgLayer = Layer::create();
	bgLayer->setPosition(Vec2(0, 0));
	bgLayer->setAnchorPoint(Vec2(0, 0));
	bgLayer->ignoreAnchorPointForPosition(false);
	bgLayer->addChild(bg);
	this->addChild(bgLayer, -1);

	this->last_key = 'D';

	for (int i = 0; i < 4; i++) {
		group[i] = false;
		monster_bomb[i] = 3;
	}

	m_factory = Factory::getInstance();

	isMove = false;
	vertical_isMove = false;

	magic_num = 1;

	auto back = Label::createWithTTF("BACK", "fonts/Marker Felt.ttf", 30);
	back->setColor(Color3B(255, 255, 255));
	auto back_item = MenuItemLabel::create(back, CC_CALLBACK_1(HelloWorld::quit, this));
	back_item->setPosition(Vec2(visibleSize.width - 50, visibleSize.height - 30));
	
	auto menu = Menu::create(back_item, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void HelloWorld::quit(cocos2d::Ref* pSender) {
	auto scene = MenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
	
}

void HelloWorld::update(float f) {
	if ((vertical_isMove||isMove) && !isAttack && !isHurt) {
		if (vertical_isMove) {
			this->vertical_movePlayer(vertical_movekey);
		}
		if (isMove) {
			this->movePlayer(movekey);
		}
		move();
	}
	else if (!isMove && !vertical_isMove && !isAttack && !isHurt) stop(); 
// 背景总宽度： 2710
// 人物默认位于 330:1000 的位置
	for (int i = 0; i < 4; i++) {
		auto p = bgLayer->convertToNodeSpace(player->getPosition());
		if (p.x > 400*i && p.x < 400*(i + 1) && group[i] == false) {
			group[i] = true;
			for (int j = 0; j < 3; j++) {
				auto m = fac->createMonster();
				auto moster_y = 50*(j+1);
				bgLayer->addChild(m);
				Vec2 Point = bgLayer->convertToNodeSpace(Vec2(400*i+ 700, moster_y));
				//Vec2 Point = bgLayer->convertToWorldSpaceAR(Vec2(moster_x, moster_y));
				m->setPosition(Point);
				m->setTag(i);
			}
		}
	}

	for (auto i : fac->getMonster()) {
		if (this->bgLayer->convertToWorldSpace(i->getPosition()).x - this->player->getPosition().x < 600) {
			if (monster_bomb[i->getTag()] > 0) {
				monster_bomb[i->getTag()]--;
				if (i->getTag() == 0)
					throwBomb(i, 1.9f);   //未知bug，第一波兵扔的炸弹高一点，所以要落久一点
				else
					throwBomb(i, 1.4f);
			}
		}
	}
	
	if (m_factory->getMonsterNum() == 0 && group[3] == true) {
		stop();
		bool_num = 0;
		auto win_cat = Sprite::create("victorycat.png");
		win_cat->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(win_cat, 0);

		auto win_word = Sprite::create("victory-hd.png");
		win_word->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y-100));
		this->addChild(win_word, 0);
	}

}

//键盘监听
void HelloWorld::addKeyboardListener() {
	auto keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyboard->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);
}

//攻击事件
void HelloWorld::attackCallback(cocos2d::Ref* pSender) {
	if (bool_num) {
		bool_num = 0;
		//攻击动画
		auto attack_animation = Animation::createWithSpriteFrames(attack, 0.1f);
		auto attack_animate = Animate::create(attack_animation);
		player->runAction(attack_animate);

		auto delayTime = DelayTime::create(0.4f);
		auto func = CallFunc::create([this]()
		{
			isAttack = 0;
			bool_num = 1;
			//判断是否还有按键在
			if (A_press || D_press ) {
				isMove = true;
			}
			if (W_press || S_press) {
				vertical_isMove = true;
			}
			setMovekey();
		});
		auto seq = Sequence::create(delayTime, func, nullptr);
		this->runAction(seq);

		//判断范围内是否有怪物
		Rect playerRect = player->getBoundingBox();
		Rect attackRect = Rect(playerRect.getMinX() - 40, playerRect.getMinY(),
			playerRect.getMaxX() - playerRect.getMinX() + 80,
			playerRect.getMaxY() - playerRect.getMinY());

		Sprite* collision = HelloWorld::collider(attackRect);
		if (collision != NULL) {
			auto x = collision->getPosition().x;
			auto y = collision->getPosition().y;

			//怪物动作结束后掉血瓶
			auto delayTime = DelayTime::create(0.8f);
			auto func = CallFunc::create([this, x, y]()
			{
				int ran = random(1, 2);
				if (ran == 1) {
					auto blood = Sprite::create("blood.png");
					blood->setPosition(Vec2(x, y + 15));
					bloods.pushBack(blood);
					blood->runAction(MoveBy::create(0.1f, Vec2(0, -20)));
					bgLayer->addChild(blood);
				}
				else if (ran == 2) {
					auto magic = Sprite::create("magic.png");
					magic->setPosition(Vec2(x, y + 15));
					magics.pushBack(magic);
					magic->runAction(MoveBy::create(0.1f, Vec2(0, -20)));
					bgLayer->addChild(magic);
				}
			});
			auto seq = Sequence::create(delayTime, func, nullptr);
			this->runAction(seq);

			//移除怪物
			fac->removeMonster(collision);

			//增加击杀数并显示
			attacknum++;
			database->setIntegerForKey("killNum", attacknum);
			database->flush();
			int i = database->getIntegerForKey("killNum");
			log("%d", i);

			char str[10];
			sprintf(str, "%d", i);
			killnum->setString(str);
		}
	}

}

//通过update调度器移动player
void HelloWorld::movePlayer(char c) {
	if (!bool_num)
		return;
	if (c == 'A') {
		if (last_key == 'D')
			player->setFlipX(true);
		last_key = 'A';
		auto bg_x = bgLayer->getPosition().x + 10;
		auto bg_y = bgLayer->getPosition().y;
		if (bg_x <= 10 && player->getPosition().x <= visibleSize.width * 0.33){
			auto moveTo = MoveTo::create(0.2, Vec2(bg_x, bg_y));
			bgLayer->runAction(moveTo);
		}
		else {
			auto player_x = player->getPosition().x - 10;
			auto player_y = player->getPosition().y;
			if (player_x >= origin.x + 50) {
				auto moveTo = MoveTo::create(0.2, Vec2(player_x, player_y));
				player->runAction(moveTo);
			}
		}
	}
	else if (c == 'D') {
		if (last_key == 'A')
			player->setFlipX(false);
		last_key = 'D';
		auto bg_x = bgLayer->getPosition().x - 10;
		auto bg_y = bgLayer->getPosition().y;
		if (bg_x >= (origin.x - visibleSize.width * 1.85) && player->getPosition().x >= visibleSize.width * 0.33) {
			auto moveTo = MoveTo::create(0.2, Vec2(bg_x, bg_y));
			bgLayer->runAction(moveTo);
		}
		else {
			auto player_x = player->getPosition().x + 10;
			auto player_y = player->getPosition().y;
			if (player_x <= origin.x + visibleSize.width - 50) {
				auto moveTo = MoveTo::create(0.2, Vec2(player_x, player_y));
				player->runAction(moveTo);
			}
		}
	}
}

void HelloWorld::vertical_movePlayer(char c) {
	if (!bool_num)
		return;
	if (c == 'W') {
		auto player_x = player->getPosition().x;
		auto player_y = player->getPosition().y + 10;
		if (player_y <= origin.y + visibleSize.height * 0.35) {
			auto moveTo = MoveTo::create(0.2, Vec2(player_x, player_y));
			player->runAction(moveTo);
		}
	}
	else if (c == 'S') {
		auto player_x = player->getPosition().x;
		auto player_y = player->getPosition().y - 10;
		if (player_y >= origin.y + 50) {
			auto moveTo = MoveTo::create(0.2, Vec2(player_x, player_y));
			player->runAction(moveTo);
		}
	}
}

//移动动画，按一下只能执行一次
int if_move = 1;
void HelloWorld::move() {
	if (!bool_num)
		return;
	if (if_move) {
		auto run_animation = Animation::createWithSpriteFrames(run, 0.1f);
		auto run_animate = RepeatForever::create(Animate::create(run_animation));
		player->runAction(run_animate);
		if_move = 0;
	}
}

//停止动画，释放键盘调用
void HelloWorld::stop() {
	if (!bool_num)
		return;
	if_move = 1;
	auto x = player->getPosition().x;
	auto y = player->getPosition().y;
	this->removeChild(player);
	auto frame = SpriteFrame::create("hero_03_move_01.png", Rect(0, 0, 213, 170));
	player = Sprite::createWithSpriteFrame(frame);
	player->setPosition(Vec2(x, y));
	if (this->last_key == 'A')
		player->setFlipX(true);
	this->addChild(player, 2);
}

//计时器
void HelloWorld::updateTime(float data) {
	if (!bool_num)
		return;
	if (bool_num) {
		dtime++;
		if (dtime < 0)
		{
			dtime = 0;
		}
		char str[10];
		sprintf(str, "%d", dtime);
		time->setString(str);
	}
}

//通过调度器使怪物向player移动
void HelloWorld::Movetoplayer(float data) {
	if (bool_num) {
		//auto m = fac->createMonster();
		//auto moster_x = random(origin.x, visibleSize.width);
		//auto moster_y = 100;
		//bgLayer->addChild(m);
		//Vec2 Point = bgLayer->convertToWorldSpaceAR(Vec2(moster_x, moster_y));
		//m->setPosition(Point);
		for (auto i : fac->getMonster()) {
			if (this->bgLayer->convertToWorldSpace(i->getPosition()).x < player->getPositionX())
				i->setFlipX(true);
			else
				i->setFlipX(false);
		}
		Vec2 ConvertPoint = player->convertToNodeSpaceAR(bgLayer->getPosition());
		fac->moveMonster(-ConvertPoint, 1);
	}
}

//player受伤
void HelloWorld::Playerhurt() {
	auto tag = pT->getPercentage();
	tag -= 20;
	auto progressTo = ProgressTo::create(2, tag);
	pT->runAction(progressTo);
	//if (tag == 100) {
	//	auto progressTo = ProgressTo::create(2, 80);
	//	pT->runAction(progressTo);
	//}
	//else if (tag == 80) {
	//	auto progressTo = ProgressTo::create(2, 60);
	//	pT->runAction(progressTo);
	//}
	//else if (tag == 60) {
	//	auto progressTo = ProgressTo::create(2, 40);
	//	pT->runAction(progressTo);
	//}
	//else if (tag == 40) {
	//	auto progressTo = ProgressTo::create(2, 20);
	//	pT->runAction(progressTo);
	//}
	//else if (tag == 20) {
	//	auto progressTo = ProgressTo::create(2, 0);
	//	pT->runAction(progressTo);
	//}
}

//player恢复
void HelloWorld::Playerrecover() {
	auto tag = pT->getPercentage();
	tag += 20;
	if (tag > 100)
		tag = 100;
	auto progressTo = ProgressTo::create(2, tag);
	pT->runAction(progressTo);
	//if (tag == 0) {
	//	auto progressTo = ProgressTo::create(2, 20);
	//	pT->runAction(progressTo);
	//}
	//else if (tag == 80) {
	//	auto progressTo = ProgressTo::create(2, 100);
	//	pT->runAction(progressTo);
	//}
	//else if (tag == 60) {
	//	auto progressTo = ProgressTo::create(2, 80);
	//	pT->runAction(progressTo);
	//}
	//else if (tag == 40) {
	//	auto progressTo = ProgressTo::create(2, 60);
	//	pT->runAction(progressTo);
	//}
	//else if (tag == 20) {
	//	auto progressTo = ProgressTo::create(2, 40);
	//	pT->runAction(progressTo);
	//}
}

//player受到到攻击
void HelloWorld::hitByMonster(float data) {
	if (bool_num) {
		Rect playerRect = player->getBoundingBox();
		Rect attackRect = Rect(playerRect.getMinX() + 50, playerRect.getMinY(),
			playerRect.getMaxX() - playerRect.getMinX() -20,
			playerRect.getMaxY() - playerRect.getMinY()-100);

		Sprite* collision = HelloWorld::collider(attackRect);
		if (collision != NULL) {
			bool_num = 0;
			isMove = false;
			vertical_isMove = false;
			fac->removeMonster(collision);
			Playerhurt();
			//受伤动画
			auto hurt_animation = Animation::createWithSpriteFrames(hurt, 0.5f);
			auto hurt_animate = Animate::create(hurt_animation);
			player->runAction(hurt_animate);
			isHurt = true;

			auto delayTime = DelayTime::create(1.0f);
			auto func = CallFunc::create([this]()
			{
				isHurt = false;
				bool_num = 1;
			});
			auto seq = Sequence::create(delayTime, func, nullptr);
			this->runAction(seq);
		}
		ifdead();
	}
}

void HelloWorld::getDrug(float data) {
	if (bool_num) {
		Rect playerRect = player->getBoundingBox();
		for (auto i : bloods) {
			Vec2 ConvertPoint = bgLayer->convertToWorldSpaceAR(i->getPosition());
			if (playerRect.containsPoint(ConvertPoint)) {
				this->bgLayer->removeChild(i);
				bloods.eraseObject(i);
				Playerrecover();
				break;
			}
		}
		for (auto i : magics) {
			Vec2 ConvertPoint = bgLayer->convertToWorldSpaceAR(i->getPosition());
			if (playerRect.containsPoint(ConvertPoint)) {
				this->bgLayer->removeChild(i);
				magics.eraseObject(i);
				magic_num++;
				char str[10];
				sprintf(str, "%d", magic_num);
				magic_label->setString(str);
				break;
			}
		}
	}
}

void HelloWorld::getBomb(float data) {
	if (bool_num) {
		Rect playerRect = player->getBoundingBox();
		for (auto i : bombs) {
			Vec2 ConvertPoint = bgLayer->convertToWorldSpaceAR(i->getPosition());
			if (playerRect.containsPoint(ConvertPoint)) {
				auto mine_animation = Animation::createWithSpriteFrames(mine, 0.1f);
				auto mine_animate = Animate::create(mine_animation);
				i->runAction(mine_animate);
				auto delayTime = DelayTime::create(0.4f);
				auto func = CallFunc::create([this, i]()
				{
					bombs.eraseObject(i);
					this->bgLayer->removeChild(i);
					Playerhurt();
					//受伤动画
					auto hurt_animation = Animation::createWithSpriteFrames(hurt, 0.5f);
					auto hurt_animate = Animate::create(hurt_animation);
					player->runAction(hurt_animate);
					isHurt = true;

					auto delayTime = DelayTime::create(1.0f);
					auto func = CallFunc::create([this]()
					{
						isHurt = false;
						bool_num = 1;
					});
					auto seq = Sequence::create(delayTime, func, nullptr);
					this->runAction(seq);
				});
				auto seq = Sequence::create(delayTime, func, nullptr);
				this->runAction(seq);
				break;
			}
		}
	}
}

//死亡
void HelloWorld::ifdead() {
	auto tag = pT->getPercentage();
	if (tag == 0) {
		bool_num = 0;

		auto dead_animation = Animation::createWithSpriteFrames(dead, 0.1f);
		auto dead_animate = Animate::create(dead_animation);
		player->runAction(dead_animate);

		auto Gameover = Label::createWithTTF("Game Over", "fonts/arial.ttf", 80);
		Gameover->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
		this->addChild(Gameover);
	}
}

//判断周围有没有怪物
Sprite* HelloWorld::collider(Rect rect) {
	for (auto i : fac->getMonster()) {
		Vec2 ConvertPoint = bgLayer->convertToWorldSpaceAR(i->getPosition());
		if (rect.containsPoint(ConvertPoint)) {
			return i;
		}
	}
	return NULL;
}

void HelloWorld::throwBomb(Sprite* m, float time) {
	auto bomb_sprite = Sprite::create("bomb.png");
	bomb_sprite->setPosition(m->getPosition());
	auto bomb_body = PhysicsBody::createCircle(bomb_sprite->getContentSize().width / 2, PhysicsMaterial(1, 1, 0));
	bomb_body->setGravityEnable(true);
	bomb_body->setCollisionBitmask(0x00);
	bomb_sprite->setPhysicsBody(bomb_body);
	this->bgLayer->addChild(bomb_sprite, 0);
	bomb_sprite->getPhysicsBody()->setVelocity(Vec2(-200,200));
	bombs.pushBack(bomb_sprite);
	auto delayTime = DelayTime::create(time);
	auto func = CallFunc::create([this, bomb_sprite, bomb_body]()
	{
		if (bombs.getIndex(bomb_sprite) >= 0) {
			bomb_body->setGravityEnable(false);
			bomb_body->setVelocity(Vec2(0, 0));
		}
	});
	auto seq = Sequence::create(delayTime, func, nullptr);
	this->runAction(seq);
}

void HelloWorld::thunder_skill() {
	if (magic_num <= 0)
		return;
	magic_num--;
	char str[10];
	sprintf(str, "%d", magic_num);
	magic_label->setString(str);
	for (auto i : fac->getMonster()) {
		Vec2 ConvertPoint = i->getPosition();
		ConvertPoint.y += 180;
		auto thunder_sprite = Sprite::create("thunder_beam_01.png");
		thunder_sprite->setPosition(ConvertPoint);
		this->bgLayer->addChild(thunder_sprite, 0);
		auto thunder_animation = Animation::createWithSpriteFrames(thunder, 0.1f);
		auto thunder_animate = Animate::create(thunder_animation);
		thunder_sprite->runAction(thunder_animate);
		auto delayTime = DelayTime::create(0.5f);
		auto func = CallFunc::create([this, thunder_sprite,i]()
		{
			int x = i->getPosition().x;
			int y = i->getPosition().y;
			this->bgLayer->removeChild(thunder_sprite);
			//怪物动作结束后掉血瓶
			auto delayTime = DelayTime::create(0.8f);
			auto func = CallFunc::create([this, x, y]()
			{
				int ran = random(1, 2);
				if (ran == 1) {
					auto blood = Sprite::create("blood.png");
					blood->setPosition(Vec2(x, y + 15));
					bloods.pushBack(blood);
					blood->runAction(MoveBy::create(0.1f, Vec2(0, -20)));
					bgLayer->addChild(blood);
				}
				else if (ran == 2) {
					auto magic = Sprite::create("magic.png");
					magic->setPosition(Vec2(x, y + 15));
					magics.pushBack(magic);
					magic->runAction(MoveBy::create(0.1f, Vec2(0, -20)));
					bgLayer->addChild(magic);
				}
			});
			auto seq = Sequence::create(delayTime, func, nullptr);
			this->runAction(seq);

			//移除怪物
			fac->removeMonster(i);

			//增加击杀数并显示
			attacknum++;
			database->setIntegerForKey("killNum", attacknum);
			database->flush();
			int i = database->getIntegerForKey("killNum");
			log("%d", i);

			char str[10];
			sprintf(str, "%d", i);
			killnum->setString(str);
		});
		auto seq = Sequence::create(delayTime, func, nullptr);
		this->runAction(seq);
	}
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		A_press = true;
		movekey = 'A';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		D_press = true;
		movekey = 'D';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		W_press = true;
		vertical_movekey = 'W';
		vertical_isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		S_press = true;
		vertical_movekey = 'S';
		vertical_isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_Z:
	case EventKeyboard::KeyCode::KEY_CAPITAL_Z:
	case EventKeyboard::KeyCode::KEY_J:
	case EventKeyboard::KeyCode::KEY_CAPITAL_J:
		if (bool_num) 
			stop();
		isAttack = true;
		isMove = false;
		vertical_isMove = false;
		attackCallback(this);
		break;
	case EventKeyboard::KeyCode::KEY_X:
	case EventKeyboard::KeyCode::KEY_CAPITAL_X:
	case EventKeyboard::KeyCode::KEY_K:
	case EventKeyboard::KeyCode::KEY_CAPITAL_K:
		thunder_skill();
		break;
	}
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
		A_press = false;
		if (!D_press)
			isMove = false;
		else setMovekey();
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
		D_press = false;
		if (!A_press)
			isMove = false;
		else setMovekey();
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
		W_press = false;
		if (!S_press)
			vertical_isMove = false;
		else setMovekey();
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
		S_press = false;
		if (!W_press)
			vertical_isMove = false;
		else setMovekey();
		break;
	}
}

void HelloWorld::setMovekey() {
	if (A_press) 
		movekey = 'A';
	else if (D_press) 
		movekey = 'D';

	if (S_press) 
		vertical_movekey = 'S';
	else if (W_press) 
		vertical_movekey = 'W';
}