#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
	void movePlayer(char c);
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
	void Playerrecover();    
	void move();
	void stop();
	void ifdead();
	void update(float f);
	Sprite* collider(Rect rect);
	void setMovekey();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> hurt;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	cocos2d::Sprite* bg;
	Vector<Sprite*> bloods;
	Vector<Sprite*> magics;
	char movekey;
	bool isMove;
	int dtime;
	bool A_press;
	bool S_press;
	bool D_press;
	bool W_press;
	bool isAttack;//ÊÇ·ñ¹¥»÷
	bool isHurt; //ÊÇ·ñ±»¹¥»÷
	cocos2d::ProgressTimer* pT;
	cocos2d::Label* killnum;
	cocos2d::Layer* bgLayer;
	void addKeyboardListener();
};
