#include<iostream>  
#include<sstream>
#include<assert.h>  



class Date
{
public:
	Date(int year = 1900, int month = 1, int day = 1)//���캯��  
		:_year(year)
		, _month(month)
		, _day(day)
	{    //�������  
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
			std::cout << "�Ƿ�����" << std::endl;
			_year = 1900;
			_month = 1;
			_day = 1;
		}
	}
	//���캯��        
	Date(const Date& d)   //��������  
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}

	~Date() {}	//��������

	////����������  

	Date& operator=(const Date& d);	//��ֵ=  

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

	Date& operator++();				// ǰ��++  

	Date operator++(int);			// ����++  

	Date& operator--();				// ǰ��--  

	Date operator--(int);			// ����--	

	int operator-(const Date& d);

	bool IsLeap();					// �ж�����

	int _GetMonthDay(int year, int month);  // ĳ������

	friend std::ostream& operator<<(std::ostream& os, const Date& d);	// ���  

	friend std::istream& operator>>(std::istream& is, Date& d);			// ����  

private:
	int _year;
	int _month;
	int _day;

};