#include"Monster.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
auto visibleSize = Director::getInstance()->getVisibleSize();
auto origin = Director::getInstance()->getVisibleOrigin();

Factory::Factory() {
	initSpriteFrame();
}
Factory* Factory::getInstance() {
	if (factory == NULL) {
		factory = new Factory();
	}
	return factory;
}
void Factory::initSpriteFrame() {
	//怪物死
	monsterDead.reserve(4);
	for (int i = 1; i <= 4; i++) {
		char dead_pic[25];
		sprintf(dead_pic, "explo_02_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(dead_pic, Rect(0, 0, 140, 140));
		monsterDead.pushBack(frame);
	}

	//怪物移动
	monsterMove.reserve(2);
	for (int i = 1; i <= 2; i++) {
		char move_pic[25];
		sprintf(move_pic, "enemy_04_move_0%d.png", i);
		SpriteFrame* frame = SpriteFrame::create(move_pic, Rect(0, 0, 100, 100));
		monsterMove.pushBack(frame);
	}
}

Sprite* Factory::createMonster() {
	Sprite* mons = Sprite::create("enemy_04_move_01.png");
	monster.pushBack(mons);
	return mons;
}

//移除怪物
void Factory::removeMonster(Sprite* sp) {
	Animation* anim = Animation::createWithSpriteFrames(monsterDead, 0.2);
	Animate* ani = Animate::create(anim);
	Sequence* seq = Sequence::create(ani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), NULL);
	sp->runAction(seq);
	monster.eraseObject(sp);
}

void Factory::moveMonster(Vec2 playerPos, float time) {
	for (auto i : monster) {
		Animation* anim = Animation::createWithSpriteFrames(monsterMove, 0.5);
		Animate* ani = Animate::create(anim);
		i->runAction(ani);

		Vec2 Pos = i->getPosition();
		Vec2 dir = playerPos - Pos;
		dir.normalize();
		i->runAction(MoveBy::create(time, dir * 50));
	}
}

Vector<Sprite*> Factory::getMonster() {
	return monster;
}






