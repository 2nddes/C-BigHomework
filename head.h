#ifndef HEAD_H
#define HEAD_H

#include<time.h>

struct CAccount
{
	float discount;
	char userName[40];//�û���
	char password[40];//��½����
	char RealName[40];//��Ա����
	char Position[100];//�ͻ���ַ
	long long phoneNumber;//��Ա�ֻ���
	int grade; //0����Ա������Ϊ��ͨ�û�
	float totalAmount;//�ܵ���
	struct CAccount* next;
};
struct COrder
{
	int orderNum;//�������
	char userName[40];//�û���
	char goodsName[40];//��Ʒ����
	int bottleNums;//�µ�ƿ��
	long long teleNum;//���ߵ绰
	char address[100];//�ͻ���ַ
	char realName[40];//��������
	struct tm time;//����ʱ��
	int orderStatus;//����״̬��0Ϊ��ɣ�1Ϊδ���
	float singleMoney;//�������
	int StatementOFChangingOrRefunding;//0 δ�˻�����1 �����˻���2 ���ڻ�����3 �˻������
	char giftName[40];
	int giftNum;

	struct COrder* next;
};
struct CGoods
{
	int goodsNum;//��Ʒ���
	char brand[40];//��ƷƷ��(ȫ��)
	char kind[20];//��ζ���� �������� ̼������ �������� ��Ȫˮ ����ˮ ���� �׾� ơ�� ��� ����
	int netContent;//ÿƿ��������ml��
	int boxesNumber;//ÿ������
	int concentration;//�Ƶ�Ũ��
	float price;//ÿ��۸�
	int stock;//���
	int sales;//����
	struct CGoods* next;
};
struct CTrolley
{
	char userName[40];//�û���
	int goodsNumber;//��Ʒ���
	char brand[20];//��ƷƷ��
	int units;//1Ϊ�䣬2Ϊƿ
	int boxesNumber;//ÿ������
	int trolleyGoodsNumber;//��Ʒƿ��***��ת��Ϊƿ
	struct CTrolley* next;
};
struct CFestivalDiscount {
	float discountSetted;
	float discountNow;
	struct tm deadline;
	struct tm startTime;
};
struct CFullDiscount {
	int stageOfDiscoiunt;//�ۿ۽��ݣ��������10���ۿ۽���
	float discount[10];
	float price[10];
};
struct CGrade {
	int stageOfGrade;
	float price[10];
	float discount[10];
};
struct CGift {
	char gift[40];
	int nums;
	float threshold;
}Gift;

struct CAccount* pLoggedInUser;//ָ��ǰ�ѵ�¼�û��Ľṹ���ָ��

struct CAccount* headAccount;
struct COrder* headOrder;
struct CGoods* headGoods;
struct CTrolley* headTrolley;
struct CFullDiscount* headFullDiscount;
struct CFestivalDiscount* headFestivalDiscount;
struct CGrade* headGrade;

//����βָ��
struct CAccount* tailAccount;
struct COrder* tailOrder;
struct CGoods* tailGoods;
struct CTrolley* tailTrolley;

int scan(int* );//���밢��������

/////////////////////////////////////////////////////////////////////////////
//��ȡ����
void LoadAccountData();//�����˻��ļ�
void LoadGoodsData();//������Ʒ�ļ�
void LoadOrderData();//���ض����ļ�
void LoadTrolleyData();//���ع��ﳵ�ļ�
void LoadFullDiscountData();//���������ۿ��ļ�
void LoadFestivalDiscountData();//���ػ�ۿ��ļ�
void LoadGiftData();//������Ʒ�ļ�
void LoadGradeData();//���صȼ��ļ�
/////////////////////////////////////////////////////////////////////////////
//������Ϣ
void SaveGoodsData();
void SaveGoodsTrolleyData();
void SaveAccountData();
void SaveOrderData();
void SaveGiftData();
void SaveGradeData();
/////////////////////////////////////////////////////////////////////////////
//�ͷ�����
void releaseAccount();
void releaseData();
/////////////////////////////////////////////////////////////////////////////

#endif 