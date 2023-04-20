#define _CRT_SECURE_NO_WARNINGS
#include "discount.h"
#include "head.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//检查输入的时间是否非法
int CheckTime(int* time) {
	const int month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // 每个月的天数（暂时不考虑闰年）
	// 检查年份是否合法
	if (time[0] < 0) {
		printf("\n\t年份不能为负数！不能输入除阿拉伯数字外的其他字符！");
		return 1;
	}
	// 检查月份是否合法
	if (time[1] < 1 || time[1] > 12) {
		printf("\n\t月份不合法！不能输入除阿拉伯数字外的其他字符！");
		return 1;
	}
	// 检查日期是否合法
	int max_day = month_days[time[1] - 1]; // 获取该月份的最大天数
	if (time[1] == 2 && time[0] % 4 == 0 && (time[0] % 100 != 0 || time[0] % 400 == 0)) {
		// 如果是闰年的2月，最大天数为29天
		max_day = 29;
	}
	if (time[2] < 1 || time[2] > max_day) {
		printf("\n\t日期不合法！不能输入除阿拉伯数字外的其他字符！");
		return 1;
	}
	// 检查小时是否合法
	if (time[3] < 0 || time[3] > 23) {
		printf("\n\t小时不合法！不能输入除阿拉伯数字外的其他字符！");
		return 1;
	}
	// 检查分钟是否合法
	if (time[4] < 0 || time[4] > 59) {
		printf("\n\t分钟不合法！不能输入除阿拉伯数字外的其他字符！");
		return 1;
	}
	// 检查秒数是否合法
	if (time[5] < 0 || time[5] > 59) {
		printf("\n\t秒数不合法！不能输入除阿拉伯数字外的其他字符！");
		return 1;
	}
	return 0;
}
//找到当前消费额能够享受的最高满减折扣
double GetFullDiscount(int previousTotalPrice) {
	float discount = 10;//默认打10折

	int stageOfDiscount = headFullDiscount->stageOfDiscoiunt;
	int i = 0;

	while (previousTotalPrice > headFullDiscount->price[i] && i < stageOfDiscount) {
		i++;//找到当前消费额能够享受的最高满减折扣对应的最低消费额
	}
	if (i == stageOfDiscount) {
		discount = headFullDiscount->discount[i - 1];
		//当前消费额已经超过了最高满减折扣对应的最低消费额
	}
	else if (previousTotalPrice <= headFullDiscount->price[i]) {
		if (i == 0) {
			discount = 10;
			//没有达到最低满减折扣对应的最低消费额，不享受满减折扣
		}
		else {
			discount = headFullDiscount->discount[i - 1];
		}
	}

	return discount;
}
//获取经过满减折扣和节假日折扣之后的商品价格
double GetPriceAfterActivaiyDiscountAndFullDiscount(double totalPrice) {
	double priceAfterDiscount = totalPrice;

	priceAfterDiscount = totalPrice * (headFestivalDiscount->discountNow * 0.1) * (0.1 * GetFullDiscount(totalPrice * (headFestivalDiscount->discountNow * 0.1)));

	return priceAfterDiscount;
}

/******调整折扣*/
//1. 检查当前时间是否在能够享受节假日折扣的时间范围之内
void ResetFestivalDiscount() {
	time_t now;
	time(&now);

	if (now < mktime(&(headFestivalDiscount->startTime)) || now > mktime(&(headFestivalDiscount->deadline))) {
		headFestivalDiscount->discountNow = 10;
		FILE* fp = fopen("festivalDiscount.dat", "wb");
		fwrite(headFestivalDiscount, sizeof(struct CFestivalDiscount), 1, fp);
		fclose(fp);
	}
	else headFestivalDiscount->discountNow = headFestivalDiscount->discountSetted;

	return;
}
//2. 调整节假日折扣额以及顾客能够享受节假日折扣的截止时间
void SetFestivalDiscount() {
	float festivalDiscount = 10;
	float num = -1;

	if (headFestivalDiscount->discountSetted * 100 < 1000) {
		printf("\n\t当前已设置折扣活动！活动期间用户购买任意商品均可享受%.2f折\n", headFestivalDiscount->discountSetted);
		printf("\n\t活动开始时间为%4d年%2d月%2d日%2d时%2d分%2d秒\n", headFestivalDiscount->startTime.tm_year + 1900, headFestivalDiscount->startTime.tm_mon + 1,
			headFestivalDiscount->startTime.tm_mday, headFestivalDiscount->startTime.tm_hour, headFestivalDiscount->startTime.tm_min, headFestivalDiscount->startTime.tm_sec);
		printf("\t活动截止时间为%4d年%2d月%2d日%2d时%2d分%2d秒\n", headFestivalDiscount->deadline.tm_year + 1900, headFestivalDiscount->deadline.tm_mon + 1,
			headFestivalDiscount->deadline.tm_mday, headFestivalDiscount->deadline.tm_hour, headFestivalDiscount->deadline.tm_min, headFestivalDiscount->deadline.tm_sec);
		printf("\n\t输入0可返回,输入其他任意正整数可重新设置折扣：");
	}
	else {
		printf("\n\t当前还未设置任何折扣活动！\n");
		printf("\n\t输入0可返回,输入其他任意正整数可设置折扣：");
	}
	while (1) {
		int n1 = -1;
		scan(&n1);
		if (n1 == 0) {
			system("cls");
			return;
		}
		else if (n1 > 0) break;
		else;
	}

	//设置折扣
	while (1) {
		num = -1;
		printf("\n\t请输入折扣（输入9打九折），输入0返回:");
		scanf_s("%f", &num);
		getchar();
		if (num * 100 > 0 && (num * 100 < 1000 || fabs(num - 10) < 1e-6)) {
			festivalDiscount = num;
			break;
		}
		else if (fabs(num - 0) < 1e-6) {
			return;
		}
		else {
			printf("\t输入有误，请重新输入！\n");
		}
	}

	struct tm start;
	struct tm end;
	int timeStart[6]; // 定义int型数组time，用于存储时间
	int timeEnd[6];
	const int month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // 每个月的天数（暂时不考虑闰年）
	// 从键盘中读入时间，依次为年月日时分秒
	while (1) {
		while (1) {
			printf("\n\t请输入活动开始时间，格式为 yyyy mm dd hh mm ss（分别对应年、月、日、时、分、秒）：");
			printf("\n\t");
			int i = 0;
			int valid = 0;
			for (i = 0; i < 6; i++) {
				valid = 0;
				while (!valid) {
					if (scanf("%d", &timeStart[i]) != 1) {
						printf("\t输入无效，请重新输入：");
						while (getchar() != '\n');
					}
					else {
						valid = 1;
						getchar();
					}
				}
			}
			if (CheckTime(timeStart) == 1) {
				printf("请重新输入！\n");
			}
			else if (CheckTime(timeStart) == 0) {
				break;
			}
		}

		//设置截止日期
		while (1) {
			printf("\t请输入活动截止时间，格式为 yyyy mm dd hh mm ss（分别对应年、月、日、时、分、秒）：");
			printf("\n\t");
			int i = 0;
			int valid = 0;
			for (i = 0; i < 6; i++) {
				valid = 0;
				while (!valid) {
					if (scanf("%d", &timeEnd[i]) != 1) {
						printf("\t输入无效，请重新输入：");
						while (getchar() != '\n');
					}
					else {
						valid = 1;
						getchar();
					}
				}
			}
			if (CheckTime(timeEnd) == 1) {
				printf("\n\t请重新输入！\n");
			}
			else if (CheckTime(timeEnd) == 0) {
				break;
			}
		}

		end.tm_year = timeEnd[0] - 1900;
		end.tm_mon = timeEnd[1] - 1;
		end.tm_mday = timeEnd[2];
		end.tm_hour = timeEnd[3];
		end.tm_min = timeEnd[4];
		end.tm_sec = timeEnd[5];
		start.tm_year = timeStart[0] - 1900;
		start.tm_mon = timeStart[1] - 1;
		start.tm_mday = timeStart[2];
		start.tm_hour = timeStart[3];
		start.tm_min = timeStart[4];
		start.tm_sec = timeStart[5];

		//检查日期设置是否合理
		if (difftime(mktime(&(start)), mktime(&end)) > 0) {
			printf("\n\t活动开始时间不能迟于活动截止时间！请重新设置！\n");
		}
		else break;
	}

	headFestivalDiscount->deadline = end;
	headFestivalDiscount->startTime = start;
	headFestivalDiscount->discountSetted = festivalDiscount;
	ResetFestivalDiscount();

	FILE* fp = fopen("festivalDiscount.dat", "wb");
	fwrite(headFestivalDiscount, sizeof(struct CFestivalDiscount), 1, fp);
	fclose(fp);

	printf("\n\t设置成功！活动期间打%.2f折\n", headFestivalDiscount->discountSetted);
	printf("\n\t活动开始时间为%4d年%2d月%2d日%2d时%2d分%2d秒\n", headFestivalDiscount->startTime.tm_year + 1900, headFestivalDiscount->startTime.tm_mon + 1,
		headFestivalDiscount->startTime.tm_mday, headFestivalDiscount->startTime.tm_hour, headFestivalDiscount->startTime.tm_min, headFestivalDiscount->startTime.tm_sec);
	printf("\t活动截止时间为%4d年%2d月%2d日%2d时%2d分%2d秒\n", headFestivalDiscount->deadline.tm_year + 1900, headFestivalDiscount->deadline.tm_mon + 1,
		headFestivalDiscount->deadline.tm_mday, headFestivalDiscount->deadline.tm_hour, headFestivalDiscount->deadline.tm_min, headFestivalDiscount->deadline.tm_sec);
	printf("\n\t输入任意键可以返回上一界面\n");
	getchar();

}
//3. 设置满减折扣
void SetFullDiscount() {
	int num_ = -1;
	if (headFullDiscount->stageOfDiscoiunt == 1 && fabs(headFullDiscount->discount[0] - 10) < 1e-6) {
		printf("\n\t您当前还未设置任何满减优惠！输入[0]返回，输入其他正整数设置满减优惠：");
		int n1 = -1;
		scan(&n1);
		if (n1 == 0) return;
	}
	else {
		printf("\n\t您当前设置了%d个折扣阶梯。每个折扣阶梯对应的最低消费额和折扣率分别为：\n", headFullDiscount->stageOfDiscoiunt);
		int i = 0;
		while (i != headFullDiscount->stageOfDiscoiunt) {
			printf("\t%.2f元%.2f折\n", headFullDiscount->price[i], headFullDiscount->discount[i]);
			i++;
		}
		printf("\n\t输入[0]返回，输入其他正整数设置满减优惠：");
		int n1 = -1;
		scan(&n1);
		if (n1 == 0) return;
	}
	while (1) {
		num_ = -1;
		printf("\n\t请输入您要设置的折扣层次数，最多支持10个结构层次数,输入0返回：");
		scan(&num_);
		if (num_ >= 1 && num_ <= 10) {
			headFullDiscount->stageOfDiscoiunt = num_;
			break;
		}
		else if (num_ == 0) {
			return;
		}
		else {
			printf("\t您的输入有误，请重新输入！");
		}
	}
	int i = 0;
	float discount[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	float price[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	while (1)
	{
		int valid = 0;
		for (i = 0; i != headFullDiscount->stageOfDiscoiunt; i++) {
			printf("\n\t请依次满足输入第%d层折扣的最低消费额和相应折扣(折扣必须大于0）,如\"800 9\", 表示折扣前订单总价满800元可享受9折 :", i + 1);
			valid = 0;
			while (!valid) {
				if (scanf("%f", &price[i]) != 1 || scanf("%f", &discount[i]) != 1) {
					printf("\t输入无效!");
					printf("\t请重新输入：");
					while (getchar() != '\n');
				}
				else {
					if (price[i] * 100 <= 0 || 100 * discount[i] <= 0 || discount[i] * 100 >= 1000) {
						printf("\t任一折扣的最低购买金额不得小于等于0 \t任一折扣不得小于等于0，且不能大于10!\n");
						printf("\t输入无效!");
						printf("\n\t请重新输入：");
					}
					else
					{
						valid = 1;
						getchar();
					}
				}
			}
		}

		i--;
		while (i != 0) {
			if (price[i] - price[i - 1] < 0 || discount[i] - discount[i - 1] > 0) {
				break;
			}
			i--;
		}
		if (i == 0) {
			break;
		}
		else {
			printf("\n\t第n层折扣力度必须大于第n-1层，第n层折扣最低消费额必须大于第n-1层！\n");
		}
	}

	for (i = 0; i < num_; i++) {
		headFullDiscount->discount[i] = discount[i];
		headFullDiscount->price[i] = price[i];
	}
	for (i = num_; i < 10; i++) {
		headFullDiscount->discount[i] = discount[num_ - 1];
		headFullDiscount->price[i] = price[num_ - 1];
	}


	FILE* fp = fopen("fullDiscount.dat", "wb");
	fwrite(headFullDiscount, sizeof(struct CFullDiscount), 1, fp);
	fclose(fp);

	printf("\n\t您已成功设置%d个折扣阶梯。每个折扣阶梯对应的最低消费额和折扣率分别为：\n", headFullDiscount->stageOfDiscoiunt);
	i = 0;
	while (i != headFullDiscount->stageOfDiscoiunt) {
		printf("\t%.2f元%.2f折\n", headFullDiscount->price[i], headFullDiscount->discount[i]);
		i++;
	}
	printf("\n\t输入任意字符可以返回\n");
	getchar();

}
//4. 折扣设置主界面
void SetDiscount() {
	system("cls");
	int num = -1;
	while (1) {
		num = -1;
		printf("\n\t************************  折扣设置  ****************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--          返回           --0-*****************\n");
		printf("\n\t**************-1--      节假日折扣设置      --1-*****************\n");
		printf("\n\t**************-2--       满减折扣设置       --2-*****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请在此输入操作序号：");
		scanf_s("%d", &num);
		getchar();
		if (num == 0) return;
		else if (num == 1) {
			SetFestivalDiscount();
			break;
		}
		else if (num == 2) {
			SetFullDiscount();
			break;
		}
		else {
			printf("\n\t您的输入有误，请重新输入\n");
		}
	}
}
