#include"head.h"
#include<stdlib.h>
extern int PrePage();
extern int MainPage();

int main(void) {
	system("color 70");//���ÿ���̨��ɫ
	LoadAccountData();//�����˻�����
	pLoggedInUser = NULL;//ָ��ǰ��¼�û���ȫ��ָ��

	while (1) {
		if (PrePage())
			break;//�˳�ϵͳ
		else {
			if (MainPage() == 0)continue;//�˳���¼
			else break;//�˳�ϵͳ
		}
	}
	releaseAccount();
	return 0;
}