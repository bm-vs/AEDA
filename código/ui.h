#ifndef UI_H_
#define UI_H_

#include <iostream>
#include <vector>
#include <stack>
#include "whatsapl.h"
#include "input.h"

#define NEW_USER -1
#define NEW_GROUP -1

using namespace std;

extern Database database;
extern int control_var;

// =========================================================================================================================================
// MAIN
void mainMenu();

// =========================================================================================================================================
// USER
void userMenu();
void createUserMenu();
void viewUserMenu();
void editUserMenu();
void deleteUserMenu();

// =========================================================================================================================================
// GROUPS
void groupsMenu();
void createGroupsMenu();
void viewGroupsMenu();
void editGroupMenu();
void deleteGroupMenu();

void selectModMenu(GroupUser * &moderator, vector<GroupUser *> &users);
void addGroupUsersMenu(vector<GroupUser *> &users);
void editGroupUsersMenu(Group *g);
void removeGroupUsersMenu(Group *g);
void banGroupUsersMenu(Group *g);
void unbanGroupUsersMenu(Group *g);
void manageRequestsMenu(Group *g);

// =========================================================================================================================================
// CONVERSATIONS
void conversationsMenu();
void createConversationMenu();
void viewConversationsMenu();
void editConversationMenu();
void deleteConversationMenu();

void addMessageMenu(Conversation *conversation);
void approveMessagesMenu(Conversation *conversation);
void listConversations();

#endif
