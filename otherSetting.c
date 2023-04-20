#define _CRT_SECURE_NO_WARNINGS

#include "head.h"
#include "otherSetting.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define LEARNING_RATE 0.01 // ѧϰ����
#define EPOCHS 1000 // ��������

int sampleNum = 0;
float y[100000];
float logY[100000] = { 0 };
float x[100000][2]; //x[i][0]�洢ʱ�䣬x[i][1]�洢�������
float w0[10000] = { 0.0 }; // �ؾ�
float w1[10000] = { 0.0 };
float w2[10000] = { 0.0 };// �ع�ϵ��
float x_mean[10000] = { 0 };
float y_mean[10000] = { 0 };



/*
y_hat��ÿ��������Ԥ��ֵ�����ع�ģ�͵����ֵ��
error��ÿ������������Ԥ��ֵ����ʵֵ֮��Ĳ�ࡣ
w0_gradient���ؾ���ݶȣ��������ڽؾ�ĵ�����
w1_gradient��б�ʵ��ݶȣ���������б�ʵĵ�����
LEARNING_RATE��ѧϰ���ʣ���ÿ�θ��»ع�ϵ���Ĳ�����С��
EPOCHS��������������ѵ�������лع�ϵ�����µĴ�����
i�����ڵ�����ѭ����������
j������ѭ�����������ļ�������
*/




void SetDiscount();
void GiftSet();
int CheckTime(int* time);

float covert_TM_To_INT(struct tm* timeinfo) {
	int month = timeinfo->tm_mon + 1;      //�·ݼ���1
	int day = timeinfo->tm_mday;           //����

	int result =  month * 100 + day;

	return result;
}
//��struct tm��ʽ������һһ��Ӧת��Ϊint��������

float mean(float values[], int length, int k) {//��һά�������ֵ
	int i;
	float sum = 0.0;
	for (i = 0; i < length; i++) {
		sum += values[i + k * length];

	}
	float mean = (float)(sum / length);
	return mean;
}
float meanX(float values[][2], int length, int k) {//��һά�������ֵ
	int i;
	float sum = 0.0;
	for (i = 0; i < length; i++) {
		sum += values[i + k * length][0];
	}
	float mean = (float)(sum / length);
	return mean;
}


float variance(float* values, float mean, int length) {//���������ƽ���ͣ�û�г���n
	float sum = 0.0;
	int i;
	for (i = 0; i < length; i++) {
		sum += (values[i] - mean) * (values[i] - mean);
	}
	return sum;
}
float varianceX(float values[][2], float mean, int length, int k) {//���������ƽ���ͣ�û�г���n
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



//����Ԥ��
//�ɾ�ֵ������ƻع�ϵ��
// ���������coef* ���ϵ��������
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


//������Ԥ��ģ��
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
	//�����һ���µ�����Ϊ��������Ԥ��
	if (state == 1) {
		nowTime.tm_wday -= 30 ;//30��ǰ������
		time_t timeBefore_1_Mon = mktime(&nowTime);
		if (difftime(timeBefore_1_Mon, timeOfOrder) > 0) return 0;
		else return 1;
	}
	//�����һ�ܵ�����Ϊ��������Ԥ��
	else if (state == 2) {
		nowTime.tm_wday -= 7;//7��ǰ������
		time_t timeBefore_7_Day = mktime(&nowTime);
		if (difftime(timeBefore_7_Day, timeOfOrder) > 0) return 0;
		else return 1;
	}
}
//�ж������ʱ���Ƿ�����Ҫ��ʱ�䷶Χ֮��


void SalesPrediction() {


	int num = -1;
	while (1) {
		num = -1;
		system("cls");
		printf("\n\t************************  ����Ԥ��  ****************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--����                  ����--0-****************\n");
		printf("\n\t**************-1--����Ԥ��          ָ��Ԥ��--2-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		scan(&num);
		if (num == 0) {
			system("cls");
			return;
		}
		else if (num == 1 || num == 2) {
			struct COrder* currOrder = headOrder;
			int n1 = -1;
			int state = 0;
			//ѡ����������
			while (1) {
				n1 = -1;
				sampleNum = 0;
				printf("\n\t����[1]�����30������Ϊ����Ԥ�⣬����[2]�����7������Ϊ����Ԥ�� ��");
				scan(&n1);
				if (n1 == 1) {
					state = 1;
					break;
				}
				else if (n1 == 2) {
					state = 2;
					break;
				}
				else printf("\n\t������������������ݶ�Ӧ�ı�ţ����������룡\n");
			}
			
			//*****************��ȡ��������*****************
			struct COrder* pOrder = headOrder;
			struct CGoods* p = headGoods;
			int i = 0, j = 0;
			sampleNum = 0;
			int totalGoods = 0;//��Ʒ����
			//1. ������������
			while (p != NULL) {
				p = p->next;
				totalGoods++;
				if (state == 1)
					sampleNum += 30;
				else if (state == 2)
					sampleNum += 7;
			}

			//2. ��ȡ��Ʒ�����Ϣ�Լ�ʱ����Ϣ
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

			//3. ��ȡָ��ʱ����ָ����Ʒ��������Ϣ
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
							//�ҵ������е���Ʒ���ƶ�Ӧ����Ʒ���
							p = headGoods;
							while (p != NULL && strcmp(p->brand, currOrder->goodsName) != 0) {
								p = p->next;
							}
							float int_time = covert_TM_To_INT(&currOrder->time);
							//�ҵ������ڸ�������ɵ���Ʒ���Ϊx[1][0]�Ķ���
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
							//�ҵ������е���Ʒ���ƶ�Ӧ����Ʒ���
							p = headGoods;
							while (p != NULL && strcmp(p->brand, currOrder->goodsName) != 0) {
								p = p->next;
							}
							float int_time = covert_TM_To_INT(&currOrder->time);
							//�ҵ������ڸ�������ɵ���Ʒ���Ϊx[1][0]�Ķ���
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
			//*****************��ȡ�����������*****************

			//*****************����ع鷽�̹�ʽ�ؾ��б��*****************
			if(num == 1)LinearSalesPrediction(state);
			
			else if(num ==2)LogarithmicSalesPrediction(state);
			//*****************����ع鷽�̹�ʽ�ؾ��б�����*****************


			if(num == 1) printf("\n\t-----------------------------------------------------�� �� Ԥ ��---------------------------------------------\n\n");
			else printf("\n\t-----------------------------------------------------�� �� Ԥ ��---------------------------------------------\n\n");
			printf("\t               ********************-0--����                              ����--0-********************\n\n");
			printf("\t               ********************-1--Ԥ�ⵥ����Ʒ����      Ԥ��������Ʒ����--2-********************\n\n");
			printf("\t               -------------------------------------------------------------------------------------\n\n");


			int n2 = -1;
			while (1) {
				n2 = -1;
				printf("\n\t�����������ţ�");
				scan(&n2);
				if (n2 == 0) {
					system("cls");
					break;
					//
				}
				else if (n2 == 1) {
					//һ . ������ҪԤ���ʱ��**********************************
					struct tm TM_timeOfPrediction;
					int timeOfPrediction[6];
					const int month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // ÿ���µ���������ʱ���������꣩
					while (1) {
						printf("\n\t��������ҪԤ���ʱ�䣬��ʽΪ yyyy mm dd���ֱ��Ӧ�ꡢ�¡��գ���");
						printf("\n\t");
						int i = 0;
						int valid = 0;
						for (i = 0; i < 3; i++) {
							valid = 0;
							while (!valid) {
								if (scanf("%d", &timeOfPrediction[i]) != 1) {
									printf("\t������Ч�����������룺");
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
							printf("\n\t����Ԥ�����֮ǰ��ʱ�䣡���������룡\n");
						}
						else if (CheckTime(timeOfPrediction) == 0) {
							break;
						}
					}
					//��. �����Ϣ**********************************
					printf("\n\t%4d��%2d��%2d�յ���Ʒ����Ԥ�����£�\n", timeOfPrediction[0], timeOfPrediction[1], timeOfPrediction[2]);
					char* arrName[3] = {"��Ʒ���","��Ʒ����", "Ԥ������(ƿ)"};
					printf("\n\t%-10s\t%-30s\t%-12s\n", arrName[0], arrName[1], arrName[2]);
					printf("\t-----------------------------------------------------------");
					p = headGoods;
					while (p != NULL) {
						//��ȡб�ʽؾ������m
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
					printf("\n\t���ܵ���Ʒ����Ԥ�����£�\n");
					char* arrName[3] = { "��Ʒ���","��Ʒ����", "Ԥ������(ƿ)" };
					printf("\n\t%-10s\t%-30s\t%-12s\n", arrName[0], arrName[1], arrName[2]);
					printf("\t-----------------------------------------------------------");
					p = headGoods;
					int hatMoney = 0;
					while (p != NULL) {
						hatMoney = 0;

						time_t now = time(NULL);
						struct tm time_7_day_before = *localtime(&now);

						//��ȡб�ʽؾ������m
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
							//����Ԥ��
							if (num == 1) {
								if ((int)(w0[m] + w1[m] * covert_TM_To_INT(&time_7_day_before) + w2[m] * p->goodsNum) < 0) {
									hatMoney += 0;
								}
								else
								hatMoney += (int)(w0[m] + w1[m] * covert_TM_To_INT(&time_7_day_before) );
								
							}
							//������Ԥ��
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
				else printf("\n\tû������������ݶ�Ӧ�ı�ţ����������룡\n");
			}


			break;
		}
		else printf("\n\t����������������������Ӧ�ı�ţ����������룡\n");
	}
}
//����Ԥ��������


void OtherSetting() {
	
	struct CFullDiscount* pFullDiscount = headFullDiscount;
	struct CFestivalDiscount* pFestivalDiscount = headFestivalDiscount;
	struct COrder* pOrder = headOrder;
	while (1) {
		system("cls");
		pFullDiscount = headFullDiscount;
		pFestivalDiscount = headFestivalDiscount;
		pOrder = headOrder;
		printf("\n\t************************  ��������  ****************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--          ����           --0-*****************\n");
		printf("\n\t**************-1--        ��Ʒ����          --1-****************\n");
		printf("\n\t**************-2--        �ۿ۹���          --2-****************\n");
		printf("\n\t**************-3--        ����Ԥ��          --3-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
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
			printf("\n\t���������������������룡\n");
		}
	}


}
