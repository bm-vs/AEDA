#ifndef WHATSAPL_H_
#define WHATSAPL_H_

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <tr1/unordered_set>
#include <ctime>
#include "BST.h"

using namespace std;

class User;
class UserPtr;
class Message;
class Group;
class Conversation;
class Date;
class Time;
class GroupUser;


extern int control_var;


//=====================================================================================================================
// CLASSES


// =========================================================================================================================================
// ERROR

class ModNotSet {};
class NoUsersInGroup {};

// =========================================================================================================================================
// DATE

class Date {
private:
	int year;
	int month;
	int day;
public:
	Date() {};
	Date(int year, int month, int day);

	int getYear() const;
	int getMonth() const;
	int getDay() const;

	bool operator ==(const Date &d) const;
	bool operator <(const Date &d) const;
	friend ostream& operator<<(ostream &out, const Date &d);
};

// =========================================================================================================================================
// TIME

class Time : public Date {
private:
	int hour;
	int minute;
public:
	Time() {};
	Time(int year, int month, int day, int hour, int minute);

	int getHour() const;
	int getMinute() const;

	friend ostream& operator<<(ostream &out, const Time &t);
	bool operator < (const Time &t2) const;
};



// =========================================================================================================================================
// GROUP USER

class GroupUser {
public:
	User *user;
	Date group_join_date;
	int bans;
	GroupUser() {};
	bool operator==(GroupUser &u) const {return u.user == user;};
};

// =========================================================================================================================================
// USER

class User {
private:
	string username;
	string password;
	string name;
	string email;
	long cellphone;
	Date join_date;
public:
	User() {};
	User(string username, string password, string name, string email, long cellphone, Date join_date);
	string getUsername() const;
	string getPassword() const;
	string getName() const;
	string getEmail() const;
	long getCellphone() const;
	Date getJoinDate() const;
	void setUsername(string username);
	void setPassword(string password);
	void setName(string name);
	void setEmail(string email);
	void setCellphone(long cellphone);
	void setJoinDate(Date join_date);

	int NumMsg3days();
	int NumGroups();
	void print();

	bool operator ==(User &u) const;
};

class UserPtr {
public:
	User *u;
	UserPtr(User *user) : u(user) {};
	bool operator <(const UserPtr &user2) const {
		int c1 = u->NumMsg3days();
		int c2 = user2.u->NumMsg3days();

		if( c1 > c2){
			return true;
		}else if(c1 < c2){
			return false;
		}else{
			int n1 = u->NumGroups();
			int n2 = user2.u->NumGroups();

			if(n1 > n2) return true;
			else if (n1 < n2) return false;
			else return u->getUsername() < user2.u->getUsername();
		}

	};
};

struct eqstr {
	bool operator() (User *u1, User *u2) const {
		return ((u1->getUsername() == u2->getUsername()) ||
				(u1->getEmail() == u2->getEmail()) ||
				(u1->getCellphone() == u2->getCellphone()));
	}
};

struct hstr {
	int operator() (User *u) const {
		int v = 0;
		string s1 = u->getUsername();

		for (unsigned int i = 0; i < s1.size(); i++) {
			v = 37*v + s1[i];
		}

		return v;
	}
};




// =========================================================================================================================================
// GROUP

class Group {
private:
	string title;
	GroupUser *moderator;
	Date creation_date;
public:
	vector<GroupUser *> users;
	vector<GroupUser *> banned_users;
	vector<User *> requested_users;

	Group(){};
	Group(string title, GroupUser *moderator, vector<GroupUser *> users, Date creation_date);
	string getTitle()const;
	GroupUser *getModerator() const;
	Date getDate() const;

	void setTitle(string title);
	void setModerator(GroupUser *moderator);
	void setDate(Date creation_date);
	void setBannedUsers(vector<GroupUser *> banned_users);
	void setRequests(vector<User *> requested_users);

	void addUser(GroupUser *user);
	void print();

	void removeUser(User *u);
	void banUser(GroupUser *u);
	void unbanUser(int pos);
	void addRequest(User *u);

	bool operator ==(Group &g) const;

};


// =========================================================================================================================================
// MESSAGE

class Message {
private:
	string type;
	string content;
	Time time_sent;
	User *sender;
	GroupUser *group_sender;
public:
	Message() {};
	Message(User *sender, string type, string content, Time time_sent);
	Message(GroupUser *group_sender, string type, string content, Time time_sent);

	string getType() const;
	string getContent() const;
	Time getTime() const;
	User *getSender() const;
	GroupUser *getGroupSender() const;

	friend ostream& operator<<(ostream &out, const Message &m);
};

struct messageComparison {
	bool operator ()(Message *m1, Message *m2) {
		if (m1->getGroupSender()->group_join_date == m2->getGroupSender()->group_join_date) {
			return m2->getTime() < m1->getTime();
		}
		else {
			return m1->getGroupSender()->group_join_date < m2->getGroupSender()->group_join_date;
		}
	}
};


// =========================================================================================================================================
// CONVERSATION

class Conversation {
public:
	vector<Message *> messages;
	priority_queue<Message *, vector<Message *>, messageComparison> pending_messages;
	Group * group;
	vector<User *> users;

	Conversation() {};
	Conversation(Group *group);
	Conversation(User *user1, User *user2);

	void addMessage(Message *message);
	void addPendingMessage(Message *message);

	void orderMessages();

	void print();
};


// =========================================================================================================================================
// DATABASE

class Database {
public:
	vector<User *> users;
	vector<Group *> groups;
	vector<Conversation *> conversations;
	BST<UserPtr> favusers;
	tr1::unordered_set<User *, hstr, eqstr> inactive_users;

	Database();
	void addUser(User *u);
	void addGroup(Group *g);
	void addConversation(Conversation *c);
	void setUser(User *u, int pos);
	void deleteUser(int pos);
	void deleteGroup(int pos);
	void deleteConversation(int pos);
	void orderMessages();

	void printFavUsers();
	void makeUsersInactive();
};


//=====================================================================================================================
// SEARCH FUNCTIONS

// SEARCH
int userSearch(string username);
int userSearch(vector<User *> &users, User * &user);

int groupUserSearch(vector<GroupUser *> &users, GroupUser * &user);
int groupUserSearch(vector<GroupUser *> &users, string username);

int groupSearch(vector<Group *> &groups, Group * &group);
int groupSearch(string title);

int conversationSearch(string search);
int conversationSearch(string username1, string username2);

//=====================================================================================================================
// CHECK DATE

bool checkDate(struct tm *now, Message *message, int difference);



#endif
