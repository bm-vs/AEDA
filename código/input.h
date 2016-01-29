#ifndef INPUT_H_
#define INPUT_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include "whatsapl.h"
#include "ui.h"
using namespace std;

extern Database database;
extern int control_var;

// =========================================================================================================================================
// FILES

// USERS
void ReadInputUsers();
void saveUsers();

// GROUPS
void ReadInputGroups();
void saveGroups();

// CONVERSATIONS
void ReadInputConversations();
void saveConversations();

// PENDING MESSAGES
void ReadPendingMessages();
void savePendingMessages();


// =========================================================================================================================================
// CHECK FUNCTIONS

void checkInput();
bool checkDay(const unsigned int & day, const unsigned int & month, const unsigned int & year);


// =========================================================================================================================================
// INPUT - get user parameters

void inputControl(int min, int max);

void inputUsername(string &username);
void inputPassword(string &password);
void inputName(string &name);
void inputEmail(string &email);
void inputCellphone(long &cellphone);
void inputDate(Date &date);

void inputTitle(string &title);

void inputSender(string &sender);
void inputType(string &type);
void inputContent(string &content);
void inputTime(Time &time_sent);


// =========================================================================================================================================
// CHECK USERS - get user parameters for user creation - check if they are valid/already used

void createUsername(string &username, int user_pos);
void createPassword(string &password);
void createEmail(string &email, int user_pos);
void createCellphone(long &cellphone, int user_pos);


// =========================================================================================================================================
// CHECK GROUPS

void createTitle(string &title, int user_pos);


#endif
