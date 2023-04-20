#ifndef TROLLEY_H
#define TROLLEY_H

extern struct CAccount* pLoggedInUser;//ָ��ǰ�ѵ�¼�û��Ľṹ���ָ��

extern struct CAccount* headAccount;
extern struct COrder* headOrder;
extern struct CGoods* headGoods;
extern struct CTrolley* headTrolley;
extern struct CFullDiscount* headFullDiscount;
extern struct CFestivalDiscount* headFestivalDiscount;

//����βָ��
extern struct CAccount* tailAccount;
extern struct COrder* tailOrder;
extern struct CGoods* tailGoods;
extern struct CTrolley* tailTrolley;
extern struct CFullDiscount* tailFullDiscount;
extern struct CFestivalDiscount* tailFestivalDiscount;

extern void AddGiftOrder(float totalCost);

extern void OrderAdd(double totalCost, int goodsNum, int nums);


//�޸��µ����µ�һϵ����Ϣ�ı䣬�����û��������۶�,�û���Ա�ȼ������ﳵ��Ʒ����Ʒ���,��Ʒ����,���������Լ����ܵ���Ʒ����
void InformationChange(int numPurchaseGoods, int goodsSerialNumber);

//���ﳵ����
void GoodsTrolley();
//���ﳵ����
//--�����Ʒ�����ﳵ
void GoodsTrolleyAdd(struct CGoods* pNow);
//--ɾ�����ﳵ�е�ĳ����Ʒ
void GoodsTrolleyDelete(int goodsSerialNumber);
void GoodsTrolleyDeleteNoRemind(int goodsSerialNumber);

//--�޸Ĺ��ﳵ����Ʒ����
void GoodsTrolleyChange(int goodsSerialNumber);
//--������ﳵ��������Ʒ
void GoodsTrolleyClear();
//�µ�
//--�����ﳵ��ĳ����Ʒ
void GoodsTrolleyBuy(int goodsSerialNumber);
void GoodsTrolleyBuyNoRemind(struct CTrolley* pTro);
//--�����ﳵ��������Ʒ
void TotalGoodsTrolleyBuy();
//֧�������֧����ʽ
//--���㹺�ﳵ��ĳ����Ʒ�����ܽ��
void AllGoodsTotalCost(int goodsSerialNumber, double* totalCost);
double TotalCostMoney(int goodsSerialNumber, struct CTrolley* pTro);
//--ѡ��֧����ʽ
int ModeOfPayment();

#endif 
