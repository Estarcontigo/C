//book.h
#include<string>
#include<list>
#include<vector>
#include<map>
#include<sstream>
#include<unordered_map>
using namespace std;
typedef enum {
	哲学, 社会学, 理财, 创业, 传记, 编程
}CategoryEnum;
string categoryArray[] = { "哲学", "社会学", "理财", "创业", "传记", "编程" };
map <string, CategoryEnum> categoryEnum;
void enumToMap(void) {
	categoryEnum["哲学"] = 哲学;
	categoryEnum["社会学"] = 社会学;
	categoryEnum["理财"] = 理财;
	categoryEnum["创业"] = 创业;
	categoryEnum["传记"] = 传记;
	categoryEnum["编程"] = 编程;
}
class Book {
public:
	string name;
	string publisher;
	vector<string> writers;
	CategoryEnum category;
	float price;

	double Book::changeStringToDouble(string);
	CategoryEnum Book::changeStringToCategory(string);
	Book(vector<string>);
	bool Book::operator==(const Book&book);
};
Book::Book(vector<string> bookInfo) {
	this->name = bookInfo[0];
	this->publisher = bookInfo[1];
	this->price = changeStringToDouble(bookInfo[2]);
	this->category = changeStringToCategory(bookInfo[3]);
	for (vector<string>::iterator it = bookInfo.begin() + 4; it != bookInfo.end(); it++) {
		this->writers.push_back(*it);
	}
}
bool Book::operator==(const Book&book) {
	if (this->category == book.category&&this->name == book.name&&this->price == book.price&&this->publisher == book.publisher&&this->writers == book.writers) {
		return true;
	}
	return false;
}
double Book::changeStringToDouble(string s) {
	stringstream ss;
	double price;
	ss << s;
	ss >> price;
	return price;
}
CategoryEnum Book::changeStringToCategory(string s) {
	return categoryEnum[s];
}




