#include "ui.h"

using namespace std;

// =========================================================================================================================================
// MAIN

void mainMenu() {
	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "WHATSAPL" << endl << endl;
	cout << "1 - Users" << endl;
	cout << "2 - Groups" << endl;
	cout << "3 - Conversations" << endl;
	cout << "4 - Save" << endl << endl;

	cout << "0 - Save and Exit" << endl;
	inputControl(0, 4);
}

// =========================================================================================================================================
// USER

void userMenu() {
	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "USERS" << endl << endl;
	cout << "1 - Create user" << endl;
	cout << "2 - View users" << endl;
	cout << "3 - Edit user" << endl;
	cout << "4 - Delete user" << endl << endl;
	cout << "0 - Back" << endl;

	inputControl(0, 4);
}

void createUserMenu() {
	string username, password, email, name;
	long cellphone;
	Date join_date;

	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "CREATE USER" << endl;

	// Username
	createUsername(username, NEW_USER);

	// Password
	createPassword(password);

	// Name
	inputName(name);

	// Email
	createEmail(email, NEW_USER);

	// Cellphone
	createCellphone(cellphone, NEW_USER);

	// Date
	cout << "Join date" << endl;
	cin.ignore(256, '\n');
	inputDate(join_date);

	// Create user
	User *new_user = new User (username, password, name, email, cellphone, join_date);

	// Add user to database
	database.addUser(new_user);
	cout << endl << "User created" << endl;
}

void viewUserMenu() {
	start:
	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "VIEW USERS" << endl << endl;

	cout << "1 - Active users" << endl;
	cout << "2 - Most active users" << endl;
	cout << "3 - Inactive users" << endl << endl;
	cout << "0 - Back" << endl;
	inputControl(0, 3);

	if (control_var == 0) {
		return;
	}

	// Print active users
	if (control_var == 1) {
		for (unsigned int i = 0; i < database.users.size(); i++) {
			database.users[i]->print();
			cout << endl;
		}

		cout << "0 - Back" << endl;
		inputControl(0, 0);
		goto start;
	}
	// Print most active users
	if (control_var == 2) {
		cout << endl;
		database.printFavUsers();
		cout << endl << "0 - Back" << endl;
		inputControl(0, 0);
		goto start;
	}
	// Print inactive users
	if (control_var == 3) {
		tr1::unordered_set<User *, hstr, eqstr>::iterator it;
		for (it = database.inactive_users.begin(); it != database.inactive_users.end(); it++) {
			(*it)->print();
			cout << endl;
		}

		cout << "0 - Back" << endl;
		inputControl(0, 0);
		goto start;
	}
}

void editUserMenu() {
	int user_pos;
	string username, password, name, email;
	long cellphone;
	Date join_date;

	// Select user to edit
	select:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "SELECT USER TO EDIT" << endl << endl;

		cout << "1 - Search username" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 1);

		switch (control_var) {
			case 1: {
				cout << endl << endl;
				inputUsername(username);
				break;
			}
			case 0: {
				return;
				break;
			}
		}

	edit:
		// User position in active users vector
		user_pos = userSearch(username);

		// Search user in inactive users if not found in active
		tr1::unordered_set<User *, hstr, eqstr>::iterator it;
		User *temp = new User(username,"","","",0,Date(0,0,0));

		if (user_pos == -1) {
			it = database.inactive_users.find(temp);

			if (it == database.inactive_users.end()) {
				delete temp;
				cout << "User not found" << endl;
				goto select;
			}
			else {
				temp = *it;
			}
		}
		else {
			delete temp;
		}

		// Edit user
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "EDIT USER" << endl << endl;

		cout << "1 - Username" << endl;
		cout << "2 - Password" << endl;
		cout << "3 - Name" << endl;
		cout << "4 - Email" << endl;
		cout << "5 - Cellphone" << endl;
		cout << "6 - Date joined" << endl;
		if (user_pos == -1) {
			cout << "7 - Make active" << endl << endl;
		}
		else {
			cout << "7 - Make inactive" << endl << endl;
		}

		cout << "0 - Back" << endl;

		inputControl(0, 7);

		cout << endl << endl;
		switch (control_var) {
			case 1: { // Username
				createUsername(username, user_pos);
				if (user_pos != -1) {
					database.users[user_pos]->setUsername(username);
				}
				else {
					database.inactive_users.erase(it);
					temp->setUsername(username);
					database.inactive_users.insert(temp);
				}
				goto edit;
				break;
			}
			case 2: { // Password
				createPassword(password);
				if (user_pos != -1) {
					database.users[user_pos]->setPassword(password);
				}
				else {
					database.inactive_users.erase(it);
					temp->setPassword(password);
					database.inactive_users.insert(temp);
				}
				goto edit;
				break;
			}
			case 3: { // Name
				inputName(name);
				if (user_pos != -1) {
					database.users[user_pos]->setName(name);
				}
				else {
					database.inactive_users.erase(it);
					temp->setName(name);
					database.inactive_users.insert(temp);
				}
				goto edit;
				break;
			}
			case 4: { // Email
				createEmail(email, user_pos);
				if (user_pos != -1) {
					database.users[user_pos]->setEmail(email);
				}
				else {
					database.inactive_users.erase(it);
					temp->setEmail(email);
					database.inactive_users.insert(temp);
				}
				goto edit;
				break;
			}
			case 5: { // Cellphone
				createCellphone(cellphone, user_pos);
				if (user_pos != -1) {
					database.users[user_pos]->setCellphone(cellphone);
				}
				else {
					database.inactive_users.erase(it);
					temp->setCellphone(cellphone);
					database.inactive_users.insert(temp);
				}
				goto edit;
				break;
			}
			case 6: { // Date
				cout << "Join date" << endl;
				cin.ignore(256, '\n');
				inputDate(join_date);
				if (user_pos != -1) {
					database.users[user_pos]->setJoinDate(join_date);
				}
				else {
					database.inactive_users.erase(it);
					temp->setJoinDate(join_date);
					database.inactive_users.insert(temp);
				}
				goto edit;
				break;
			}
			case 7: {
				// Make active
				if (user_pos == -1) {
					database.inactive_users.erase(it);
					database.users.push_back(temp);
				}

				// Make inactive
				else {
					temp = database.users[user_pos];
					database.users.erase(database.users.begin() + user_pos);
					database.inactive_users.insert(temp);
				}

				goto edit;
				break;
			}
			case 0: {
				return;
				break;
			}
		}
}

void deleteUserMenu() {
	string delete_user;
	string username;
	int user_pos;

	// Select user to delete
	select:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "SELECT USER TO DELETE" << endl << endl;

		cout << "1 - Search username" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 1);

		switch (control_var) {
			case 1: {
				cout << endl << endl;
				inputUsername(username);
				break;
			}
			case 0: {
				return;
				break;
			}
		}

		// User position in active users vector
		user_pos = userSearch(username);

		// Search user in inactive users if not found in active
		tr1::unordered_set<User *, hstr, eqstr>::iterator it;
		User *temp = new User(username,"","","",0,Date(0,0,0));

		if (user_pos == -1) {
			it = database.inactive_users.find(temp);

			if (it == database.inactive_users.end()) {
				delete temp;
				cout << "User not found" << endl;
				goto select;
			}
			else {
				temp = *it;
			}
		}
		else {
			delete temp;
		}

		// Delete user from database
		cout << endl;
		if (user_pos == -1) {
			temp->print();
		}
		else {
			database.users[user_pos]->print();
		}
		cout << endl << "Delete user?" << endl;

		cout << "1 - Yes" << endl;
		cout << "2 - No" << endl;
		inputControl(1, 2);

		if (control_var == 1) {
			if (user_pos == -1) {
				database.addUser(temp);
				database.inactive_users.erase(it);
				database.deleteUser(database.users.size() - 1);
			}
			else {
				database.deleteUser(user_pos);
			}
			cout << endl << "User deleted" << endl;
		}
}


// =========================================================================================================================================
// GROUPS

void groupsMenu(){
	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "GROUPS" << endl << endl;
	cout << "1 - Create group" << endl;
	cout << "2 - View groups" << endl;
	cout << "3 - Edit group" << endl;
	cout << "4 - Delete groups" << endl << endl;
	cout << "0 - Back" << endl;

	inputControl(0, 4);
}

void createGroupsMenu() {
	string title;
	GroupUser *moderator;
	vector<GroupUser *> users;
	Date creation_date;

	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "CREATE GROUP" << endl << endl;

	// Create title
	createTitle(title, NEW_GROUP);

	// Set creation date
	cout << endl << endl;
	cout << "Group creation date" << endl;
	inputDate(creation_date);

	// Choose moderator
	try {
		selectModMenu(moderator, users);
	}
	catch (ModNotSet&){
		return;
	}

	// Add users
	addGroupUsersMenu(users);

	Group *new_group = new Group(title, moderator, users, creation_date);

	database.addGroup(new_group);
	cout << endl << "Group created" << endl;
}

void viewGroupsMenu(){
	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "VIEW GROUPS" << endl << endl;

	for(unsigned int i = 0; i < database.groups.size(); i++){
		database.groups[i]->print();
		cout << endl;
	}

	cout << "0 - Back" << endl;
	inputControl(0, 0);
}

void editGroupMenu() {
	int group_vec_pos, mod_pos;
	string title, username;
	Date creation_date;
	GroupUser *moderator;
	vector<GroupUser> users;


	start:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "EDIT GROUPS" << endl << endl;

		cout << "1 - Search title" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 1);

		switch(control_var) {
			case 1: {
				cout << endl << endl;
				cin.ignore(256, '\n');
				inputTitle(title);
				group_vec_pos = groupSearch(title);
				break;
			}
			case 0: {
				return;
				break;
			}
		}

		if (group_vec_pos == -1) {
			cout << "Group not found" << endl;
			goto start;
		}

	edit:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "EDIT GROUPS" << endl << endl;

		cout << "1 - Title" << endl;
		cout << "2 - Moderator" << endl;
		cout << "3 - Users" << endl;
		cout << "4 - Creation date" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 4);

		switch (control_var) {
			case 1: { // Title
				cout << endl << endl;
				createTitle(title, group_vec_pos);
				database.groups[group_vec_pos]->setTitle(title);
				goto edit;
				break;
			}
			case 2: { // Moderator
				try { // the mod not being set is only a problem the group isnt yet created
					selectModMenu(moderator, database.groups[group_vec_pos]->users);
				}
				catch (ModNotSet&) {
				}
				if (control_var == 1) {
					database.groups[group_vec_pos]->setModerator(moderator);
					mod_pos = groupUserSearch(database.groups[group_vec_pos]->users, moderator);
					swap(database.groups[group_vec_pos]->users[0], database.groups[group_vec_pos]->users[mod_pos]);
				}
				goto edit;
				break;
			}
			case 3: { // Users
				// If after removing or banning members the delete the group if it has no members
				try {
					editGroupUsersMenu(database.groups[group_vec_pos]);
				}
				catch(ModNotSet&) {
					// Do nothing - the mod not being set is only a problem if creating the group
				}
				catch(NoUsersInGroup&) {
					database.deleteGroup(group_vec_pos);
				}
				goto edit;
				break;
			}
			case 4: { // Creation date
				cout << endl << endl;
				cout << "Group creation date" << endl;
				cin.ignore(256, '\n');
				inputDate(creation_date);
				database.groups[group_vec_pos]->setDate(creation_date);
				goto edit;
				break;
			}
			case 0: {
				goto start;
				break;
			}
		}
}

void deleteGroupMenu() {
	string title;
	int group_pos;

	select:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "SELECT GROUP" << endl << endl;

		cout << "1 - Search title" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 1);

		switch (control_var) {
			case 1: {
				cin.ignore(256, '\n');
				cout << endl << endl;
				inputTitle(title);
				group_pos = groupSearch(title);
				break;
			}
			case 0: {
				return;
				break;
			}
		}

		if (group_pos == -1) {
			cout << "Group not found" << endl;
			goto select;
		}

		// Delete group from database
		cout << endl;
		database.groups[group_pos]->print();
		cout << endl << "Delete group?" << endl;

		cout << "1 - Yes" << endl;
		cout << "2 - No" << endl;
		inputControl(1, 2);

		if (control_var == 1) {
			database.deleteGroup(group_pos);
			cout << "Group deleted" << endl;
		}
}



void editGroupUsersMenu(Group *g) {
	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "EDIT GROUP USERS" << endl << endl;

	cout << "1 - Add" << endl;
	cout << "2 - Remove" << endl;
	cout << "3 - Ban" << endl;
	cout << "4 - Unban" << endl;
	cout << "5 - Manage join requests" << endl << endl;

	cout << "0 - Back" << endl;

	inputControl(0, 5);

	switch(control_var) {
		case 1: {
			addGroupUsersMenu(g->users);
			break;
		}
		case 2: {
			try {
				removeGroupUsersMenu(g);
			}
			catch (NoUsersInGroup&) {
				throw NoUsersInGroup();
			}
			break;
		}
		case 3: {
			try {
				banGroupUsersMenu(g);
			}
			catch (NoUsersInGroup&) {
				throw NoUsersInGroup();
			}

			break;
		}
		case 4: {
			unbanGroupUsersMenu(g);
			break;
		}
		case 5: {
			manageRequestsMenu(g);
			break;
		}
	}

	return;
}

void selectModMenu(GroupUser * &moderator, vector<GroupUser *> &users) {
	GroupUser *user = new GroupUser;
	Date date;
	int user_pos;
	string username;

	start:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "SELECT GROUP MODERATOR" << endl << endl;

		cout << "1 - Search username" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 1);

		switch (control_var) {
			case 1: {
				if (cin.fail()) {
					cin.ignore();
				}
				cout << endl << endl;
				cout << "Username: ";
				cin >> username;
				break;
			}
			case 0: {
				delete user;
				throw ModNotSet();
				break;
			}
		}

	// User position in active users vector
	user_pos = userSearch(username);

	// Search user in inactive users if not found in active
	tr1::unordered_set<User *, hstr, eqstr>::iterator it;
	User *temp = new User(username,"","","",0,Date(0,0,0));

	if (user_pos == -1) {
		it = database.inactive_users.find(temp);

		if (it == database.inactive_users.end()) {
			delete temp;
			cout << "User not found" << endl;
			goto start;
		}
		else {
			temp = *it;
		}
	}
	else {
		delete temp;
	}

	// Active user
	if (user_pos != -1) {
		user->user = database.users[user_pos];
	}
	// Inactive user
	else {
		user->user = temp;
	}

	user_pos = groupUserSearch(users, user);

	if (user_pos == -1) {// User not already in group
		cout << "Group join date" << endl;
		cin.ignore(256, '\n');
		inputDate(date);
		user->group_join_date = date;
		user->bans = 0;
		users.push_back(user);
		moderator = user;
	}
	else {
		moderator = users[user_pos];
	}

	return;
}

void addGroupUsersMenu(vector<GroupUser *> &users) {
	start:
		GroupUser *user = new GroupUser;
		int user_pos;
		string username;

		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "ADD GROUP USERS" << endl << endl;

		cout << "1 - Search username" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 2);

		switch (control_var) {
			case 1: {
				cout << endl << endl;
				inputUsername(username);
				break;
			}
			case 0: {
				delete user;
				return;
				break;
			}
		}

		// User position in active users vector
		user_pos = userSearch(username);

		// Search user in inactive users if not found in active
		tr1::unordered_set<User *, hstr, eqstr>::iterator it;
		User *temp = new User(username,"","","",0,Date(0,0,0));

		if (user_pos == -1) {
			it = database.inactive_users.find(temp);

			if (it == database.inactive_users.end()) {
				delete temp;
				cout << "User not found" << endl;
				goto start;
			}
			else {
				temp = *it;
			}
		}
		else {
			delete temp;
		}

		// Active user
		if (user_pos != -1) {
			user->user = database.users[user_pos];
		}
		// Inactive user
		else {
			user->user = temp;
		}

		user_pos = groupUserSearch(users, user);

		if (user_pos != -1) {// User not already in group
			cout << "User already in group" << endl;
		}
		else {
			Date date;
			cin.ignore(256, '\n');
			cout << "Group join date" << endl;
			inputDate(date);

			user->group_join_date = date;
			user->bans = 0;

			users.push_back(user);
		}

		goto start;
}

void removeGroupUsersMenu(Group *g) {
	start:
		int user_pos;
		string username;

		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "REMOVE GROUP USERS" << endl << endl;

		cout << "1 - Search username" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 1);

		switch (control_var) {
			case 1: {
				cout << endl << endl;
				inputUsername(username);
				user_pos = groupUserSearch(g->users, username);
				if (user_pos == -1) {
					cout << "User not found" << endl;
					goto start;
				}
				break;
			}
			case 0: {
				return;
				break;
			}
		}

		User *u = g->users[user_pos]->user;

		// Remove user from group
		cout << endl << "Remove user?" << endl;

		cout << "1 - Yes" << endl;
		cout << "2 - No" << endl;
		inputControl(1, 2);

		if (control_var == 1) {
			// Remove user from group
			g->removeUser(u);

			// If group is left with no users delete it
			if (g->users.empty()) {
				throw NoUsersInGroup();
			}
		}
}

void banGroupUsersMenu(Group *g) {
	string username;
	int user_pos;

	start:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "BAN GROUP USERS" << endl << endl;

		cout << "1 - Search username" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 1);

		if (control_var == 0) {
			return;
		}

		cout << endl << endl;
		inputUsername(username);
		user_pos = groupUserSearch(g->users, username);
		if (user_pos == -1) {
			cout << "User not found" << endl;
			goto start;
		}


		// Ban user from group
		cout << endl << "Ban user?" << endl;

		cout << "1 - Yes" << endl;
		cout << "2 - No" << endl;
		inputControl(1, 2);

		if (control_var == 1) {
			g->users[user_pos]->bans++;


			// Ban user
			g->banUser(g->users[user_pos]);

			// Remove user from group
			g->removeUser(g->users[user_pos]->user);

			// If group is left with no users delete it
			if (g->users.empty()) {
				throw NoUsersInGroup();
			}
		}
}

void unbanGroupUsersMenu(Group *g) {
	string username;
	int user_pos;

	start:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "UNBAN GROUP USER" << endl << endl;

		cout << "1 - Search username" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 1);

		if (control_var == 0) {
			return;
		}

		cout << endl << endl;
		inputUsername(username);
		user_pos = groupUserSearch(g->banned_users, username);
		if (user_pos == -1) {
			cout << "User not found" << endl;
			goto start;
		}

		// Add user to group
		g->addUser(g->banned_users[user_pos]);

		// Unban user
		g->unbanUser(user_pos);
}

void manageRequestsMenu(Group *g) {
	string username;
	int requested_user_pos;
	Date d;

	start:
		GroupUser *u = new GroupUser;

		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "MANAGE JOIN REQUESTS" << endl << endl;

		cout << "Users:" << endl;
		for (unsigned int i = 0; i < g->requested_users.size(); i++) {
			cout << i + 1 << " - " << g->requested_users[i]->getUsername() << endl;
		}

		cout << endl << "0 - Back" << endl;

		inputControl(0, g->requested_users.size());
		requested_user_pos = control_var - 1;

		if (control_var == 0) {
			delete u;
			return;
		}
		else {
			u->user = g->requested_users[requested_user_pos];

			cout << endl << endl;
			cout << "1 - Accept" << endl;
			cout << "2 - Decline" << endl << endl;
			cout << "0 - Back" << endl;

			inputControl(0, 2);

			switch(control_var) {
				case 1: { // Add user to group users, erase from requested users

					// Check if user is already in group users
					for (unsigned int i = 0; i < g->users.size(); i++) {
						if (g->users[i]->user == u->user) {
							cout << "User already in group" << endl;
							goto start;
						}
					}

					cout << endl << endl;
					cout << "Group join date" << endl;
					inputDate(d);
					u->group_join_date = d;
					u->bans = 0;

					g->addUser(u);

					g->requested_users.erase(g->requested_users.begin() + requested_user_pos);
					goto start;
					break;
				}
				case 2: { // Just erase from requested users
					g->requested_users.erase(g->requested_users.begin() + requested_user_pos);
					delete u;
					goto start;
					break;
				}
				case 0: {
					return;
					break;
				}
			}
		}
}



// =========================================================================================================================================
// CONVERSATIONS

void conversationsMenu(){
	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "CONVERSATIONS" << endl << endl;

	cout << "1 - Create conversation" << endl;
	cout << "2 - View conversations" << endl;
	cout << "3 - Edit conversation" << endl;
	cout << "4 - Delete conversation" << endl << endl;
	cout << "0 - Back" << endl;

	inputControl(0, 4);
}

void createConversationMenu() {
	Conversation *conversation;
	string title, username1, username2;
	int user1_vec_pos, user2_vec_pos;
	tr1::unordered_set<User *, hstr, eqstr>::iterator it1, it2;
	User *temp1;
	User *temp2;
	int group_vec_pos;

	start:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "CREATE CONVERSATION" << endl << endl;

		cout << "1 - Group" << endl;
		cout << "2 - Private" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 2);

		switch(control_var) {
			case 1: {
				cin.ignore(256, '\n');

				cout << endl << endl;
				inputTitle(title);
				group_vec_pos = groupSearch(title);
				if (group_vec_pos == -1) {
					cout << "Group not found" << endl;
					goto start;
				}

				conversation = new Conversation(database.groups[group_vec_pos]);

				// Check if conversation already exists
				for (unsigned int i = 0; i < database.conversations.size(); i++) {
						if (conversation->group == database.conversations[i]->group) {
							cout << "Conversation already exists" << endl;
							delete conversation;
							goto start;
						}
				}

				break;
			}
			case 2: {
				bool valid = false;
				do {
					cout << endl << endl;
					cout << "User 1" << endl;
					inputUsername(username1);

					temp1 = new User(username1,"","","",0,Date(0,0,0));

					// User position in active users vector
					user1_vec_pos = userSearch(username1);


					// Search user in inactive users if not found in active
					if (user1_vec_pos == -1) {
						it1 = database.inactive_users.find(temp1);

						if (it1 == database.inactive_users.end()) {
							delete temp1;
							cout << "User not found" << endl;
						}
						else {
							temp1 = *it1;
							valid = true;
						}
					}
					else {
						delete temp1;
					}

				} while (user1_vec_pos == -1 && !valid);

				do {
					cout << endl;
					cout << "User 2" << endl;
					inputUsername(username2);

					temp2 = new User(username2,"","","",0,Date(0,0,0));

					// User position in active users vector
					user2_vec_pos = userSearch(username2);

					// Search user in inactive users if not found in active
					if (user2_vec_pos == -1) {
						it2 = database.inactive_users.find(temp2);

						if (it2 == database.inactive_users.end()) {
							delete temp2;
							cout << "User not found" << endl;
						}
						else {
							temp2 = *it2;
							valid = true;
						}
					}
					else {
						delete temp2;
					}

				} while (user2_vec_pos == -1);

				if (user1_vec_pos == -1 && user2_vec_pos == -1) {
					conversation = new Conversation(*it1, *it2);
				}
				else if (user1_vec_pos == -1) {
					conversation = new Conversation(*it1, database.users[user2_vec_pos]);
				}
				else if (user2_vec_pos == -1) {
					conversation = new Conversation(database.users[user1_vec_pos], *it2);
				}
				else {
					conversation = new Conversation(database.users[user1_vec_pos], database.users[user2_vec_pos]);
				}


				// Check if conversation already exists
				for (unsigned int i = 0; i < database.conversations.size(); i++) {
					if (database.conversations[i]->group == NULL) {
						if ((database.conversations[i]->users[0]->getUsername() == username1 &&
										database.conversations[i]->users[1]->getUsername() == username2) ||
										(database.conversations[i]->users[1]->getUsername() == username1 &&
												database.conversations[i]->users[0]->getUsername() == username2)) {
							cout << "Conversation already exists" << endl;
							delete conversation;
							goto start;
						}
					}
				}

				break;
			}
			case 0: {
				return;
			}
		}


	database.addConversation(conversation);
}

void viewConversationsMenu() {
	start:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "VIEW CONVERSATIONS" << endl << endl;

		listConversations();

		if (control_var == 0) {
			return;
		}
		else {
			cout << endl << endl;
			cout << "==================================================" << endl;
			cout << "VIEW CONVERSATION" << endl << endl;
			database.conversations[control_var - 1]->print();
		}

		cout << endl << "0 - Back" << endl;
		inputControl(0, 0);
		goto start;
}

void editConversationMenu() {
	Conversation *conversation;

	select:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "EDIT CONVERSATION" << endl << endl;

		listConversations();

		if (control_var == 0) {
			return;
		}

		// Selected conversation to edit
		conversation = database.conversations[control_var - 1];

	edit:

	if (conversation->group != NULL) {
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "EDIT CONVERSATION" << endl << endl;

		cout << "1 - Add message" << endl;
		cout << "2 - Approve messages" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 2);

		switch(control_var) {
			case 1: {
				addMessageMenu(conversation);
				goto edit;
				break;
			}
			case 2: {
				approveMessagesMenu(conversation);
				goto edit;
				break;
			}
			case 0: {
				goto select;
				break;
			}
		}
	}
	else {
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "EDIT CONVERSATION" << endl << endl;

		cout << "1 - Add message" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 1);

		switch(control_var) {
			case 1: {
				addMessageMenu(conversation);
				goto edit;
				break;
			}
			case 0: {
				goto select;
				break;
			}
		}
	}
}

void deleteConversationMenu() {
	string title, username1, username2;
	int conversation_vec_pos;

	start:
		cout << endl << endl;
		cout << "==================================================" << endl;
		cout << "DELETE CONVERSATION" << endl << endl;

		cout << "1 - Group" << endl;
		cout << "2 - Private" << endl << endl;

		cout << "0 - Back" << endl;

		inputControl(0, 2);

		switch(control_var) {
			case 1: {
				cin.ignore(256, '\n');
				cout << endl << endl;
				inputTitle(title);
				conversation_vec_pos = conversationSearch(title);
				if (conversation_vec_pos == -1) {
					cout << "Conversation not found" << endl;
					goto start;
				}

				break;
			}
			case 2: {
				cout << endl << endl;
				cout << "User 1" << endl;
				inputUsername(username1);

				cout << endl;
				cout << "User 2" << endl;
				inputUsername(username2);

				conversation_vec_pos = conversationSearch(username1, username2);

				if (conversation_vec_pos == -1) {
					cout << "Conversation not found" << endl;
					goto start;
				}

				break;
			}
			case 0: {
				return;
			}
		}

		// Delete conversation from database
		cout << endl;
		if (database.conversations[conversation_vec_pos]->group == NULL) { // Private conversation
			cout << "Private: [" << database.conversations[conversation_vec_pos]->users[0]->getUsername()
					<< ", " << database.conversations[conversation_vec_pos]->users[1]->getUsername() << "]" << endl;
		}
		else {
			cout << "Group: [" << database.conversations[conversation_vec_pos]->group->getTitle()
					<< "]" << endl;
		}

		cout << endl << "Delete conversation?" << endl;

		cout << "1 - Yes" << endl;
		cout << "2 - No" << endl;
		inputControl(1, 2);

		if (control_var == 1) {
			database.deleteConversation(conversation_vec_pos);
			cout << endl << "Conversation deleted" << endl;
		}
}



void addMessageMenu(Conversation *conversation) {
	Message *message;
	User *u;
	GroupUser *gu;
	string sender, type, content;
	Time time_sent;
	bool valid;

	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "ADD MESSAGE TO CONVERSARTION" << endl << endl;

	// SENDER
	do {
		valid = false;
		inputSender(sender);
		// Check if sender belongs to the conversation

		if (!conversation->users.empty()) {
			if (conversation->users[0]->getUsername() == sender) { //Users
				u = conversation->users[0];
				valid = true;
			}

			if (conversation->users[1]->getUsername() == sender) {
				u = conversation->users[1];
				valid = true;
			}
		}

		if (conversation->group != NULL) {
			for (unsigned int i = 0; i < conversation->group->users.size(); i++) {
				if (conversation->group->users[i]->user->getUsername() == sender) {
					gu = conversation->group->users[i];
					valid = true;
				}
			}
		}

		if (valid == false) {
			cout << "User not found" << endl;
		}

	} while(!valid);

	// TYPE
	inputType(type);

	// CONTENT
	inputContent(content);

	// TIME
	cout << "Time sent" << endl;
	inputTime(time_sent);

	// Add message
	cout << endl << "Add message?" << endl;

	cout << "1 - Yes" << endl;
	cout << "2 - No" << endl;
	inputControl(1, 2);



	if (control_var == 1) {
		// If its a group message add it to pending
		if (conversation->group != NULL) {
			message = new Message(gu, type, content, time_sent);
			conversation->addPendingMessage(message);
		}

		// If its a private message add it
		else {
			message = new Message(u, type, content, time_sent);
			conversation->addMessage(message);
			conversation->orderMessages();

			tr1::unordered_set<User *, hstr, eqstr>::iterator it = database.inactive_users.find(u);
			time_t t = time(0);
			struct tm * now = localtime(&t);

			// If message was sent in the last 30 days
			if (checkDate(now, message, 30)) {
				// If the user is inactive make it active
				if (it != database.inactive_users.end()) {
					database.inactive_users.erase(it);
					database.users.push_back(u);
				}
			}
		}
	}
}


void approveMessagesMenu(Conversation *conversation) {
	stack<Message *> next;

	start:
	cout << endl << endl;
	cout << "==================================================" << endl;
	cout << "APROVE MESSAGES" << endl << endl;

	if (conversation->pending_messages.empty()) {
		cout << "No more messages to aprove." << endl << endl;
		cout << "0 - Back" << endl;

		inputControl(0, 0);

		while (!next.empty()) {
			conversation->pending_messages.push(next.top());
			next.pop();
		}

		return;
	}
	else {
		Message *m = conversation->pending_messages.top();
		cout << *m << endl << endl;

		cout << "Approve message?" << endl;
		cout << "1 - Yes" << endl;
		cout << "2 - No" << endl << endl;

		if (conversation->pending_messages.size() > 1) {
			cout << "3 - Next" << endl;
			cout << "0 - Back" << endl;
			inputControl(0, 3);
		}
		else {
			cout << "0 - Back" << endl;
			inputControl(0, 2);
		}

		if (control_var == 0) {
			while (!next.empty()) {
				conversation->pending_messages.push(next.top());
				next.pop();
			}

			return;
		}
		// Add message
		else if (control_var == 1) {
			conversation->addMessage(m);
			conversation->pending_messages.pop();
			conversation->orderMessages();

			tr1::unordered_set<User *, hstr, eqstr>::iterator it = database.inactive_users.find(m->getGroupSender()->user);
			time_t t = time(0);
			struct tm * now = localtime(&t);

			// If message was sent in the last 30 days
			if (checkDate(now, m, 30)) {
				// If the user is inactive make it active
				if (it != database.inactive_users.end()) {
					database.inactive_users.erase(it);
					database.users.push_back(m->getGroupSender()->user);
				}
			}

			goto start;
		}
		// Remove message and ban
		else if (control_var == 2) {

			cout << endl;
			cout << "Ban user?" << endl;
			cout << "1 - Yes" << endl;
			cout << "2 - No" << endl;

			inputControl(1, 2);
			if (control_var == 1) {
				int user_pos = groupUserSearch(conversation->group->users,
						conversation->pending_messages.top()->getGroupSender()->user->getName());
				conversation->group->users[user_pos]->bans++;
				conversation->group->banUser(conversation->group->users[user_pos]);
				conversation->group->removeUser(conversation->group->users[user_pos]->user);

				conversation->pending_messages.pop();

				if (conversation->group->users.empty()) {
					throw NoUsersInGroup();
				}

				goto start;
			}
			else if (control_var == 2) {
				conversation->pending_messages.pop();
				goto start;
			}
		}
		// Next
		else if (control_var == 3) {
			next.push(m);
			conversation->pending_messages.pop();
			goto start;
		}
	}


}


void listConversations() {
	for (unsigned int i = 0; i < database.conversations.size(); i++) {
		if (database.conversations[i]->group == NULL) { // Private conversation
			cout << i + 1 << " - Private: [" << database.conversations[i]->users[0]->getUsername() << ", "
					<< database.conversations[i]->users[1]->getUsername() << "]" << endl;
		}
		else {
			cout << i + 1 << " - Group: [" << database.conversations[i]->group->getTitle() << "]" << endl;
		}
	}

	cout << endl << "0 - Back" << endl;

	inputControl(0, database.conversations.size());
}
