#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GameOverScene.h"
#include "WinScene.h"

#include <vector>

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    Sprite* ball;
    Sprite* paddle;
    Sprite* edgeSp;
    
    PhysicsWorld* m_world;
    
    void setPhyWorld(PhysicsWorld* world)
    {
        m_world = world;
    };
    
    void update( float dt );
    
    void addListeners();
    
    bool onContactBegin(PhysicsContact& contact);
    
    int key;
    
    int score;
    
    Label* label = Label::createWithTTF("Score", "visitor1.ttf", 20);
    
private:
    void GoToGameOverScene( cocos2d::Ref *sender );
    void GoToWinScene( cocos2d::Ref *sender );
};

#endif // __GAME_SCENE_H__
