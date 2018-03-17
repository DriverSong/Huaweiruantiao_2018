#include<iostream>  
#include<sstream>
#include<assert.h>  



class Date
{
public:
	Date(int year = 1900, int month = 1, int day = 1)//构造函数  
		:_year(year)
		, _month(month)
		, _day(day)
	{    //参数检测  
		if ((year >= 1900)
			&& (month >= 1 && month <= 12)
			&& (day >= 1 && day <= _GetMonthDay(year, month)))
		{
			_year = year;
			_month = month;
			_day = day;
		}
		else
		{
			std::cout << "非法日期" << std::endl;
			_year = 1900;
			_month = 1;
			_day = 1;
		}
	}
	//构造函数        
	Date(const Date& d)   //拷贝构造  
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}

	~Date() {}	//析构函数

	////操作符重载  

	Date& operator=(const Date& d);	//赋值=  

	bool operator==(const Date& d);	// ==  

	bool operator!=(const Date& d);	// !=  

	bool operator>(const Date& d);	// >  

	bool operator>=(const Date& d);	// >=  

	bool operator<(const Date& d);	// <  

	bool operator<=(const Date& d);	// <=  

	Date operator+(int days);		// +  

	Date& operator+=(int days);		// +=  

	Date operator-(int days);		// -

	Date& operator-=(int days);		//-=  

	Date& operator++();				// 前置++  

	Date operator++(int);			// 后置++  

	Date& operator--();				// 前置--  

	Date operator--(int);			// 后置--	

	int operator-(const Date& d);

	bool IsLeap();					// 判断闰年

	int _GetMonthDay(int year, int month);  // 某月天数

	friend std::ostream& operator<<(std::ostream& os, const Date& d);	// 输出  

	friend std::istream& operator>>(std::istream& is, Date& d);			// 输入  

private:
	int _year;
	int _month;
	int _day;

};