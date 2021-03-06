#pragma once
#include "cocos2d.h"
#include "Monster.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
	void movePlayer(char c);
	void vertical_movePlayer(char c);
    virtual bool init();
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);	
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	void deadCallback(cocos2d::Ref* pSender);
	void attackCallback(cocos2d::Ref* pSender);
	//void UpCallback(cocos2d::Ref* pSender);
	void LeftCallback(cocos2d::Ref* pSender);
	//void DownCallback(cocos2d::Ref* pSender);
	void RightCallback(cocos2d::Ref* pSender);
	void updateTime(float data);
	void Movetoplayer(float data);
	void Playerhurt();
	void hitByMonster(float data);
	void getDrug(float data);
	void getBomb(float data);
	void Playerrecover();
	void throwBomb(Sprite* monster, float time);
	void setPhysicsWorld(PhysicsWorld * world);
	void move();
	void stop();
	void ifdead();
	void quit(cocos2d::Ref* pSender);
	void save(cocos2d::Ref* pSender);
	void update(float f);
	Sprite* collider(Rect rect);
	void setMovekey();
	void thunder_skill();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> hurt;
	cocos2d::Vector<SpriteFrame*> thunder;
	cocos2d::Vector<SpriteFrame*> mine;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	cocos2d::Label* magic_label;
	cocos2d::Sprite* bg;
	Vector<Sprite*> bloods;
	Vector<Sprite*> magics;
	Vector<Sprite*> bombs;
	char movekey;
	char vertical_movekey;
	bool isMove;
	bool vertical_isMove;
	int dtime;
	int magic_num;
	bool A_press;
	bool S_press;
	bool D_press;
	bool W_press;
	bool isAttack;//是否攻击
	bool isHurt; //是否被攻击
	cocos2d::ProgressTimer* pT;
	cocos2d::Label* killnum;
	cocos2d::Layer* bgLayer;
	void addKeyboardListener();
	char last_key;  //上一次水平移动是左还是右，用于对玩家图片的翻转
	bool group[4];
	int monster_bomb[4];
	Factory* m_factory;
	PhysicsWorld* m_world;
};
