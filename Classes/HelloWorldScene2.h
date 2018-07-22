#pragma once
#include "cocos2d.h"
#include "Monster.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace std;
class HelloWorld2 : public cocos2d::Scene {
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
    void Playerrecover();
    void move();
    void stop();
    void ifdead();
    void farfire();
    void meet();
    void update(float f);
	void quit(cocos2d::Ref* pSender);
	void save(cocos2d::Ref* pSender);
    void shock_wave_skill(cocos2d::Ref* pSender);
    Sprite* collider(Rect rect);
    void setMovekey();
	void setPhysicsWorld(PhysicsWorld* world);
	void getDrug(float data);
	void getBomb(float data);
	void throwBomb(Sprite* m, float time);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld2);
private:
    cocos2d::Sprite* player;
    list<Sprite*> bullets;
    cocos2d::Sprite* shock_wave;
    cocos2d::Vector<SpriteFrame*> attack;
    cocos2d::Vector<SpriteFrame*> dead;
    cocos2d::Vector<SpriteFrame*> run;
    cocos2d::Vector<SpriteFrame*> hurt;
    cocos2d::Vector<SpriteFrame*> skill;
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
    bool A_press;
    bool S_press;
    bool D_press;
    bool W_press;
    bool isAttack; //是否攻击
    bool isHurt;   //是否被攻击
    cocos2d::ProgressTimer* pT;
    cocos2d::Label* killnum;
    cocos2d::Layer* bgLayer;
    void addKeyboardListener();
    char last_key;  //上一次水平移动是左还是右，用于对玩家图片的翻转
    bool group[4];
	int monster_bomb[4];
    Factory* m_factory;
    boolean bool_num;
    int attacknum;
    int if_move;
	int magic_num;
	PhysicsWorld* m_world;
	cocos2d::Vector<SpriteFrame*> mine;
};
