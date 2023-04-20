#define _CRT_SECURE_NO_WARNINGS

#include "head.h"
#include "otherSetting.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define LEARNING_RATE 0.01 // 学习速率
#define EPOCHS 1000 // 迭代次数

int sampleNum = 0;
float y[100000];
float logY[100000] = { 0 };
float x[100000][2]; //x[i][0]存储时间，x[i][1]存储订单编号
float w0[10000] = { 0.0 }; // 截距
float w1[10000] = { 0.0 };
float w2[10000] = { 0.0 };// 回归系数
float x_mean[10000] = { 0 };
float y_mean[10000] = { 0 };



/*
y_hat：每个样本的预测值，即回归模型的输出值。
error：每个样本的误差，即预测值与真实值之间的差距。
w0_gradient：截距的梯度，即误差关于截距的导数。
w1_gradient：斜率的梯度，即误差关于斜率的导数。
LEARNING_RATE：学习速率，即每次更新回归系数的步长大小。
EPOCHS：迭代次数，即训练过程中回归系数更新的次数。
i：用于迭代的循环计数器。
j：用于循环遍历样本的计数器。
*/




void SetDiscount();
void GiftSet();
int CheckTime(int* time);

float covert_TM_To_INT(struct tm* timeinfo) {
	int month = timeinfo->tm_mon + 1;      //月份加上1
	int day = timeinfo->tm_mday;           //日期

	int result =  month * 100 + day;

	return result;
}
//将struct tm格式的数据一一对应转换为int类型数据

float mean(float values[], int length, int k) {//对一维数组求均值
	int i;
	float sum = 0.0;
	for (i = 0; i < length; i++) {
		sum += values[i + k * length];

	}
	float mean = (float)(sum / length);
	return mean;
}
float meanX(float values[][2], int length, int k) {//对一维数组求均值
	int i;
	float sum = 0.0;
	for (i = 0; i < length; i++) {
		sum += values[i + k * length][0];
	}
	float mean = (float)(sum / length);
	return mean;
}


float variance(float* values, float mean, int length) {//这里求的是平方和，没有除以n
	float sum = 0.0;
	int i;
	for (i = 0; i < length; i++) {
		sum += (values[i] - mean) * (values[i] - mean);
	}
	return sum;
}
float varianceX(float values[][2], float mean, int length, int k) {//这里求的是平方和，没有除以n
	float sum = 0.0;
	int i;
	for (i = 0; i < length; i++) {
		sum += (values[i + k*length][0] - mean) * (values[i + k*length][0] - mean);
	}
	return sum;
}

float covariance(float x[][2], float mean_x[], float y[], float mean_y[], int length, int k) {
	float cov = 0.0;
	int i = 0;
	for (i = 0; i < length; i++) {
		cov += (x[i + k * length][0] - mean_x[k]) * (y[i + k * length] - mean_y[k]);
	}
	return cov;
}



//线性预测
//由均值方差估计回归系数
// 输入参数：coef* 存放系数的数组
void LinearSalesPrediction(int state) {
	int length;
	int totalGoods;
	if (state == 1) {
		length = 30;
	}
	else length = 7;
	totalGoods = sampleNum / length;

	int i = 0;
	for (i = 0; i < totalGoods; i++) {
		x_mean[i] = 0;
		y_mean[i] = 0;
		w1[i] = 0;
		w0[i] = 0;
	}

	for (i = 0; i < totalGoods; i++) {
		x_mean[i] = meanX(x, length, i);
		y_mean[i] = mean(y, length, i);


		w1[i] = covariance(x, x_mean, y, y_mean, length, i) / varianceX(x, x_mean[i], length, i);
		w0[i] = y_mean[i] - w1[i] * x_mean[i];
	}
}


//对数型预测模型
void LogarithmicSalesPrediction(int state) {
	int length;
	int totalGoods;
	if (state == 1) {
		length = 30;
	}
	else length = 7;
	totalGoods = sampleNum / length;
	 
	
	 
	int i = 0;
	for (i = 0; i < sampleNum; i++) {
		logY[i] = log(y[i]);
	}

	for (i = 0; i < totalGoods; i++) {
		x_mean[i] = meanX(x, length, i);
		y_mean[i] = mean(logY, length, i);
		
		w1[i] = covariance(x, x_mean, logY, y_mean, length, i) / varianceX(x, x_mean[i], length, i);
		w0[i] = y_mean[i] - w1[i] * x_mean[i];
		w0[i] = exp(w0[i]);
	}
}

int JudgementOfTime(int state, time_t timeOfOrder) {
	time_t now = time(NULL);
	struct tm nowTime = *localtime(&now);	
	//以最近一个月的销量为样本进行预测
	if (state == 1) {
		nowTime.tm_wday -= 30 ;//30天前的日期
		time_t timeBefore_1_Mon = mktime(&nowTime);
		if (difftime(timeBefore_1_Mon, timeOfOrder) > 0) return 0;
		else return 1;
	}
	//以最近一周的销量为样本进行预测
	else if (state == 2) {
		nowTime.tm_wday -= 7;//7天前的日期
		time_t timeBefore_7_Day = mktime(&nowTime);
		if (difftime(timeBefore_7_Day, timeOfOrder) > 0) return 0;
		else return 1;
	}
}
//判断输入的时间是否在需要的时间范围之内


void SalesPrediction() {


	int num = -1;
	while (1) {
		num = -1;
		system("cls");
		printf("\n\t************************  销量预测  ****************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--返回                  返回--0-****************\n");
		printf("\n\t**************-1--线性预测          指数预测--2-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		scan(&num);
		if (num == 0) {
			system("cls");
			return;
		}
		else if (num == 1 || num == 2) {
			struct COrder* currOrder = headOrder;
			int n1 = -1;
			int state = 0;
			//选择样本数据
			while (1) {
				n1 = -1;
				sampleNum = 0;
				printf("\n\t输入[1]以最近30天销量为样本预测，输入[2]以最近7天销量为样本预测 ：");
				scan(&n1);
				if (n1 == 1) {
					state = 1;
					break;
				}
				else if (n1 == 2) {
					state = 2;
					break;
				}
				else printf("\n\t不存在与您输入的数据对应的编号，请重新输入！\n");
			}
			
			//*****************读取样本数据*****************
			struct COrder* pOrder = headOrder;
			struct CGoods* p = headGoods;
			int i = 0, j = 0;
			sampleNum = 0;
			int totalGoods = 0;//商品总数
			//1. 计算样本总数
			while (p != NULL) {
				p = p->next;
				totalGoods++;
				if (state == 1)
					sampleNum += 30;
				else if (state == 2)
					sampleNum += 7;
			}

			//2. 读取样品编号信息以及时间信息
			i = 0;
			p = headGoods;
			for (i = 0; i < totalGoods; i++) {
				int j = 0;
				if (state == 1) {
					time_t now = time(NULL);
					now -= 30 * 24 * 60 * 60;
					struct tm nowTime = *localtime(&now);

					for (j = 0; j < 30; j++) {

						now += 1 * 24 * 60 * 60;
						nowTime = *localtime(&now);

						x[j + 30 * (i)][0] = covert_TM_To_INT(&nowTime);
						x[j + 30 * (i)][1] = p->goodsNum;
					}
				}
				else if (state == 2) {

					time_t now = time(NULL);
					now -= 7 * 24 * 60 * 60;
					struct tm nowTime = *localtime(&now);

					
					for (j = 0; j < 7;j++) {
						
						now += 1 * 24 * 60 * 60;
						nowTime = *localtime(&now);

						x[j + 7 * (i)][0] = covert_TM_To_INT(&nowTime);
						x[j + 7 * (i)][1] = p->goodsNum;
					}
				}
				p = p->next;
			}

			//3. 读取指定时间内指定商品的销量信息
			i = 0; j = 0;
			currOrder = headOrder;
			p = headGoods;
			for (i = 0; i < totalGoods; i++) {
				if (state == 1) {
					for (j = 0; j < 30; j++) {
						int findState = 0;
						int totalBottles = 0;
						currOrder = headOrder;
						while (currOrder != NULL) {
							//找到订单中的商品名称对应的商品编号
							p = headGoods;
							while (p != NULL && strcmp(p->brand, currOrder->goodsName) != 0) {
								p = p->next;
							}
							float int_time = covert_TM_To_INT(&currOrder->time);
							//找到所有在该日期完成的商品编号为x[1][0]的订单
							if (int_time == x[j + 30 * (i)][0] && p->goodsNum == x[j + 30 * (i)][1]) { totalBottles += currOrder->bottleNums; findState = 1; }
							currOrder = currOrder->next;
						}
						if (findState == 0) {
							y[j + 30 * (i)] = 0;
						}
						else y[j + 30 * (i)] = totalBottles;
					}
				}
				else {
					for (j = 0; j < 7; j++) {
						int findState = 0;
						int totalBottles = 0;
						currOrder = headOrder;
						while (currOrder != NULL) {
							//找到订单中的商品名称对应的商品编号
							p = headGoods;
							while (p != NULL && strcmp(p->brand, currOrder->goodsName) != 0) {
								p = p->next;
							}
							float int_time = covert_TM_To_INT(&currOrder->time);
							//找到所有在该日期完成的商品编号为x[1][0]的订单
							if (int_time == x[j + 7 * (i)][0] && p->goodsNum == x[j + 7 * (i)][1]) { totalBottles += currOrder->bottleNums; findState = 1; }
							currOrder = currOrder->next;
						}
						if (findState == 0) {
							y[j + 7 * (i)] = 0;
						}
						else y[j + 7 * (i)] = totalBottles;
					}
				}
			
			}
			//*****************读取样本数据完毕*****************

			//*****************计算回归方程公式截距和斜率*****************
			if(num == 1)LinearSalesPrediction(state);
			
			else if(num ==2)LogarithmicSalesPrediction(state);
			//*****************计算回归方程公式截距和斜率完毕*****************


			if(num == 1) printf("\n\t-----------------------------------------------------线 性 预 测---------------------------------------------\n\n");
			else printf("\n\t-----------------------------------------------------对 数 预 测---------------------------------------------\n\n");
			printf("\t               ********************-0--返回                              返回--0-********************\n\n");
			printf("\t               ********************-1--预测单天商品销量      预测下周商品销量--2-********************\n\n");
			printf("\t               -------------------------------------------------------------------------------------\n\n");


			int n2 = -1;
			while (1) {
				n2 = -1;
				printf("\n\t请输入操作序号：");
				scan(&n2);
				if (n2 == 0) {
					system("cls");
					break;
					//
				}
				else if (n2 == 1) {
					//一 . 输入需要预测的时间**********************************
					struct tm TM_timeOfPrediction;
					int timeOfPrediction[6];
					const int month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // 每个月的天数（暂时不考虑闰年）
					while (1) {
						printf("\n\t请输入需要预测的时间，格式为 yyyy mm dd（分别对应年、月、日）：");
						printf("\n\t");
						int i = 0;
						int valid = 0;
						for (i = 0; i < 3; i++) {
							valid = 0;
							while (!valid) {
								if (scanf("%d", &timeOfPrediction[i]) != 1) {
									printf("\t输入无效，请重新输入：");
									while (getchar() != '\n');
								}
								else {
									valid = 1;
									getchar();
								}
							}
						}

						TM_timeOfPrediction.tm_year = timeOfPrediction[0] - 1900;
						TM_timeOfPrediction.tm_mon = timeOfPrediction[1] - 1;
						TM_timeOfPrediction.tm_mday = timeOfPrediction[2];
						TM_timeOfPrediction.tm_hour = timeOfPrediction[3] = 0;
						TM_timeOfPrediction.tm_min = timeOfPrediction[4] = 0;
						TM_timeOfPrediction.tm_sec = timeOfPrediction[5] = 0;

						if (CheckTime(timeOfPrediction) == 1 || difftime(time(NULL), mktime(&TM_timeOfPrediction)) >= 0) {
							printf("\n\t不能预测今天之前的时间！请重新输入！\n");
						}
						else if (CheckTime(timeOfPrediction) == 0) {
							break;
						}
					}
					//二. 输出信息**********************************
					printf("\n\t%4d年%2d月%2d日的商品销量预测如下：\n", timeOfPrediction[0], timeOfPrediction[1], timeOfPrediction[2]);
					char* arrName[3] = {"商品编号","商品名称", "预测销量(瓶)"};
					printf("\n\t%-10s\t%-30s\t%-12s\n", arrName[0], arrName[1], arrName[2]);
					printf("\t-----------------------------------------------------------");
					p = headGoods;
					while (p != NULL) {
						//获取斜率截距的坐标m
						int m = 0;
						int days;
						for (m = 0; m < sampleNum; m++) {
							if (x[m][1] == p->goodsNum) {
								break;
							}
						}
						if (state == 1) {
							m -= m % 30;
							days = 30;
							m /= days;
						}
						else {
							m -= m % 7;
							days = 7;
							m /= days;
						}


						if (num == 1) {
							if (p->goodsNum, p->brand, (int)(w0[m] + w1[m] * covert_TM_To_INT(&TM_timeOfPrediction)) > 0) {
								printf("\n\t%-10d\t%-30s\t%-12d\n", p->goodsNum, p->brand, (int)(w0[m] + w1[m] * covert_TM_To_INT(&TM_timeOfPrediction)));
							}
							else printf("\n\t%-10d\t%-30s\t%-12d\n", p->goodsNum, p->brand, 0);
						}
						else {
							if((int)(w0[m] * exp(w1[m] * covert_TM_To_INT(&TM_timeOfPrediction))) > 0)
							printf("\n\t%-10d\t%-30s\t%-12d\n", p->goodsNum, p->brand, (int)(w0[m] * exp(w1[m] * covert_TM_To_INT(&TM_timeOfPrediction)))); 
							else printf("\n\t%-10d\t%-30s\t%-12d\n", p->goodsNum, p->brand, 0);
						}
							
						printf("\t-----------------------------------------------------------");
						p = p->next;
					}
				}
				else if (n2 == 2) {
					printf("\n\t下周的商品销量预测如下：\n");
					char* arrName[3] = { "商品编号","商品名称", "预测销量(瓶)" };
					printf("\n\t%-10s\t%-30s\t%-12s\n", arrName[0], arrName[1], arrName[2]);
					printf("\t-----------------------------------------------------------");
					p = headGoods;
					int hatMoney = 0;
					while (p != NULL) {
						hatMoney = 0;

						time_t now = time(NULL);
						struct tm time_7_day_before = *localtime(&now);

						//获取斜率截距的坐标m
						int m = 0;
						int days;
						for (m = 0; m < sampleNum; m++) {
							if (x[m][1] == p->goodsNum) {
								break;
							}
						}
						if (state == 1) {
							m -= m%30;
							days = 30;
							m /= days;
						}
						else { 
							m -= m % 7; 
							days = 7;
							m /= days;
						}

						int i = 0;
						for (i = 0; i < 7; i++) {
							now += 1 * 24 * 60 * 60;
							time_7_day_before = * localtime(&now);
							//线性预测
							if (num == 1) {
								if ((int)(w0[m] + w1[m] * covert_TM_To_INT(&time_7_day_before) + w2[m] * p->goodsNum) < 0) {
									hatMoney += 0;
								}
								else
								hatMoney += (int)(w0[m] + w1[m] * covert_TM_To_INT(&time_7_day_before) );
								
							}
							//非线性预测
							else {
								
								hatMoney += (int)(w0[m] * exp(w1[m] * covert_TM_To_INT(&time_7_day_before)));
								if (hatMoney < 0) {
									hatMoney = 0;
								}
							}
						}
						printf("\n\t%-10d\t%-30s\t%-12d\n", p->goodsNum, p->brand, hatMoney);
						printf("\t-----------------------------------------------------------");
						p = p->next;
					}

				}
				else printf("\n\t没有您输入的数据对应的编号，请重新输入！\n");
			}


			break;
		}
		else printf("\n\t不存在与您输入的数据相对应的编号！请重新输入！\n");
	}
}
//销量预测主界面


void OtherSetting() {
	
	struct CFullDiscount* pFullDiscount = headFullDiscount;
	struct CFestivalDiscount* pFestivalDiscount = headFestivalDiscount;
	struct COrder* pOrder = headOrder;
	while (1) {
		system("cls");
		pFullDiscount = headFullDiscount;
		pFestivalDiscount = headFestivalDiscount;
		pOrder = headOrder;
		printf("\n\t************************  其它设置  ****************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--          返回           --0-*****************\n");
		printf("\n\t**************-1--        赠品管理          --1-****************\n");
		printf("\n\t**************-2--        折扣管理          --2-****************\n");
		printf("\n\t**************-3--        销量预测          --3-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		int n1;
		n1 = -1;
		scan(&n1);
		if (n1 == 0) {
			system("cls");
			return;
		}
		else if (n1 == 1) {
			GiftSet();
		}
		else if (n1 == 2) {
			SetDiscount();
		}
		else if (n1 == 3) {
			SalesPrediction();
		}
		else {
			system("cls"); 
			printf("\n\t您的输入有误，请重新输入！\n");
		}
	}


}
