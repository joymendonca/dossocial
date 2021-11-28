#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <mysql.h>
#include <sstream>
#include <ctime>

using namespace std;

int postLogin(string id);

int isFriend(string uId, string fId)
{
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
	conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);

    if(conn)
    {
        int qstate = mysql_query(conn, "SELECT sender,receiver,pending,accepted FROM friends");

        if(!qstate)
        {
             res = mysql_store_result(conn);

             while(row = mysql_fetch_row(res))
             {
                string tSender = row[0], tReceiver = row[1], tPending = row[2], tAccepted = row[3];
                //cout<<"Userid: "<<tUsrid<<"\tPass: "<<tPswd<<endl;
                if (tSender == fId || tSender == uId)
                {
                    if (tReceiver == fId || tReceiver == uId)
                    {
                        if(tAccepted == "1")
                            return (1);
                        if(tPending == "1")
                            return (2);
                    }
                }

             }

        }
    }
    return (0);
}

string getInfo(int op, string input)
{
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
	conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);

    if(conn)
    {
        int qstate = mysql_query(conn, "SELECT id,userid FROM login");

        if(!qstate)
        {
             res = mysql_store_result(conn);

             while(row = mysql_fetch_row(res))
             {
                string tId = row[0], tName = row[1];
                //cout<<"Userid: "<<tUsrid<<"\tPass: "<<tPswd<<endl;
                if (op == 1)
                {
                    if(tId == input)
                        return (tName);
                }

                else if (op == 2)
                {
                    if (input == tName)
                        return(tId);
                }

             }

        }
    }
    return ("ERROR");
}


int changePassword(string id)
{
    system("cls");
    string cPwd, nPwd, nPwd2;
    int state[1];

    cout<<"Enter Current Password: ";
    cin>>cPwd;
    cout<<endl<<"Enter New Password: ";
    cin>>nPwd;
    cout<<endl<<"Confirm New Password: ";
    cin>>nPwd2;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
	conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);

    if(conn)
    {
        int qstate = mysql_query(conn, "SELECT id,userid,pswd FROM login");

        if(!qstate)
        {
            res = mysql_store_result(conn);

             while(row = mysql_fetch_row(res))
             {
                string tId = row[0], tPswd = row[2];
                //cout<<"Userid: "<<tUsrid<<"\tPass: "<<tPswd<<endl;
                if(tId == id && tPswd == cPwd)
                    state[0] = 1;
             }
        }

    }

    if (nPwd == nPwd2)
        state[1] = 1;

    if(state[0] == 1 && state[1] == 1)
    {
                int qstate = 0;
                stringstream ss;
                ss<<"UPDATE login SET pswd = '" << nPwd << "' WHERE id = "<<id<<";";
                //ss<<"INSERT INTO chat(name, msg) VALUES('" << name << "','" << msg << "')";
                string query = ss.str();
                const char* q = query.c_str();
                qstate = mysql_query(conn, q);

        cout<<endl<<"PASSWORD CAN BE SUCCESSFULLY CHANGED"<<endl;
        postLogin(id);
    }

    cout<<endl<<"[ERROR] SOMETHING WENT WRONG"<<endl;


    return 0;
}

int uInbox(string id)
{
    system("cls");

    cout<<"Reached Inbox"<<endl;

    string name;
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
	conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);


    if(conn)
    {
        int qstate = mysql_query(conn, "SELECT id,sender,receiver,msg,date FROM chat ORDER BY id DESC;");

        if(!qstate)
        {
             res = mysql_store_result(conn);

             while(row = mysql_fetch_row(res))
             {
                string sender = row[1], receiver = row[2], msg = row[3], date = row[4];
                string sName = getInfo(1, sender);
                if(receiver == id)
                {
                    cout<<"----------------------------"<<endl;
                    cout<<"<"<<date<<"> "<<"<"<<sName<<">: "<<msg<<endl;
                }
             }
        }
    }

}

int sMsg(string id)
{
    system("cls");

    string fName;
    cout<<"Send to: ";
    cin>>fName;
    string fId = getInfo(2, fName);
    int friends = isFriend(id, fId);

    string name;
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
	conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);

    if(friends == 1)
    {
        string msg;
        cout<<endl<<"Your message: ";
        cin.ignore();
        getline(cin,msg);
        int qstate = 0;
        stringstream ss;
        ss<<"INSERT INTO chat(sender,receiver, msg) VALUES('" << id << "','" << fId << "','" << msg <<"')";
        string query = ss.str();
        const char* q = query.c_str();
        qstate = mysql_query(conn, q);
    }

    else
        cout<<"[ERROR] You are either not friends with the user or the userid is invalid"<<endl;

    return 0;
}

int frndReq(string id)
{
    system("cls");
    cout<<endl<<"Reached Friend Requests"<<endl<<endl;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
	conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);


    if(conn)
    {
        int qstate = mysql_query(conn, "SELECT sender,receiver,pending FROM friends;");

        if(!qstate)
        {
             res = mysql_store_result(conn);

             string tempId[20];
             int limit,i=0;
             while(row = mysql_fetch_row(res))
             {
                string tSender = row[0], tReceiever = row[1], tStatus = row[2];

                //cout<<tSender<<" "<<tReceiever<<" "<<tStatus<<endl;

                if(tReceiever == id && tStatus == "1" )
                {
                    i = i+1;
                    tempId[i] = tSender;
                    string name = getInfo(1, tSender);
                    cout<<i<<". "<<name<<endl;
                    limit = i;
                }

              }

              int op;
              cout<<endl<<"Press 1 to Accept, Press 2 to Delete Req, Press 3 to Exit"<<endl;
              cin>>op;


              if(op == 1)
              {
                  int a;
                  cout<<endl<<"ENTER OPTION: ";
                  cin>>a;

                  if (a>limit || a <= 0)
                    {
                        cout<<"[ERROR] INVALID INPUT"<<endl;
                        return 0;
                    }

                  qstate = 0;
                  string sender = tempId[a];
                  stringstream ss;
                  ss<<"UPDATE friends SET pending = '0', accepted = '1' WHERE sender = "<<sender<<" AND receiver = "<<id<<";";
                  string query = ss.str();
                  const char* q = query.c_str();
                  qstate = mysql_query(conn, q);
                  cout<<"FRIEND REQUEST HAS BEEN ACCEPTED."<<endl<<"YOU ARE NOW FRIENDS WITH "<<getInfo(1, sender)<<endl;
              }

              if(op==2)
              {
                  int a;
                  cout<<endl<<"ENTER OPTION: ";
                  cin>>a;

                  if (a>limit || a <= 0)
                    {
                        cout<<"[ERROR] INVALID INPUT"<<endl;
                        return 0;
                    }
                  qstate = 0;
                  string sender = tempId[a];
                  stringstream ss;
                  ss<<"DELETE from friends WHERE sender = "<<sender<<" AND receiver = "<<id<<";";
                  string query = ss.str();
                  const char* q = query.c_str();
                  qstate = mysql_query(conn, q);
                  cout<<"FRIEND REQUEST HAS BEEN DELETED"<<endl;
              }

        }

    }

    return 0;
}


int addFriend(string id)
{
    system("cls");

    string fid;
    cout<<"Enter the ID you want to add: ";
    cin>>fid;

    string temp = getInfo(2, fid);
    if (temp == "ERROR")
    {
        cout<<"This user id does not exist! Try again."<<endl;
        return 0;
    }

    cout<<"The ID of "<<fid<<" is: "<<temp<<endl;

    int status = isFriend(id,temp);

    /*if(status == 0)
        cout<<endl<<"STATUS: No Req"<<endl; */
    if(status == 1)
        cout<<endl<<"STATUS: Already Friends"<<endl;
    if(status == 2)
        cout<<endl<<"STATUS: Req Already Sent"<<endl;

    if(status == 0)
    {
        MYSQL* conn;
        MYSQL_ROW row;
        MYSQL_RES* res;
        conn = mysql_init(0);
        conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);

        int qstate = 0;

        stringstream ss;

        ss<<"INSERT INTO friends(sender, receiver, pending, accepted) VALUES('" << id << "','" << temp << "','1','0')";

        string query = ss.str();
        const char* q = query.c_str();
        qstate = mysql_query(conn, q);

        cout<<endl<<"REQUEST SUCCESSFULLY SENT TO: "<<fid<<endl;
    }

    return 0;
}

int gWall(string id)
{
    string name;
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
	conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);


    if(conn)
    {
        int qstate = mysql_query(conn, "SELECT id,name,msg,date FROM wall ORDER BY id DESC;");

        if(!qstate)
        {
             res = mysql_store_result(conn);

             while(row = mysql_fetch_row(res))
             {
                string userid = row[1], message = row[2], date = row[3];
                string tId = getInfo(2, userid);
                int friends = isFriend(id, tId);
                if(tId == id || friends == 1)
                {
                    cout<<"----------------------------"<<endl;
                    cout<<"<"<<date<<"> "<<"<"<<userid<<">: "<<message<<endl;
                }
             }

             int op;
             cout<<"----------------------------"<<endl;
             cout<<endl<<endl<<"Press 1 to Update status, Press 0 to exit"<<endl;
             cin>>op;

             if(op == 1)
             {
                 string status;
                 cout<<endl<<"Whats on your mind? : ";
                 cin.ignore();
                 getline(cin,status);
                 string name = getInfo(1, id);
                 qstate = 0;
				 stringstream ss;
				 ss<<"INSERT INTO wall(name, msg) VALUES('" << name << "','" << status << "')";
				 string query = ss.str();
				 const char* q = query.c_str();
				 qstate = mysql_query(conn, q);
             }
        }

    }

    //postLogin(id);
    return 0;
}

int postLogin(string id)
{
    system("cls");
    int op;
    cout<<"1. Wall"<<endl<<"2. Friend Requests"<<endl<<"3. Add Friend"<<endl<<"4. Send Message"<<endl<<"5. Inbox"<<endl<<"6. Change Password"<<endl<<"0. Logout"<<endl<<endl<<"Enter Your Option: ";
    cin>>op;

    switch(op)
    {
    case 0:
        return 0;
        break;
    case 1:
        gWall(id);
        break;
    case 2:
        frndReq(id);
        break;
    case 3:
        addFriend(id);
        break;
    case 4:
        sMsg(id);
        break;
    case 5:
        uInbox(id);
        break;
    case 6:
        changePassword(id);
        break;
    default:
        cout<<"[ERROR] Invalid Input!";
    }
}

//LOGIN FUNCTION
int login()
{
    string usrid,pswd;
    int loggedin=0;

    cout<<"UserID: ";
    cin>>usrid;
    cout<<"Password: ";
    cin>>pswd;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
	conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);

    if(conn)
    {
        int qstate = mysql_query(conn, "SELECT id,userid,pswd FROM login");

        if(!qstate)
        {
             res = mysql_store_result(conn);

             while(row = mysql_fetch_row(res))
             {
                string tUsrid = row[1], tPswd = row[2];
                //cout<<"Userid: "<<tUsrid<<"\tPass: "<<tPswd<<endl;
                if(tUsrid == usrid && tPswd == pswd)
                {
                    loggedin = 2;
                    string tempid = row[0];
                    postLogin(tempid);
                }
             }

             if(!loggedin)
                cout<<"Userid or Password invalid please try again."<<endl;
                system("pause");
        }

    }

    return 0;
}


//Registration Function
int reg()
{
    string usrid,pswrd;
    cout<<"Enter new Username: ";
    cin>>usrid;
    cout<<"Enter new Password: ";
    cin>>pswrd;

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
	conn = mysql_init(0);
    conn = mysql_real_connect(conn, "127.0.0.1", "admin", "admin", "joy_test", 0, NULL, 0);

    int qstate = 0;

    stringstream ss;

    ss<<"INSERT INTO login(userid, pswd) VALUES('" << usrid << "','" << pswrd << "')";

    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);
    if(qstate == 0)
    {
        cout<<"Successfully Registered!"<<endl;
        system("pause");
        return 0;
    }
    else
        cout<<"Failed!"<<endl<<endl<<endl;
        system("pause");
    return 0;
}

int main()
{

    int choice;
    cout<<"\t\t\t\t _______________________________"<<endl;
    cout<<"\t\t\t\t|---Joy's Social Media System---|"<<endl;
    cout<<"\t\t\t\t|_______________________________|"<<endl;

    cout<<"\t\t\t\t\t    1. Login"<<endl;
    cout<<"\t\t\t\t\t    2. Register"<<endl;
    cout<<"\t\t\t\t\t    3. Exit"<<endl;
    cin>>choice;

    switch(choice)
    {
    case 1:
        login();
        break;
    case 2:
        reg();
        break;
    case 3:
        return 0;
        break;
    default:
        cout<<"INVALID!"<<endl;
        break;
    }

    main();
    return 0;

}
