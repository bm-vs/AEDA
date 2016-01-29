ATENÇÃO !!!
1. Caso deseje ter os ficheiros users.txt/groups.txt/conversations.txt
num local especifico é NECESSÁRIO muda a localização dos ficheiros no ficheiro
input.cpp nas funções read/save.

2.Formato dos ficheiros
	2.1 Users
		[username] [password] [name] [email] [cellphone] [date]
	2.2 Groups
		[number of groups]
		[group title]
		[creation date]
		[n. of users in group] [n. of banned users] [n. of requests]
		para users/banned: [username] [group join date] [times banned]
		para requests: [username]
	2.3 Conversations
		[number of conversations]
		[1 - group conversations, 2 - private conversation]
		[group title / users usernames]
		[number of messages]
		[sender, type, time sent]
		[content]
	2.4 Pending Messages
		[number of conversations]
		repeat {
			[group title]
			[number of messages]
			[sender, type, time sent]
			[content]
		}
