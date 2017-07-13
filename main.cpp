//main.cpp
#include"book.h"
#include<iostream>
#include<fstream>
#include<cassert>
using namespace std;
int bookNum = 0;
list<Book> bookName;
unordered_map<string, list<Book>> bookPublisher;
unordered_map<string, list<Book>> bookWriter;
list<Book> bookCategory[6];
map<float, list<Book>> bookPrice;
/*
publisher和writer的方法大部分合在一起了，把他们作为参数的都是
*/
int* createNext(string s) {
	int len = s.length();
	int j = 0;
	int* next = new int[len + 1];
	next[0] = next[1] = 0;
	for (int i = 1; i < len; i++) {
		while (j > 0 && s[i] != s[j]) {
			j = next[j];
		}
		if (s[i] == s[j]) {
			j++;
		}
		next[i + 1] = j;
	}
	return next;
}
bool kmp(string original, string find) {
	int*next = createNext(original);
	int j = 0;
	for (int i = 0; i < original.length(); i++) {
		while (j > 0 && original[i] != find[j]) {
			j = next[j];
		}
		if (original[i] == find[j]) {
			j++;
		}
		if (j == find.length()) {
			return true;
		}
	}
	return false;
}

void insertBookToPublisher(Book book) {
	unordered_map<string, list<Book> >::iterator it = bookPublisher.find(book.publisher);
	if (it == bookPublisher.end()) {
		list<Book> tmp;
		tmp.push_back(book);
		bookPublisher.emplace(book.publisher, tmp);
	}
	else {
		it->second.push_back(book);
	}
}
void insertBookToWriter(Book book) {
	for (vector<string>::iterator vecit = book.writers.begin(); vecit != book.writers.end(); vecit++) {
		unordered_map<string, list<Book> >::iterator it = bookWriter.find(*vecit);
		if (it == bookWriter.end()) {
			list<Book> tmp;
			tmp.push_back(book);
			bookWriter.emplace(*vecit, tmp);
		}
		else {
			it->second.push_back(book);
		}
	}
}
void insertBookToPrice(Book book) {
	map<float, list<Book> >::iterator it = bookPrice.find(book.price);
	if (it == bookPrice.end()) {
		list<Book> tmp;
		tmp.push_back(book);
		bookPrice.emplace(book.price, tmp);
	}
	else {
		it->second.push_back(book);
	}
}
void insertBookToLibrary(vector<string> bookinfo) {
	Book book(bookinfo);
	bookName.push_back(book);
	bookCategory[book.category].push_back(book);
	insertBookToPublisher(book);
	insertBookToWriter(book);
	insertBookToPrice(book);
	bookNum++;
}

void printBookInfo(Book book) {
	cout << book.name << "\t\t";
	cout << book.publisher << "\t\t";
	cout << book.price << "\t\t";
	cout << categoryArray[book.category] << "\t\t";
	for (vector<string>::iterator it = book.writers.begin(); it != book.writers.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}

void searchBookByKeyword(string s) {//按名字的关键字搜索
	bool hasKeyword = false;
	for (list<Book>::iterator it = bookName.begin(); it != bookName.end(); it++) {
		Book book = *it;
		if (kmp(book.name, s)) {
			printBookInfo(book);
			hasKeyword = true;
		}
	}
	if (!hasKeyword) {
		cout << "无结果！" << endl;
	}
}
void searchBookByString(unordered_map<string, list<Book>> map, string s) {//按出版社和作者完全匹配搜索
	list<Book> bookList = map[s];
	for (list<Book>::iterator it = bookList.begin(); it != bookList.end(); it++) {
		printBookInfo(*it);
	}
}
void searchBookByPrice(float minprice, float maxprice) {
	for (map<float, list<Book> >::iterator it = bookPrice.begin(); it != bookPrice.end(); it++) {
		pair<float, list<Book> > floatpair = *it;
		if (floatpair.first > maxprice) {
			break;
		}
		if (floatpair.first >= minprice) {
			for (list<Book>::iterator listit = floatpair.second.begin(); listit != floatpair.second.end(); listit++) {
				printBookInfo(*listit);
			}
		}
	}
}
void serchBookByCategory(int i) {
	for (list<Book>::iterator it = bookCategory[i].begin(); it != bookCategory[i].end(); it++) {
		printBookInfo(*it);
	}
}

void removeBookInPublisher(Book book) {
	unordered_map<string, list<Book> >::iterator it = bookPublisher.find(book.publisher);
	it->second.remove(book);
}
void removeBookInWriter(Book book) {
	for (vector<string>::iterator it = book.writers.begin(); it != book.writers.end(); it++) {
		unordered_map<string, list<Book> >::iterator mapit = bookWriter.find(*it);
		mapit->second.remove(book);
	}	
}
void removeBookInPrice(Book book) {
	map<float, list<Book> >::iterator it = bookPrice.find(book.price);
	it->second.remove(book);
}
bool removeBookByName(string name) {
	list<Book>::iterator it = bookName.begin();
	for (; it != bookName.end(); it++) {
		Book tmp = *it;
		if (tmp.name == name) {
			break;
		}
	}
	if (it != bookName.end()) {
		Book book = *it;
		bookName.remove(book);
		removeBookInPublisher(book);
		removeBookInWriter(book);
		bookCategory[book.category].remove(book);
		removeBookInPrice(book);
		bookNum--;
		return true;
	}
	else {
		return false;
	}
}

void countBookByCategory(void) {
	for (int i = 0; i < 6; i++) {
		int sum = 0;
		cout << categoryArray[i] << "类共有" << bookCategory[i].size() << "本" << endl;
	}
}
void countBookByUnorderedMap(unordered_map<string, list<Book>> map) {
	for (unordered_map<string, list<Book> >::iterator it = map.begin(); it != map.end(); it++) {
		cout << it->first << "共有" << it->second.size() << "本书在库" << endl;
	}
}

vector<string> splitString(string str, string pattern) {
	int splitPosition;
	vector<string> results;
	str.append(pattern);
	int strLength = str.length();
	for (int i = 0; i < strLength; i++) {
		splitPosition = str.find(pattern, i);
		if (splitPosition < strLength) {
			string s = str.substr(i, splitPosition - i);
			results.push_back(s);
			i = splitPosition + pattern.length() - 1;
		}
	}
	return results;
}
void readFile(string filePath) {
	ifstream inFileStream;
	inFileStream.open(filePath.data());
	assert(inFileStream.is_open());
	string oneLine;
	while (getline(inFileStream, oneLine)) {
		vector<string> splitedOneLine = splitString(oneLine, " ");
		insertBookToLibrary(splitedOneLine);
	}
	inFileStream.close();
}
string changeBookToString(Book book) {
	string s;
	s.append(book.name);
	s.append(" ");
	s.append(book.publisher);
	s.append(" ");
	s.append(to_string(book.price));
	s.append(" ");
	s.append(categoryArray[book.category]);
	s.append(" ");
	for (vector<string>::iterator it = book.writers.begin(); it != book.writers.end(); it++) {
		s.append(*it);
		if (it != book.writers.end() - 1) {
			s.append(" ");
		}
	}
	return s;
}
void saveFile(void) {
	ofstream outFileStream;
	outFileStream.open("Library.txt");
	for (list<Book>::iterator it = bookName.begin(); it != bookName.end(); it++) {
		outFileStream << changeBookToString(*it) << endl;
	}
	outFileStream.close();
}

void addOneBook(string bookName) {//手动添加书籍
	vector<string> bookInfo;
	bookInfo.push_back(bookName);
	string s;
	cout << "该书籍出版社名称是：" << endl;
	cin >> s;
	bookInfo.push_back(s);
	cout << "该书籍价格是：" << endl;
	cin >> s;
	bookInfo.push_back(s);
	cout << "该书籍类别是：" << endl;
	cin >> s;
	bookInfo.push_back(s);
	cout << "该书籍作者名称是（按0退出输入）：" << endl;
	cin >> s;
	while (s != "0") {
		bookInfo.push_back(s);
		cin >> s;
	}
	insertBookToLibrary(bookInfo);//插入接口
}

int mainUI(void) {
	int choice;
	cout << "\n\t\t\t〓〓〓〓〓图书管理系统〓〓〓〓〓\n\n";
	cout << "\t\t\t[1]:查询图书\t\n";
	cout << "\t\t\t[2]:添加图书\t\n";
	cout << "\t\t\t[3]:删除图书\t\n";
	cout << "\t\t\t[4]:批量导入图书\t\n";
	cout << "\t\t\t[5]:统计图书\t\n";
	cout << "\t\t\t〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓\n\n";
	cout << "\t\t\t0:退出\n\n";
	cout << "请选择<0~5>:";
	cin >> choice;
	return choice;
}
int searchUI(void) {
	system("cls");
	int choice;
	cout << "\n\t\t\t〓〓〓〓〓图书管理系统〓〓〓〓〓\n\n";
	cout << "1.按书籍名称查询\n\n";
	cout << "2.按出版社查询\n\n";
	cout << "3.按作者查询\n\n";
	cout << "4.按价格区间查询\n\n";
	cout << "5.按书籍类别查询\n\n";
	cout << "0.返回主菜单\n\n";
	cout << "请选择:";
	cin >> choice;
	return choice;
}
int countUI(void) {
	system("cls");
	int choice;
	cout << "\n\t\t\t〓〓〓〓〓图书管理系统〓〓〓〓〓\n\n";
	cout << "1.按类别统计\n\n";
	cout << "2.按出版社统计\n\n";
	cout << "3.按作者统计\n\n";
	cout << "0.返回主菜单\n\n";
	cout << "请选择:";
	cin >> choice;
	return choice;
}
int main() {
	enumToMap();
	readFile("Library.txt");
	cout << "\n\t\t\t〓〓〓〓〓数据读取完毕〓〓〓〓〓\n\n" << endl;
	cout << "\n欢迎您进入图书管理系统!（按任意键进入）\n\n";
	string bookInfo;
	bool isExit = false;
	while (getchar() == '\n') {
		string isContinue;
		int choice = mainUI();
		switch (choice) {
		case 0:
			system("cls");
			cout << "\n\t\t\t〓〓〓〓〓图书管理系统〓〓〓〓〓\n\n";
			cout << "\n感谢您的使用!\n\n";
			isExit = true;
			saveFile();
			break;
		case 1:
			choice = searchUI();
			if (choice == 1) {
				cout << "请输入您想查询的图书名称关键词:";
				cin >> bookInfo;
				cout << "查询结果如下:\n\n";
				searchBookByKeyword(bookInfo);
			}
			else if (choice == 2) {
				cout << "请输入您想查询的出版社名称:";
				cin >> bookInfo;
				cout << "查询结果如下:\n\n";
				searchBookByString(bookPublisher, bookInfo);
			}
			else if (choice == 3) {
				cout << "请输入您想查询的作者:";
				cin >> bookInfo;
				cout << "查询结果如下:\n\n";
				searchBookByString(bookWriter, bookInfo);
			}
			else if (choice == 4) {
				float priceHigh, priceLow;
				cout << "请输入最高价格:";
				cin >> priceHigh;
				cout << "\n" << "请输入最低价格:";
				cin >> priceLow;
				cout << "查询结果如下:\n\n";
				searchBookByPrice(priceLow, priceHigh);
			}
			else if (choice == 5) {
				cout << "请输入您想查询的书籍类别:" << endl;
				cout << "1.哲学\n\n";
				cout << "2.社会学\n\n";
				cout << "3.理财n\n";
				cout << "4.创业\n\n";
				cout << "5.传记\n\n";
				cout << "6.编程\n\n";
				cout << "请选择:";
				cin >> choice;
				cout << "查询结果如下:\n\n";
				serchBookByCategory(choice);
			}
			else if (choice == 0) {
				break;
			}
			break;
		case 2:
			system("cls");
			cout << "\n\t\t\t〓〓〓〓〓图书管理系统〓〓〓〓〓\n\n";
			cout << "请输入您想添加的书籍的名称：（按0退出）\n" << endl;
			cin >> bookInfo;
			if (bookInfo == "0") {
				break;
			}
			else {
				addOneBook(bookInfo);
				break;
			}
		case 3:
			system("cls");
			cout << "\n\t\t\t〓〓〓〓〓图书管理系统〓〓〓〓〓\n\n";
			cout << "输入您想删除的图书名称：（按0退出）\n";
			cin >> bookInfo;
			if (bookInfo == "0") {
				break;
			}
			else {
				bool hasRemoved = removeBookByName(bookInfo);
				if (hasRemoved) {
					cout << "已成功删除该书" << endl;
				}
				else {
					cout << "这本书不在库里" << endl;
				}
				break;
			}
		case 4:
			system("cls");
			cout << "\n\t\t\t〓〓〓〓〓图书管理系统〓〓〓〓〓\n\n";
			cout << "请输入您想导入的文件的地址：（按0退出）" << endl;
			cin >> bookInfo;
			if (bookInfo == "0") {
				break;
			}
			else {
				readFile(bookInfo);
				break;
			}
		case 5:
			choice = countUI();
			if (choice == 1) {
				cout << "按类别统计结果如下：" << endl;
				countBookByCategory();
			}
			else if (choice == 2) {
				cout << "按出版社统计结果如下：" << endl;
				countBookByUnorderedMap(bookPublisher);
			}
			else if (choice == 3) {
				cout << "按作者统计结果如下：" << endl;
				countBookByUnorderedMap(bookWriter);
			}
			else {
				break;
			}
		}
		if (isExit) {
			break;
		}
	}
	return 0;
}
