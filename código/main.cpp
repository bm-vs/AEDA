#include <iostream>
#include "whatsapl.h"
#include "ui.h"

using namespace std;

Database database;
int control_var;

int main() {

	ReadInputUsers();
	ReadInputGroups();
	ReadInputConversations();
	ReadPendingMessages();

	database.makeUsersInactive();
	database.orderMessages();

	// Main Menu
	start:
	mainMenu();
	switch (control_var) {
		case 1: { // Users
			goto users;
			break;
		}
		case 2: { // Groups
			goto groups;
		}
		case 3: { // Conversations
			goto conversations;
		}
		case 4: { // Save
			goto save;
		}
		case 0: {
			goto exit;
			break;
		}
	}

	// Users menu
	users:
	userMenu();
	switch (control_var) {
		case 1: { // Create user
			createUserMenu();
			goto users;
			break;
		}
		case 2: { // View user
			viewUserMenu();
			goto users;
			break;
		}
		case 3: { // Edit user
			editUserMenu();
			goto users;
			break;
		}
		case 4: { // Delete user
			deleteUserMenu();
			goto users;
			break;
		}
		case 0: { // Back
			goto start;
			break;
		}
	}

	// Groups Menu
	groups:
	groupsMenu();
	switch (control_var){
		case 1:{ // Create Group
			createGroupsMenu();
			goto groups;
		}
		case 2:{ // View Groups
			viewGroupsMenu();
			goto groups;
		}
		case 3:{ // Edit Groups
			editGroupMenu();
			goto groups;
		}
		case 4:{ // Delete Groups
			deleteGroupMenu();
			goto groups;
		}
		case 0: { // Back
			goto start;
			break;
		}
	}

	// Conversations menu
	conversations:
	conversationsMenu();
	switch(control_var) {
		case 1:{ // Create Conversation
			createConversationMenu();
			goto conversations;
			break;
		}
		case 2:{ // View Conversations
			viewConversationsMenu();
			goto conversations;
			break;
		}
		case 3: { // Edit Conversations
			editConversationMenu();
			goto conversations;
			break;
		}
		case 4: { // Delete Conversations
			deleteConversationMenu();
			goto conversations;
			break;
		}
		case 0: { // Back
			goto start;
			break;
		}
	}

	// Save
	save:
	saveUsers();
	saveGroups();
	saveConversations();
	savePendingMessages();
	cout << endl << "Saved" << endl;
	goto start;


	// Exit
	exit:
	saveUsers();
	saveGroups();
	saveConversations();
	savePendingMessages();

}
