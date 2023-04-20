#ifndef TROLLEY_H
#define TROLLEY_H

extern struct CAccount* pLoggedInUser;//指向当前已登录用户的结构体的指针

extern struct CAccount* headAccount;
extern struct COrder* headOrder;
extern struct CGoods* headGoods;
extern struct CTrolley* headTrolley;
extern struct CFullDiscount* headFullDiscount;
extern struct CFestivalDiscount* headFestivalDiscount;

//链表尾指针
extern struct CAccount* tailAccount;
extern struct COrder* tailOrder;
extern struct CGoods* tailGoods;
extern struct CTrolley* tailTrolley;
extern struct CFullDiscount* tailFullDiscount;
extern struct CFestivalDiscount* tailFestivalDiscount;

extern void AddGiftOrder(float totalCost);

extern void OrderAdd(double totalCost, int goodsNum, int nums);


//修改下单导致的一系列信息改变，包括用户的总销售额,用户会员等级，购物车商品，商品库存,商品销量,新增订单以及可能的赠品订单
void InformationChange(int numPurchaseGoods, int goodsSerialNumber);

//购物车界面
void GoodsTrolley();
//购物车需求
//--添加商品至购物车
void GoodsTrolleyAdd(struct CGoods* pNow);
//--删除购物车中的某件商品
void GoodsTrolleyDelete(int goodsSerialNumber);
void GoodsTrolleyDeleteNoRemind(int goodsSerialNumber);

//--修改购物车中商品数量
void GoodsTrolleyChange(int goodsSerialNumber);
//--清除购物车中所有商品
void GoodsTrolleyClear();
//下单
//--购买购物车中某件商品
void GoodsTrolleyBuy(int goodsSerialNumber);
void GoodsTrolleyBuyNoRemind(struct CTrolley* pTro);
//--购买购物车中所有商品
void TotalGoodsTrolleyBuy();
//支付金额与支付方式
//--计算购物车中某件商品花费总金额
void AllGoodsTotalCost(int goodsSerialNumber, double* totalCost);
double TotalCostMoney(int goodsSerialNumber, struct CTrolley* pTro);
//--选择支付方式
int ModeOfPayment();

#endif 
