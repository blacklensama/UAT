#include "HelloWorldScene.h"
#include "BlackJack.h"
#include "list.h"
USING_NS_CC;

void HelloWorld::keyBackClicked()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

void HelloWorld::updateUI(float dt)
{
	std::string str = singletion::Instance()->get();

	if (str == "")
	{
		return;
	}
	std::cout << str << std::endl;
	
	CCNode* node = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1);

	if (node != NULL)
	{
		std::cout << node->getChildrenCount() << std::endl;
		CCLabelTTF* ttf = (CCLabelTTF*)node->getChildByTag(100);
		ttf->setString(str.c_str());
		Command c(str);
		c.print();
		singletion::Instance()->setResult("hello world");
	}

	
}

CCScene* HelloWorld::scene()
{

	pthread_create(&p1, NULL, test_thread, NULL);

    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

	scene->schedule(schedule_selector(HelloWorld::updateUI), 0.0);

	scene->scheduleUpdate();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer, 0, 1);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	
	
	//////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	this->setKeypadEnabled(true);

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
   
	

	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

	CCMenuItemImage *pStartItem = CCMenuItemImage::create(
										"start-1.png",
										"start-2.png",
										this,
										menu_selector(HelloWorld::menuStartGame));

	pStartItem->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - 50));
	
	CCMenuItemImage *pExitItem = CCMenuItemImage::create(
										"exit-1.png",
										"exit-2.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback));



	pExitItem->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - 100));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pStartItem , pExitItem,NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1, 1);

	

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1, 2);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("writebg.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0, 3);

	CCTextFieldTTF* text = CCTextFieldTTF::textFieldWithPlaceHolder(  
		"Input Your Name...", "Arial", 20);  

	text->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/3));

	this->addChild(text, 4, 4);

	text->setColor(ccc3(0, 255, 255));

	text->attachWithIME();

	CCLabelTTF* testTTF = CCLabelTTF::create("test", "Arial", 24);

	testTTF->setColor(ccc3(255, 0, 0));

	testTTF->setPosition(ccp(visibleSize.width/8, visibleSize.height/6));

	this->addChild(testTTF, 1, 100);

    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::menuStartGame(CCObject* pSender)
{
	CCScene* newScene = BlackJack::scene();

	CCTransitionFadeBL* b = CCTransitionFadeBL::create(2, newScene);

	CCDirector::sharedDirector()->replaceScene(b);
}