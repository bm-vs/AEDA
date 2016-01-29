#include "input.h"

using namespace std;

//=========================================================================================================================================
// FILES

// USERS
void ReadInputUsers(){
        //======================================================================
        ifstream input;
        input.open("users.txt");
        if(input.fail()){
                cerr << "users.txt failed to open - file created on exit" << endl;
				return;
        }
        else if(input.peek() == std::ifstream::traits_type::eof()) {
        		input.close();
        		return;
        }

        string user,username,password,name,email;
        int year,month,day;
        long cellphone;
        while(true){
                getline(input,user);
                if (input.eof()) { // Stop reading when reads eof
                	break;
                }
                istringstream i(user);
                i >> username >> password >> name >> email >> cellphone >> year >> month >> day;
                Date d (year, month, day);

                User *new_user = new User (username, password, name, email, cellphone, d);
                database.addUser(new_user);
        }
        input.close();
}

void saveUsers() {
        ofstream output("users.txt");
        for(unsigned int i=0;i < database.users.size();i++){
                output << database.users.at(i)->getUsername()<< " " << database.users.at(i)->getPassword()<< " ";
                output << database.users.at(i)->getName() << " ";
                output << database.users.at(i)->getEmail() << " " << database.users.at(i)->getCellphone()<< " ";
                output << database.users.at(i)->getJoinDate().getYear() << " "
                		<< database.users.at(i)->getJoinDate().getMonth() << " "
						<< database.users.at(i)->getJoinDate().getDay() << endl;
        }
        tr1::unordered_set<User *, hstr, eqstr>::iterator it;
        for (it = database.inactive_users.begin(); it != database.inactive_users.end(); it++) {
        		User * u = *it;
        		output << u->getUsername()<< " " << u->getPassword()<< " ";
				output << u->getName() << " ";
				output << u->getEmail() << " " << u->getCellphone()<< " ";
				output << u->getJoinDate().getYear() << " "
						<< u->getJoinDate().getMonth() << " "
						<< u->getJoinDate().getDay() << endl;
        }

        output.close();
}

// GROUPS
void ReadInputGroups(){
        //======================================================================
        ifstream input;
        input.open("groups.txt");
        if(input.fail()){
                cerr << "groups.txt failed to open - file created on exit" << endl;
				return;
        }
        else if(input.peek() == std::ifstream::traits_type::eof()) {
			input.close();
			return;
        }

        string group,title,mod_line,user_line;
        string lixo ="-";
        int year,month,day,bans,n_groups,n_members,n_banned,n_requests;

        //Get n_groups
        getline(input,group);
        istringstream n(group);
        n >> n_groups;

        while(n_groups >0){

                vector<GroupUser *> users;
                vector<GroupUser *> banned;
                vector<User *> requests;

                //Get title and date(group)
                getline(input, title);
                if (title[title.length() - 1] == '\n' || title[title.length() - 1] == '\r') {
                	title.erase(title.length() - 1);
                }

                getline(input,group);
                istringstream i(group);
                i >> year >> month >> day;
                Date d_group (year, month, day);

                getline(input, group);
                istringstream i2(group);
                i2 >> n_members >> n_banned >> n_requests;

                //Get moderator
                getline(input,group);
                istringstream m(group);
                m >> mod_line >> year >> month >> day >> bans;

                int found = userSearch(mod_line);
                GroupUser *moderator=new GroupUser;
                if(found !=-1){
                        User *mod = database.users.at(found);

                        Date m_date (year, month, day);

                        moderator->user = mod;
                        moderator->group_join_date =m_date;
                        moderator->bans =bans;
                        users.push_back(moderator);
                }
                else {
                	delete moderator;
                }


                //Get members
                while(n_members>0){
                        getline(input,group);
                        istringstream u(group);
                        u >> user_line >> year >> month >> day >> bans;
                        found =userSearch(user_line);
                        GroupUser *user_group=new GroupUser;
                        if(found !=-1){
                                User *user = database.users.at(found);
                                Date u_date (year, month, day);

                                user_group->user =user;
                                user_group->group_join_date = u_date;
                                user_group->bans =bans;
                                users.push_back(user_group);
                        }
                        else {
                        	delete user_group;
                        }
                        n_members--;
                }

                //Get banned users
				while(n_banned>0){
						getline(input,group);
						istringstream b(group);
						b >> user_line >> year >> month >> day >> bans;
						found =userSearch(user_line);
						GroupUser *user_banned=new GroupUser;
						if(found !=-1){
								User *user = database.users.at(found);
								Date u_date (year, month, day);

								user_banned->user =user;
								user_banned->group_join_date = u_date;
								user_banned->bans =bans;
								banned.push_back(user_banned);
						}
						else {
							delete user_banned;
						}
						n_banned--;
				}


                //Get user who requested to join
				while(n_requests>0){
						getline(input,group);
						istringstream r(group);
						r >> user_line;
						found =userSearch(user_line);
						if(found !=-1){
								User *user = database.users.at(found);

								requests.push_back(user);
						}
						n_requests--;
				}


                Group *g = new Group(title, moderator, users, d_group);
                g->setBannedUsers(banned);
                g->setRequests(requests);

                database.addGroup(g);
                n_groups--;
        }

}

void saveGroups(){
        ofstream output("groups.txt");
        if (database.groups.size() > 0) {
        	output << database.groups.size() << endl;
        }

        for(unsigned int i =0;i < database.groups.size();i++){
                Group *g = database.groups.at(i);
                output << g->getTitle() << endl;
                output << g->getDate().getYear() <<" "<< g->getDate().getMonth() <<" "<< g->getDate().getDay() << endl;
                output << g->users.size() - 1 << " " << g->banned_users.size() << " " << g->requested_users.size() << endl;
                for(unsigned int f=0;f<g->users.size();f++){
                        GroupUser *u =g->users.at(f);
                        output << u->user->getUsername()<< " " << u->group_join_date.getYear() << " " << u->group_join_date.getMonth()
                        		<< " " << u->group_join_date.getDay() << " " << u->bans << endl;
                }
                for(unsigned int j=0;j<g->banned_users.size();j++) {
                    	GroupUser *u =g->banned_users.at(j);
                    	output << u->user->getUsername()<< " " << u->group_join_date.getYear() << " "
                    			<< u->group_join_date.getMonth()
                    	         << " " << u->group_join_date.getDay() << " " << u->bans << endl;
                }
                for(unsigned int k=0;k<g->requested_users.size();k++) {
                		User *u =g->requested_users.at(k);

                		output << u->getUsername() << endl;
                }
        }
        output.close();
}

// CONVERSATIONS
void ReadInputConversations(){
        //======================================================================
        ifstream input;
        input.open("conversations.txt");
        if(input.fail()){
                cerr << "conversations.txt failed to open - file created on exit"<<endl;
				return; 
        }
        else if(input.peek() == std::ifstream::traits_type::eof()) {
        		input.close();
    			return;
        }


        string linha,title,sender,sender2,m_type,message;
        int n_conv,t_conv,n_men,found,year,month,day,hour,min;

        getline(input,linha);
        istringstream l(linha);
        l >>n_conv;

        while(n_conv >0){
                vector<Message *> messages;
                vector<User *> users;

                getline(input,linha);
                istringstream l(linha);
                l >> t_conv;

                if(t_conv ==1){
                		getline(input, title);
                		if (title[title.length() - 1] == '\n' || title[title.length() - 1] == '\r') {
                		       	title.erase(title.length() - 1);
                		}


                        getline(input,linha);
                        istringstream l(linha);
                        l>>n_men;

                        found =groupSearch(title);
                        if(found != -1){
                                Group * group =new Group;
                                group = database.groups.at(found);

                                while(n_men>0){
                                        getline(input,linha);
                                        istringstream l(linha);
                                        l>>sender>>m_type >> year >> month>>day>>hour>>min;

                                        found =userSearch(sender);

                                        if(found !=-1){
                                        User *u =new User;
                                        GroupUser *gu = new GroupUser;
                                        u= database.users.at(found);
                                        gu->user = u;
                                        users.push_back(u);

                                        Time m_time(year, month, day, hour, min);

                                        getline(input,message);
										if (message[message.length() - 1] == '\n' || message[message.length() - 1] == '\r') {
											message.erase(message.length() - 1);
										}

                                        Message *m =new Message(gu,m_type,message,m_time);
                                        messages.push_back(m);
                                        n_men--;
                                        }
                                }
                                Conversation *c = new Conversation;
                                c->group = group;
                                c->users = users;
                                c->messages = messages;

                                database.conversations.push_back(c);

                        }
                }
                else{
                        getline(input,linha);
                        istringstream l(linha);
                        l >> sender >> sender2;

                        getline(input,linha);
                        istringstream l2(linha);
                        l2 >> n_men;

                        found =userSearch(sender);
                        if(found !=-1){
                                User *u =new User;
                                u= database.users.at(found);
                                users.push_back(u);
                        }

                        found =userSearch(sender2);
                        if(found !=-1){
                                User *u2 =new User;
                                u2= database.users.at(found);
                                users.push_back(u2);
                        }

                        while (n_men > 0) {
                                getline(input, linha);
                                istringstream l(linha);
                                l >> sender >> m_type >> year >> month >> day >> hour>> min;

                                found = userSearch(sender);
                                if (found != -1) {
                                        User *u = new User;
                                        u = database.users.at(found);
                                        users.push_back(u);

                                        Time m_time(year, month, day, hour, min);

										getline(input, message);
										if (message[message.length() - 1] == '\n' || message[message.length() - 1] == '\r') {
											message.erase(message.length() - 1);
										}


                                        Message *m = new Message(u, m_type, message, m_time);
                                        messages.push_back(m);
                                        n_men--;
                                }
                        }

                        Conversation *c = new Conversation;
                        c->group = NULL;
                        c->users = users;
                        c->messages =messages;

                        database.conversations.push_back(c);
                }

                n_conv--;
        }
}

void saveConversations(){
        ofstream output("conversations.txt");
		if (database.conversations.size() > 0) {
			output << database.conversations.size() << endl;
		}


        for (unsigned int i = 0; i < database.conversations.size(); i++) {
                Conversation *c = database.conversations.at(i);
                if (c->group != NULL) {
                        output << "1" << endl;
                        output << c->group->getTitle() << endl;
                        output << c->messages.size() << endl;
                        for (unsigned int f = 0; f < c->messages.size(); f++) {
                                Message *m = c->messages.at(f);
                                output << m->getGroupSender()->user->getUsername() << " " << m->getType() << " "
                                                << m->getTime().getYear();
                                output << " " << m->getTime().getMonth() << " "
                                                << m->getTime().getDay();
                                output << " " << m->getTime().getHour() << " " << m->getTime().getMinute()
                                                << endl;
								output << m->getContent() << endl;
                        }
                } else {
                        output << "2" << endl;
                        output << c->users.at(0)->getUsername() << " "
                                        << c->users.at(1)->getUsername() << " " << endl;
                        output << c->messages.size() << endl;
						for (unsigned int f = 0; f < c->messages.size(); f++) {
								Message *m = c->messages.at(f);
								output << m->getSender()->getUsername() << " " << m->getType() << " "
											<< m->getTime().getYear();
								output << " " << m->getTime().getMonth() << " "
											<< m->getTime().getDay();
								output << " " << m->getTime().getHour() << " "
											<< m->getTime().getMinute() << endl;
								output << m->getContent() << endl;
                        }
                }
        }

}


// PENDING MESSAGES
void ReadPendingMessages() {
	ifstream input;
	input.open("pendingmessages.txt");
	if(input.fail()){
			cerr << "pendingmessages.txt failed to open - file created on exit"<<endl;
			return;
	}
	else if(input.peek() == std::ifstream::traits_type::eof()) {
			input.close();
			return;
	}

	int n_groups;
	string linha;

	getline(input,linha);
	istringstream l(linha);
	l >> n_groups;

	for (int i = 0; i < n_groups; i++) {
		string title;
		getline(input,linha);
		istringstream l1(linha);
		l1 >> title;

		int n_messages;
		getline(input,linha);
		istringstream l2(linha);
		l2 >> n_messages;

		int conversation_pos = conversationSearch(title);
		int group_pos = groupSearch(title);
		for (int j = 0; j < n_messages; j++) {
			string sender, m_type, message;
			int year, month, day, hour, min;

			getline(input,linha);
			istringstream l(linha);
			l>>sender>>m_type >> year >> month>>day>>hour>>min;

			int group_user_pos = groupUserSearch(database.groups[group_pos]->users, sender);
			if(group_user_pos != -1){
			GroupUser *gu = database.groups[group_pos]->users[group_user_pos];

			Time m_time(year, month, day, hour, min);

			getline(input,message);
			if (message[message.length() - 1] == '\n' || message[message.length() - 1] == '\r') {
				message.erase(message.length() - 1);
			}

			Message *m =new Message(gu,m_type,message,m_time);

			database.conversations[conversation_pos]->pending_messages.push(m);
			}
		}
	}
}


void savePendingMessages() {
	ofstream output("pendingmessages.txt");

	int n_groups = 0;

	for (int i = 0; i < database.conversations.size(); i++) {
		if (database.conversations[i]->pending_messages.size() > 0) {
			n_groups ++;
		}
	}

	output << n_groups << endl;

	for (int i = 0; i < database.conversations.size(); i++) {
		if (database.conversations[i]->pending_messages.size() > 0) {
			output << database.conversations[i]->group->getTitle() << endl;
			output << database.conversations[i]->pending_messages.size() << endl;

			vector<Message *> pd;

			while (!database.conversations[i]->pending_messages.empty()) {
				pd.push_back(database.conversations[i]->pending_messages.top());
				database.conversations[i]->pending_messages.pop();
			}

			for (int j = 0; j < pd.size(); j++) {
				Message *m = pd[j];
				output << m->getGroupSender()->user->getUsername() << " " << m->getType() << " "
							<< m->getTime().getYear();
				output << " " << m->getTime().getMonth() << " "
							<< m->getTime().getDay();
				output << " " << m->getTime().getHour() << " "
							<< m->getTime().getMinute() << endl;
				output << m->getContent() << endl;

				database.conversations[i]->pending_messages.push(pd[j]);
			}
		}
	}
}



// =========================================================================================================================================
// CHECK FUNCTIONS

void checkInput() {
	if (cin.fail()) {
		cin.clear();
	}

	cin.ignore(256, '\n');
}

bool checkDay(const unsigned int & day, const unsigned int & month, const unsigned int & year) {
	if (day < 1) {
		return false;
	}

	switch (month) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:{ // 31 day month
		if (day > 31) {
			return false;
		}
		else {
			return true;
		}
	}
	case 2:  {// February
		if (year % 4 == 0) {
			if (day > 29) {
				return false;
			}
			else {
				return true;
			}
		}
		else {
			if (day > 28) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	default:
		if (day > 30) {
			return false;
		}
		else {
			return true;
		}
	}

}


// =========================================================================================================================================
// INPUT

void inputControl(int min, int max) {
	bool valid;
	do {
		cin >> control_var;
		valid = ((control_var >= min) && (control_var <= max) && !cin.fail());
		if (!valid) {
			checkInput();
		}
	} while (!valid);
}

void inputUsername(string &username) {
	do {
		checkInput();
		cout << "Username: ";
		cin >> username;
	} while (cin.fail());
}

void inputPassword(string &password) {
	do {
		checkInput();
		cout << "Password: ";
		cin >> password;
	} while (cin.fail());
}

void inputName(string &name) {
	do {
		checkInput();
		cout << "Name: ";
		cin >> name;

	} while(cin.fail());
}

void inputEmail(string &email) {
	do {
		checkInput();
		cout << "Email: ";
		cin >> email;
	} while (cin.fail());
}

void inputCellphone(long &cellphone) {
	do {
		checkInput();
		cout << "Cellphone: ";
		cin >> cellphone;
	} while (cin.fail());
}

void inputDate(Date &date) {
	bool valid;
	int year, month, day;

	start:
		valid = true;

		cout << "   " << "Year: ";
		cin >> year;
		if (year < 0 || cin.fail()) {
			cin.clear();
			cout << "   " << "Invalid year" << endl;
			goto start;
		}

		cin.ignore(256, '\n');
		cout << "   " << "Month: ";
		cin >> month;
		if (month < 1 || month > 12 || cin.fail()) {
			cin.clear();
			cout << "   " << "Invalid month" << endl;
			goto start;
		}

		cin.ignore(256, '\n');
		cout << "   " << "Day: ";
		cin >> day;
		valid = checkDay(day, month, year);
		if (!valid || cin.fail()) {
			cin.clear();
			cout << "   " << "Invalid day" << endl;
			goto start;
		}

		date = Date(year, month, day);
}

void inputTitle(string &title) {
	do {
		if (cin.fail()) {
			cin.ignore();
		}
		cout << "Title: ";
		getline(cin,title);
	} while (cin.fail());
}

void inputSender(string &sender) {
	do {
		checkInput();
		cout << "Sender username: ";
		cin >> sender;
	} while (cin.fail());
}

void inputType(string &type) {
	do {
		checkInput();
		cout << "Type: ";
		cin >> type;
	} while(cin.fail());
}

void inputContent(string &content) {
	cin.ignore(256, '\n');

	do {
		if (cin.fail()) {
			cin.clear();
		}
		cout << "Content: ";
		getline(cin,content);
	} while (cin.fail());
}

void inputTime(Time &time_sent) {
	int hour, minute;
	Date date;

	inputDate(date);

	start:
		cin.ignore(256, '\n');
		cout << "   " << "Hour: ";
		cin >> hour;
		if (hour < 0 || hour > 23 || cin.fail()) {
			cin.clear();
			cout << "   " << "Invalid hour" << endl;
			goto start;
		}

		cin.ignore(256, '\n');
		cout << "   " << "Minute: ";
		cin >> minute;
		if (minute < 0 || minute > 59 || cin.fail()) {
			cin.clear();
			cout << "   " << "Invalid minute" << endl;
			goto start;
		}


		time_sent = Time(date.getYear(), date.getMonth(), date.getDay(), hour, minute);
}


// =========================================================================================================================================
// CHECKS - USERS

void createUsername(string &username, int user_pos) {
	bool valid;

	do {
		valid = true;

		checkInput();
		cout << "Username: ";
		cin >> username;

		// Check active users
		for (unsigned int i = 0; i < database.users.size(); i++) {
			if (username == database.users[i]->getUsername() && i != user_pos) {
				cout << "Username already in use." << endl;
				valid = false;
			}
		}

		// Check inactive users
		tr1::unordered_set<User *, hstr, eqstr>::iterator it;
		for (it = database.inactive_users.begin(); it != database.inactive_users.end(); it++) {
			if (username == (*it)->getUsername()) {
				cout << "Username already in use." << endl;
				valid = false;
			}
		}


	} while (cin.fail() || !valid);
}

void createPassword(string &password) {
	string tmp_password;

	do {
		checkInput();
		cout << "Password: ";
		cin >> password;

		checkInput();
		cout << "Confirm password: ";
		cin >> tmp_password;

		if (tmp_password != password) {
			cout << "Passwords dont match" << endl;
		}

	} while (cin.fail() || tmp_password != password);
}

void createEmail(string &email, int user_pos) {
	bool valid;

	do {
		valid = true;

		checkInput();
		cout << "Email: ";
		cin >> email;

		// Check active users
		for (unsigned int i = 0; i < database.users.size(); i++) {
			if (email == database.users[i]->getEmail() && i != user_pos) {
				cout << "Email already in use." << endl;
				valid = false;
			}
		}

		// Check inactive users
		tr1::unordered_set<User *, hstr, eqstr>::iterator it;
		for (it = database.inactive_users.begin(); it != database.inactive_users.end(); it++) {
			if (email == (*it)->getEmail()) {
				cout << "Email already in use." << endl;
				valid = false;
			}
		}
	} while (cin.fail() || !valid);
}

void createCellphone(long &cellphone, int user_pos) {
	bool valid;

	do {
		valid = true;

		checkInput();
		cout << "Cellphone: ";
		cin >> cellphone;

		// Check active users
		for (unsigned int i = 0; i < database.users.size(); i++) {
			if (cellphone == database.users[i]->getCellphone()
					&& i != user_pos) {
				cout << "Cellphone already in use." << endl;
				valid = false;
			}
		}

		// Check inactive users
		tr1::unordered_set<User *, hstr, eqstr>::iterator it;
		for (it = database.inactive_users.begin(); it != database.inactive_users.end(); it++) {
			if (cellphone == (*it)->getCellphone()) {
				cout << "Cellphone already in use." << endl;
				valid = false;
			}
		}

	} while (cin.fail() || !valid);
}


// =========================================================================================================================================
// CHECK - GROUPS

void createTitle(string &title, int user_pos){
	bool valid;

	cin.ignore(256, '\n');

	do {
		valid = true;

		if (cin.fail()) {
			cin.clear();
		}
		cout << "Title: ";
		getline(cin,title);
		for(unsigned int i = 0; i < database.groups.size(); i++){
			if(title == database.groups[i]->getTitle() && i != user_pos){
				cout << "Title already in use." << endl;
				valid = false;
			}
		}
	} while (cin.fail() || !valid);

}
