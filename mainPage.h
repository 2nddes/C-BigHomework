#ifndef MAINPAGE_H
#define MAINPAGE_H
extern struct CAccount* pLoggedInUser;//ָ��ǰ�ѵ�¼�û��Ľṹ���ָ��

extern struct CAccount* headAccount;
extern struct COrder* headOrder;
extern struct CGoods* headGoods;
extern struct CTrolley* headTrolley;
extern struct CFullDiscount* headFullDiscount;
extern struct CFestivalDiscount* headFestivalDiscount;

extern struct CAccount* tailAccount;

//��¼ǰ������
int MainPage();
//��¼��������
int PrePage();

#endif 