#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Monster.h"
#pragma execution_character_set("utf-8")
#define database UserDefault::getInstance()
#define fac Factory::getInstance()
USING_NS_CC;

boolean bool_num = 1;
int attacknum = 0;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
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

	//hp��
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//ʹ��hp������progressBar
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

	// ��������
	attack.reserve(4);
	for (int i = 1; i <= 4; i++) {
		char attack_pic[25];
		sprintf(attack_pic, "hero_03_attack_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(attack_pic, Rect(0, 0, 213, 170));
		attack.pushBack(frame);
		if (i == 4) attack.pushBack(frame0);
	}
	
	//��������
	dead.reserve(4);
	for (int i = 1; i < 4; i++) {
		char dead_pic[25];
		sprintf(dead_pic, "explo_01_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(dead_pic, Rect(0, 0, 120, 120));
		dead.pushBack(frame);
	}
	SpriteFrame* dead_frame = SpriteFrame::create("ghost_dog.png", Rect(0, 0, 70, 100));
	dead.pushBack(dead_frame);

	//�ƶ�����
	run.reserve(2);
	for (int i = 1; i <= 2; i++) {
		char run_pic[25];
		sprintf(run_pic, "hero_03_move_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(run_pic, Rect(0, 0, 213, 170));
		run.pushBack(frame);
	}

	//����������
	hurt.reserve(2);
	SpriteFrame* hurt_frame = SpriteFrame::create("hero_03_damage_01.png", Rect(0, 0, 213, 170));
	hurt.pushBack(hurt_frame);
	hurt.pushBack(frame0);

	//��ʱ��
	time = Label::createWithTTF("120", "fonts/arial.ttf", 36);
	time->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 70));
	this->addChild(time);
	schedule(schedule_selector(HelloWorld::updateTime), 1);
	dtime = 120;

	//��ɱ����
	attacknum = database->getIntegerForKey("killNum");
	char str[10];
	sprintf(str, "%d", attacknum);
	killnum = Label::createWithTTF(str, "fonts/arial.ttf", 36);
	killnum->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 30));
	this->addChild(killnum);

	//������
	schedule(schedule_selector(HelloWorld::Movetoplayer), 3);
	schedule(schedule_selector(HelloWorld::hitByMonster), 0.5);
	schedule(schedule_selector(HelloWorld::update), 0.05);

	addKeyboardListener();

	//����ͼƬ
	bg = Sprite::create("timg.jpg");
	bg->setPosition(Vec2(origin.x + visibleSize.width * 1.4, visibleSize.height / 2 + origin.y));

	bgLayer = Layer::create();
	bgLayer->setPosition(Vec2(0, 0));
	bgLayer->setAnchorPoint(Vec2(0, 0));
	bgLayer->ignoreAnchorPointForPosition(false);
	bgLayer->addChild(bg);
	this->addChild(bgLayer, -1);

	this->last_key = 'D';

	return true;
}


void HelloWorld::update(float f) {
	if (isMove && !isAttack && !isHurt) {
		this->movePlayer(movekey);
		move();
	}
	else if (!isMove && !isAttack && !isHurt) stop();
}

//���̼���
void HelloWorld::addKeyboardListener() {
	auto keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyboard->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);
}

//�����¼�
void HelloWorld::attackCallback(cocos2d::Ref* pSender) {
	if (bool_num) {
		bool_num = 0;
		//��������
		auto attack_animation = Animation::createWithSpriteFrames(attack, 0.1f);
		auto attack_animate = Animate::create(attack_animation);
		player->runAction(attack_animate);

		auto delayTime = DelayTime::create(0.4f);
		auto func = CallFunc::create([this]()
		{
			isAttack = 0;
			bool_num = 1;
			//�ж��Ƿ��а�����
			if (A_press || D_press || W_press || S_press) {
				isMove = true;
				setMovekey();
			}
		});
		auto seq = Sequence::create(delayTime, func, nullptr);
		this->runAction(seq);

		//�жϷ�Χ���Ƿ��й���
		Rect playerRect = player->getBoundingBox();
		Rect attackRect = Rect(playerRect.getMinX() - 40, playerRect.getMinY(),
			playerRect.getMaxX() - playerRect.getMinX() + 80,
			playerRect.getMaxY() - playerRect.getMinY());

		Sprite* collision = HelloWorld::collider(attackRect);
		if (collision != NULL) {
			auto x = collision->getPosition().x;
			auto y = collision->getPosition().y;

			//���ﶯ���������Ѫƿ
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

			//�Ƴ�����
			fac->removeMonster(collision);

			//���ӻ�ɱ������ʾ
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

//ͨ��update�������ƶ�player
void HelloWorld::movePlayer(char c) {
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
	else if (c == 'W') {
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

//�ƶ���������һ��ֻ��ִ��һ��
int if_move = 1;
void HelloWorld::move() {
	if (if_move) {
		auto run_animation = Animation::createWithSpriteFrames(run, 0.1f);
		auto run_animate = RepeatForever::create(Animate::create(run_animation));
		player->runAction(run_animate);
		if_move = 0;
	}
}

//ֹͣ�������ͷż��̵���
void HelloWorld::stop() {
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

//��ʱ��
void HelloWorld::updateTime(float data) {
	if (bool_num) {
		dtime--;
		if (dtime < 0)
		{
			dtime = 0;
		}
		char str[10];
		sprintf(str, "%d", dtime);
		time->setString(str);
	}
}

//ͨ��������ʹ������player�ƶ�
void HelloWorld::Movetoplayer(float data) {
	if (bool_num) {
		auto m = fac->createMonster();
		auto moster_x = random(origin.x, visibleSize.width);
		auto moster_y = 100;
		bgLayer->addChild(m);
		Vec2 Point = bgLayer->convertToWorldSpaceAR(Vec2(moster_x, moster_y));
		m->setPosition(Point);

		Vec2 ConvertPoint = player->convertToNodeSpaceAR(bgLayer->getPosition());
		fac->moveMonster(-ConvertPoint, 1);
	}
}

//player����
void HelloWorld::Playerhurt() {
	auto tag = pT->getPercentage();
	if (tag == 100) {
		auto progressTo = ProgressTo::create(2, 80);
		pT->runAction(progressTo);
	}
	else if (tag == 80) {
		auto progressTo = ProgressTo::create(2, 60);
		pT->runAction(progressTo);
	}
	else if (tag == 60) {
		auto progressTo = ProgressTo::create(2, 40);
		pT->runAction(progressTo);
	}
	else if (tag == 40) {
		auto progressTo = ProgressTo::create(2, 20);
		pT->runAction(progressTo);
	}
	else if (tag == 20) {
		auto progressTo = ProgressTo::create(2, 0);
		pT->runAction(progressTo);
	}
}

//player�ָ�
void HelloWorld::Playerrecover() {
	auto tag = pT->getPercentage();
	if (tag == 0) {
		auto progressTo = ProgressTo::create(2, 20);
		pT->runAction(progressTo);
	}
	else if (tag == 80) {
		auto progressTo = ProgressTo::create(2, 100);
		pT->runAction(progressTo);
	}
	else if (tag == 60) {
		auto progressTo = ProgressTo::create(2, 80);
		pT->runAction(progressTo);
	}
	else if (tag == 40) {
		auto progressTo = ProgressTo::create(2, 60);
		pT->runAction(progressTo);
	}
	else if (tag == 20) {
		auto progressTo = ProgressTo::create(2, 40);
		pT->runAction(progressTo);
	}
}

//player�ܵ�������
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
			fac->removeMonster(collision);
			Playerhurt();
			//���˶���
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

//����
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

//�ж���Χ��û�й���
Sprite* HelloWorld::collider(Rect rect) {
	for (auto i : fac->getMonster()) {
		Vec2 ConvertPoint = bgLayer->convertToWorldSpaceAR(i->getPosition());
		if (rect.containsPoint(ConvertPoint)) {
			return i;
		}
	}
	return NULL;
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
		movekey = 'W';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
	case EventKeyboard::KeyCode::KEY_S:
		S_press = true;
		movekey = 'S';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_J:
	case EventKeyboard::KeyCode::KEY_CAPITAL_J:
		if (bool_num) 
			stop();
		isAttack = true;
		isMove = false;
		attackCallback(this);
		break;
	}
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
		A_press = false;
		if (!(D_press || S_press || W_press))
			isMove = false;
		else setMovekey();
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
		D_press = false;
		if (!(A_press || S_press || W_press))
			isMove = false;
		else setMovekey();
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
		W_press = false;
		if (!(D_press || S_press || A_press))
			isMove = false;
		else setMovekey();
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_CAPITAL_S:
		S_press = false;
		if (!(D_press || A_press || W_press))
			isMove = false;
		else setMovekey();
	}
}

void HelloWorld::setMovekey() {
	if (A_press) movekey = 'A';
	else if (D_press) movekey = 'D';
	else if (S_press) movekey = 'S';
	else if (W_press) movekey = 'W';
}