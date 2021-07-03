#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define MAX_ACC 50
#define PATH1 "students.txt"

#define STD 1
#define TCH 2


void RegisterAccount();
void AskLogin();
void UpdateAccounts();
void LoadAccounts();
void UpdateAnnouncements();
void LoadAnnouncements();
void UpdateAssignments();
void LoadAssignments();
void UpdateResources();
void LoadResources();
void ResetResources();
void UpdateSyllabus();
void LoadSyllabus();
void ResetSyllabus();
void LoadSubmittedAssignments();
void SaveSubmittedAssignments();
void UpdateSchedules();
void LoadSchedules();
void ShowAssignments();
void ChangePassword();
void ResetSubAssign();
#define LSA LoadSubmittedAssignments
#define SSA SaveSubmittedAssignments

void Syllabus();
void Schedules();
void Announcements();
void Resources();
void Assignments();
void (*Function[5])() = {Syllabus, Schedules, Announcements, Resources, Assignments};

int TOTAL_ACC = 0, userid, sub, semester, status, subid; //subid always changing for studnets, constant for teachers. status is TCH or STD, semester starts from 0

char Subjects[8][10][50] = {
	{"EE117	Applied Physics", "MT119	Calculus and Analytical Geometry", "SS150	English Composition and Comprehension","SL150	English Composition and Comprehension - Lab","CS118	Programming Fundamentals", "CL118	Programming Fundamentals - Lab","CL117	Intro to Info. and Comm. Technologies", "SS111	Islamic and Religious Studies", "NULL" },
	{"CL103	Computer Programming Lab", "CS103	Computer Programming", "SS122	English Composition", "MT115	Calculus - II", "EL227	Digital Logic Design - Lab", "EE227	Digital Logic Design", "SS113	Pakistan Studies", "NULL" },
	{"EL213	Comp. Organization and Assembly Lang. Lab", "EE213	Comp. Organization and Assembly Lang.", "CS211	Discrete Structures", "CS201	Data Structures", "MT104	Linear Algebra", "CS207	Assembly Language Programming", "NULL" },
	{"CL205	Operating Systems Lab", "CL203	Database Systems Lab", "CS203	Database Systems", "CS302	Design and Analysis of Algorithms", "CS205	Operating Systems", "CS208	Assembly Language Programming", "NULL" },
	{"CL309	Object Oriented Analysis and Design Lab", "CL307	Computer Networks Lab", "CS301	Theory of Automata", "CS307	Computer Networks", "CS309	Object Oriented Analysis and Design", "MT206	Probability and Statistics", "CL101	Introduction to Computing Lab", "NULL" },
	{"CS401	Artificial Intelligence", "CS303	Software Engineering", "SS108	Technical and Business Writing", "CL102	Engineering Surveying - Lab", "NULL" },
	{"CL103	Computer Programming Lab", "CS422	Human Computer Interaction", "CS491	Project - I", "EE204	Computer Architecture", "CS214	Programming Fundamentals", "NULL" },
	{"CS449	Professional Issues in IT", "CS492	Project - II", "CL104	Engineering Drawing - Lab", "CL115	Computer Programming - Lab", "CL114	IT in Business - Lab", "NULL" }
};

const int nos[8] = {8,7,6,6,7,4,5,5}; //number of subjects a semester has

char Announcement[75][10][200];

char Resource[75][100];
char Assignment[75][100];
char SubAssign[75][100];
char syllabus[75][100];
char Schedule[75][100];

int noa[75] = {0}; //Number of announcements a subject id has.

static char * GetSubject(int a)
{
	int b,c;
	b = a/10;
	c = a%10;
	return Subjects[b][c];
}

int GetSubjectNumber(int a, int b) //THis function returns the subject number by getting semester and subject
{
	return (a*10 + b);
}

int exists(char* file)
{
	FILE *fp = fopen(file, "r");
	if(fp != NULL){
		fclose(fp);
		return 1;
	}
	else
	{
		fclose(fp);
		return 0;
	}
}

struct subassign
{
	char link[100];
	int marks;
};
struct Students
{
	char username[25];
	char password[25];
	char fname[10];
	char lname[10];
	char roll[9];
	int sem;
	int subject;
	struct subassign suba[10];
} Std[MAX_ACC];

void CorrectLink(char *c)
{
	int i;
	for(i=0;i<strlen(c); i++)
		if(c[i] == '\\')
			c[i] = '/';
}

int main()
{
	LoadSyllabus();
	LoadResources();
	LoadAnnouncements();
	LoadAccounts();
	LoadAssignments();
	LoadSubmittedAssignments();
	printf("Welcome to SLATE!\n");
	printf("Do you have an account already? If so choose Login! Else register your own\naccount for free.\n1. Login\n2. Register\n3. Exit\n");
	int opt;
	scanf("%d", &opt);
	if(opt == 1)
		AskLogin();
	else if(opt == 2)
		RegisterAccount();
	else if(opt == 3)
		exit(0);
	else printf("Select a valid option.");
}

void DisplayPage(int page) //Page 0 is subjects shown, page 1 is whats to offer in subjects and more to come!
{
	int i, limit;
	if(page == 0)
	{
		int input;
		system("@cls||clear");
		printf("Welcome back, %s %s!               ROLL NO. %s\n", Std[userid].fname, Std[userid].lname, Std[userid].roll);
		printf("1. Log out\n2. Courses\n3. Recent Announcements\n4. Change Password\n");
		scanf("%d", &input);
		if(input == 1) {
			system("@cls||clear");
			main();
		}
		else if(input == 2)
			DisplayPage(1);
		else if(input == 4)
			ChangePassword();
		else
			printf("Under development");
	}
	else if(page == 1)
	{
		system("@cls||clear");
		printf("0. Go Back\n");
		if(status == STD) {
			for(i=0;i<nos[semester]; i++) {
				printf("%d. %s\n", i+1, Subjects[semester][i]);
				limit =i;
			}
		}
		else
			printf("1.%s\n", GetSubject(Std[userid].subject));
		while(1)
		{
			fflush(stdin);
			scanf("%d", &sub);
			if(sub == 0) break;
			else if(status == STD) {
				if(sub >=1 && sub <= nos[semester]) {
					subid = GetSubjectNumber(semester, sub-1);
					break;
				}
			}
			else if(status == TCH)
				if(sub ==1)
					break;
			printf("\nPlease choose a valid option\n");
		}
		if(sub == 0) DisplayPage(0);
		else
			DisplayPage(2);
	}
	else if(page == 2)
	{
		system("@cls||clear");
		int input;
		printf("----------%s--------\n", GetSubject(subid));
		printf("0. Go Back\n1. Syllabus\n2. Schedule\n3. Announcements\n4. Resources\n5. Assignments\n");
		scanf("%d", &input); 
		while(input <0 && input >5 ) {
			printf("\nEnter a valid option, please.\n");
			scanf("%d", &input);
		}
		if(input == 0)
			DisplayPage(1);
		else {
			system("@cls||clear");
			(*Function[input-1])();
	}
	}
}

void Syllabus()
{
	
	printf("----------%s--------\n----------SYLLABUS----------\n", GetSubject(subid));
	int a;
	if(status == STD) //----------------------------STUDENTS-----------------------------------
	{
		printf("0.Go Back\n1.View syllabus\n");
		while(1)
		{
			fflush(stdin);
			scanf("%d", &a);
			if(a>= 0 && a<=1)
				break;
			printf("\nPlease choose a correct option.\n");
		}
		if(a == 0)
			DisplayPage(2);
		else if(a == 1)
		{
			FILE *fp = fopen(syllabus[subid], "r");
			if(fp == NULL)
				printf("Error while opening file\n");
			char c;
			while( (c=fgetc(fp))!= EOF )
			{
				printf("%c",c);
			}
			printf("\nPress any key to continue");
			getch();
			system("@cls||clear");
			Syllabus();
		}
	}
	else
	{
		printf("0.Go Back\n1.Add syllabus\n");
		while(1)
		{
			fflush(stdin);
			scanf("%d", &a);
			if(a>= 0 && a<=1)
				break;
			printf("\nPlease choose a correct option.\n");
		}
		if(a == 0)
			DisplayPage(2);
		else if(a == 1)
		{
			printf("Enter the link to the syllabus:\n");
			fflush(stdin);
			while(1)
			{
				gets(syllabus[subid]);
				CorrectLink(syllabus[subid]);
				if(exists(syllabus[subid]))
					break;
				printf("\nLink entered is incorrect, enter again\n");
			}
			system("@cls||clear");
			printf("syllabus Added Successful!\n");
			UpdateSyllabus();
			Syllabus();
		}
	}
}

void Schedules()
{
	printf("----------%s--------\n----------SCHEDULES----------\n", GetSubject(subid));
	int a;
	if(status == STD) //----------------------------STUDENTS-----------------------------------
	{
		printf("0.Go Back\n1.View Schedules\n");
		while(1)
		{
			fflush(stdin);
			scanf("%d", &a);
			if(a>= 0 && a<=1)
				break;
			printf("\nPlease choose a correct option.\n");
		}
		if(a == 0)
			DisplayPage(2);
		else if(a == 1)
		{
			if(strlen(Schedule[subid]) == 0)
				printf("No schedule available for this course yet.\n");
			else
			{
				FILE *fp = fopen(Schedule[subid], "r");
				if(fp == NULL)
					printf("Error while opening file\n");
				char c;
				while( (c=fgetc(fp))!= EOF )
				{
					printf("%c",c);
				}
			}
			printf("\nPress any key to continue");
			getch();
			system("@cls||clear");
			Schedules();
		}
	}
	else
	{
		printf("0.Go Back\n1.Add Schedules\n");
		while(1)
		{
			fflush(stdin);
			scanf("%d", &a);
			if(a>= 0 && a<=1)
				break;
			printf("\nPlease choose a correct option.\n");
		}
		if(a == 0)
			DisplayPage(2);
		else if(a == 1)
		{
			printf("Enter the link to the schedule:\n");
			fflush(stdin);
			gets(Schedule[subid]);
			system("@cls||clear");
			printf("Schedule Added Successfully!\n");
			UpdateSchedules();
			Schedules();
		}
	}
}

void Announcements()
{
	int i;
	printf("----------%s--------\n----------ANNOUNCEMENTS----------\n", GetSubject(subid));
	if(Std[userid].subject == -1) // ``````````````STUDENTS`````````````````````
	{
		if(noa[subid] == 0)
			printf("There are no announcements yet.\n");
		else
			for(i=0;i<noa[subid];i++)
				printf("%d.%s\n", i+1,Announcement[subid][i]);
		printf("Press any key to go back");
		getch();
		system("@cls||clear");
		DisplayPage(2);
	}
	else // `````````````````````TEACHERS`````````````````````
	{
		printf("0.Go Back\n1.View Announcements\n2.Add Announcements\n3.Delete Announcements\n");
		int inp;
		while(1) {
			fflush(stdin);
			scanf("%d", &inp);
			if(inp >= 0 && inp <=4)
				break;
			else
				printf("\nPlease choose a valid option\n");
		}
		if(inp == 0)
			DisplayPage(2);
		else if(inp == 1)   // --------------VIEW ANNOUNCEMENTS---------------
		{
			if(noa[subid] == 0)
				printf("There are no announcements yet.\n");
			else
				for(i=0;i<noa[subid];i++)
					printf("%d.%s\n", i+1,Announcement[subid][i]);
			printf("Press any key to go back");
			getch();
			system("@cls||clear");
			Announcements();
		}
		else if(inp == 2)// --------------ADD ANNOUNCEMENTS---------------
		{
			if(noa[subid] == 10) {
				system("@cls||clear");
				printf("Max announcements 10 allowed, delete one in order to add more\n");
				Announcements();
			}
			else
			{
				printf("Enter the announcement:\n");
				char c[200];
				fflush(stdin);
				gets(c);
				strcpy(Announcement[subid][noa[subid]], c);
				noa[subid] ++;
				system("@cls||clear");
				printf("Announcement added successfully\n");
				UpdateAnnouncements();
				Announcements();
			}
		}
		else if(inp ==3) //--------------------DELETE ANNOUNCEMENTS-----------------	
		{
			if(noa[subid] == 0)
			{
				system("@cls||clear");
				printf("There are no announcements to be deleted.\n");
				Announcements();
			}
			else
			{
				int i, check =0;
				printf("\n\nSelect announcement you want to delete:\n");
				for(i=0;i<noa[subid];i++)
					printf("\n%d.%s", i+1, Announcement[subid][i]);
				int a;
				while(1)
				{
					fflush(stdin);
					scanf("%d", &a);
					if(a >= 1 && a<=noa[subid])
						break;
					else
						printf("\nInvalid option, please choose a valid one.\n");
				}
				a--;
				if(a+1== noa[subid])
					Announcement[subid][a][0] = '\0';
				else
				{
					for(i=a;i<noa[subid]-1;i++)
					{
						strcpy(Announcement[subid][i], Announcement[subid][i+1]);
						Announcement[subid][i+1][0] = '\0';
					}
				}
				noa[subid] --;
				system("@cls||clear");
				printf("Announcement deleted succesfully!\n");
				Announcements();

			}
			
		}
	}
	
}
void Resources()
{
	printf("----------%s--------\n----------RESOURCES----------\n", GetSubject(subid));
	int a;
	if(status == STD) //----------------------------STUDENTS-----------------------------------
	{
		printf("0.Go Back\n1.View Resources\n");
		while(1)
		{
			fflush(stdin);
			scanf("%d", &a);
			if(a>= 0 && a<=1)
				break;
			printf("\nPlease choose a correct option.\n");
		}
		if(a == 0)
			DisplayPage(2);
		else if(a == 1)
		{
			if(strlen(Resource[subid]) != 0)
			{
				FILE *fp = fopen(Resource[subid], "r");
				if(fp == NULL)
					printf("Error while opening file\n");
				char c;
				while( (c=fgetc(fp))!= EOF )
				{
					printf("%c",c);
				}
				printf("\nPress any key to continue");
				getch(); 
				system("@cls||clear");
				Resources();
			}
			else
			{
				printf("There are no resources available\nPress any key to continue");
				getch(); 
				system("@cls||clear");
				Resources();
			}
		}
	}
	else  //--------------------TEACHERS---------------
	{
		printf("0.Go Back\n1.Add Resources\n");
		while(1)
		{
			fflush(stdin);
			scanf("%d", &a);
			if(a>= 0 && a<=1)
				break;
			printf("\nPlease choose a correct option.\n");
		}
		if(a == 0)
			DisplayPage(2);
		else if(a == 1)
		{
			printf("Enter the link to the resources:\n");
			fflush(stdin);
			while(1)
			{
				fflush(stdin);
				gets(Resource[subid]);
				CorrectLink(Resource[subid]);
				if(exists(Resource[subid]))
					break;
				printf("\nLink entered is incorrect, enter again\n");
			}
			system("@cls||clear");
			printf("Resources Added Successful!\n");
			UpdateResources();
			Resources();
		}
	}
}
void Assignments()
{
	printf("----------%s--------\n----------ASSIGNMENTS----------\n", GetSubject(subid));
	int a;
	if(status == STD) //----------------------------STUDENTS-----------------------------------
	{
		printf("0.Go Back\n1.View Assignment\n2.Submit Assignment\n3.View Result Of Last Assignment\n");
		while(1)
		{
			fflush(stdin);
			scanf("%d", &a);
			if(a>= 0 && a<=3)
				break;
			printf("\nPlease choose a correct option.\n");
		}
		if(a == 0)
			DisplayPage(2);
		else if(a == 1)
		{
			if(strlen(Assignment[subid]) == 0)
				printf("There are no assignments for this subject yet\n");
			else
			{
				FILE *fp = fopen(Assignment[subid], "r");
				if(fp == NULL)
					printf("Error while opening file %s\n", Assignment[subid]);
				char c;
				while( (c=fgetc(fp))!= EOF )
				{
					printf("%c",c);
				}
				fclose(fp);
			}
			printf("\nPress any key to continue");
			getch();
			system("@cls||clear");
			Assignments();
		}
		else if(a==2)
		{
			if(strlen(Assignment[subid]) == 0)
				printf("There are not assignments for this course yet!\n");
			else
			{
				printf("Enter the link for the file you want to submit:\n");
				fflush(stdin);
				while(1)
				{
					gets(Std[userid].suba[sub-1].link);
					CorrectLink(Std[userid].suba[sub].link);
					if(exists(Std[userid].suba[sub-1].link))
						break;
					printf("\nThere was a problem with your link, enter again a different one\n");
				}
				SaveSubmittedAssignments();
				printf("Assignment submitted successfully!\n");
			}
			system("@cls||clear");
			Assignments();
		}
		else if(a==3)
		{
			if(Std[userid].suba[subid%10].marks == -1)
				printf("Assignment not marked yet!\n");
			else
				printf("Marks obtained: %d", Std[userid].suba[subid%10].marks);
			printf("\nPress any key to continue\n");
			getch();
			Assignments();
		}
	}
	else //--------------------TEACHERS---------------
	{
		printf("0.Go Back\n1.Add Assignment\n2.View submitted Assignments\n");
		while(1)
		{
			fflush(stdin);
			scanf("%d", &a);
			if(a>= 0 && a<=2)
				break;
			printf("\nPlease choose a correct option.\n");
		}
		if(a == 0)
			DisplayPage(2);
		else if(a == 1)
		{
			printf("Enter the link to the assignment:\n");
			fflush(stdin);
			while(1)
			{
				gets(Assignment[subid]);
				if(exists(Assignment[subid]))
					break;
				printf("\nEntered link is not correct, re-enter.\n");
			}
			printf("Assignement Added Successful!\n");
			system("@cls||clear");
			UpdateAssignments();
			Assignments();
		}
		else if(a == 2)
		{
			ShowAssignments();
		}
	}
}

void ShowAssignments()
{
	int i,j, userids[MAX_ACC] = {0},count=0;
	for(i=1;i<=TOTAL_ACC;i++)
	{
		if(Std[i].sem == subid/10)
		{
			if(strcmp(Std[i].suba[subid%10].link, "NULL"))
			{
				count ++;
				if(Std[i].suba[subid%10].marks != -1)
					printf("%d.%-10s %-10s %-10s Marks given:%-5d\n", count, Std[i].roll, Std[i].fname, Std[i].lname, Std[i].suba[subid%10].marks);
				else	
					printf("%d.%-10s %-10s %-10s [UNMARKED]\n", count, Std[i].roll, Std[i].fname, Std[i].lname);
				userids[count] = i;
			}
		}
	}
	if(count != 0)
	{
		printf("0.Go Back\n");
		int inp;
		printf("\nChoose student to mark their assignment subid: %d\n", subid);
		while(1) {
			fflush(stdin);
			scanf("%d", &inp);
			if(inp >= 0 && inp <=count)
				break;
			printf("\nSelect a valid option please.\n");
		}
		if(inp == 0) {
			system("@cls||clear");
			Assignments();
		}
		else
		{
			system("@cls||clear");
			char c;
			FILE *fp = fopen(Std[userids[count]].suba[subid%10].link, "r");
			if(fp == NULL)
				printf("unable to open: %s\n",Std[userids[inp]].suba[subid%10].link);
			while( (c=fgetc(fp))!= EOF )
			{
				printf("%c",c);
			}
			printf("\n\n\nMark this assignment:\n");
			int mark;
			while(1)
			{
				fflush(stdin);
				scanf("%d", &mark);
				if(inp >=1 && mark <=100)
					break;
				printf("\nEnter valid marks i.e 1-100!\n");
			}
			Std[userids[inp]].suba[subid%10].marks = mark;
			SaveSubmittedAssignments();
			system("@cls||clear");
			printf("Marked assignment successfully!\n");
			ShowAssignments();
		}
	}
	else{
		printf("No assignments available\nPress any key to continue\n");
		getch();
		system("@cls||clear");
		Assignments();
	}
}

void GetPass(char *pass)
{
	int i=0;
	char c;
	
	while( (c=getch())!= 13) {
		if ((c == 127 || c == 8))
		{
			if(i == 0)
				continue;
			int j;
			i--;
			pass[i] = '\0';
			printf("\r");
			for(j=0;j<i+2;j++) printf(" ");
			printf("\r");
			for(j=0;j<i;j++) printf("*");
			continue;
		}
       	pass[i] = c;
      	printf("*");
       	i++;
       	pass[i] = '\0';
	}
}

void ChangePassword()
{
	char pass [25];
	system("@cls||clear");
	printf("Enter Current Password: \n");
	while(1)
	{
		GetPass(pass);
		if(!strcmp(pass, Std[userid].password))
			break;
		else
			printf("\nInvalid password, enter again.\n");
	}
	char newpass[25];
	printf("\nEnter New Password: \n") ;
	GetPass(newpass);
	strcpy(Std[userid].password,newpass);
	UpdateAccounts();
	printf("\nPassword Change Successful\n");
	printf("\npress any key to continue...\n");
	getch();
	system("@cls||clear");
	main();
}

void RegisterAccount()
{
	TOTAL_ACC++;
	char username[25], name[25], roll[9];
	int opt;
	system("@cls||clear");
	printf("1. Registration for Teachers\n2. Registration for Students\n");
	while(1)
	{
		fflush(stdin);
		scanf("%d", &opt);
		if((opt == 1 || opt == 2))
			break;
		printf("\nPlease enter a valid option.\n");
	}
	printf("Enter Your Username you will be using:\n");
	scanf(" %s",username);
	while(CheckUser(username))
	{
		printf("This username already exists, please enter a different one again!\n");
		scanf("%s", username);
	}
	strcpy(Std[TOTAL_ACC].username, username);
	printf("\nEnter the password:\n");
	GetPass(Std[TOTAL_ACC].password);
	printf("\nEnter your first name:\n");
	scanf("%s", Std[TOTAL_ACC].fname);
	printf("Enter your last name:\n");
	scanf("%s", Std[TOTAL_ACC].lname);
	if(opt == 2)  //-------------STUDENTS----------------
	{
		printf("\nEnter your semester:\n");
		scanf("%d", &Std[TOTAL_ACC].sem);
		while(Std[TOTAL_ACC].sem <1 || Std[TOTAL_ACC].sem > 9) {
			printf("\nEnter a valid semester please. (1-8)\n");
			scanf("%d", &Std[TOTAL_ACC].sem);
		}
		printf("Enter your roll number [Format example: 18K-1254]:\n ");
		scanf("%s", Std[TOTAL_ACC].roll);
		while(!(isValidRoll(Std[TOTAL_ACC].roll, 0) && isValidRoll(Std[TOTAL_ACC].roll, 1))) {
			printf("\nThis roll number already exists or is in invalid format. Correct example: 18K-1254\n");
			scanf("%s", Std[TOTAL_ACC].roll);
		}
		Std[TOTAL_ACC].subject = -1;
	}
	else //-------------TEACHERS----------------
	{
		int i,a, limit;
		printf("Enter the semester you will be teaching:\n");
		scanf("%d", &Std[TOTAL_ACC].sem);
		while(Std[TOTAL_ACC].sem <1 || Std[TOTAL_ACC].sem >=9) {
			printf("\nEnter a valid semester please. (1-8)\n");
			scanf("%d", &Std[TOTAL_ACC].sem);
		}
		for(i=0;i<nos[Std[TOTAL_ACC].sem-1]; i++) {
			printf("%d. %s\n", i+1, Subjects[Std[TOTAL_ACC].sem-1][i]);
			limit = i;
		} 
		while(1)
		{
			fflush(stdin);
			scanf("%d", &a);
			if(a>=1 && a<=nos[Std[TOTAL_ACC].sem])
				break;
			printf("\nPlease enter a valid option\n");
		}
		int x = (Std[TOTAL_ACC].sem -1)*10 + (a-1);
		Std[TOTAL_ACC].subject = x;
		printf("Enter your roll number [Format example: 18K-1254]:\n ");
		scanf("%s", Std[TOTAL_ACC].roll);
		while(!(isValidRoll(Std[TOTAL_ACC].roll, 0) && isValidRoll(Std[TOTAL_ACC].roll, 1))) {
			printf("\nThis roll number already exists or is in invalid format. Correct example: 18K-1254\n");
			scanf("%s", Std[TOTAL_ACC].roll);
		}
	}
	int x;
	for(x=0;x<10;x++)
		Std[TOTAL_ACC].suba[x].marks = -1; 
	Std[TOTAL_ACC].sem--;
	UpdateAccounts();
	system("@cls||clear");
	printf("Registration successfull!\n");
	main();
}

int isValidRoll(char *roll, int mode)
{
	if(mode == 0)
	{
		int i;
		for(i=1;i<=TOTAL_ACC;i++)
			if(i!=TOTAL_ACC)
				if(!strcasecmp(roll, Std[i].roll))
				return 0;
		return 1;
	}
	else if(mode == 1)
	{
		if(roll[0] == '1' && isNum(roll[1])&& (roll[2] == 'K' || roll[2] == 'k') && roll[3] == '-'&& isNum(roll[4])&& isNum(roll[5])&& isNum(roll[6])&& isNum(roll[7]))
			return 1;
		else 
			return 0;
	}
	
}

int isNum(int a) {
	if(a>=48 && a <=57) return 1;
	else return 0;
}

void AskLogin()
{
	char us[25];
	printf("Enter your username\n");
	scanf("%s", us);
	userid = CheckUser(us);
	while(!userid)
	{
		printf("\nNo such username exists, enter a different one. OR Enter 1 to go back.\n");
		scanf("%s", us);
		userid = CheckUser(us);
		if(strlen(us) == 1 && us[0] == '1') {
			main();
			break;
		}
	}
	char c, pass[25];
	printf("Enter your password:\n");
	while(1) {
		GetPass(pass);
		if(!strcmp(pass, Std[userid].password))
			break;
		else
			printf("\nInvalid password, enter again.\n");
	}
	if(Std[userid].subject == -1) 
		status = STD;
	else
	{			
		status = TCH;
		subid = Std[userid].subject;
	}
	semester = Std[userid].sem;
		DisplayPage(0);
}


void LoadAccounts()
{
	int i;
	FILE *data = fopen(PATH1, "r");
	if(data == NULL) printf("NO PREVIOUS RECORD FOUND\n");
	fscanf(data,"%d", &TOTAL_ACC);
	for(i=1;i<=TOTAL_ACC;i++)
	{
		fscanf(data, "%s %s %d %s %s %s %d\n", Std[i].username, Std[i].password,&Std[i].sem, Std[i].fname, Std[i].lname, Std[i].roll, &Std[i].subject);
	}
	fclose(data);
}

void UpdateAccounts()
{
	int i;
	FILE *data = fopen(PATH1, "w");
	if(data == NULL) printf("Error occured while writing the file");
	fprintf(data, "%d\n", TOTAL_ACC);
	for(i=1;i<=TOTAL_ACC;i++)
	{
		fprintf(data,"%-15s %-15s %-10d %-10s %-10s %-10s %-3d\n", Std[i].username, Std[i].password,Std[i].sem, Std[i].fname, Std[i].lname, Std[i].roll, Std[i].subject);
	}
	fclose(data);
}

void LoadAnnouncements()
{
	int i;
	FILE *ann = fopen("announcements.txt", "r");
	for(i=0;i<=76;i++)
		fscanf(ann, "%d ", &noa[i]);
	for(i=0;i<77;i++)
	{
		if(noa[i] != 0)
		{
			int j;
			for(j=0;j<noa[i];j++)
			{
				fgets(Announcement[i][j], 200, ann);
			}
		}
	}
}

void UpdateAnnouncements()
{
	int i;
	FILE *ann = fopen("announcements.txt", "w");
	if(ann == NULL) printf("Error occured while writing the file");
	for(i=0;i<=76;i++)
		fprintf(ann, "%d ", noa[i]);
	fprintf(ann,"\n");
	for(i=0;i<77;i++)
	{
		if(noa[i] >0)
		{
			int j;
			for(j=0;j<noa[i];j++)
				fprintf(ann,"%s\n", Announcement[i][j]);
		}
	}
	fclose(ann);
}

void UpdateAssignments()
{
	FILE *fp = fopen("assignments.txt", "w");
	int i;
	for(i=0;i<77;i++){
		if(strlen(Assignment[i]))
			fprintf(fp, "1 ");
		else
			fprintf(fp, "0 ");
	}
	for(i=0;i<77;i++)
	{
		if(strlen(Assignment[i]) != 0)
			fprintf(fp, "\n%s", Assignment[i]);
	}
	fclose(fp);
}

void LoadAssignments()
{
	FILE *fp = fopen("assignments.txt", "r");
	int i, nor[77];
	for(i=0;i<77;i++)
		fscanf(fp, "%d", &nor[i]);
	for(i=0;i<77;i++)
		if(nor[i] == 1)
			fscanf(fp, "%s", Assignment[i]);
	fclose(fp);
}

void UpdateResources()
{
	FILE *fp = fopen("resources.txt", "w");
	int i;
	for(i=0;i<77;i++)
		if(strlen(Resource[i]))
			fprintf(fp, "1 ");
		else
			fprintf(fp, "0 ");
	for(i=0;i<77;i++)
	{
		if(strlen(Resource[i]) != 0)
			fprintf(fp, "\n%s", Resource[i]);
	}
	fclose(fp);
}

void LoadResources()
{
	FILE *fp = fopen("resources.txt", "r");
	int i, nor[77];
	for(i=0;i<77;i++)
		fscanf(fp, "%d", &nor[i]);
	for(i=0;i<77;i++)
		if(nor[i] == 1)
			fscanf(fp, "%d", Resource[i]);
	fclose(fp);
}

void UpdateSchedules()
{
	FILE *fp = fopen("schedules.txt", "w");
	int i;
	for(i=0;i<77;i++)
		if(strlen(Schedule[i]))
			fprintf(fp, "1 ");
		else
			fprintf(fp, "0 ");
	for(i=0;i<77;i++)
	{
		if(strlen(Schedule[i]) != 0)
			fprintf(fp, "\n%s", Schedule[i]);
	}
	fclose(fp);
}

void LoadSchedules()
{
	FILE *fp = fopen("schedules.txt", "r");
	int i, nor[77];
	for(i=0;i<77;i++)
		fscanf(fp, "%d", &nor[i]);
	for(i=0;i<77;i++)
		if(nor[i] == 1)
			fscanf(fp, "%d", Schedule[i]);
	fclose(fp);
}

void UpdateSyllabus()
{
	FILE *fp = fopen("syllabus.txt", "w");
	int i;
	for(i=0;i<77;i++)
		if(strlen(syllabus[i]))
			fprintf(fp, "1 ");
		else
			fprintf(fp, "0 ");
	for(i=0;i<77;i++)
	{
		if(strlen(syllabus[i]) != 0)
			fprintf(fp, "\n%s", syllabus[i]);
	}
	fclose(fp);
}

void LoadSyllabus()
{
	FILE *fp = fopen("syllabus.txt", "r");
	int i, nor[77];
	for(i=0;i<77;i++)
		fscanf(fp, "%d", &nor[i]);
	for(i=0;i<77;i++)
		if(nor[i] == 1)
			fscanf(fp, "%d", syllabus[i]);
	fclose(fp);
}

void ResetSubAssign()
{
	int i;
	for(i=1;i<=TOTAL_ACC;i++)
	{
		int j;
		for(j=0;j<nos[Std[i].sem];j++){
			strcpy(Std[i].suba[j].link, "NULL");
			Std[i].suba[j].marks = -1;
		}
	}
}

void SaveSubmittedAssignments()
{
	int i;
	FILE *fp = fopen("subassign.txt", "w");
	if(fp != NULL)
	{
		for(i=1;i<=TOTAL_ACC;i++)
		{
			int j;
			for(j=0;j<nos[Std[i].sem];j++)
				fprintf(fp,"%s ", Std[i].suba[j].link);
			fprintf(fp,"\n");
			for(j=0;j<nos[Std[i].sem];j++)
				fprintf(fp,"%d ", Std[i].suba[j].marks);
			fprintf(fp,"\n");
		}
	}
	fclose(fp);
}

void LoadSubmittedAssignments()
{
	ResetSubAssign();
	int i;
	FILE *fp = fopen("subassign.txt", "r");
	if(fp != NULL)
	{
		for(i=1;i<=TOTAL_ACC;i++)
		{
			int j;
			for(j=0;j<nos[Std[i].sem];j++)
				fscanf(fp,"%s", Std[i].suba[j].link);
			for(j=0;j<nos[Std[i].sem];j++)
				fscanf(fp,"%d", &Std[i].suba[j].marks);	
		}
	}
	fclose(fp);
}

int CheckUser(char *name)
{
	int i;
	for(i=1;i<=TOTAL_ACC;i++)
	{
		if(!strcasecmp(Std[i].username, name))
			return i;
	}
	return 0;
}
