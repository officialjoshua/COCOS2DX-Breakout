#include "cocos2d.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "WinScene.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"
#include <vector>

USING_NS_CC;

int key;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    // auto scene = Scene::create();
    // change to:
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_NONE );
    
    // set gravity
    Vect gravity( 0.0f, 0.0f );
    scene->getPhysicsWorld()->setGravity( gravity );
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    layer->setPhyWorld( scene->getPhysicsWorld() );

    // add layer as a child to scene
    scene->addChild( layer );
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    /*
     *  Create Scoreboard
     */
    
    score = 0;
    
    std::string scoreStr = "Score: " + std::to_string(score);
    label->setString(scoreStr);
    
    label->setPosition( Vec2( 105 , 650 ) );
    
    this->addChild( label, 100 );
    
    auto scoreBoard = Sprite::create( "scoreboard.jpg" );
    scoreBoard->setPosition( Vec2( 105 , 650 ) );
    
    this->addChild( scoreBoard, 99 );
    
    /*
     *  Create Background
     */
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto backgroundSprite = Sprite::create( "img_mainmenu.jpg" );
    backgroundSprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
    
    this->addChild( backgroundSprite );
    
    /*
     *  Create Edge Sprite
     */
    
    edgeSp = Sprite::create();
    
    auto boundBody = PhysicsBody::createEdgeBox( visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3 ); // Create physics body
    
    boundBody->getShape( 0 )->setRestitution( 1.0f );
    boundBody->getShape( 0 )->setFriction( 0.0f );
    boundBody->getShape( 0 )->setDensity( 15.0f );
    
    /* Set the position and the centre of Box in the center of the screen & Set physics Body */
    edgeSp->setPosition( Point( visibleSize.width / 2, visibleSize.height / 2 ) );
    edgeSp->setPhysicsBody( boundBody ); //
    
    /* This is the important command, if not available, there is nothing happening when colliding */
    boundBody->setContactTestBitmask(0x000002);
    
    this->addChild( edgeSp );
    
    /* Tag == 0, to check object when colliding belongs to some kind */
    edgeSp->setTag( 0 );
    
    /*
     *  Create Ball Sprite
     */
    
    ball = Sprite::create( "Pusheen.png");
    ball->setPosition( 100, 100 );
    
    /* Set physics shape */
    auto ballBody = PhysicsBody::createCircle( ball->getContentSize().width / 2 );
    
    ballBody->getShape( 0 )->setRestitution( 1.0f );
    ballBody->getShape( 0 )->setFriction( 0.0f );
    ballBody->getShape( 0 )->setDensity( 0.8f );
    /* Not set acceleration */
    ballBody->setGravityEnable( false );
    
    Vect force = Vect( 1010000.0f, 1010000.0f );      /* Create a force Vector to act with the direction of 45 degree, because x = y */
    ballBody->applyImpulse( force );                  /* Push a force into the ball edge */
    
    /* Set physics body */
    ball->setPhysicsBody( ballBody );
    ballBody->setContactTestBitmask( 0x000001 );
    
    ball->setTag( 1 );
    this->addChild( ball );
    
    /*
     *  Create Paddle Sprite
     */
    
    paddle = Sprite::create( "Clouds.png" );
    auto paddleBody = PhysicsBody::createBox( paddle->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT );
    
    paddleBody->getShape( 0 )->setRestitution( 1.0f );
    paddleBody->getShape( 0 )->setFriction( 0.0f );
    paddleBody->getShape( 0 )->setDensity( 10.f );
    paddleBody->setGravityEnable( false );

    /* Set static when reacting, no restitution, no changing position */
    paddleBody->setDynamic( false );
    paddle->setPosition( visibleSize.width / 2, 50 );
    paddle->setPhysicsBody( paddleBody );
    
    /* With reaction */
    paddleBody->setContactTestBitmask( 0x000001 );
    ball->setTag( 2 );
    this->addChild( paddle );

    /*
     * Create walls
     */
    
    for (int i = 0; i < 7; i++) {
        static int padding = 20;
        
        auto block = Sprite::create("Poptart.png");
        auto blockBody = PhysicsBody::createBox(block->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
        
        blockBody->getShape(0)->setDensity(10.0f);
        blockBody->getShape(0)->setFriction(1.0f);
        blockBody->getShape(0)->setRestitution(1.f);
        blockBody->setDynamic(false);
        
        // Create the distance even among the bricks
        int xOffset = padding + block->getContentSize().width / 2 +
                ((block->getContentSize().width + padding)*i);
        
        block->setPosition(xOffset, 660);
        blockBody->setContactTestBitmask(0x000001);
        block->setPhysicsBody(blockBody);
        block->setTag(3);
        
        this->addChild(block);
    }
    
    for (int i = 0; i < 7; i++) {
        static int padding = 20;
        
        auto block = Sprite::create("Poptart.png");
        auto blockBody = PhysicsBody::createBox(block->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
        
        blockBody->getShape(0)->setDensity(10.0f);
        blockBody->getShape(0)->setFriction(0.0f);
        blockBody->getShape(0)->setRestitution(1.f);
        blockBody->setDynamic(false);
        
        // Create the distance even among the bricks
        int xOffset = padding + block->getContentSize().width / 2 +
        ((block->getContentSize().width + padding)*i);
        
        block->setPosition(xOffset, 570);
        blockBody->setContactTestBitmask(0x000001);
        block->setPhysicsBody(blockBody);
        block->setTag(3);
        
        this->addChild(block);
    }
    
    /*
     *  Add collision listeners
     */
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto contactListener = EventListenerPhysicsContact::create();
    
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    this->addListeners();
    
    this->scheduleUpdate();
    
    return true;
}

bool GameScene::onContactBegin(PhysicsContact& contact)
{
    // Get two collided object
    auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
    auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
    
    // Check kinds of objects
    int tagA = spriteA->getTag();
    int tagB = spriteB->getTag();
    
    if (tagA == 3)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "score.m4a" );
        this->removeChild(spriteA,true);
        score = score + 10;
        
        std::string scoreStr = "Score: " + std::to_string(score);
        label->setString(scoreStr);
    }
    else
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "bump.m4a" );
    
    if (tagB == 3)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "score.m4a" );
        this->removeChild(spriteB,true);
        score = score + 10;
        
        std::string scoreStr = "Score: " + std::to_string(score);
        label->setString(scoreStr);
    }
    else
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "bump.m4a" );
    
    if ( (tagA == 0 || tagB  == 0 ) & ( ball->getPositionY() <= paddle->getPositionY()))
    {
        CCLOG( "Lose" );
        GoToGameOverScene( this );
    }
    
    return true;
}

void GameScene::addListeners()
{
    // implement key listeners here like WASD key movements.
    auto eventListener = EventListenerKeyboard::create();
    
    eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event)
    {
        Vec2 loc = event->getCurrentTarget()->getPosition();
        
        switch(keyCode)
        {
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case EventKeyboard::KeyCode::KEY_A:
                key = 1;
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case EventKeyboard::KeyCode::KEY_D:
                key = 2;
                break;
            default:
                key = 0;
                break;
        }
    };
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority( eventListener, paddle );
}

void GameScene::update( float dt )
{
    if ( key != 0 )
    {
        Vec2 loc = paddle->getPosition();
        
        switch (key)
        {
            case 1:
                if ( loc.x > 0 )
                {
                    paddle->setPosition( loc.x-64, loc.y );
                }
                break;
                
            case 2:
                if ( loc.x < 1260 )
                {
                    paddle->setPosition( loc.x+64, loc.y );
                }
                break;
                
            default:
                break;
        }
        
        key = 0;
    }
    
    
    /*
     *  Check is game is done
     */
    
    bool isWin = true;
    
    Vector<PhysicsBody*> bodies = m_world->getAllBodies();
    
    for (PhysicsBody* body : bodies)
    {
        if (body->getNode()->getTag() == 3)
        {
            isWin = false;
        }
    }
    
    if (isWin == true)
    {
        CCLOG( "WIN" );
        GoToWinScene( this );
    }
}

void GameScene::GoToGameOverScene( cocos2d::Ref *sender )
{
    auto scene = GameOverScene::createScene();
    
    Director::getInstance( )->replaceScene( TransitionFade::create( 0, scene ) );
}

void GameScene::GoToWinScene( cocos2d::Ref *sender )
{
    auto scene = WinScene::createScene();
    
    Director::getInstance( )->replaceScene( TransitionFade::create( 0, scene ) );
}
