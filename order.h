#ifndef ORDER_H
#define ORDER_H

extern struct CAccount* pLoggedInUser;//ָ��ǰ�ѵ�¼�û��Ľṹ���ָ��

extern struct CAccount* headAccount;//�˻�ͷָ��
extern struct COrder* headOrder;//����ͷָ��
extern struct CGoods* headGoods;//��Ʒͷָ��
extern struct CTrolley* headTrolley;//���ﳵͷָ��
extern struct CFullDiscount* headFullDiscount;//�ۿ�ͷָ��
extern struct CFestivalDiscount* headFestivalDiscount;//�����ۿ�ͷָ��

//����βָ��
extern struct CAccount* tailAccount;//�˻�βָ��
extern struct COrder* tailOrder;//����βָ��
extern struct CGoods* tailGoods;//��Ʒβָ��
extern struct CTrolley* tailTrolley;//���ﳵβָ��
extern struct CFullDiscount* tailFullDiscount;//�ۿ�βָ��
extern struct CFestivalDiscount* tailFestivalDiscount;//�ڼ����ۿ�βָ��

//�����Ʒ��Ϣ
extern void GoodsPrint(struct CGoods* head);

void OrderStatusChangeAutomatic();
//������Ʒ����
void AddGiftOrder(float totalCost);
//�������˵�
void OrderFindAndChange();
//������ѯ
void OrderSeek();
//�����޸�
void OrderChange();
//��������
void OrderAdd(double totalCost, int goodsNum, int nums);
//����ɾ��
void OrderDelete();
//�˻���
void GoodsChangingAndRefunding();
//�����˻���ɵ���Ϣ�ı�
void InformationChangeForRefunding(struct COrder* p);
///���ڻ�����ɵ���Ϣ�ı�
void InformationChangeForChanging(struct COrder*p);
#endif 