#ifndef GOODS_H
#define GOODS_H

extern struct CAccount* pLoggedInUser;//ָ��ǰ�ѵ�¼�û��Ľṹ���ָ��

extern struct CGoods* headGoods;

static void releaseGoods(struct CGoods* head);
static struct CGoods* GoodsSearch(struct CGoods* head, const char* keyword);//��Ʒģ������
static const void GoodsSinglePrint(struct CGoods* pNow);//���������Ʒ��Ϣ
const void GoodsPrint(struct CGoods* head);//�����Ʒ�б�
static void GoodsChange(struct CGoods* pNow, int slectNum);//�޸���Ʒ��Ϣ
static void GoodsAdd();//�����Ʒ
static struct CGoods* GoodsFilter(struct CGoods* head);//��Ʒ����
static struct CGoods* decendSortGoodsBySales(struct CGoods* head);//������Ʒ�����ݼ�����
static struct CGoods* ascendSortGoodsByPrice(struct CGoods* head);//������Ʒ�۸��������
static void GoodsDelete(struct CGoods* pNow);//ɾ����Ʒ
static void GoodsList();//��������е���Ʒ�б�

extern void GoodsTrolleyAdd(struct CGoods* pNow);//�����Ʒ�����ﳵ
extern void GoodsTrolley();//���ﳵ

#endif 