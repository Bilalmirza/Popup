//
//  Popup.cpp
//  dreamWeek
//
//  Created by Bilal Mirza on 07/08/2017.
//
//

#include "Popup.h"

USING_NS_CC;

using namespace ui;

namespace  {
    cocos2d::Size CONFIRM_DIALOGUE_SIZE_OFFSET = Size(100,150);
    const float ANIMATION_TIME = 0.15f;
    const float FADE_RATIO = 150;
}

namespace FONT {
    const float LABEL_OFFSET = 50;
    const float DESCRIPTION_TEXT_SIZE = 45;
    const float TITLE_TEXT_SIZE = 70;
    const char *GAME_FONT = "fonts/Dimbo Regular.ttf";
    const float LABEL_STROKE = 4;
}

namespace IMAGEPATH {
    const char *OK_BUTTON = "res/Popup/Ok.png";
    const char *OK_BUTTON_PRESSED = "res/Popup/OkPressed.png";
    const char *CANCEL_BUTTON = "res/Popup/Cancel.png";
    const char *CANCEL_BUTTON_PRESSED = "res/Popup/CancelPressed.png";
    const char *CLOSE_BUTTON = "res/Popup/close.png";
    const char *BACKGROUND_IMAGE = "res/Popup/popUpBase.png";
}
namespace UICustom {
    
    
    
    PopupDelegates *PopupDelegates::create()
    {
        PopupDelegates *node = new (std::nothrow)PopupDelegates();
        if(node && node->init())
        {
            node->autorelease();
            return node;
        }
        CC_SAFE_DELETE(node);
        return nullptr;
    }
    
    bool PopupDelegates::init()
    {
        Size winSize = Director::getInstance()->getWinSize();
        
        if(!LayerRadialGradient::initWithColor(Color4B(0,0,0,0), Color4B(0,0,0,FADE_RATIO), winSize.width/1.7f, winSize/2, 0.075f))
        {
            return false;
        }
        this->setOpacity(0);
        show(true);
        this->setUpTouches();
        
        
        return true;
    }
    
    void PopupDelegates::show(const bool animated)
    {
        if(animated){
            this->runAction(FadeTo::create(ANIMATION_TIME,FADE_RATIO));
        }
        else{
            this->setOpacity(FADE_RATIO);
        }
    }
    void PopupDelegates::dismiss(const bool animated)
    {
        if(animated){
            this->runAction(Sequence::create(FadeTo::create(ANIMATION_TIME,0),RemoveSelf::create(), NULL));
        }
        else{
            this->removeFromParentAndCleanup(true);
        }
    }
    
    void PopupDelegates::setUpTouches()
    {
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch* touch, Event* event){
            if(_bg){
                if(!_bg->getBoundingBox().containsPoint(this->convertToNodeSpace( touch->getLocation() )))
                {
                    this->dismiss(true);
                }
            }
            else{
                this->dismiss(true);
            }
            return true;
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }

    
    
    
    
    
    
    Popup *Popup::createAsMessage(const std::string &title, const std::string &msg)
    {
        return createAsConfirmDialogue(title, msg, NULL);
    }
    
    Popup *Popup::createAsConfirmDialogue(const std::string &title, const std::string &msg, const std::function<void ()> &YesFunc )
    {
        return create(title, msg, NULL, YesFunc);
    }
    
    Popup *Popup::create(const std::string &title, const std::string &msg, cocos2d::Label *lbl, const std::function<void ()> &YesFunc)
    {
        Popup *node = new (std::nothrow)Popup();
        Size winSize = Director::getInstance()->getWinSize();
        if(node && node->init())
        {
            
            if(!lbl){
                lbl = Label::createWithTTF(msg, FONT::GAME_FONT,FONT::DESCRIPTION_TEXT_SIZE);
            }
            lbl->setPosition(winSize.width/2,winSize.height/2 - FONT::LABEL_OFFSET/2);
            lbl->enableOutline(Color4B::BLACK,FONT::LABEL_STROKE);
            lbl->setAlignment(cocos2d::TextHAlignment::CENTER, cocos2d::TextVAlignment::CENTER);
            lbl->enableShadow(Color4B::BLACK, Size(0, -2));

            
            if(YesFunc){
                MenuItemImage *yesButton = MenuItemImage::create(IMAGEPATH::OK_BUTTON,IMAGEPATH::OK_BUTTON_PRESSED,[=](Ref *sender){
                    
                    YesFunc();
                    node->dismiss(true);
                });
                
                MenuItemImage *noButton = MenuItemImage::create(IMAGEPATH::CANCEL_BUTTON, IMAGEPATH::CANCEL_BUTTON_PRESSED, [node](Ref *sender){
                    node->dismiss(true);
                });
                
                
                Menu *menu = Menu::create(yesButton,noButton,NULL);
                node->addChild(menu,2);
                menu->setPosition(winSize.width/2, winSize.height/2 - lbl->getContentSize().height/2 - 75);
                menu->alignItemsHorizontallyWithPadding(FONT::LABEL_OFFSET/2);
                
                lbl->setPosition(winSize/2);
                CONFIRM_DIALOGUE_SIZE_OFFSET = Size(CONFIRM_DIALOGUE_SIZE_OFFSET.width,300);
            }
            node->addChild(lbl,3);
            node->initBg(lbl->getContentSize() + CONFIRM_DIALOGUE_SIZE_OFFSET, title);
            node->autorelease();
            return node;
        }
        
        CC_SAFE_DELETE(node);
        return nullptr;
    }
    

    
    void Popup::initBg(Size size,const std::string &title)
    {
        
        Size winSize = Director::getInstance()->getWinSize();

        _bg = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE);
        this->addChild(_bg);
        
        _bg->setPosition(Point(winSize.width/2,winSize.height/2));
        _bg->setScale9Enabled(true);
        _bg->setContentSize(size);
        
        ui::ImageView *fill = ui::ImageView::create(IMAGEPATH::BACKGROUND_IMAGE);
        _bg->addChild(fill);
        fill->setColor(Color3B(210,210,210));
        fill->setScale9Enabled(true);
        fill->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        fill->setPosition(Point(FONT::LABEL_OFFSET/4,FONT::LABEL_OFFSET/4));
        fill->setContentSize(Size(size.width - FONT::LABEL_OFFSET/2, size.height - FONT::LABEL_OFFSET* 2));
        
        
        Label *heading = Label::createWithTTF(title, FONT::GAME_FONT, FONT::TITLE_TEXT_SIZE);
        heading->setPosition(_bg->getContentSize().width/2 , _bg->getContentSize().height - FONT::LABEL_OFFSET);
        _bg->addChild(heading);
        heading->enableOutline(Color4B::BLACK,FONT::LABEL_STROKE);
        heading->enableShadow(Color4B::BLACK, Size(0, -3));
    }
    
    
}

