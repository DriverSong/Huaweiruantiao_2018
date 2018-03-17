#include "Date.h"

bool Date::IsLeap()
{
	if ((_year % 4 == 0 && _year % 100 != 0) || _year % 400 == 0)
	{
		return true;
	}
	return false;
}
int Date::_GetMonthDay(int year, int month)  //һ�����ж�����  
{
	int arr_monthday[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	assert(month > 0 && month <= 12);
	int day = arr_monthday[month];
	if ((IsLeap()) && month == 2)
		day++;
	return day;
}

Date& Date:: operator=(const Date& d)  //��ֵ  
{
	if (this != &d)
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}
	return *this;
}

bool Date:: operator==(const Date& d)
{
	return (_year == d._year) && (_month == d._month) && (_day == d._day);
}
bool Date:: operator!=(const Date& d)
{
	return(!(*this == d));
}

bool Date:: operator>(const Date& d)
{
	return (_year > d._year) //��  
		|| (_year == d._year) && (_month > d._month)// ��-��  
		|| (_year == d._year) && (_month == d._month) && (_day > d._day);
	//��-��-��  
}
bool Date:: operator>=(const Date& d)
{
	return (*this == d) || (*this > d);
}

bool Date:: operator<(const Date& d)
{
	return (_year < d._year)
		|| (_year == d._year) && (_month < d._month)
		|| (_year == d._year) && (_month == d._month) && (_day < d._day);
}

bool Date:: operator<=(const Date& d)
{
	return (*this == d) || (*this < d);
}

Date& Date::operator++()  //�Լ� ����  //ǰ��++  
{
	//Date tmp(*this);  

	_day++;
	if (_day > _GetMonthDay(_year, _month))
	{
		_day -= _GetMonthDay(_year, _month);
		_month++;
		if (_month > 12)
		{
			_year++;
			_month = 1;
		}
	}
	return *this;
}

Date Date::operator++(int)  //����++  
{
	Date temp(*this);
	_day++;
	if (_day > _GetMonthDay(_year, _month))
	{
		_day = _GetMonthDay(_year, _month) - _day;
		_month++;
		if (_month > 12)
		{
			_year++;
			_month = 1;
		}
	}
	return temp;
}

Date& Date::operator--()  //ǰ��--  
{
	_day--;
	if (_day < 1)
	{
		_month--;
		if (_month < 1)
		{
			_year--;
			_month = 12;
		}
		_day += _GetMonthDay(_year, _month);
	}
	return *this;
}

Date Date::operator--(int)        //����--  
{
	Date temp(*this);
	_day--;
	if (_day < 1)
	{
		_month--;
		if (_month < 1)
		{
			_year--;
			_month = 12;
		}
		_day += _GetMonthDay(_year, _month);
	}
	return temp;
}

int Date::operator-(const Date & d)
{
	Date Max(*this);
	Date Min(d);
	int flag = 1;
	if (*this < d)
	{
		//ʹ�ÿ⺯��    std::swap(Max,Min);  
		Max = d;
		Min = *this;
		flag = -1;
	}
	int days = 0;
	while (Max != Min)
	{
		--Max;
		++days;
	}
	return days*flag;
}

Date Date::operator+(int days)         /*****************������*****************/
{
	Date tmp(*this);
	if (days < 0)
	{
		return  tmp - (-days);
	}
	tmp._day += days;
	while (tmp._day > _GetMonthDay(tmp._year, tmp._month))
	{
		tmp._day -= _GetMonthDay(tmp._year, tmp._month);
		if (tmp._month == 12)
		{
			tmp._year++;
			tmp._month = 1;
		}
		else
		{
			tmp._month++;
		}
	}
	return tmp;
}

Date& Date:: operator +=(int days)//   +=  
{
	*this = *this + days;
	return *this;   //����*this,������ �������ҷ���ֵ  
}

Date Date::operator-(int days)    /***************������******************/
{
	Date tmp(*this);
	if (days < 0)
	{
		return  tmp + (-days);
	}
	tmp._day -= days;
	while (tmp._day <= 0)  //ע��û��1��0�š�����  
	{
		if (tmp._month == 1)
		{
			tmp._year--;
			tmp._month = 12;
		}//У������  
		else
		{
			--tmp._month;
		}
		tmp._day += _GetMonthDay(tmp._year, tmp._month);//��һ���µ�����������  
	}
	return tmp;
}

Date& Date:: operator-=(int days)  //-=  
{
	*this = *this - days;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Date& d)
{
	os << d._year << "-" << d._month << "-" << d._day;
	return os;
}

std::istream& operator>>(std::istream& is, Date& d)
{
	char c;
	is >> d._year >> c >> d._month >> c >> d._day;

	return is;
}
