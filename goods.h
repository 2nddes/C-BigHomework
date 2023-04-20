#ifndef GOODS_H
#define GOODS_H

extern struct CAccount* pLoggedInUser;//指向当前已登录用户的结构体的指针

extern struct CGoods* headGoods;

static void releaseGoods(struct CGoods* head);
static struct CGoods* GoodsSearch(struct CGoods* head, const char* keyword);//商品模糊搜索
static const void GoodsSinglePrint(struct CGoods* pNow);//输出单个商品信息
const void GoodsPrint(struct CGoods* head);//输出商品列表
static void GoodsChange(struct CGoods* pNow, int slectNum);//修改商品信息
static void GoodsAdd();//添加商品
static struct CGoods* GoodsFilter(struct CGoods* head);//商品过滤
static struct CGoods* decendSortGoodsBySales(struct CGoods* head);//按照商品销量递减排序
static struct CGoods* ascendSortGoodsByPrice(struct CGoods* head);//按照商品价格递增排序
static void GoodsDelete(struct CGoods* pNow);//删除商品
static void GoodsList();//输出链表中的商品列表

extern void GoodsTrolleyAdd(struct CGoods* pNow);//添加商品到购物车
extern void GoodsTrolley();//购物车

#endif 