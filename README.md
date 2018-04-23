# Popup
A cocos2d-x class that allows you to create Popups for Messages and confimation.
This class swallows all touch events until the popup is closed.
Popup can be closed by tapping outside the bg or pressing the cancel button.
Pressing ok button will fire an event.

![Alt Text](https://media.giphy.com/media/9S5evRB6NYuvQg6GXO/giphy.gif)

Usage:

```
//Confimation:

UICustom::Popup *popup = UICustom::Popup::createAsConfirmDialogue("Test 1", "This is a confirmation Popup", [=](){
 log("Ok is pressed");
});
this->addChild(popup);



//Message:

UICustom::Popup *popup = UICustom::Popup::createAsMessage("Test 2", "This is a Message Popup");
this->addChild(popup);





//Creating with Label 

Label *lbl = Label::createWithTTF("This Popup is created with a label and its properties the width of the label is 300px.","fonts/Dimbo Regular.ttf" , 40);
lbl->setWidth(300);
UICustom::Popup *popup = UICustom::Popup::create("Test 3", "", lbl, [= ](){

});
this->addChild(popup);

```


To create your own popup inherit from class PopupDelegate

Resources: Funtique by Vasili Tkach
