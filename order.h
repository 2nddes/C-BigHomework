#ifndef ORDER_H
#define ORDER_H

extern struct CAccount* pLoggedInUser;//指向当前已登录用户的结构体的指针

extern struct CAccount* headAccount;//账户头指针
extern struct COrder* headOrder;//订单头指针
extern struct CGoods* headGoods;//商品头指针
extern struct CTrolley* headTrolley;//购物车头指针
extern struct CFullDiscount* headFullDiscount;//折扣头指针
extern struct CFestivalDiscount* headFestivalDiscount;//节日折扣头指针

//链表尾指针
extern struct CAccount* tailAccount;//账户尾指针
extern struct COrder* tailOrder;//订单尾指针
extern struct CGoods* tailGoods;//商品尾指针
extern struct CTrolley* tailTrolley;//购物车尾指针
extern struct CFullDiscount* tailFullDiscount;//折扣尾指针
extern struct CFestivalDiscount* tailFestivalDiscount;//节假日折扣尾指针

//输出商品信息
extern void GoodsPrint(struct CGoods* head);

void OrderStatusChangeAutomatic();
//增加赠品订单
void AddGiftOrder(float totalCost);
//订单主菜单
void OrderFindAndChange();
//订单查询
void OrderSeek();
//订单修改
void OrderChange();
//订单增加
void OrderAdd(double totalCost, int goodsNum, int nums);
//订单删除
void OrderDelete();
//退换货
void GoodsChangingAndRefunding();
//由于退货造成的信息改变
void InformationChangeForRefunding(struct COrder* p);
///由于换货造成的信息改变
void InformationChangeForChanging(struct COrder*p);
#endif 