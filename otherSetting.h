#ifndef OTHERSETTING_H
#define OTHERSETTING_H

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

void OtherSetting();

#endif 