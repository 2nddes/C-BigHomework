#ifndef MAINPAGE_H
#define MAINPAGE_H
extern struct CAccount* pLoggedInUser;//指向当前已登录用户的结构体的指针

extern struct CAccount* headAccount;
extern struct COrder* headOrder;
extern struct CGoods* headGoods;
extern struct CTrolley* headTrolley;
extern struct CFullDiscount* headFullDiscount;
extern struct CFestivalDiscount* headFestivalDiscount;

extern struct CAccount* tailAccount;

//登录前主界面
int MainPage();
//登录后主界面
int PrePage();

#endif 