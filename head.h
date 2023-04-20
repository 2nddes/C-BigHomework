#ifndef HEAD_H
#define HEAD_H

#include<time.h>

struct CAccount
{
	float discount;
	char userName[40];//用户名
	char password[40];//登陆密码
	char RealName[40];//会员姓名
	char Position[100];//送货地址
	long long phoneNumber;//会员手机号
	int grade; //0管理员，其他为普通用户
	float totalAmount;//总单额
	struct CAccount* next;
};
struct COrder
{
	int orderNum;//订单编号
	char userName[40];//用户名
	char goodsName[40];//商品名称
	int bottleNums;//下单瓶数
	long long teleNum;//订者电话
	char address[100];//送货地址
	char realName[40];//订者姓名
	struct tm time;//订单时间
	int orderStatus;//订单状态，0为完成，1为未完成
	float singleMoney;//订单金额
	int StatementOFChangingOrRefunding;//0 未退换货；1 正在退货；2 正在换货；3 退换货完毕
	char giftName[40];
	int giftNum;

	struct COrder* next;
};
struct CGoods
{
	int goodsNum;//商品编号
	char brand[40];//商品品牌(全称)
	char kind[20];//果味饮料 茶类饮料 碳酸饮料 功能饮料 矿泉水 纯净水 咖啡 白酒 啤酒 红酒 果酒
	int netContent;//每瓶净含量（ml）
	int boxesNumber;//每箱数量
	int concentration;//酒的浓度
	float price;//每箱价格
	int stock;//库存
	int sales;//销量
	struct CGoods* next;
};
struct CTrolley
{
	char userName[40];//用户名
	int goodsNumber;//商品编号
	char brand[20];//商品品牌
	int units;//1为箱，2为瓶
	int boxesNumber;//每箱数量
	int trolleyGoodsNumber;//商品瓶数***箱转换为瓶
	struct CTrolley* next;
};
struct CFestivalDiscount {
	float discountSetted;
	float discountNow;
	struct tm deadline;
	struct tm startTime;
};
struct CFullDiscount {
	int stageOfDiscoiunt;//折扣阶梯，最多允许10个折扣阶梯
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

struct CAccount* pLoggedInUser;//指向当前已登录用户的结构体的指针

struct CAccount* headAccount;
struct COrder* headOrder;
struct CGoods* headGoods;
struct CTrolley* headTrolley;
struct CFullDiscount* headFullDiscount;
struct CFestivalDiscount* headFestivalDiscount;
struct CGrade* headGrade;

//链表尾指针
struct CAccount* tailAccount;
struct COrder* tailOrder;
struct CGoods* tailGoods;
struct CTrolley* tailTrolley;

int scan(int* );//输入阿拉伯数字

/////////////////////////////////////////////////////////////////////////////
//读取数据
void LoadAccountData();//加载账户文件
void LoadGoodsData();//加载商品文件
void LoadOrderData();//加载订单文件
void LoadTrolleyData();//加载购物车文件
void LoadFullDiscountData();//加载满减折扣文件
void LoadFestivalDiscountData();//加载活动折扣文件
void LoadGiftData();//加载赠品文件
void LoadGradeData();//加载等级文件
/////////////////////////////////////////////////////////////////////////////
//保存信息
void SaveGoodsData();
void SaveGoodsTrolleyData();
void SaveAccountData();
void SaveOrderData();
void SaveGiftData();
void SaveGradeData();
/////////////////////////////////////////////////////////////////////////////
//释放链表
void releaseAccount();
void releaseData();
/////////////////////////////////////////////////////////////////////////////

#endif 