#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>   // Including all variables

void book_room();
void search_menu();
void show_userinfo(int);
void all_bookings();
void back();
void reciept_show(int);
void change_choise();
void search_choise();
void error();			//function prototyping

struct date			//defining structures
{
	int day;
	int month;
	int year;
};

struct user_info {
	char roomID[9];
	char dates[50];
	char name[50];
	char IC[20];
	char connum[30];
	char email[40];
	int days[1];
};

const char* date_check(char* p, char* q)		//this function will ask input date and check it for valid format, then return it
{
	int datein[3], dateout[3], count = 0, daysint;
	char checkindate[10], checkoutdate[10], returnchar[30], days[4];
	while (1)
	{

		printf("\nEnter check-in-date(DD/MM/YY):");
		scanf("%s", checkindate);
		if (checkindate[0] == '0' && strlen(checkindate) == 1)
			return "0";
		if (checkdateformat(checkindate, &datein) == 1)
			break;

	}
	while (1)
	{
		printf("Enter check-out-date(DD/MM/YY):");
		scanf("%s", checkoutdate);
		if (checkoutdate[0] == 0 && strlen(checkoutdate) == 1)
			return "0";
		if (checkdateformat(checkoutdate, &dateout) == 1)
		{
			if (checkinandout(datein, dateout) == 1)
				break;
		}
	}
	daysint = calculatedays(datein, dateout);
	_itoa(daysint, days, 10);
	for (int i = 0; i < 9; i++)
	{
		*(p + count) = checkindate[i];
		count++;

	}

	*(p + count) = '\0';
	count = 0;
	for (int i = 0; i < 9; i++)
	{
		*(q + count) = checkoutdate[i];
		count++;
	}
	*(q + count) = '\0';
	count = 0;
	for (int i = 0; days[i]; i++)
	{
		returnchar[count] = days[i];
		count++;
	}
	returnchar[count] = '\0';
	return returnchar;
}

const char* datafromline(int parameter, char line[])		//this function is taking data from line which is line[] 
{
	int  colnum1 = -1, colnum2, lastchar = 0, outchcount = 0;		// which data it will take, depends on "parameter"
	char outch[50];
	for (int i = 0; i < parameter; i++)
	{
		colnum2 = colnum1;
		colnum1 = colSearch2(line, lastchar);
		lastchar = colnum1 + 1;
	}

	for (int i = colnum2 + 1; i < colnum1; i++)
	{
		outch[outchcount] = line[i];
		outchcount++;
	}
	outch[outchcount] = '\0';
	return outch;

} 

const char* get_user_line(int n)  // this function is taking user line from file. line is under number n.
{
	int count = 0;
	FILE* userinfo;
	userinfo = fopen("login_info.txt", "r");
	while (!(feof(userinfo)))
	{
		char line[1000];
		if (fgets(line, 1000, userinfo) != NULL)
		{
			if (count == n)
			{
				fclose(userinfo);
				return line;
			}
			count++;
		}
	}
	fclose(userinfo);
}

void main()  //main function
{
	int menu_num, x = 1;
	do
	{

		menu_num = nextmenu();
		switch (menu_num)
		{
		case 1:	rooms_choise(); break;
		case 2: book_room(); break;
		case 3:	search_choise(); break;
		case 4: change_choise(); break;
		case 5: reciepts(); break;
		case 0: exit(1); break;
		default:
			printf("Something went wrong, please try again");
		}
	} while (x = 1);


}

int nextmenu()  // firstly prints available pages. then waits for user to write his choice 
{
	system("cls");
	int input1;
	char inpt2[30];
	printf("\n\t\t|| Welcome! ||\n\nTo navigate use numbers:\n\n\t|1| Show rooms\n\t|2| Book room\n\t|3| Search bookings\n\t|4| Change booking details\n\t|5| Show reciept\n\n\t|0| Exit\n\n>>>");
	scanf("%s", &inpt2);
	input1 = atoi(inpt2);
	if (inpt2[0] != '0')
		if (input1 == 0)
			return 8;
	return input1;

}

void search_choise()  // firstly prints available pages. then waits for user to write his choice 
{
	while (1)
	{
		system("cls");
		char choise[2];
		printf("\n\t|1| Show all bookings\n\t|2| Search by booking details\n\n\t|9| Go back\n\t|0| Exit\n>>>");
		scanf("%s", &choise);
		if (choise[0] == '1')
		{
			all_bookings();
			back();
		}
		else if (choise[0] == '2')
		{
			system("cls");
			search_menu(1);
		}
		else if (atoi(choise) == 9)
			break;
		else if (choise[0] == '0')
			exit(0);

	}

}

int rooms_choise()			// firstly prints available pages. then waits for user to write his choice 
{
	int x = 0;
	do
	{
		system("cls");
		int show_all[11] = { 1,1,1,1,1,1,1,1,1,1,1 };
		char c[1000], inptc[2];
		printf("\n\t|1| Show all rooms\n\t|2| Filter by price\n\n\t|9| Go back\n\t|0| Exit\n\n>>>");
		scanf("%s", &inptc);
		if (inptc[0] == '1')
		{
			printf("All rooms are shown below:\n");
			all_rooms(show_all);
			back();
		}
		else if (inptc[0] == '2')
			filter_price();
		else if (atoi(inptc) == 9)
			x = 1;
		else if (inptc[0] == '0')
			exit(0);

	} while (x == 0);

	return 0;
}

int all_rooms(int show[]) // opens the file and shows rooms according to parameter array show[]
{
	int count = 1;
	system("cls");
	FILE* f;
	f = fopen("rooms.txt", "r");
	printf("\n\t#\t|RoomID\t\t|Room Type\t\t|Price Per Day\n\n");
	while (!feof(f))
	{
		char line[1000], roominfo[50];
		if (fgets(line, 1000, f) != NULL)
		{
			if (show[count] == 1)
			{
				printf("\t%d", count);
				strcpy(roominfo, datafromline(1, line));
				printf("\t%s", roominfo);
				strcpy(roominfo, datafromline(2, line));
				printf("\t\t%-10s", roominfo);
				strcpy(roominfo, datafromline(3, line));
				printf("\t%20s\n", roominfo);
			}
			count++;
		}
	}
	printf("\n");
	fclose(f);

}

void back()  // just waits for user to press enter or any input
{
	char temp[20];
	printf("Press enter to go back");
	fseek(stdin, 0, SEEK_END);
	gets(temp);
}

int filter_price()   //will ask user to enter min and max prices and then only show rooms under this prie
{
	system("cls");
	int minprice, line = 0, maxprice, lines[10], count = 1;
	char minpricec[4], maxpricec[4];
	while (1)
	{
		printf("Enter a min price: ");
		scanf("%s", &minpricec);
		printf("Enter a max price: ");
		scanf("%s", &maxpricec);
		minprice = atoi(minpricec);
		maxprice = atoi(maxpricec);
		if (minprice > maxprice)
			printf("Minimal price cannot be greater than maximum price. Please try again.\n");
		else if (maxprice > minprice)
			break;
		else
			error();

	}
	system("cls");
	printf("\n\t#\t|RoomID\t\t|Room Type\t\t|Price Per Day\n\n");
	FILE* f;
	f = fopen("rooms.txt", "r");
	while (!feof(f))
	{
		char line[1000], roominfo[50], temp[4];
		int number;
		if (fgets(line, 1000, f) != NULL)
		{
			strcpy(roominfo, datafromline(3, line));
			temp[0] = roominfo[2];
			temp[1] = roominfo[3];
			temp[2] = roominfo[4];
 			number = atoi(temp);
			if (number >= minprice && number <= maxprice)
			{
				printf("\t%d", count);
				strcpy(roominfo, datafromline(1, line));
				printf("\t%s", roominfo);
				strcpy(roominfo, datafromline(2, line));
				printf("\t\t%-10s", roominfo);
				strcpy(roominfo, datafromline(3, line));
				printf("\t%20s\n", roominfo);
				count++;
			}
			
		}

	}
	back();
}

void all_bookings() // shows all bokings made
{
	system("cls");
	printf("  #   \tIC/pasport\t\tName\t\tRoomID\t\tDays of Stay\t\tPhone Number\t\tEmail%-12CCheck-in\t\tCheck-out\n");
	for (int i = 0; i < linecount(); i++)
		show_userinfo(i);
}

void book_room()  // book room page. will ask user to enter data, then validate it and if everything is correct, write it into the file
{
	int run = 1;
	while (run == 1)
	{
		struct user_info cu1;
		int allR[11] = { 1,1,1,1,1,1,1,1,1,1,1 }, passing;
		char datein[30], dateout[30];
		all_rooms(allR);
		printf("!Enter 0 anywhere to exit booking menu!\n\n");
		fseek(stdin, 0, SEEK_END);

		while (1)
		{
			printf("Enter roomID: ");
			gets(cu1.roomID);
			passing = atoi(cu1.roomID);
			if (passing > 200)
				passing -= 100;
			if (inp_check1(cu1.roomID, "123450") == 1)
				if (passing > 100 && passing < 106)
					break;
			if (cu1.roomID[0] == '0' && strlen(cu1.roomID) == 1)
			{
				run = 0;
				break;
			}
			error();
		}
		if (run == 0)
			break;
		while (1)
		{
			strcpy(cu1.dates, date_check(&datein, &dateout));
			if (cu1.dates[0] == '0' && strlen(cu1.dates) == 1)
			{
				run = 0;
				break;
			}
			if (inp_check1(cu1.dates, "1234567890%/") == 1)
				if (avlb_check(datein, dateout, cu1.roomID) == 1)
					break;
			search(3, cu1.roomID, 1, &datein);
			printf("This room is already booked for this date. Please try another date. \n");

		}
		if (run == 0)
			break;
		while (1)
		{
			printf("Enter IC/passport number: ");
			fseek(stdin, 0, SEEK_END);
			gets(cu1.IC);
			if (cu1.IC[0] == '0' && strlen(cu1.IC) == 1)
			{
				run = 0;
				break;
			}
			if (inp_check1(cu1.IC, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890") == 1)
				break;

			error();
		}
		if (run == 0)
			break;
		while (1)
		{
			printf("Enter full name as in IC/passport: ");
			gets(cu1.name);
			if (cu1.name[0] == '0' && strlen(cu1.name) == 1)
			{
				run = 0;
				break;
			}
			if (inp_check1(cu1.name, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM- ") == 1)
				break;
			error();
		}
		if (run == 0)
			break;
		while (1)
		{
			printf("Enter phone number: ");
			gets(cu1.connum);
			if (cu1.connum[0] == '0' && strlen(cu1.connum) == 1)
			{
				run = 0;
				break;
			}
			if (inp_check1(cu1.connum, "+1234567890()- ") == 1)
				break;
			error();
		}
		if (run == 0)
			break;
		while (1)
		{
			printf("Enter email : ");
			gets(cu1.email);
			if (cu1.email[0] == '0' && strlen(cu1.email) == 1)
			{
				run = 0;
				break;
			}
			if (inp_check1(cu1.email, "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890_.@") == 1)
				break;
			error();
		}
		if (run == 0)
			break;
		FILE* guestinfo;
		guestinfo = fopen("login_info.txt", "a+");
		fprintf(guestinfo, "%s:%s:%s:%s:%s:%s:%s:%s:\n", cu1.IC, cu1.name, cu1.roomID, cu1.dates, cu1.connum, cu1.email, datein, dateout);
		fclose(guestinfo);
		run = 0;
		back();
	}

}

int avlb_check(char dayin[], char dayout[], char roomID[])  // checkis if room is available for entered dates
{
	int bookedroomline[40], datein1[3], dateout1[3], dayin1[3], dayout1[3], daysbw, daysbw1, x = 0, count = 0;
	char datein[20], dateout[20], line[1000], temp[5], daysin[5];
	if (search(3, roomID, 3, &bookedroomline) == 1)
	{
		while (1)
		{
			strcpy(line, get_user_line(bookedroomline[count]));
			strcpy(datein, datafromline(7, line));
			strcpy(dateout, datafromline(8, line));
			strcpy(daysin, datafromline(4, line));

			temp[0] = dayin[0];
			temp[1] = dayin[1];
			dayin1[0] = atoi(temp);
			temp[0] = dayin[3];
			temp[1] = dayin[4];
			dayin1[1] = atoi(temp);
			temp[0] = dayin[6];
			temp[1] = dayin[7];
			dayin1[2] = atoi(temp);

			temp[0] = datein[0];
			temp[1] = datein[1];
			datein1[0] = atoi(temp);
			temp[0] = datein[3];
			temp[1] = datein[4];
			datein1[1] = atoi(temp);
			temp[0] = datein[6];
			temp[1] = datein[7];
			datein1[2] = atoi(temp);

			temp[0] = dayout[0];
			temp[1] = dayout[1];
			dayout1[0] = atoi(temp);
			temp[0] = dayout[3];
			temp[1] = dayout[4];
			dayout1[1] = atoi(temp);
			temp[0] = dayout[6];
			temp[1] = dayout[7];
			dayout1[2] = atoi(temp);

			temp[0] = dateout[0];
			temp[1] = dateout[1];
			dateout1[0] = atoi(temp);
			temp[0] = dateout[3];
			temp[1] = dateout[4];
			dateout1[1] = atoi(temp);
			temp[0] = dateout[6];
			temp[1] = dateout[7];
			dateout1[2] = atoi(temp);


			daysbw = (calculatedays(datein1, dayin1));
			daysbw1 = (calculatedays(dayin1, dateout1));
			if (daysbw >= 0 && daysbw1 >= 0)
				return 0;
			daysbw1 = (calculatedays(dayout1, datein1));
			if (daysbw1 > 0)
				x = 1;
			daysbw = (calculatedays(dateout1, dayin1));
			if (daysbw > 0)
				x = 1;

			count++;
			if (bookedroomline[count] == 0 && x == 1)
				return 1;
			else if (bookedroomline[count] == 0 && x == 0)
				return 0;
		}
	}
	else
		return 1;
}

int checkdateformat(char date[], int* q)		//another validation function. checks date
{
	while (1)
	{
		struct date datevar;
		int daysch = 0, month31[7] = { 1,3,5,7,8,10,12 };
		char tempd[2], tempm[2], tempy[2];
		tempd[0] = date[0];
		tempd[1] = date[1];
		tempm[0] = date[3];
		tempm[1] = date[4];
		tempy[0] = date[6];
		tempy[1] = date[7];
		datevar.month = atoi(tempm);
		if (datevar.month > 12 || datevar.month < 1)
		{
			error();
			return 0;
		}
		datevar.day = atoi(tempd);
		for (int i = 0; i < 7; i++)
		{
			if (datevar.month == month31[i])
			{
				daysch = 31;
				break;
			}
			else if (datevar.month == 2)
			{
				daysch = 28;
				break;
			}
		}
		if (daysch == 0)
			daysch = 30;
		if (datevar.day > daysch || datevar.day < 1)
		{
			printf("Error");
			return 0;
			break;
		}
		datevar.year = atoi(tempy);
		if (datevar.year > 22 || datevar.year < 21)
		{
			printf("Error");
			return 0;
			break;
		}
		*q = datevar.day;
		*(q + 1) = datevar.month;
		*(q + 2) = datevar.year;
		return 1;
	}
}

int checkinandout(int datein[], int dateout[])		// one more validation for date
{
	while (1)
	{
		if (dateout[2] < datein[2])
		{
			printf("Check-in year cannot be greater than check-out year\n");
			break;
		}
		if (dateout[1] < datein[1])
		{
			if (dateout[2] == datein[2])
			{
				printf("Month of check-out cannot be earlier than month of check-in\n");
				break;
			}
		}
		if (dateout[0] < datein[0])
		{
			if (dateout[1] == datein[1])
			{
				printf("Day of check-out cannot be earlier than month of check-in\n");
				break;
			}
		}

		return 1;
	}
}

int calculatedays(int datein[], int dateout[])		//calculates days between two date
{
	long d1, m1, y1, d2, m2, y2;
	d1 = datein[0];
	m1 = datein[1];
	y1 = datein[2] + 2000;
	d2 = dateout[0];
	m2 = dateout[1];
	y2 = dateout[2] + 2000;
	d1 = DatDif(d1, m1, y1, d2, m2, y2);
	return d1;
}

long DatDif(d1, m1, y1, d2, m2, y2)			// part of calculation function
{
	long suma;
	suma = rbdug(d2, m2, y2) - rbdug(d1, m1, y1);
	if (y1 != y2) {
		if (y1 < y2) {
			suma += Godn(y1, y2);
		}
		else {
			suma -= Godn(y2, y1);
		}
	}
	return(suma);
}// end DatDif

long Godn(yy1, yy2)			// part of calculation function
{
	long jj, bb;
	bb = 0;
	for (jj = yy1; jj < yy2; jj++) {
		bb += 365;
		if (IsLeapG(jj) == 1) bb += 1;
	}
	return(bb);
}// end Godn

long rbdug(d, m, y)			// part of calculation function
{
	long a, r[13];
	r[1] = 0; r[2] = 31; r[3] = 59; r[4] = 90;
	r[5] = 120; r[6] = 151; r[7] = 181; r[8] = 212;
	r[9] = 243; r[10] = 273; r[11] = 304; r[12] = 334;
	a = r[m] + d;
	if ((IsLeapG(y) == 1) && (m > 2)) a += 1;
	return(a);
}//end rbdug

long IsLeapG(yr)		// part of calculation function
{			
	if (((((yr % 400) == 0) || ((yr % 100) != 0)) && ((yr % 4) == 0))) {
		return(1);
	}
	else {
		return(0);
	}
}//end IsLeapG

void error()  //just error
{
	printf("Error, please try again\n");
}

int inp_check1(char datatocheck[], char allowedsymbols[]) // first parameter is data which will be checked, second one is allowed symbols in this data
{

	int charlen = strlen(datatocheck), temp, metreq;
	for (int i = 0; i < charlen; i++)
	{
		temp = i;
		metreq = 0;
		for (int q = 0; q < strlen(allowedsymbols); q++)
			if (datatocheck[temp] == allowedsymbols[q])
			{
				metreq = 1;
				break;
			}
		if (metreq == 0)
			return 0;
	}
	return 1;
}

void search_menu(int funcitonnum)	// firstly prints available parameters to make search by one of them. 
{									//	then waits for user to write his choice and ask to write data which will be searched
	int user_choise, empty[40];
	char searchdata[60];
	printf("\tChoose parameter to search: \n\t|1| IC/passport number\n\t|2| Name\n\t|3| RoomID\n\t|4| Days of stay\n\t|5| Phone number\n\t|6| Email\n\t|7| Check-in date\n\t|8| Check-out date\n\n>>>");
	scanf("%d", &user_choise);
	printf("\nEnter a data to search: ");
	fseek(stdin, 0, SEEK_END);
	gets(searchdata);
	system("cls");
	search(user_choise, searchdata, funcitonnum, &empty);
	back();
}

void change_choise()    //shows available pages and then waits for user to write his choice 
{
	while (1)
	{
		system("cls");
		int linenum, parameter;
		char choise[2];
		printf("\n\t|1| Show all bookings\n\t|2| Search booking details\n\n\t|9| Go back\n\t|0| Exit\n\n>>>");
		scanf("%s", &choise);
		if (choise[0] == '1')
		{
			all_bookings();
			printf("Enter a booking number(#) to change: ");
			scanf("%d", &linenum);
			printf("\tChoose parameter to change: \n\t|1| IC/passport number\n\t|2| Name\n\t|3| RoomID\n\t|4| Days of stay\n\t|5| Phone number\n\t|6| Email\n\n>>>");
			scanf("%d", &parameter);
			change_menu(linenum, parameter);
		}
		else if (choise[0] == '2')
		{
			system("cls");
			search_menu(2);
		}
		else if (atoi(choise) == 9)
			break;
		else if (choise[0] == '0')
			exit(0);
	}
}

int linecount()			//counts lies in user info file
{
	int count = 0;
	FILE* userinfo;
	userinfo = fopen("login_info.txt", "r");
	while (!(feof(userinfo)))
	{
		char line[1000];
		if (fgets(line, 1000, userinfo) != NULL)
		{
			count++;
		}
	}
	fclose(userinfo);
	return count;
}

void show_userinfo(int numline)		// will show user info based on parameter, number of line 
{

	char userline[1000], founddata[60];
	int  colnum1 = -1, colnum2, datanum = 0;
	printf("  %-6d", numline + 1);
	struct user_info cu2;
	strcpy(userline, get_user_line(numline));
	for (int i = 0; i < 8; i++)
	{
		colnum2 = colnum1;
		colnum1 = colSearch(userline);
		int countchar = 0;
		for (int i = colnum2 + 1; i < colnum1; i++)
		{
			founddata[countchar] = userline[i];
			countchar++;
		}
		founddata[countchar] = '\0';
		if (datanum > -1 && datanum < 8)
			printf("%-20s", founddata);
		else if (datanum == 8)
			printf("%-20s", founddata);
		datanum++;
	}
	printf("\n");

}

int search(int parameter, char datatosearch[], int functionnum, int* q)			//will get parameters, and start searching this parameters line by line in the file
{
	/*system("cls");*/
	if (functionnum != 3)
		printf("  #\tIC/pasport\t\tName\t\tRoomID\t\tDays of Stay\t\tPhone Number\t\tEmail\t\tCheck-in\t\tCheck-out\n");
	int showed_lines[30], count = 0, linetochange = 0;
	for (int i = 0; i < linecount(); i++)
	{

		char line[1000];
		strcpy(line, get_user_line(i));
		int  colnum1 = -1, colnum2, countchar = 0;
		char founddata[60];
		for (int i = 0; i < parameter; i++)
		{
			colnum2 = colnum1;
			colnum1 = colSearch(line);
		}
		for (int i = colnum2 + 1; i < colnum1; i++)
		{
			founddata[countchar] = line[i];
			countchar++;
		}
		founddata[countchar] = '\0';
		if (strcmp(founddata, datatosearch) == 0)
		{
			*(q + count) = i;
			count++;
			if (functionnum != 3)
				show_userinfo(i);

		}

	}
	if (count == 0 && functionnum != 3)
	{
		printf("\nNo results...\n\n");
	}
	else if (count > 0)
	{
		if (functionnum == 2)
		{
			printf("Enter number of booking to change:");
			scanf("%d", &linetochange);
			change_menu(linetochange, parameter);
		}
		else if (functionnum == 1 || functionnum == 3)
		{
			*(q + count) = 0;
			return 1;
		}
	}

}

int colSearch(char line[])  //function to search : number
{
	for (int i = 0; line[i]; i++)
	{
		if (line[i] == ':')
		{
			line[i] = "$\n";
			return i;
		}
	}
}

int colSearch2(char line[], int lastchar)   //another function to search : number
{
	for (int i = lastchar; line[i]; i++)
	{
		if (line[i] == ':')
		{
			return i;
		}
	}
}

int change_menu(int line_tochange, int parameter)		
{

	int linecount = 0;

	FILE* userinfo, * tempfile;
	userinfo = fopen("login_info.txt", "a+");
	tempfile = fopen("tempfile.txt", "w+");

	while (!(feof(userinfo)))
	{
		char line[1000];
		if (fgets(line, 1000, userinfo) != NULL)
		{
			if (linecount == line_tochange - 1)
			{
				int  colnum1 = -1, colnum2, lastchar = 0;
				char newdata[300];
				fseek(stdin, 0, SEEK_END);
				printf("Enter new data: ");
				gets(newdata);

				for (int i = 0; i < parameter; i++)
				{
					colnum2 = colnum1;
					colnum1 = colSearch2(line, lastchar);
					lastchar = colnum1 + 1;
				}

				for (int i = 0; i < colnum2 + 1; i++)
				{
					fprintf(tempfile, "%c", line[i]);
				}

				fprintf(tempfile, "%s", newdata);
				for (int i = colnum1; i < 1000; i++)
				{
					if (line[i] == '\0')
						break;
					fprintf(tempfile, "%c", line[i]);
				}
			}
			else
				fprintf(tempfile, "%s", line);
			linecount++;

		}
	}
	fclose(tempfile);
	fclose(userinfo);
	tempfile = fopen("tempfile.txt", "r+");
	userinfo = fopen("login_info.txt", "w+");
	while (!(feof(tempfile)))
	{
		char line[1000];
		if (fgets(line, 1000, tempfile) != NULL)
		{
			fprintf(userinfo, "%s", line);
		}
	}
	fclose(userinfo);
	fclose(tempfile);
	remove("tempfile.txt");
}

int reciepts()			// function will show user all booking and ask which one's reciept to show
{
	int recieptNum, lines = linecount(), err = 0;
	char recieptchar[3];
	while (1)
	{
		all_bookings();
		if (err == 1)
			error();
		printf("Enter booking number to show reciept:");
		scanf("%s", &recieptchar);
		recieptNum = atoi(recieptchar);
		if (recieptNum <= lines && recieptNum >= 1)
			break;
		err = 1;
	}

	reciept_show(recieptNum - 1);

}

const char* roomtotype(char roomid[])		//converts roomID to the name of room
{
	char roomidc[2];
	int roomn;
	roomidc[0] = roomid[2];
	roomn = atoi(roomidc);
	switch (roomn)
	{
	case 1: return "Superior"; break;
	case 2: return "Deluxe"; break;
	case 3: return "Studio"; break;
	case 4: return "Executive Suite"; break;
	case 5: return "Deluxe Suite"; break;

	default:
		break;
	}
}

void reciept_show(int recieptNum)			//it will get parameter of which room to show, and will show that room's reciept
{
	char line[1000], name[50], roomID[4], days[5], roomtype[20];
	int daysint, totalprice, roomprice;
	strcpy(line, get_user_line(recieptNum));
	strcpy(name, datafromline(2, line));
	strcpy(roomID, datafromline(3, line));
	strcpy(days, datafromline(4, line));
	daysint = atoi(days);
	roomprice = roomtoprice(roomID);
	strcpy(roomtype, roomtotype(roomID));
	totalprice = ((daysint * 10) + (roomprice * daysint));
	totalprice = (totalprice * .1 + totalprice);
	printf("\n\t\t|Reciept|\n\n\tName: %s\n\tRoom Type: %s\n\t___________________\n\tNights of stay --------- %s\n\tRoom price per night --------- %dRM\n\tTourism tax per night --------- 10RM\n\tService tax 10%% --------- %dRM\n\t___________________\n\n\tTotal price --------- %dRM\n\n\n", name, roomtype, days, roomprice, (totalprice * 10 / 110), totalprice);
	back();

}

int roomtoprice(char roomid[])		//converts roomID to the price of room
{
	char roomidc[2];
	int priceroom;
	roomidc[0] = roomid[2];
	priceroom = atoi(roomidc);
	switch (priceroom)
	{
	case 1: return 180; break;
	case 2: return 200; break;
	case 3: return 250; break;
	case 4: return 400; break;
	case 5: return 500; break;

	default:
		break;
	}
	return 0;
}
