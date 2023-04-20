#include"head.h"
#include<stdlib.h>
extern int PrePage();
extern int MainPage();

int main(void) {
	system("color 70");//设置控制台颜色
	LoadAccountData();//加载账户数据
	pLoggedInUser = NULL;//指向当前登录用户的全局指针

	while (1) {
		if (PrePage())
			break;//退出系统
		else {
			if (MainPage() == 0)continue;//退出登录
			else break;//退出系统
		}
	}
	releaseAccount();
	return 0;
}