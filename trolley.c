#define _CRT_SECURE_NO_WARNINGS
#include "head.h"
#include "trolley.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void GradeChange();
double GetPriceAfterActivaiyDiscountAndFullDiscount(double totalPrice);

void InformationChange(int numPurchaseGoods, int goodsSerialNumber) {
	//goodsSerialNumberΪ��Ʒ��� 
	//numPurchaseGoodsΪ�������Ʒ����

	struct CGoods* pGood = headGoods; struct CGoods* pGood0 = NULL;
	struct CTrolley* pTro = headTrolley; struct CTrolley* pTro0 = NULL;
	struct CAccount* pAccou = headAccount; struct CAccount* pAccou0 = NULL;
	struct COrder* pOrder = headOrder; struct COrder* pOrder0 = NULL;
	//��Ҫ�ı���� �û��������۶�,�û���Ա�ȼ������ﳵ��Ʒ����Ʒ���,��Ʒ����,���������Լ����ܵ���Ʒ����
	while (pTro != NULL) {
		if (pTro->goodsNumber == goodsSerialNumber && strcmp(pTro->userName, pLoggedInUser->userName) == 0) break;
		pTro0 = pTro;
		pTro = pTro->next;
	}
	while (pGood != NULL && pGood->goodsNum != goodsSerialNumber) {
		pGood0 = pGood;
		pGood = pGood->next;
	}

	//�û��ȼ������۸���
	float totalCost = (float)(TotalCostMoney(goodsSerialNumber, pTro));
	pLoggedInUser->totalAmount += totalCost;
	////GradeChange(pLoggedInUser);

	//��Ʒ��桢��������
	pGood->stock -= pTro->trolleyGoodsNumber;
	pGood->sales += pTro->trolleyGoodsNumber;


	//����������Ʒ�����Լ���Ʒ����
	OrderAdd(totalCost, goodsSerialNumber, pTro->trolleyGoodsNumber);

	//�ڹ��ﳵ��ɾ������Ʒ
	GoodsTrolleyDeleteNoRemind(goodsSerialNumber);


	//���޸ĺ�ĸ�����Ϣ�洢���ļ�����ȥ

	SaveGoodsTrolleyData(headTrolley);
	SaveGoodsData(headGoods);
	SaveOrderData(headOrder);
	SaveAccountData(headAccount);

}

//���ﳵ����
void GoodsTrolley() {
	system("cls");
	struct CTrolley* pTro = headTrolley;
	struct CGoods* pGood = NULL;

	while (1)
	{
		system("cls");
		pTro = headTrolley;

		printf("\n\t*******************************************************   ���ﳵ   *********************************************\n\n");
		char* arrName[8] = { "��Ʒ���","��ˮ����","��ˮ���","�ƾ�Ũ��","��ˮ������","��ˮ����(ƿ)","��ˮ����","��ˮ���" };
		printf("\n\t%8s %16s %12s %10s %12s %12s %17s %17s\n\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5], arrName[6], arrName[7]);

		int numTrolleyGoods = 0;//���ڼ�¼���û����ﳵ����Ʒ������

		while (pTro != NULL) {

			//�����˵Ĺ��ﳵ��Ϣ���洢��ͬһ���ļ��У��������û����ҵ���ǰ�û����ﳵ�е�������Ʒ��Ϣ
			if (strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
				//�ҵ��˵�ǰ�û���һ����Ʒ����Ҫ������Ʒ��Ϣ���
				pGood = headGoods;
				int stateFindGoods = 0;//�Ƿ��ܹ����ֽ�������������Ʒ�����ҵ�����Ʒ
				while (pGood != NULL) {
					//������ﳵ����Ʒ��Ϣ
					if (strcmp(pGood->brand, pTro->brand) == 0) {
						stateFindGoods = 1;
						numTrolleyGoods++;
						printf("\n\t%-8d %16s %12s %9d�� %12d %12.2f",
							pGood->goodsNum, pGood->brand, pGood->kind, pGood->concentration, pGood->netContent
							, pGood->price);
						printf("%8d����%04dƿ", pTro->trolleyGoodsNumber / pGood->boxesNumber, pTro->trolleyGoodsNumber % pGood->boxesNumber);
						printf("%8d����%04dƿ\n", pGood->stock / pGood->boxesNumber, pGood->stock % pGood->boxesNumber);
					}
					pGood = pGood->next;
				}
				if (pGood == NULL && stateFindGoods == 0) {
					printf("\n\t���Ĺ��ﳵ������Ϊ\"%s\"����Ʒ�ѱ��¼ܣ���ȴ���Ʒ�����ϼܻ���ɾ������Ʒ��\n\t����0ɾ������Ʒ�������������ּ�����������ﳵ��������Ʒ��", pTro->brand);
					int n1 = 0;
					scan(&n1);
					if (n1 == 0) {
						struct CTrolley* p = pTro->next;
						GoodsTrolleyDeleteNoRemind(pTro->goodsNumber);
						pTro = p;
						numTrolleyGoods--;
					}
					else {

						pTro = pTro->next;
					}
				}
				else pTro = pTro->next;
			}
			else pTro = pTro->next;

		}


		if (numTrolleyGoods == 0) {
			printf("\n\n\t���Ĺ��ﳵ��������Ʒ��\n");
		}
		else {
			printf("\n\n\tĿǰ�����Ĺ��ﳵ�й���%d����Ʒ\n", numTrolleyGoods);
		}


		printf("\n\t----------------------------------------------------------------------------------------------------------------\n");
		printf("\n\t-----------------------------------------------------��  ��  ��  ��---------------------------------------------\n\n");
		printf("\t               ********************-0--����                              ����--0-********************\n\n");
		printf("\t               ********************-1--һ��ɾ����Ʒ               һ��������Ʒ--2-********************\n\n");
		printf("\t               ********************-3--������Ʒ����               ���������Ʒ--3-********************\n\n");
		printf("\t               -------------------------------------------------------------------------------------\n\n");
		printf("\n\t�����������ţ�");

		int n1;
		n1 = -1;
		scan(&n1);

		if (n1 == 0) {
			system("cls");
			return;
		}
		else if (n1 == 1) {
			GoodsTrolleyClear();
			system("cls");
			break;
		}
		else if (n1 == 2) {
			TotalGoodsTrolleyBuy();

		}
		else if (n1 == 3) {
			int n2 = -1;
			while (1) {
				n2 = -1;
				printf("\n\t��������Ʒ���,����[0]�ɷ��أ�");
				scan(&n2);
				if (n2 == 0) {
					system("cls");
					break;
				}
				else if (n2 >= 1 && n2 <= 100000) {
					int state = 0;
					struct CTrolley* p = headTrolley;
					while (p != NULL) {
						if (p->goodsNumber == n2 && strcmp(p->userName, pLoggedInUser->userName) == 0) {
							state = 1;
						}
						p = p->next;
					}
					if (state == 0) {
						printf("\n\t���Ĺ��ﳵ�в���������ı�Ŷ�Ӧ����Ʒ���밴��������������룡");
						getchar();
					}
					else {
						int num0 = -1;
						while (1) {
							num0 = -1;
							printf("\n\t����[1]�µ�������[2]ɾ����Ʒ������[3]�޸���Ʒ����������[0]����,�������������ַ���Ч:");
							scan(&num0);
							if (num0 == 1) {
								GoodsTrolleyBuy(n2);
								break;
							}
							else if (num0 == 2) {
								GoodsTrolleyDelete(n2);
								break;
							}
							else if (num0 == 3) {
								GoodsTrolleyChange(n2);
								break;
							}
							else if (num0 == 0) {
								break;
							}
							else {}
						}
					}
				}
				else {
					printf("\n\t��Ų��Ϲ涨�����������룡\n");
				}
			}

		}
	}
}

/******-----------------------------------------------------------*/
/*���ﳵ����*/
 /******-----------------------------------------------------------*/
//--�����Ʒ�����ﳵ
void GoodsTrolleyAdd(struct CGoods* pNow) {
	struct CTrolley* pTro = NULL;
	while (pTro == NULL) {
		pTro = (struct CTrolley*)malloc(sizeof(struct CTrolley));
	}//��֤ptro����ָ��NULL
	pTro->next = NULL;
	strcpy(pTro->userName, pLoggedInUser->userName);
	strcpy(pTro->brand, pNow->brand);
	pTro->goodsNumber = pNow->goodsNum;
	pTro->boxesNumber = pNow->boxesNumber;
	pTro->trolleyGoodsNumber = 0;

	int numDW = -1;//��λ   ��Ϊ��λ�����Ʒ�����ﳵ������1����Ҫ��ƿΪ��λ�����Ʒ�����ﳵ������2����0�ɷ���
	while (1) {
		numDW = -1;
		printf("\n\t��Ҫ����Ϊ��λ�����Ʒ�����ﳵ������1����Ҫ��ƿΪ��λ�����Ʒ�����ﳵ������2����0�ɷ��أ�");
		scan(&numDW);
		if (numDW == 1 || numDW == 2) {
			break;
		}
		if (numDW == 0) {
			system("cls");
			return;
		}
		else {
			printf("\n\t������Ĵ����������������룡");
		}
	}

	int num = -1;
	while (1) {
		num = -1;
		printf("\n\t��������Ҫ��ӵ����ﳵ����Ʒ���������ڵ���1��С�ڵ���1000��,��0�ɷ��أ�");
		scan(&num);
		if (num >= 1 && num <= 1000) {
			if (numDW == 1) {
				pTro->trolleyGoodsNumber = num * (pNow->boxesNumber);
			}
			else if (numDW == 2) { pTro->trolleyGoodsNumber = num; }
			break;
		}
		if (num == 0) {
			system("cls");
			return;
		}
		else {
			printf("\n\t������Ĵ����������������룡\n");
		}
	}

	//������ﳵ�����и���Ʒ����Ʒ������ԭ���Ļ����ϼ������
	struct CTrolley* p = headTrolley;
	struct CTrolley* p0 = NULL;
	if (p == NULL) {
		headTrolley = pTro;
		pTro->next = NULL;
	}
	else {
		int stateExisted = 0;
		while (p != NULL) {
			if (strcmp(p->brand, pTro->brand) == 0 && strcmp(p->userName, pLoggedInUser->userName) == 0) {
				stateExisted = 1;
				p->trolleyGoodsNumber += pTro->trolleyGoodsNumber;
				break;
			}
			p0 = p;
			p = p->next;
		}
		if (stateExisted == 0) {
			p0->next = pTro;
			pTro->next = NULL;
		}
	}

	SaveGoodsTrolleyData();

	printf("\n\t��ӳɹ����������������\n");
	getchar();
	system("cls");

	return;

}
//--ɾ�����ﳵ�е�ĳ����Ʒ
void GoodsTrolleyDeleteNoRemind(int goodsSerialNumber) {//����Ʒ�ӹ��ﳵ��ɾ��
	struct CTrolley* p = headTrolley;
	struct CTrolley* p0 = NULL;
	while (p != NULL) {
		if (p->goodsNumber == goodsSerialNumber && strcmp(p->userName, pLoggedInUser->userName) == 0) {
			if (p0 == NULL) {
				if (headTrolley == p) {

				}
				headTrolley = headTrolley->next;
				free(p);
				p = headTrolley;
			}
			else {
				p0->next = p->next;
				free(p);
				p = p0->next;
				break;
			}
		}
		else {
			p0 = p;
			p = p->next;
		}
	}
	SaveGoodsTrolleyData(headTrolley);

}
void GoodsTrolleyDelete(int goodsSerialNumber) {//����Ʒ�ӹ��ﳵ��ɾ��
	int num_ = -1;
	while (1) {
		num_ = -1;
		printf("\n\t��ȷ��Ҫɾ������Ʒ�𣿰�[1]ȷ�ϣ���[0]����: ");
		scanf_s("%d", &num_);
		getchar();
		if (num_ == 1) {
			struct CTrolley* p = headTrolley;
			struct CTrolley* p0 = NULL;
			while (p != NULL) {
				if (p->goodsNumber == goodsSerialNumber && strcmp(p->userName, pLoggedInUser->userName) == 0) {
					if (p0 == NULL) {
						headTrolley = headTrolley->next;
						free(p);
						p = headTrolley;
					}
					else {
						p0->next = p->next;
						free(p);
						p = p0->next;
					}
				}
				else {
					p0 = p;
					p = p->next;
				}
			}
			SaveGoodsTrolleyData(headTrolley);
			printf("\n\t�ɹ��Ƴ�����Ʒ��\n");
			break;
		}
		else if (num_ == 0) {
			//releaseTrolley(headTrolley);
			return;
		}
		else {
			printf("\n\t������Ĵ����������������룡\n");
		}
	}

}
//--�޸Ĺ��ﳵ����Ʒ����
void GoodsTrolleyChange(int goodsSerialNumber) {
	//�޸Ĺ��ﳵ�е���Ʒ����

	//�޸���Ʒ��Ϣ����Ҫ˼·��ɾ��ԭ�ڵ㣬���½ڵ����ͷ���루�������޸ĺ����Ʒ�ͻ���ʾ�ڵ�һ�ţ�

	struct CTrolley* pTro = headTrolley;//pTro��¼����Ʒ�ڹ��ﳵ�еĵ�ַ����֮����Ҫ��ɾ������Ʒ

	while (pTro != NULL) {
		if (pTro->goodsNumber == goodsSerialNumber && strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
			break;
		}
		pTro = pTro->next;
	}
	if (pTro == NULL) {
		printf("\n\t���������Ʒ�������\n");
		return;
	}

	//pNew��֮����Ҫ���¼ӵĹ��ﳵ��Ʒ�ڵ�
	struct CTrolley* pNew = (struct CTrolley*)malloc(sizeof(struct CTrolley));
	*pNew = *pTro;

	while (1)
	{
		int indexA = 0;
		printf("\n\t������Ʒ����[1], ������Ʒ����[2], ɾ����Ʒ[3], ����[0]:");
		scan(&indexA);
		if (indexA == 0) {
			system("cls");
			return;
		}
		else if (indexA == 3) {
			GoodsTrolleyDelete(goodsSerialNumber);
			system("cls");
			break;
		}
		else if (indexA == 1) {
			int numXiang = 0;
			int numPing = 0;
			while (1) {
				printf("\n\t��ʾ����ǰ��Ʒ1�京%dƿ��ˮ\n", pTro->boxesNumber);
				printf("\n\t������������Ҫ���ӵ�������ƿ������������10000��������֮���Կո������");

				int i = 0;
				int valid = 0;
				int num[2];
				for (i = 0; i < 2; i++) {
					valid = 0;
					while (!valid) {
						if (scanf("%d", &num[i]) != 1) {
							printf("\t������Ч�����������룺");
							while (getchar() != '\n');
						}
						else {
							valid = 1;

						}
					}
				}

				numXiang = num[0];
				numPing = num[1];
				if (numXiang >= 0 && numXiang <= 10000 && numPing >= 0 && numPing <= 10000) {
					break;
				}
				else if (numXiang > 10000) {
					printf("\n\t�������Ʒ�������࣬���������룡\n");
				}
				else {
					printf("\n\t�������Ʒ������ʽ�������������룡\n");
				}
				if (numPing >= 0 && numPing <= 10000);
				else if (numPing > 10000) {
					printf("\n\t�������Ʒƿ�����࣬���������룡\n");
				}
				else {
					printf("\n\t�������Ʒƿ����ʽ�������������룡\n");
				}
				getchar();
			}
			pNew->trolleyGoodsNumber += (numPing + numXiang * pTro->boxesNumber);

			GoodsTrolleyDeleteNoRemind(goodsSerialNumber);//ɾ������Ʒ
			pNew->next = headTrolley;
			headTrolley = pNew;//�������Ʒ

			SaveGoodsTrolleyData(headTrolley);
			printf("\n\t���Ĺ��ﳵ��ǰ���и���Ʒ%d��%dƿ������������ء�\n", pNew->trolleyGoodsNumber / pNew->boxesNumber, pNew->trolleyGoodsNumber % pNew->boxesNumber);
			getchar();
			getchar();
			system("cls");
			return;
		}
		else if (indexA == 2) {
			int numXiang = 0;
			int numPing = 0;
			while (1) {
				printf("\n\t��ʾ����ǰ��Ʒ1�京%dƿ��ˮ\n", pTro->boxesNumber);
				printf("\n\t������������Ҫ���ٵ�������ƿ������������10000��������֮���Կո������");

				int i = 0;
				int valid = 0;
				int num[2];
				for (i = 0; i < 2; i++) {
					valid = 0;
					while (!valid) {
						if (scanf("%d", &num[i]) != 1) {
							printf("\t������Ч�����������룺");
							while (getchar() != '\n');
						}
						else {
							valid = 1;

						}
					}
				}

				numXiang = num[0];
				numPing = num[1];
				if (numXiang >= 0 && numXiang <= 100000 && numPing >= 0 && numPing <= 100000) {
					break;
				}
				else if (numXiang > 100000) {
					printf("\n\t�������Ʒ�������࣬���������룡\n");
				}
				else {
					printf("\n\t�������Ʒ������ʽ�������������룡\n");
				}
				if (numPing >= 0 && numPing <= 100000);
				else if (numPing > 100000) {
					printf("\n\t�������Ʒƿ�����࣬���������룡\n");
				}
				else {
					printf("\n\t�������Ʒƿ����ʽ�������������룡\n");
				}
			}
			if (pNew->trolleyGoodsNumber >= (numPing + numXiang * pTro->boxesNumber)) {
				pNew->trolleyGoodsNumber -= (numPing + numXiang * pTro->boxesNumber);
			}
			else {
				int n2 = 0;
				while (1) {
					printf("\n\t���Ĺ��ﳵ�и���Ʒ����С������Ҫ���ٵ����������� 1 ��ɾ������Ʒ������ 0 ��ȡ��������Ʒ�����Ĳ���������:");
					scan(&n2);
					if (n2 == 1) {
						GoodsTrolleyDeleteNoRemind(goodsSerialNumber);
						//system("cls");
						return;
					}
					else if (n2 == 0) {
						//system("cls");
						return;
					}
					else {
						printf("\n\t����������������룡\n");
					}
				}
			}

			GoodsTrolleyDeleteNoRemind(goodsSerialNumber);//ɾ������Ʒ
			pNew->next = headTrolley;
			headTrolley = pNew;//�������Ʒ

			SaveGoodsTrolleyData(headTrolley);
			printf("\n\t���Ĺ��ﳵ��ǰ���и���Ʒ%d��%dƿ������������ء�\n", pNew->trolleyGoodsNumber / pNew->boxesNumber, pNew->trolleyGoodsNumber % pNew->boxesNumber);
			getchar();
			getchar();
			//	system("cls");
			return;
		}
	}
}
//--������ﳵ��������Ʒ
void GoodsTrolleyClear() {//��չ��ﳵ
	//LoadTrolleyData();
	struct CTrolley* pTro = headTrolley;
	struct CTrolley* pTro_ = NULL;//pTro��ǰ�ýڵ�
	while (pTro != NULL) {
		pTro_ = pTro;
		pTro = pTro->next;
		if (strcmp(pTro_->userName, pLoggedInUser->userName) == 0) {
			GoodsTrolleyDeleteNoRemind(pTro_->goodsNumber);
		}
	}
	SaveGoodsTrolleyData(headTrolley);
	printf("\n\t��ɾ�����Ĺ��ﳵ��������Ʒ�����������ַ����أ�\n");
	getchar();
}

/******-----------------------------------------------------------*/
/*�µ�*/
/******-----------------------------------------------------------*/
//--�����ﳵ��ĳ����Ʒ


void GoodsTrolleyBuy(int goodsSerialNumber) {//�����ﳵ�е���Ʒ
	//LoadTrolleyData();
	struct CTrolley* pTro = headTrolley;
	struct CGoods* pGood = headGoods;
	while (pTro != NULL) {
		if (pTro->goodsNumber == goodsSerialNumber && strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
			pGood = headGoods;
			while (pGood != NULL) {
				if (pGood->goodsNum == goodsSerialNumber) {
					if (pGood->stock < pTro->trolleyGoodsNumber) {
						printf("\n\t���Ϊ%d����Ʒ��治�㣡����Ĺ��ﳵ��Ʒ���������µ�������0�ɷ��أ�����������������ǰ��������Ʒ����", pTro->goodsNumber);
						int n1;
						scan(&n1);
						if (n1 == 0) return;
						else {
							GoodsTrolleyChange(goodsSerialNumber);
							return;
						}
					}
					else {
						double cost = TotalCostMoney(goodsSerialNumber, pTro);
						printf("\n\t��ʾ����ǰ��Ʒ1�京%dƿ��ˮ\n", pTro->boxesNumber);
						printf("\n\t�������Ʒ���軨�� %.2lf Ԫ��ȷ�Ϲ�����\n", cost);
						int num = -1;
						while (1) {
							num = -1;
							printf("\n\t����1ȷ�Ϲ�������0����:");
							scan(&num);
							if (num == 0) {
								return;
							}
							else if (num == 1) {
								if (ModeOfPayment() == 0) {
									printf("\n\t֧��ȡ�������������ַ��ɷ��ء�\n");
									printf("\n\t");
									getchar();
									return;
								}
								else {
									InformationChange(pTro->trolleyGoodsNumber, pTro->goodsNumber);//��������Ʒ������ɵ���Ϣ�ı�
									printf("\n\t���ѳɹ��������Ʒ��\n");
									printf("\n\t");
									getchar();
									return;
								}
							}
							else {
								printf("\n\t������Ĵ����������������룡\n");
							}
						}
					}
				}
				pGood = pGood->next;
			}
		}
		pTro = pTro->next;
		if (pGood == NULL) {
			printf("\n\t����Ʒ���¼ܣ�\n");
			GoodsTrolleyDelete(pTro->goodsNumber);
			return;
		}
	}
}
void GoodsTrolleyBuyNoRemind(struct CTrolley* pTro) {
	struct CGoods* pGood = headGoods;
	while (pGood != NULL && (pGood->goodsNum != pTro->goodsNumber || pGood->stock < pTro->trolleyGoodsNumber)) {
		pGood = pGood->next;
	}
	if (pGood == NULL);//����Ʒ�ѱ��¼�
	else {
		InformationChange(pTro->trolleyGoodsNumber, pGood->goodsNum);
	}

}
//--�����ﳵ��������Ʒ
void TotalGoodsTrolleyBuy() {
	double cost = 0;
	struct CTrolley* pTro = headTrolley;
	while (pTro != NULL) {
		if (strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
			AllGoodsTotalCost(pTro->goodsNumber, &cost);
		}
		pTro = pTro->next;
	}
	if (fabs(cost) < 1e-6) {
		printf("\n\t���Ĺ��ﳵ�����޿���㹻����Ʒ������������ء�\n");
		getchar();
		return;
	}

	printf("\n\t�����ﳵ�����п��������Ʒ���軨��%.2lfԪ\n", cost);
	printf("\n\t����[1]ȷ�Ϲ�������[0]ȡ������:");
	int n1 = -1;
	while (1) {
		n1 = -1;
		scan(&n1);
		if (n1 == 1) {
			if (ModeOfPayment() == 0) {
				printf("\n\t����ȡ������\n");
				getchar();
				return;
			}
			else {
				printf("\n\t����ɹ���\n");

				pTro = headTrolley;
				struct CTrolley* pTro_ = NULL;
				while (pTro != NULL) {
					pTro_ = pTro;
					pTro = pTro->next;
					if (strcmp(pTro_->userName, pLoggedInUser->userName) == 0) {
						GoodsTrolleyBuyNoRemind(pTro_);
					}
				}

				if (strcmp(pTro_->userName, pLoggedInUser->userName) == 0) {
					GoodsTrolleyBuyNoRemind(pTro_);
				}

				SaveGoodsTrolleyData();
				getchar();

				return;
			}
		}
		else if (n1 == 0) {
			return;
		}
		else {
			printf("\t�����������������룡\n");
		}
	}
}
//֧�������֧����ʽ
//--���㹺�ﳵ��ĳ����Ʒ�����ܽ��
double TotalCostMoney(int goodsSerialNumber, struct CTrolley* pTro) {

	struct CGoods* pGood = headGoods;
	while (pGood != NULL && pGood->goodsNum != goodsSerialNumber) {
		pGood = pGood->next;
	}
	double allMoney = -1;
	allMoney = pLoggedInUser->discount * 0.1 * GetPriceAfterActivaiyDiscountAndFullDiscount(pGood->price * pTro->trolleyGoodsNumber * pLoggedInUser->discount / 10);

	return allMoney;
}
void AllGoodsTotalCost(int goodsSerialNumber, double* totalCost) {//�����ﳵ�е���Ʒ
	struct CTrolley* pTro = headTrolley;
	struct CGoods* pGood = headGoods;
	while (pTro != NULL) {
		if (pTro->goodsNumber == goodsSerialNumber && strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
			pGood = headGoods;
			while (pGood != NULL) {
				if (pGood->goodsNum == goodsSerialNumber) {
					if (pGood->stock < pTro->trolleyGoodsNumber) {}
					else {
						*totalCost += TotalCostMoney(goodsSerialNumber, pTro);
					}
				}
				pGood = pGood->next;
			}
		}
		pTro = pTro->next;
	}
}
//--ѡ��֧����ʽ
int ModeOfPayment() {
	int num_ = -1;
	while (1) {
		printf("\n\t��ѡ�����ĸ��ʽ ����1ʹ��֧����֧��������2ʹ��΢��֧��,����0���أ�");
		num_ = -1;
		scan(&num_);
		if (num_ == 0) {
			return 0;
		}
		if (num_ == 1) {
			printf("\n\tʹ��֧��������ɹ���\n");
			return 1;
		}
		if (num_ == 2) {
			printf("\n\tʹ��΢�Ÿ���ɹ���\n");
			return 1;
		}
		else {
			printf("\n\t������Ĵ����������������룡\n");
		}
	}
}