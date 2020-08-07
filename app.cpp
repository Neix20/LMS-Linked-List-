#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	<iomanip>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"


using namespace std;

bool ReadFile(string, List*);
bool DeleteRecord(List*, char*);
bool Display(List*, int, int);
bool InsertBook(string, List*);
bool SearchStudent(List*, char* id, LibStudent&);
bool computeAndDisplayStatistics(List*);
bool printStuWithSameBook(List*, char*);
bool displayWarnedStudent(List*, List*, List*);
int menu();
void split_String(string, char* [], int, int);
int calculateTotalDays(string);
int calculateTotalDays(int, int, int);
void int_validator(string, int&);
void centerText(string);
void showRecords(List*, bool, bool);


int main() {
	menu();
	cout << "\n\n";
	system("pause");
	return 0;
}

bool ReadFile(string filename, List* ll) {
	ifstream input(filename);
	Node* temp;
	char trash[1000], firstName[30], lastName[30];
	LibStudent student;
	bool duplicate = false;

	if (input) {//If file exists, continue with the program , else return false.
		while (input >> trash >> trash >> trash >> student.id
			>> trash >> trash >> firstName >> lastName
			>> trash >> trash >> student.course
			>> trash >> trash >> trash >> student.phone_no)
		{
			duplicate = false;
			strcat(strcat(firstName, " "), lastName);
			strcpy(student.name, firstName);

			if (ll->count > 1) {//For this code, if the list exist, it checks for duplicate. If list count is larger than 1, it means the list exist.
				temp = ll->head;
				for (int i = 1; i <= ll->count; i++) {

					if (temp->item.compareName2(student))//If the student name is same as the name inside the linked list, it means it is duplicate.
						duplicate = true;

					temp = temp->next;
				}
			}

			if (duplicate) {//If it's duplicate, I will skip over this line,and continue the next line.
				centerText("A duplicate record was found!! It shall be ignored.");
				continue;
			}

			ll->insert(student);//After each line, and after checking whether it's duplicate, I then insert it into the linked list.
		}
	}
	else
		return false;
	
	
	return true;
}

bool DeleteRecord(List* ll, char* id) {
	Node* temp = ll->head;
	int count(1);//This variable is created to determine the item count.
	LibStudent s;

	while (temp != NULL) {//The temp ptr traverses the list to check whether the id exists in list or not. It will stop when it reaches a null value, which is the end of the list.
		if (strcmp(temp->item.id, id) == 0) {//If the id is identical to the id inside the list item, it will print the student record and return true
			cout << "\nStudent record found!";
			temp->item.print(cout);
			ll->remove(count);
			return true;
		}
		temp = temp->next;
		count++;
	}
	
	return false;//If the id does not exist it will return false
}

bool SearchStudent(List* ll, char* id, LibStudent& stu) {
	Node* temp = ll->head;//Same as the previous function, I create a temporary pointer to point to the head of the list. I use a method called shallow copying to copy the list.
	int count(1);

	while (temp != NULL) {
		if (strcmp(temp->item.id, id) == 0) {//If the id is identical to the id inside the list item, it will print the student record and return true
			ll->get(count, stu);
			return true;
		}
		temp = temp->next;
		count++;
	}
	return false;//If the id does not exist it will return false
}

bool InsertBook(string filename, List* ll) {
	char id[10];
	string author, borrowedDate, dueDate;//Author, borrowedDate,dueDate variable are created because author, borrowedDate and dueDate are all in a string of text. I have to separate them into their arrays and datatype forms.
	char* date[4];//This array is used to record the dates after borrowedDate and dueDate variable are seperated.
	string refundDate = "29/3/2020";
	LibBook books;
	Node* temp;
	ifstream input(filename);
	bool duplicate = false;

	if (input) {//If file exists
		while (input >> id >> author >> books.title >> books.publisher >> books.ISBN >> books.yearPublished >> books.callNum >> borrowedDate >> dueDate) {
			books.fine = 0;
			for (int i = 0; i < author.length(); i++)//This function converts underscore into whitespaces.
				if (author[i] == '_')
					author[i] = ' ';

			for (int i = 0; i < strlen(books.title); i++)
				if (books.title[i] == '_')
					books.title[i] = ' ';

			for (int i = 0; i < strlen(books.publisher); i++)
				if (books.publisher[i] == '_')
					books.publisher[i] = ' ';

			split_String(author, books.author, 10, 100);//This functions seperates the string from the '/'.

			split_String(borrowedDate, date, 4, 5);

			books.borrow.day = atoi(date[0]);
			books.borrow.month = atoi(date[1]);
			books.borrow.year = atoi(date[2]);

			split_String(dueDate, date, 4, 5);

			books.due.day = atoi(date[0]);
			books.due.month = atoi(date[1]);
			books.due.year = atoi(date[2]);

			if(calculateTotalDays(refundDate)> calculateTotalDays(dueDate))
				books.fine = (calculateTotalDays(refundDate) - calculateTotalDays(dueDate)) * 0.5;

			temp = ll->head;//For each time the loop is run, temp points to the list head. This is because we have to match the book student id with the id in the list.

			duplicate = false;//For each time the loop is run, duplicate is set false as default.

			while (temp != NULL) {//The temp ptr traverses the  list. It will stops when it reaches a Null value, which is the end of the list.
				if (strcmp(temp->item.id, id) == 0) {

					if (temp->item.totalbook >= 15) {
						centerText("Unfortunately, this student book list is already full. Please try again.");
						break;
					}

					if (temp->item.totalbook > 0)//This checks whether is there any records in the student book list. If it's larger than zero, then there is a list.
						for (int i = 0; i < temp->item.totalbook; i++)
							if (temp->item.book[i].compareCallNum(books))//This functions checks the call number of the book. If book A call num is same as the previous call numbers, we set the duplicate as true.
								duplicate = true;

					if (duplicate) {
						centerText("A duplicate record was found!! It shall be ignored.");//if duplicate is true, the loop will break, and temp pointer will stop traversing.
						break;
					}

					temp->item.totalbook++;//Update the student book Count.
					temp->item.book[temp->item.totalbook - 1] = books;//Update the student book list. All arrays start from 0, that's why to store the 2nd book, we have to store it in book[1].
					break;
				}
				temp = temp->next;//Travels to the next item
			}

		}
		input.close();

		temp = ll->head;
		while (temp != NULL) {
			temp->item.calculateTotalFine();
			temp = temp->next;
		}
	}
	else {
		return false;
	}
	return true;
}

bool Display(List* ll, int source, int detail) {
	int count(1);
	Node* temp;

	if (source == 1 && detail == 1) {
		ofstream output("student_booklist.txt");
		temp = ll->head;
		if (output) {//If there's enough space in the computer to create a file, continue with the program. Else return false.
			while (temp != NULL) {//While the temp ptr hasn't reach the end of the list
				output << "\nSTUDENT " << count++;
				temp->item.print(output);//For each student in the list, output the student into the file.
				output << "\nBOOK LIST: " << endl;
				for (int i = 1; i <= temp->item.totalbook; i++) {//For each books in the list, output the books into the file.
					output << "\nBook " << i << endl;
					temp->item.book[i - 1].print(output);
				}
				output << "\n************************************************************************************************************************" << endl;
				temp = temp->next;
			}
			output.close();
		}
		else
			return false;
	}
	else if (source == 1 && detail == 2) {
		ofstream output("student_info.txt");
		temp = ll->head;//Repoints to the head of the list
		if (output) {
			while (temp != NULL) {//Same as above function
				output << "\nSTUDENT " << count++;
				temp->item.print(output);
				output << "\n************************************************************************************************************************" << endl;
				temp = temp->next;
			}
			output.close();
		}
		else
			return false;
	}
	else if (source == 2 && detail == 1) {
		temp = ll->head;//Same as above function
		while (temp != NULL) {
			cout << "\nSTUDENT " << count++;
			temp->item.print(cout);//For each student in the list, output the student into the program.
			cout << "\nBOOK LIST: " << endl;
			for (int i = 1; i <= temp->item.totalbook; i++) {//For each books in the list, output the books into the program.
				cout << "\nBook " << i << endl;
				temp->item.book[i - 1].print(cout);
			}
			cout << "\n************************************************************************************************************************" << endl;
			temp = temp->next;
		}
	}
	else if (source == 2 && detail == 2) {
		temp = ll->head;//Same as above function
		while (temp != NULL) {
			cout << "\nSTUDENT " << count++;
				temp->item.print(cout);//For each student in the list, output the student into the program.
			cout << "\n************************************************************************************************************************" << endl;
			temp = temp->next;
		}
	}
	return true;
}

bool computeAndDisplayStatistics(List* ll) {
	if (ll->empty())//If list is empty, return false.
		return false;
	string courses[5] = { "CS","IA","IB","CN","CT" };
	Node* temp;
	int numOfStudents(0), numOfBooksBorrowed(0),overdueBooks(0);
	string refundDate = "29/3/2020";
	double totalFine(0);

	cout << "Course"
		<< "   Number of Students"
		<< "   Total Books Borrowed"
		<< "   Total Overdue Books"
		<< "   Total Overdue Fine (RM)"
		<< endl;

	cout << right
		<< fixed << setprecision(2);

	for (int i = 0; i < 5; i++) {

		temp = ll->head; numOfStudents = 0; numOfBooksBorrowed = 0; overdueBooks = 0; totalFine = 0;//All the variable resets for the different courses

		while (temp != NULL) {
			if (strcmp(temp->item.course, courses[i].c_str()) == 0) {//If the courses of the current student in the list is identical to the array course, then begin the program.
				numOfStudents++;
				numOfBooksBorrowed += temp->item.totalbook;

				for (int i = 0; i < temp->item.totalbook; i++)//Loop through the entire book list in the student.
					if (calculateTotalDays(refundDate) > calculateTotalDays(temp->item.book[i].due.day, temp->item.book[i].due.month, temp->item.book[i].due.year))//If the refund Date is larger than due Date, it means the book is overdue.
						overdueBooks++;//The number of overdue Books increases for the course.
				
				totalFine += temp->item.total_fine;
			}
			temp = temp->next;
		}

		cout << setw(4) << courses[i]
			<< setw(15) << numOfStudents
			<< setw(23) << numOfBooksBorrowed
			<< setw(22) << overdueBooks
			<< setw(26) << totalFine
			<< endl;
	}

	return true;
}

bool printStuWithSameBook(List* ll, char* callNum) {
	if (ll->empty())
		return false;
	Node* temp = ll->head;
	int count(0);
	bool flag = false;
	LibBook b1, bookArr[100];//Sets the number of books to 100. b1 is created to accomodate the function compareCallNum.
	LibStudent stuArr[100];//Sets number of student to 100.
	strcpy(b1.callNum, callNum);
	while (temp != NULL) {//Loops through the entire list
		if (temp->item.totalbook > 0) {//If the current student did not borrow a book, skip through the list.
			for (int i = 0; i < temp->item.totalbook; i++) {
				if (temp->item.book[i].compareCallNum(b1)) {//Compares the current book call Number to callNum.
					flag = true; // To determine whether such book record exists or not.
					stuArr[count] = temp->item;
					bookArr[count++] = temp->item.book[i]; //If such book record exists, bookArr[count] will store the book, and count increase by 1. This is in order to print the borrow date value and due date value.
				}
			}
		}
		temp = temp->next;
	}

	if (flag) {//If book record exist.
		cout << "\nThere are " << count << " students that borrowed the book with call number " << callNum << " as shown below:" << endl;
		for (int i = 0; i < count; i++) {
			cout << "\nStudent ID = " << stuArr[i].id//Student.print() is not used, because the format is incorrect and we have to change the format a bit.
				<< "\nName = " << stuArr[i].name
				<< "\nCourse = " << stuArr[i].course
				<< "\nPhone Number = " << stuArr[i].phone_no
				<< "\nBorrow Date: ";
			bookArr[i].borrow.print(cout);
			cout << "\nDue Date: ";
			bookArr[i].due.print(cout);
			cout << endl;
		}
	}
	else
		cout << "\nNo such record exists!!" << endl;

	return true;
}

bool displayWarnedStudent(List* ll, List* l1, List* l2) {
	if (ll->empty())//Same as above function
		return false;
	int daysDifference(0), numOfBooksOverdue(0);
	string refundDate = "29/3/2020";
	bool everyBookOverdue = true;
	Node* temp = ll->head;

	while (temp != NULL) {
		everyBookOverdue = true; numOfBooksOverdue = 0;//Resets for each student in the list.
		for (int i = 0; i < temp->item.totalbook; i++) {//Loops through every book the student has borrow
			daysDifference = 0;//Days difference resets for each books in the list.
			if (calculateTotalDays(refundDate) > calculateTotalDays(temp->item.book[i].due.day, temp->item.book[i].due.month, temp->item.book[i].due.year))//Same as above function, checks whether due date is past the refund date
				daysDifference = calculateTotalDays(refundDate) - calculateTotalDays(temp->item.book[i].due.day, temp->item.book[i].due.month, temp->item.book[i].due.year);

			if (daysDifference == 0)//If days Difference is equals 0, it means that book is not overdue. Therefore, every book overdue is false,and the student will not be added into list2.
				everyBookOverdue = false;

			if (daysDifference >= 10)//If the current book has been overdued for more than 10 days, num of books increases.
				numOfBooksOverdue++;
		}
		
		if (numOfBooksOverdue > 2)//If num of books overdue is more than 2, then list 1 will be updated
			l1->insert(temp->item);

		if (everyBookOverdue && temp->item.total_fine >= 50)//If every book is overdue, and total fine for that student exceeds 50, then list 2 is updated.
			l2->insert(temp->item);

		temp = temp->next;
	}
	return true;
}

int menu() {
	List studentRecord;//This list will be used for the entire program, and every function.
	List warnedStudentList1;
	List warnedStudentList2;
	LibStudent student;
	char id[8], callNum[20];
	int choice, source, detail;
	string str1 = "\nEnter your choice: ", str2 = "\nWhere do you want to display the output (1 - File / 2 - Screen): ", str3 = "\nDo you want to display book list for every student (1 - YES / 2 - NO): ";
	Node* temp;

	do {//This repeats the entire menu if user enters a value not between 1 and 9.
		cout << "\nMenu" << endl
			<< "\n1. Read file."
			<< "\n2. Delete record."
			<< "\n3. Search student."
			<< "\n4. Insert book."
			<< "\n5. Display output"
			<< "\n6. Compute and Display Statistics"
			<< "\n7. Student with Same Book"
			<< "\n8. Display Warned Student"
			<< "\n9. Exit.";
		int_validator(str1,choice);
		system("cls");
		switch (choice) {
		case 1:
			cout << "\nREAD FILE" << endl;
			if (ReadFile("student.txt", &studentRecord)) {
				cout << "\n========================================================================================================================"
					<< endl << endl;
				centerText(to_string(studentRecord.count) + " records have been successfully read.");
				cout << endl
					<< "========================================================================================================================";	
			}
			else
				cout << "File does not exist!" << endl;
				

			cin.ignore(); //This ignores the previous cin value.
			cin.get(); //This pauses the screen, waiting for the user to input something.
			system("cls"); //Clears screen.
			break;
		case 2:
			cout << "\nDELETE RECORD" << endl;
			if (!studentRecord.empty()) {
				showRecords(&studentRecord, 1, 0);
				do {
					cout << "\nEnter student ID:";
					cin >> id;

					if (strlen(id) != 7)
						cout << "\nInvalid Student Id length!! Please try again." << endl;
				} while (strlen(id) != 7);

				if (DeleteRecord(&studentRecord, id)) {
					cout << "\nStudent record deleted successfully." << endl;
				}
				else
					cout << "\nNo such student record found!!" << endl << endl;
			}
			else
				cout << "\nThe student record list is currently empty." << endl << endl;

			cin.ignore();//Same as above function.
			cin.get();
			system("cls");
			break;
		case 3:
			student = LibStudent();//Student will be set Null value as default, each time the function is called. If this code does not exist, then if user types a random value, it will print the previous search result.

			cout << "\nSEARCH STUDENT" << endl;

			if (!studentRecord.empty()) {
				showRecords(&studentRecord, 1, 0);
				do {
					cout << "\nEnter student ID:";
					cin >> id;

					if (strlen(id) != 7)
						cout << "\nInvalid Student Id length!! Please try again." << endl;
				} while (strlen(id) != 7);

				if (SearchStudent(&studentRecord, id, student)) {
					student.print(cout);
					cout << "\nStudent record found successfully.";
					cout << endl;
				}
				else
					cout << "\nNo such student record found!!" << endl << endl;
			}
			else
				cout << "\nThe student record list is currently empty." << endl << endl;
			
			cin.ignore();
			cin.get();
			system("cls");
			break;
		case 4:
			cout << "\nINSERT BOOK" << endl << endl;
			if (InsertBook("book.txt", &studentRecord)) {
				cout << "\n========================================================================================================================"
					<< endl << endl;
				centerText("Books inserted successfully to student list.");
				cout << endl
					<< "========================================================================================================================";
			}
			else
				cout << "File does not exist!" << endl << endl;

			cin.ignore();//Same as above function.
			cin.get();
			system("cls");
			break;
		case 5:
			cout << "\nDISPLAY OUTPUT" << endl;

			if (!studentRecord.empty()) {
				do {
					int_validator(str2, source);
					if (source < 1 || source>2)
						cout << "\nError!! Enter a number either 1 or 2 only!!" << endl;
				} while (source < 1 || source>2);

				do {
					int_validator(str3, detail);
					if (detail < 1 || detail>2)
						cout << "\nError!! Enter a number either 1 or 2 only!!" << endl;
				} while (detail < 1 || detail>2);

				if (Display(&studentRecord, source, detail)) {

					if (source == 1)
						if (detail == 1)
							cout << "\nSuccessfully display output to student_booklist.txt" << endl;
						else if (detail == 2)
							cout << "\nSuccessfully display output to student_info.txt" << endl;

					cout << "\nSuccessfully display output" << endl << endl;
				}
				else
					cout << "\nFile was not created successfully!" << endl;
			}
			else
				cout << "\nThe student record list is currently empty." << endl << endl;

			cin.ignore();
			cin.get();
			system("cls");
			break;
		case 6:
			cout << "\nCOMPUTE AND DISPLAY STATISTICS" << endl << endl;

			if (computeAndDisplayStatistics(&studentRecord))
				cout << "\nSuccessfully Display Output." << endl << endl;
			else
				cout << "The student record list is currently empty." << endl << endl;

			cin.ignore();
			cin.get();
			system("cls");
			break;
		case 7:
			cout << "\nSTUDENT WITH SAME BOOK" << endl;

			if (!studentRecord.empty()) {
				showRecords(&studentRecord, 0, 1);
				cout << "\nEnter Book Call Number:";
				cin >> callNum;

				/*for (int i = 0; i < strlen(callNum); i++)
					if (isalpha(callNum[i]))
						callNum[i] = toupper(callNum[i]);*/

				if (printStuWithSameBook(&studentRecord, callNum))
					cout << "\nSuccessfully Display Output." << endl << endl;

			}
			else
				cout << "\nThe student record list is currently empty." << endl << endl;

			cin.ignore();
			cin.get();
			system("cls");
			break;
		case 8:
			warnedStudentList1 = List();//Warned student list1 and list2 will be created each time the option is selected. IF list1 and lsit2 are not reinitialized, the list will just increase in length with duplicate values.
			warnedStudentList2 = List();

			cout << "\nDISPLAY WARNED STUDENT" << endl;

			if (displayWarnedStudent(&studentRecord, &warnedStudentList1, &warnedStudentList2)) {
				if (!warnedStudentList1.empty()) {
					cout << "\nLIST 1:" << endl
						<< "\nStudents who have more than 2 book overdue for more than 10 days.";
					temp = warnedStudentList1.head;
					while (temp != NULL) {
						temp->item.print(cout);
						temp = temp->next;
					}
					cout << "\nSuccessfully Display Output." << endl << endl;
				}
				else
					cout << "Student Warned List 1 is currently empty." << endl << endl;

				if (!warnedStudentList2.empty()) {
					cout << "\nLIST 2:" << endl
						<< "\nStudents whose fine exceeds RM50 and entire book list are overdue.";
					temp = warnedStudentList2.head;
					while (temp != NULL) {
						temp->item.print(cout);
						temp = temp->next;
					}
					cout << "\nSuccessfully Display Output." << endl << endl;
				}
				else
					cout << "Student Warned List 2 is currently empty." << endl << endl;
			}
			else
				cout << "\nThe student record list is currently empty." << endl << endl;
				
			cin.ignore();
			cin.get();
			system("cls");
			break;
		}
	} while (choice != 9);

	cout << "\n========================================================================================================================"
		<< endl << endl;
	centerText("Thank you for using our program.");
	cout << endl
		<< "========================================================================================================================";

	return 0;
}

void split_String(string str, char* a[],int num,int size) {
	int numOfLoop(0), x(0), start(-1);
	bool flag = false;
	int* occurence = new int[num];//First, we have an array called occurence to store the position of the '/' character in the string
	string Split;

	for (int i = 0; i < num; i++) {//a[] is an array of pointers to array. So we have to initialize each elements in the array in order for the array to work.
		a[i] = new char[size];
		strcpy(a[i], "");// Set each element to an empty string
	}

	for (int i = 0; i < str.length(); i++)//This function stores the position of '/' into occurence array.
		if (str[i] == '/') {
			if (numOfLoop >= num) {
				flag = true;
				break;
			}
			occurence[numOfLoop++] = i;
		}

	if (flag)
		numOfLoop = 9;

	occurence[numOfLoop] = str.length();

	while (x <= numOfLoop) {//This functions creates a string based from the start position to the position of '/'
		Split = str.substr(start + 1, occurence[x] - start - 1);
		strcpy(a[x], Split.c_str());
		x++;
		start = occurence[x - 1];
	}

	if (flag)
		a[numOfLoop] = '\0';
	else
		a[numOfLoop + 1] = '\0';
}

int calculateTotalDays(string Date) {
	int a, m, y, leap_days, julian_date, day, month, year;
	int date[3];
	char* r[4];
	split_String(Date, r, 4, 5);

	for (int i = 0; i < 3; i++)
		date[i] = stoi(r[i]);

	day = date[0], month = date[1], year = date[2];
	a = ((14 - month) / 12);
	m = (month - 3) + (12 * a);
	y = year + 4800 - a;
	leap_days = (y / 4) - (y / 100) + (y / 400);
	julian_date = day + (((153 * m) + 2) / 5) + (365 * y) + leap_days - 32045;

	return julian_date;
}

int calculateTotalDays(int day, int month, int year) {
	int a, m, y, leap_days, julian_date;
	a = ((14 - month) / 12);
	m = (month - 3) + (12 * a);
	y = year + 4800 - a;
	leap_days = (y / 4) - (y / 100) + (y / 400);
	julian_date = day + (((153 * m) + 2) / 5) + (365 * y) + leap_days - 32045;
	return julian_date;
}

void int_validator(string msg,int& a) {//Ensure that user only input integer validator
	char input;
	cout << msg;
	cin >> input;
	while (!cin|| cin.peek() != '\n'|| !isdigit(input)) {
		if (cin.peek() != '\n')
			cout << "\nError!! Only one character is allowed!" << endl;
		if (!isdigit(input))
			cout << "\nError!! Only integers are allowed!" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << msg;
		cin >> input;
	}
	a = (int)input - '0';
}

void centerText(string s) {
	int l = s.length();
	int empty = 60 - (int)(l / 2);
	cout << setw(empty) << " " << s << endl;
}

void showRecords(List* ll,bool studentList,bool bookList) {
	Node* temp;
	char* callNumber[1000], * bookTitle[1000];
	string temp1, temp2;
	int CNptr(0), BTptr(0), i(1);

	for (int j = 0; j < 1000; j++) {
		callNumber[j] = new char[1000];
		bookTitle[j] = new char[1000];
		strcpy(callNumber[j], "");
		strcpy(bookTitle[j], "");
	}

	if (studentList) {
		temp = ll->head;
		
		cout << "\n========================================================================================================================" << endl;
		centerText("Available Student ID Records:");
		while (temp != NULL) {
			temp1 = temp->item.id;
			temp2 = temp->item.name;
			centerText(temp1 + " - " + temp2);
			temp = temp->next;
		}
		cout << "========================================================================================================================" << endl;
	}

	if (bookList) {
		temp = ll->head;

		while (temp != NULL) {
			for (int j = 0; j < temp->item.totalbook; j++) {
				strcpy(callNumber[CNptr++], temp->item.book[j].callNum);
				strcpy(bookTitle[BTptr++], temp->item.book[j].title);
			}
			temp = temp->next;
		}

		for (int j = 0; j < CNptr; j++)
			for (int k = j + 1; k < CNptr; k++)
				if (strcmp(callNumber[j], callNumber[k]) == 0)
					strcpy(callNumber[j], "");

		cout << "\n========================================================================================================================" << endl;
		centerText("Available Call Number Records:");
		for (int j = 0; j < CNptr; j++)
			if (strcmp(callNumber[j], ""))
				cout << left <<i++<<". "<< setw(19) << callNumber[j] << "- " << bookTitle[j] << endl;
		cout << "========================================================================================================================" << endl;
	}
}
