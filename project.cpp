//HEADER FILES USED

#include<stdio.h>
#include<iomanip>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<fstream>
#include<iostream>
using namespace std;

//CLASS USED
class account
{
	int accno;
	char name[50];
	int deposit;
	char type;
	public:
		void create_account();// FUNCTION TO CREATE NEW ACCOUNT
		void report();//FUNCTION TO SHOW ALL ACCOUNTS
		int retacc();//FUNCTION TO RETURN ACCOUNT NO.
		void dep(int);//FUNCTION TO DEPOSIT MONEY
		int retdep();//FUNCTION TO RETURN BALANCE
		int rettype();//FUNCTION TO RETURN TYPE
		void draw(int);//FUNCTION USED TO WITHDRAW MONEY
};
//FUNCTION USED TO WITHDRAW MONEY
void account::draw(int y)
{
	deposit=deposit-y;
}

//FUNCTION TO RETURN TYPE
int account::rettype()
{
	return type;
}
//FUNCTION TO RETURN BALANCE
int account::retdep()
{
	return deposit;
}
//FUNCTION TO DEPOSIT MONEY
void account::dep(int x)
{
	deposit=deposit+x;
}

//FUNCTION TO RETURN ACCOUNT NO.
int account::retacc()
{
	return accno;
}

//FUNCTION TO SHOW ALL ACCOUNTS
void account::report()
{
		cout<<accno<<setw(10)<<" "<<name<<setw(10)<<" "<<type<<setw(10)<<deposit<<endl;
}

// FUNCTION TO CREATE NEW ACCOUNT
void account::create_account()
{
	cout<<"\n\nEnter the account number(with care):";
	cin>>accno;
	cout<<"\n\nEnter The Name of The account Holder : ";
	cin.ignore();
	gets(name);
	cout<<"\nEnter Type of The account (C/S) : ";
	cin>>type;
	type=toupper(type);
	here:cout<<"\nEnter The Initial amount(if current A/c 1000 and for savings A/c 500) : ";
	cin>>deposit;
	if((deposit<500 && type=='S')||(deposit<1000 && type=='C'))
	{
		cout<<"condition for amount not satisfied for opening new account";
		goto here;
	}
}
// FOR DISPLAY SCREEN 
void gotoxy(int x,int y)
{
	COORD coord;
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
//MAIN FUNCTION
int main()
{
	int ch;
	int num,op;
	void intro();
	void new_account();
	void modify_account(int);
	void display_all();
	void display_sp(int);
	void deposit_withdraw(int,int);
	void delete_account(int);
	intro();
	do
	{
		cout<<"\n\n\n\tMAIN MENU";
		cout<<"\n\n\tPRESS:";
		cout<<"\n\n\t01. NEW ACCOUNT";
		cout<<"\n\n\t02. DEPOSIT AMOUNT";
		cout<<"\n\n\t03. WITHDRAW AMOUNT";
		cout<<"\n\n\t04. ACCOUNT ENQUIRY";
		cout<<"\n\n\t05. ALL ACCOUNT HOLDER LIST";
		cout<<"\n\n\t06. CLOSE AN ACCOUNT";
		cout<<"\n\n\t07. MODIFY AN ACCOUNT";
		cout<<"\n\n\t08. EXIT:";
		cin>>ch;
		switch(ch)
		{
			case 1:
				
				new_account();
				break;
			case 2:
				cout<<"\n\n\tEnter The account No. : ";
				cin>>num;
				deposit_withdraw(num, 1);
				break;
			case 3:
				cout<<"\n\n\tEnter The account No. : "; 
				cin>>num;
				deposit_withdraw(num, 2);
				break;
			case 4:
				cout<<"\n\n\tEnter The account No. : ";
				cin>>num;
				display_sp(num);
				break;
			case 5:
				display_all();
				break;
			case 6:
				cout<<"\n\n\tEnter The account No. : ";
				cin>>num;
				delete_account(num);
				break;
			 case 7:
				cout<<"\n\n\tEnter The account No. : ";
				cin>>num;
				modify_account(num);
				break;
			 case 8:
				cout<<"\n\n\tThanks for using bank managemnt system";
				break;
		}
    }while(ch!=8);
	return 0;
}
//FUNCTION TO DELETE AN EXISTING ACCOUNT
void delete_account(int num)
{
	account ac;
	ifstream fin;
	ofstream fout;
	fin.open("account1.dat",ios::binary|ios::in);
	fout.open("temp1.dat",ios::binary|ios::out);
	if(!fin)
	{
		cout<<"sorry!file can't be opened";
		exit(6);
	}
	fin.seekg(0,ios::beg);
	while(fin.read((char *)&ac , sizeof(account)))
	{
		if(ac.retacc()!=num)
		{
			fout.write((char *)&ac, sizeof(account));
		}
	}
	fin.close();
	fout.close();
	remove("account1.dat");
	rename("temp1.dat","account1.dat");
	cout<<"list updated successfully";
}

//FUNCTION USED IN CASE OF WITHDRAWAL AND DEPOSIT OF MONEY
void deposit_withdraw(int num,int op)
{
	account ac;
	int found=0;
	int amt;
	fstream file;
	file.open("account1.dat",ios::binary|ios::in|ios::out);
	if(!file)
	{
		cout<<"error in opening file";
		exit(4);
	}
	while(file.read((char *)&ac, sizeof(account)) && found==0)
	{
		if(ac.retacc()==num)
		{
			cout<<"\n\n\t\tACCOUNT DETAILS\n\n";
			cout<<"====================================================\n";
			cout<<"A/c no.      NAME           Type  Balance\n";
			cout<<"====================================================\n";
			ac.report();
			if(op==1)
			{
				cout<<"\n\n\tTO DEPOSITE AMOUNT ";
				cout<<"\n\nEnter The amount to be deposited:";
				cin>>amt;
				ac.dep(amt);
				cout<<"RECORD UPDATED";
			}
			if(op==2)
			{
				cout<<"\n\n\tTO WITHDRAW AMOUNT";
				cout<<"\n\nEnter the amount to be withdrawn:";
				cin>>amt;
				int bal=ac.retdep()-amt;
				if((bal<500 && ac.rettype()=='S')||(bal<1000 && ac.rettype()=='C'))
				{
					cout<<"insufficient balance";
				}
				else
				{
					ac.draw(amt);
					cout<<"RECORD UPDATED";
				}
			}
			int pos=(-1)*sizeof(account);
			file.seekp(pos,ios::cur);
			file.write((char *)&ac, sizeof(account));
			found=1;
		}
	}
	file.close();
	if(found==0)
	{
		cout<<"\n\nRECORD NOT FOUND";
	}

}
//FUNCTION TO MODIFY A/c DETAILS
void modify_account(int num)
{
	int found=0;
	account ac;
	fstream file;
	file.open("account1.dat",ios::binary|ios::in|ios::out);
	if(!file)
	{
		cout<<"sorry! error in opening file";
		exit(3);
	}
	while(!file.eof() && found==0 )
	{
		file.read((char *)&ac,sizeof(account));
		if(ac.retacc()==num)
		{
			cout<<"\n\n\t\tACCOUNT DETAILS\n\n";
			cout<<"====================================================\n";
			cout<<"A/c no.      NAME           Type  Balance\n";
			cout<<"====================================================\n";
			ac.report();
			cout<<"Enter the new details of account holder:\n";
			ac.create_account();
			int pos=(-1)*sizeof(account);
			file.seekp(pos,ios::cur);
			file.write((char *)&ac, sizeof(account));
			cout<<"\n\n\t Record updated";
			found=1;
		}
	}
	file.close();
	if(found==0)
	{
		cout<<"Account number not found";
	}
}

//FUNCTION TO SEARCH A PARTICULAR ACCOUNT
void display_sp(int num)
{
	int flag=0;
	account ac;
	ifstream fin;
	fin.open("account1.dat",ios::in|ios::binary);
	if(!fin)
	{
		cout<<"error in opening file";
		exit(5);
	}
	cout<<"\n\n\t\tACCOUNT DETAILS\n\n";
	cout<<"====================================================\n";
	cout<<"A/c no.      NAME           Type  Balance\n";
	cout<<"====================================================\n";
	while(!fin.eof())
	{
		fin.read((char *)&ac,sizeof(account));
		if(ac.retacc()==num)
		{
			ac.report();
			flag=1;
		}
		fin.read((char*)&ac,sizeof(account));
	}
	fin.close();
	if(flag==0)
	{
		cout<<"sorry! account not found";
	}
}

// FUNCTION TO WRITE IN FILE
void new_account()
{
	account ac;
	ofstream fout;
	fout.open("account1.dat",ios::binary|ios::app);
	ac.create_account();
	cout<<"\n\n\nAccount Created..";
	fout.write((char *) &ac, sizeof(account));
	fout.close();
}

//FUCTION TO READ ALL RECORDS
void display_all()
{
	account ac;
	ifstream fin;
	fin.open("account1.dat",ios::binary);
	if(!fin)
	{
		cout<<"sorry! file can't be opened";
		exit(2);
	}
	cout<<"\n\n\t\tACCOUNT HOLDER LIST\n\n";
	cout<<"====================================================\n";
	cout<<"A/c no.      NAME           Type  Balance\n";
	cout<<"====================================================\n";
	while(fin.read((char *)&ac,sizeof(account)))
	{
		ac.report();
	}
	fin.close();
}

//INTRODUCTION FUNCTION
void intro()
{
	int r,q;
	system("color 3F");
	gotoxy(0,0);
	printf("\n\n****************PROJECT FOR BANK MANAGEMENT SYSTEM******************");
	gotoxy(2,10);
	printf("                        BY: ISHITA (Roll No.-16)\n");
	gotoxy(10,14);
	printf("\n\tLOADING...");
	gotoxy(10,16);
	for(r=1;r<=20;r++)
	{
		for(q=1;q<=100000000;q++);
		printf("%c",177);
	}
	system("cls");
	system("color 3F");
	
}

