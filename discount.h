#ifndef DISCOUNT_H
#define DISCOUNT_H

extern struct CAccount* pLoggedInUser;//指向当前已登录用户的结构体的指针

extern struct CAccount* headAccount;
extern struct COrder* headOrder;
extern struct CGoods* headGoods;
extern struct CTrolley* headTrolley;
extern struct CFullDiscount* headFullDiscount;
extern struct CFestivalDiscount* headFestivalDiscount;

//检查输入的时间是否非法
int CheckTime(int* time);
//获取折扣价格
//1. 找到当前消费额能够享受的最高满减折扣
double GetFullDiscount(int previousTotalPrice);
//2. 获取经过满减折扣和节假日折扣之后的商品价格
double GetPriceAfterActivaiyDiscountAndFullDiscount(double totalPrice);

//调整折扣
void ResetFestivalDiscount();//节假日过后，折扣重置
void SetFestivalDiscount();//设置节假日折扣
void SetFullDiscount();//设置满减折扣
void SetDiscount();//设置折扣主界面

#endif 
