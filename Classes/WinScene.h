#ifndef __WIN_SCENE_H__
#define __WIN_SCENE_H__

#include "cocos2d.h"

class WinScene: public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(WinScene);
    
private:
    void GoToGameScene( cocos2d::Ref *sender );
};

#endif // __GAMEOVER_SCENE_H__
