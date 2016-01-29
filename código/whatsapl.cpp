#include "whatsapl.h"

using namespace std;

UserPtr notfound(new User("","","","",0,Date(0,0,0)));
extern Database database;





// =========================================================================================================================================
// DATE

Date::Date(int year, int month, int day) {
	this -> year = year;
	this -> month = month;
	this -> day = day;
}

int Date::getYear() const {
	return year;
}

int Date::getMonth() const {
	return month;
}

int Date::getDay() const {
	return day;
}


bool Date::operator ==(const Date &d) const {
	return ((year == d.year) && (month == d.month) && (day == d.day));
}

bool Date::operator <(const Date &d) const {
	if (getYear() < d.getYear()) {
		return true;
	}
	else if (getYear() == d.getYear()) {
		if (getMonth() < d.getMonth()) {
			return true;
		}
		else if (getMonth() == d.getMonth()) {
			if (getDay() < d.getDay()) {
				return true;
			}
		}
	}

	return false;
}


ostream& operator<<(ostream &out, const Date &d) {
	out << d.getYear() << "/" << d.getMonth() << "/" << d.getDay();

	return out;
}


// =========================================================================================================================================
// TIME

Time::Time(int year, int month, int day, int hour, int minute) : Date(year, month, day) {
	this -> hour = hour;
	this -> minute = minute;
}

int Time::getHour() const {
	return hour;
}

int Time::getMinute() const {
	return minute;
}

bool Time::operator < (const Time &t2) const {
	if (getYear() < t2.getYear()) {
		return true;
	}
	else if (getYear() == t2.getYear()) {
		if (getMonth() < t2.getMonth()) {
			return true;
		}
		else if (getMonth() == t2.getMonth()) {
			if (getDay() < t2.getDay()) {
				return true;
			}
			else if (getDay() == t2.getDay()) {
				if (getHour() < t2.getHour()) {
					return true;
				}
				else if (getHour() == t2.getHour()) {
					if (getMinute() < t2.getMinute()) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

ostream& operator<<(ostream &out, const Time &t) {
	out << t.getYear() << "/" << t.getMonth() << "/" << t.getDay() << ", " << t.getHour() << ":";
	if (t.getMinute() < 10) {
		out << "0" << t.getMinute();
	}
	else {
		out << t.getMinute();
	}

	return out;
}


// =========================================================================================================================================
// USER

User::User(string username, string password, string name, string email, long cellphone, Date join_date) {
	this -> username = username;
	this -> password = password;
	this -> name = name;
	this -> email = email;
	this -> cellphone = cellphone;
	this -> join_date = join_date;
}

string User::getUsername() const {
	return username;
}

string User::getPassword() const {
	return password;
}

string User::getName() const {
	return name;
}

string User::getEmail() const {
	return email;
}

Date User::getJoinDate() const {
	return join_date;
}

long User::getCellphone() const {
	return cellphone;
}

void User::setUsername(string username) {
	this -> username = username;
}

void User::setPassword(string password) {
	this -> password = password;
}

void User::setName(string name) {
	this -> name = name;
}

void User::setEmail(string email) {
	this -> email = email;
}

void User::setCellphone(long cellphone) {
	this -> cellphone = cellphone;
}

void User::setJoinDate(Date join_date) {
	this -> join_date = join_date;
}

// Number of messages in the last 3 days
int User::NumMsg3days(){
	int counter = 0;
    time_t t = time(0);
    struct tm * now = localtime(&t);

    for (unsigned int i = 0; i < database.conversations.size();i++){
        vector<Message *> m = database.conversations.at(i)->messages;
        for (unsigned int h = 0; h < m.size();h++){
        	if (m.at(h)->getSender() != NULL) {
				if (m.at(h)->getSender()->getUsername() == username){
					if(checkDate(now,m.at(h),3)){
						counter++;
					}
				}
        	}
        	else {
        		if (m.at(h)->getGroupSender()->user->getUsername() == username){
					if(checkDate(now,m.at(h),3)){
						counter++;
					}
				}
        	}
        }
    }
    return counter;
}

// Number of groups it belongs to
int User::NumGroups(){
    int counter = 0;

    for(unsigned int i = 0; i < database.groups.size();i++){
        Group * g = database.groups.at(i);
        for(unsigned int h = 0; h < g->users.size();h++){
            if(g->users.at(h)->user->getUsername() == username){
                counter++;
            }
        }
    }
    return counter;
}



bool User::operator ==(User &u) const {
	if ((username == u.getUsername()) ||
			(password == u.getPassword()) ||
			(name == u.getName()) ||
			(email == u.getEmail()) ||
			(cellphone == u.getCellphone()) ||
			(join_date == u.getJoinDate())) {
		return true;
	}
	else {
		return false;
	}
}

void User::print() {
	cout << "Username: " << username << endl;
	cout << "Password: " << password << endl;
	cout << "Name: " << name << endl;
	cout << "Email: " << email << endl;
	cout << "Cellphone: " << cellphone << endl;
	cout << "Join date: " << join_date << endl;
}


// =========================================================================================================================================
// DATABASE

Database::Database() : favusers(notfound) {
}

void Database::addUser(User *u) {
	users.push_back(u);
}

void Database::addGroup(Group *g){
	groups.push_back(g);
}

void Database::addConversation(Conversation *c){
	conversations.push_back(c);
}

void Database::setUser(User *u, int pos) {
	users[pos] = u;
}

void Database::deleteUser(int pos) {

	User *u = users[pos];

	// FROM GROUPS
	for (unsigned int i = 0; i < groups.size(); i++) {
		// If the user is the moderator of a group change the moderator to the next user
		if (groups[i]->getModerator()->user == u) {
			for (unsigned int j = 0; j < groups[i]->users.size(); j++) {
				if (groups[i]->users[j]->user != u) {
					groups[i]->setModerator(groups[i]->users[j]);
					swap(groups[i]->users[j], groups[i]->users[0]);
					break;
				}
			}
		}

		// Erase user from group
		for (unsigned int k = 0; k < groups[i]->users.size(); k++) {
			if (groups[i]->users[k]->user == u) {
				groups[i]->users.erase(groups[i]->users.begin() + k);
				break; // user can only be in the group one time
			}
		}

		// And from from the banned users
		for (unsigned int k = 0; k < groups[i]->banned_users.size(); k++) {
			if (groups[i]->banned_users[k]->user == u) {
				groups[i]->banned_users.erase(groups[i]->banned_users.begin() + k);
				break;
			}
		}

		// And from from the requested users
		for (unsigned int k = 0; k < groups[i]->requested_users.size(); k++) {
			if (groups[i]->requested_users[k] == u) {
				groups[i]->requested_users.erase(groups[i]->requested_users.begin() + k);
				break;
			}
		}

		// Deleted group if its left without users
		if (groups[i]->users.empty()) {
			deleteGroup(i);
		}
	}

	// FROM CONVERSATIONS
	for (unsigned int i = 0; i < conversations.size(); i++) {
		// Delete user messages in group conversations
		if (conversations[i]->group != NULL) {
			for (unsigned int j = 0; j < conversations[i]->messages.size(); j++) {
				if (conversations[i]->messages[j]->getSender() == u) {
					delete conversations[i]->messages[j];
					conversations[i]->messages.erase(conversations[i]->messages.begin() + j);
					j--;
				}
			}
		}

		// Delete private conversations user is a part of
		else {
			if (conversations[i]->users[0] == u
					|| conversations[i]->users[1] == u) {
				// Delete every message on conversation
				for (unsigned int j = 0; j < conversations[i]->messages.size(); j++) {
					delete conversations[i]->messages[j];
				}

				// Delete conversation
				delete conversations[i];
				conversations.erase(conversations.begin() + i);
				i--;
			}
		}
	}

	// FROM DATABASE
	delete u;
	users.erase(users.begin() + pos);
}

void Database::deleteGroup(int pos) {
	Group *g = groups[pos];

	// FROM CONVERSATIONS
	for (unsigned int i = 0; i < conversations.size(); i++) {

		if (conversations[i]->group == g) {
			deleteConversation(i);

			break; // group can only have one conversation
		}

	}

	// FROM DATABASE
	delete g;
	groups.erase(groups.begin() + pos);
}

void Database::deleteConversation(int pos) {
	// Delete messages in conversations
	for (unsigned int j = 0; j < conversations[pos]->messages.size(); j++) {
		delete conversations[pos]->messages[j];
	}

	// Delete conversation
	delete conversations[pos];
	conversations.erase(conversations.begin() + pos);
}

void Database::orderMessages() {
	for (int i = 0; i < conversations.size(); i++) {
		conversations[i]->orderMessages();
	}
}


void Database::printFavUsers(){
    // In case its not the first time print is called
    BST<UserPtr> favusers(notfound);


    // Insert elements in the BST
    for(unsigned int i = 0; i < users.size();i++){
    	UserPtr u(users.at(i));
        favusers.insert(u);
    }


    // Print users in order
    BSTItrIn<UserPtr> it(favusers);
    int counter = 1;
    while(!it.isAtEnd()){
    	int mess = it.retrieve().u->NumMsg3days();
        cout <<counter<<" - "<<it.retrieve().u->getUsername() << " - "<<mess;
        if (mess != 1) {
        	cout << " messages" << endl;
        }
        else {
        	cout << " message" << endl;
        }
        counter++;
        it.advance();
    }
}


void Database::makeUsersInactive() {
	bool active;
	time_t t = time(0);
	struct tm * now = localtime(&t);

	for (int i = 0; i < users.size(); i++) {
		 active = false;

		for (int j = 0; j < conversations.size(); j++) {
			for (int k = 0; k < conversations[j]->messages.size(); k++) {
				if (conversations[j]->messages[k]->getSender() == users[i]) {
					if (checkDate(now, conversations[j]->messages[k], 30)) {
						active = true;
						goto next;
					}
				}
			}
		}

		if (active == false) {
			User *u = users[i];
			users.erase(users.begin() + i);
			inactive_users.insert(u);
			i--;
		}

		next:
		continue;
	}
}



// =========================================================================================================================================
// GROUPS

Group::Group(string title, GroupUser *moderator, vector<GroupUser *> users, Date creation_date){
	this -> title = title;
	this -> moderator = moderator;
	this -> users = users;
	this -> creation_date = creation_date;
}

string Group::getTitle()const{
	return title;
}

GroupUser *Group::getModerator()const{
	return moderator;
}

Date Group::getDate() const{
	return creation_date;
}

void Group::setTitle(string title){
	this -> title = title;
}

void Group::setModerator(GroupUser *moderator){
	this -> moderator = moderator;
}

void Group::setDate(Date creation_date){
	this ->creation_date = creation_date;
}

void Group::setBannedUsers(vector<GroupUser *> banned_users) {
	this -> banned_users = banned_users;
}

void Group::setRequests(vector<User *> requested_users) {
	this -> requested_users = requested_users;
}



void Group::addUser(GroupUser *user) {
	users.push_back(user);
}

bool Group::operator ==(Group &g) const{
	if (title == g.getTitle()) {
		return true;
	}

	return false;
}

void Group::print() {
	cout << "Title: " << title << endl;
	cout << "Moderator: " << moderator->user->getUsername() << endl;
	cout << "Users:";
	for (unsigned int i = 0 ; i < users.size(); i++){
		cout << " ["<< users[i]->user->getUsername() << ", " << users[i]->group_join_date << ", " << users[i]->bans << "]";

		if(i + 1 < users.size()){
			cout << ", ";
		}
	}
	cout << endl;
	cout << "Banned users: [";
	for (unsigned int i = 0 ; i < banned_users.size();i++){
		cout << banned_users[i]->user->getUsername();
		if(i + 1 < banned_users.size()){
			cout << ", ";
		}
	}
	cout << "]" << endl;
	cout << "Requests: [";
	for (unsigned int i = 0 ; i < requested_users.size();i++){
		cout << requested_users[i]->getUsername();
		if(i + 1 < requested_users.size()){
			cout << ", ";
		}
	}
	cout << "]" << endl;
	cout << "Creation Date: " << getDate() << endl;
}

void Group::removeUser(User *u) {
	// Remove user messages from group conversation
	for (unsigned int i = 0; i < database.conversations.size(); i++) {
		if (database.conversations[i]->group != NULL) {
			if (database.conversations[i]->group->getTitle() == title) {
				for (unsigned int j = 0; j < database.conversations[i]->messages.size(); j++) {
					if (database.conversations[i]->messages[j]->getSender() == u) {
						delete database.conversations[i]->messages[j];
						database.conversations[i]->messages.erase(database.conversations[i]->messages.begin() + j);
						j--;
					}
				}

				break;
			}
		}
	}

	// If the user is the moderator of the group change the moderator to the next user
	if (moderator->user == u) {
		for (unsigned int j = 0; j < users.size(); j++) {
			if (users[j]->user != u) {
				setModerator(users[j]);
				swap(users[j], users[0]);
				break;
			}
		}
	}

	// Erase user from group
	for (unsigned int k = 0; k < users.size(); k++) {
		if (users[k]->user == u) {
			users.erase(users.begin() + k);
			break;
		}
	}
}

void Group::banUser(GroupUser *u) {
	banned_users.push_back(u);
}

void Group::unbanUser(int pos) {
	banned_users.erase(banned_users.begin() + pos);
}

void Group::addRequest(User *u) {
	requested_users.push_back(u);
}



// =========================================================================================================================================
// CONVERSATIONS

Conversation::Conversation(Group *group) {
	this -> group = group;
}

Conversation::Conversation(User *user1, User *user2) {
	group = NULL;
	users.push_back(user1);
	users.push_back(user2);
}

void Conversation::addMessage(Message *message) {
	messages.push_back(message);
}

void Conversation::addPendingMessage(Message *message) {
	pending_messages.push(message);
}


// Insertion sort
void Conversation::orderMessages() {
	for (unsigned int p = 1; p < messages.size(); p++) {
		Message *tmp = messages[p];
		int j;

		for (j = p; j > 0 && tmp->getTime() < messages[j - 1]->getTime(); j--) {
			messages[j] = messages[j - 1];
		}

		messages[j] = tmp;
	}
}

void Conversation::print() {
	for (unsigned int i = 0; i < messages.size(); i++) {
		cout << *messages[i] << endl;
	}
}


// =========================================================================================================================================
// MESSAGES

Message::Message(User *sender, string type, string content, Time time_sent) {
	this -> type = type;
	this -> content = content;
	this -> time_sent = time_sent;
	this -> sender = sender;
	this -> group_sender = NULL;
}

Message::Message(GroupUser *group_sender, string type, string content, Time time_sent) {
	this -> type = type;
	this -> content = content;
	this -> time_sent = time_sent;
	this -> sender = NULL;
	this -> group_sender = group_sender;
}

string Message::getType() const {
	return type;
}

string Message::getContent() const {
	return content;
}

Time Message::getTime() const {
	return time_sent;
}

User *Message::getSender() const {
	return sender;
}

GroupUser *Message::getGroupSender() const {
	return group_sender;
}

ostream& operator<<(ostream &out, const Message &m) {
	if (m.getSender() == NULL) {
		out << m.getGroupSender()->user->getUsername() << " - " << m.getTime() << endl;
	}
	else {
		out << m.getSender()->getUsername() << " - " << m.getTime() << endl;
	}
	out << m.getType() << ": " << m.getContent() << endl;
	out << "";
}



//=======================================================================================================================
// SEARCH FUNCTIONS

int userSearch(string username) {
	for (unsigned int i = 0; i < database.users.size(); i++) {
		if (database.users[i]->getUsername() == username) {
			return i;
		}
	}

	return -1;
}

int userSearch(vector<User *> &users, User * &user) {
	for (unsigned int i = 0; i < users.size(); i++) {
		if (users[i]->getUsername() == user->getUsername()) {
			return i;
		}
	}

	return -1;
}

int groupUserSearch(vector<GroupUser *> &users, GroupUser * &user) {
	for (unsigned int i = 0; i < users.size(); i++) {
		if (users[i]->user->getUsername() == user->user->getUsername()) {
			return i;
		}
	}

	return -1;
}

int groupUserSearch(vector<GroupUser *> &users, string username) {
	for (unsigned int i = 0; i < users.size(); i++) {
		if (users[i]->user->getUsername() == username) {
			return i;
		}
	}

	return -1;
}

int groupSearch(string title) {
	for (unsigned int i = 0; i < database.groups.size(); i++) {
		if (database.groups[i]->getTitle() == title) {
			return i;
		}
	}

	return -1;
}

int groupSearch(vector<Group *> &groups, Group * &group) {
	int group_pos = -1;

	for (unsigned int i = 0; i < groups.size(); i++) {
		if (groups[i]->getTitle() == group->getTitle()) {
			return i;
		}
	}

	return -1;
}

int conversationSearch(string search) {
	for (unsigned int i = 0; i < database.conversations.size(); i++) {
		if (database.conversations[i]->group != NULL) {
			if (database.conversations[i]->group->getTitle() == search) {
				return i;
			}
		}
	}

	return -1;
}

int conversationSearch(string username1, string username2) {
	for (unsigned int i = 0; i < database.conversations.size(); i++) {
		if (database.conversations[i]->group == NULL) {
			if ((database.conversations[i]->users[0]->getUsername() == username1
					&& database.conversations[i]->users[1]->getUsername() == username2)
					|| (database.conversations[i]->users[1]->getUsername() == username1
							&& database.conversations[i]->users[0]->getUsername() == username2)) {
				return i;
			}
		}
	}

	return -1;
}


//=====================================================================================================================
// CHECK DATE


bool checkDate(struct tm *now, Message *message, int difference) {

    Time t = message->getTime();

    int day_year = 0;
    // Get day of the year the message was sent
    for (int i = 1; i <= t.getMonth(); i++) {
        if (i == t.getMonth()) {
            day_year += t.getDay();
        }
        else if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12) {
            day_year += 31;
        }
        else if (i == 4 || i == 6 || i == 9 || i == 11) {
            day_year += 30;
        }
        else if (i == 2) {
            if (t.getYear() % 4 == 0) {
                day_year += 29;
            }
            else {
                day_year += 28;
            }
        }
    }

    int time_between = now->tm_yday + 1 - day_year;

    // Same year
    if (now->tm_year + 1900 == t.getYear()) {
        if ((time_between <= difference) && (time_between >= 0)) {
            return true;
        }
        else {
            return false;
        }
    }
    // Different years
    else if (now->tm_year + 1900 - t.getYear() == 1) {
        if (t.getYear() % 4 == 0) {
            time_between += 366;
            if ((time_between <= difference) && (time_between >= 0)) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            time_between += 365;
            if ((time_between <= difference) && (time_between >= 0)) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    else {
        return false;
    }
}

