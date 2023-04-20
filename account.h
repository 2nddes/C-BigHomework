#ifndef HEAD
#define HEAD

extern struct CAccount* pLoggedInUser;
extern struct CAccount *headAccount;
extern struct CAccount* tailAccount;
extern struct CAccount *pLoggedInUser;
extern struct CGrade* headGrade;
extern struct CGrade* tailGrade;

void AccountFindAndChange();
void LoadAccountData();
void PassWordChange();
void SecureWordChange();
void UserNameFind();
void GradeSet();
void GradeChange();
void PositionChange();
void PhoneNumberChange();

#endif