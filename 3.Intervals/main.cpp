#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

class CRangeList;

class CRange {
public:
	CRange(long long lo, long long hi)
		: c_low(lo <= hi ? lo : throw logic_error("lo > hi")), c_high(hi) {
	}

	// this is a getter of c_low
	long long getLow() const { return c_low; }

	// this is a getter of c_high
	long long getHigh() const { return c_high; }

	//This method discovers, whether an interval is only one number.
	bool loEqualsHi() const {
		return (c_low == c_high);
	}

	//This method checks whether 2 intervals overlap
	bool overlap(CRange other) const {
		// boundaries
		if (c_low == LONG_LONG_MIN && c_high == LONG_LONG_MAX){
			if (other.getHigh() >= c_low &&
					other.getLow() <= c_high)
				return true;
		}
		else if (c_low == LONG_LONG_MIN && c_high != LONG_LONG_MAX){
			if (other.getHigh() >= c_low &&
					other.getLow() <= c_high + 1)
				return true;
		}

		else if (c_low != LONG_LONG_MIN && c_high == LONG_LONG_MAX){
			if (other.getHigh() >= c_low - 1 &&
					other.getLow() <= c_high)
				return true;
		}

		else {
			if (other.getHigh() >= c_low - 1 &&
					other.getLow() <= c_high + 1)
				return true;
		}
		return false;
	}

	//This method checks whether 1 interval completely covers the other
	bool overlapCompletely(CRange other) const {
		return (other.c_low <= c_low && other.c_high >= c_high);
	}

	//This method checks whether 1 interval is inside the other
	bool overlapInside(CRange other) const {
		return (other.c_low >= c_low && other.c_high <= c_high);
	}

	//This method checks whether one interval is left to the other and overlap but don't cross
	bool overlapLeft(CRange other) const {
		return (c_low > other.c_low && c_low < other.c_high && c_high > other.c_high);
	}

	//This method checks whether one interval is right to the other and overlap but don't cross
	bool overlapRight(CRange other) const {
		return (c_high > other.c_low && c_high < other.c_high && c_low < other.c_low);
	}

	//This method merges 2 intervals into 1
	CRange &merge(CRange other) {
		if (other.c_low < this->c_low)
			c_low = other.c_low;
		if (other.c_high > this->c_high)
			c_high = other.c_high;
		return *this;
	}

	//This method merges 2 intervals to the right
	void mergeRight(CRange other) {
		if(other.c_high != LONG_LONG_MAX)
			c_low = other.c_high + 1;
		else
			c_low = other.c_high;
	}

	//This method merges 2 intervals to the left
	void mergeLeft(CRange other) {
		if(other.c_low != LONG_LONG_MIN)
			c_high = other.c_low - 1;
		else
			c_high = other.c_low;
	}

	// This friend functions concatenates 2 intervals into 1 interval list
	friend CRangeList operator+(const CRange &ls, const CRange &rs);

	// This friend functions makes a set difference of 2 intervals into 1 interval list
	friend CRangeList operator-(const CRange &ls, const CRange &rs);

private:
	long long c_low;
	long long c_high;
};

bool cmp(const CRange &a, const CRange &b) {
	return (a.getLow() < b.getLow());
}

class CRangeList {
public:
	// Implicit constructor
	CRangeList() {}

	// This method checks for the existence of number in the database of intervals; O(N)
	bool includes(long long num) const { // includes long long (number)
		for (size_t i = 0; i < R_database.size(); i++) {
			if (R_database[i].getLow() <= num && R_database[i].getHigh() >= num)
				return true;
		}
		return false;
	};

	// This method checks for the existence of interval in the database of intervals; O(N)
	bool includes(CRange range) const { // includes range
		for (size_t i = 0; i < R_database.size(); i++) {
			if (R_database[i].getLow() <= range.getLow() && R_database[i].getHigh() >= range.getHigh())
				return true;
		}
		return false;
	};

	// This method overloads += to add a Range into the RangeList
	CRangeList &operator+=(const CRange &other) {
		// if the database is empty, we just add a new interval
		if (R_database.empty()) {
			R_database.push_back(other);
			return *this;
		}

		// Find if we merge or insert a new interval
		bool merge = false;
		for (size_t i = 0; i < R_database.size(); i++) {
			if (R_database[i].overlap(other)) {
				R_database[i].merge(other);
				merge = true;
			}
		}

		// Check if new neighbours don't overlap
		if (merge) {
			for (size_t i = 0; i < R_database.size(); i++) {
				if (i == R_database.size() - 1) // Check if not last interval
					return *this;

				if (R_database[i].overlap(R_database[i + 1])) {
					R_database[i].merge(R_database[i + 1]);
					R_database.erase(R_database.begin() + i + 1);
					i--;
				}
			}
			return *this;
		}

		// Insert at the correct position
		auto it = lower_bound(R_database.begin(), R_database.end(), other, cmp);
		R_database.insert(it, other);
		return *this;
	}

	// This method overloads += to add CRangeList to the CRangeList
	CRangeList &operator+=(const CRangeList &other) {
		for (size_t i = 0; i < other.R_database.size(); i++)
			*this += other.R_database[i];
		return *this;
	}

	// -= range / range list
	CRangeList &operator-=(const CRange &other) {
		for (size_t i = 0; i < R_database.size(); i++) {
			long long lo = R_database[i].getLow();
			long long hi = R_database[i].getHigh();
			// overlap completely
			if (R_database[i].overlapCompletely(other)) {
				R_database.erase(R_database.begin() + i);
				i --;
			}
				// overlap inside
			else if (R_database[i].overlapInside(other)) {
				R_database.erase(R_database.begin() + i);
				if (other.getLow() != lo)
					R_database.insert(R_database.begin() + i, CRange(lo, other.getLow() - 1));
				if (other.getHigh() != hi) {
					if ( R_database.end() == R_database.begin() + i || R_database.end() == R_database.begin() + i + 1 ) // if inserting to the end
						R_database.emplace_back(other.getHigh() + 1, hi);
					else
						R_database.insert(R_database.begin() + i + 1, CRange(other.getHigh() + 1, hi));
					i ++;
				}
			}
				// overlap left
			else if (R_database[i].overlapLeft(other)){
				R_database[i].mergeRight(other);
			}
				// overlap right
			else if (R_database[i].overlapRight(other)){
				R_database[i].mergeLeft(other);
			}
		}
		return *this;
	}

	CRangeList &operator-=(const CRangeList &other) {
		for(size_t i = 0; i < other.R_database.size(); i++)
			*this -= other.R_database[i];
		return *this;
	}

	// This method assigns a range to CRangeList A = B
	CRangeList &operator=(const CRange &other) {
		R_database.clear();
		R_database.push_back(other);
		return *this;
	}

	//  This method assigns a CRangeList to CRangeList A = B
	CRangeList &operator=(const CRangeList &other) {
		if (&other == this) return *this; // A = A;
		R_database.clear();
		for (size_t i = 0; i < other.R_database.size(); i++) {
			R_database.push_back(other.R_database[i]);
		}
		return *this;
	}

	// This method implements CRangeList == CRangeList
	bool operator==(const CRangeList &other) const {
		if (R_database.size() != other.R_database.size())
			return false;
		for (size_t i = 0; i < R_database.size(); i++) {
			if (R_database[i].getLow() != other.R_database[i].getLow()
					|| R_database[i].getHigh() != other.R_database[i].getHigh())
				return false;
		}
		return true;
	}

	// operator !=
	bool operator!=(const CRangeList &other) const {
		return !(*this == other);
	}

	// operator <<
	friend ostream &operator<<(ostream &os, const CRangeList &list) {
		os << "{";
		for (size_t i = 0; i < list.R_database.size(); i++) {
			// just a number
			if (list.R_database[i].loEqualsHi())
				os << list.R_database[i].getLow();
				// interval
			else if (!list.R_database[i].loEqualsHi()) {
				os << "<" << list.R_database[i].getLow() << ".." <<
					 list.R_database[i].getHigh() << ">";
			}
			// if not last interval || number
			if (i != list.R_database.size() - 1)
				os << ",";
		}
		os << "}";
		return os;
	}

	friend CRangeList operator+(const CRangeList &ls, const CRange &rs);

	friend CRangeList operator-(const CRangeList &ls, const CRange &rs);

private:
	vector<CRange> R_database;
};

// Friend functions
CRangeList operator+(const CRange &ls, const CRange &rs) {
	CRangeList tmp;
	tmp += ls;
	tmp += rs;
	return tmp;
}

CRangeList operator+(const CRangeList &ls, const CRange &rs) {
	CRangeList tmp = ls;
	tmp += rs;
	return tmp;
}

CRangeList operator-(const CRange &ls, const CRange &rs){
	CRangeList tmp;
	tmp += ls;
	tmp -= rs;
	return tmp;
}

CRangeList operator-(const CRangeList &ls, const CRange &rs){
	CRangeList tmp = ls;
	tmp -= rs;
	return tmp;
}

#ifndef __PROGTEST__

string toString(const CRangeList &x) {
	ostringstream oss;
	oss << x;
	return oss.str();
}

int main(void) {
	CRangeList a, b, c, d, e, f, g, h, i, j;
	// MY TESTS
	// Testing boundaries
	j += CRange(-4168,LONG_LONG_MAX - 1);
	j += CRange(LONG_LONG_MAX,LONG_LONG_MAX);
	j -= CRange(LONG_LONG_MAX,LONG_LONG_MAX);
	cout << toString(j);
	h += CRange(0, 1000);
	h += CRange(LONG_LONG_MIN, LONG_LONG_MAX);
	h -= CRange(LONG_LONG_MIN, 0);
	assert(toString(h) == "{<1..9223372036854775807>}");
	h -= CRange(LONG_LONG_MIN, LONG_LONG_MAX);
	assert(toString(h) == "{}");
	h += CRange(0, 922337203685477580);
	h += CRange(922337203685477582, LONG_LONG_MAX - 2);
	h += CRange(LONG_LONG_MAX, LONG_LONG_MAX);
	h -= CRange(LONG_LONG_MAX, LONG_LONG_MAX);
	h -= CRange(LONG_LONG_MIN, LONG_LONG_MAX);
	h += CRange(LONG_LONG_MIN, LONG_LONG_MAX);
	h -= CRange(LONG_LONG_MIN, -8091);
	h += CRange(LONG_LONG_MAX, LONG_LONG_MAX);
	assert(toString(h) == "{<-8090..9223372036854775807>}");
	i += CRange(LONG_LONG_MIN, LONG_LONG_MAX - 1);
	i += CRange(LONG_LONG_MAX, LONG_LONG_MAX);
	cout << toString(i) << endl;
	i -= CRange(LONG_LONG_MIN, LONG_LONG_MIN);
	cout << toString(i) << endl;
	i -= CRange(LONG_LONG_MIN + 1, LONG_LONG_MIN + 1);
	cout << toString(i) << endl;
	i += CRange(LONG_LONG_MIN, LONG_LONG_MIN);
	cout << toString(i) << endl;
	i -= CRange(LONG_LONG_MAX, LONG_LONG_MAX);
	cout << toString(i) << endl;
	i -= CRange(LONG_LONG_MAX - 1, LONG_LONG_MAX - 1);
	cout << toString(i) << endl;
	i += CRange(LONG_LONG_MAX, LONG_LONG_MAX);
	cout << toString(i) << endl;
	i -= CRange(LONG_LONG_MIN, 0);
	cout << toString(i) << endl;
	i += CRange(LONG_LONG_MIN, LONG_LONG_MAX);
	cout << toString(i) << endl;
	i -= CRange(LONG_LONG_MIN, LONG_LONG_MIN);
	cout << toString(i) << endl;
	i -= CRange(LONG_LONG_MAX, LONG_LONG_MAX);
	cout << toString(i) << endl;
	i -= CRange(LONG_LONG_MIN, LONG_LONG_MIN);
	cout << toString(i) << endl;
	i -= CRange(LONG_LONG_MAX, LONG_LONG_MAX);
	cout << toString(i) << endl;
	// Testing CRangeList += CRange
	c += CRange(10, 20);
	assert(toString(c) == "{<10..20>}");
	c += CRange(5, 30);
	assert(toString(c) == "{<5..30>}");
	c += CRange(31, 40);
	assert(toString(c) == "{<5..40>}");
	c += CRange(45, 80);
	assert(toString(c) == "{<5..40>,<45..80>}");
	c += CRange(-50, 5000);
	assert(toString(c) == "{<-50..5000>}");
	c += CRange(5001, 6000);
	assert(toString(c) == "{<-50..6000>}");
	c += CRange(10000, 20000);
	assert(toString(c) == "{<-50..6000>,<10000..20000>}");
	c += CRange(7000, 8000);
	assert(toString(c) == "{<-50..6000>,<7000..8000>,<10000..20000>}");
	c += CRange(-100, -98);
	c += CRange(-96, -90);
	c += CRange(-88, -50);
	assert(toString(c) == "{<-100..-98>,<-96..-90>,<-88..6000>,<7000..8000>,<10000..20000>}");
	c += CRange(-40, -20);
	assert(toString(c) == "{<-100..-98>,<-96..-90>,<-88..6000>,<7000..8000>,<10000..20000>}");
	c += CRange(-1000, 20000);
	assert(toString(c) == "{<-1000..20000>}");

	// Testing CRangeList -= CRange
	c -= CRange(-500, -500);
	assert(toString(c) == "{<-1000..-501>,<-499..20000>}");
	c -= CRange(10, 20);
	assert(toString(c) == "{<-1000..-501>,<-499..9>,<21..20000>}");
	c -= CRange(500, 10000);
	assert(toString(c) == "{<-1000..-501>,<-499..9>,<21..499>,<10001..20000>}");
	c -= CRange(-10000, 50000);
	assert(toString(c) == "{}");

	// Testing CRangeList += CRangeList
	d += CRange(10, 20) + CRange(21, 30);
	assert(toString(d) == "{<10..30>}");

	// Testing -=
	g += CRange(0, 100) + CRange(160, 169) + CRange(171, 180) + CRange(251, 300);
	assert(toString(g) == "{<0..100>,<160..169>,<171..180>,<251..300>}");
	g -= CRange(10, 90) - CRange(20, 30) - CRange(40, 50) - CRange(60, 90) + CRange(70, 80);
	assert(toString(g) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}");

	// Testing CRangeList == CRangeList
	// Testing CRangeList != CRangeList
	e += CRange(10, 20) + CRange(21, 30);
	f += CRange(10, 20) + CRange(21, 30);
	assert(e == f);
	assert(!(e != f));
	f += CRange(0, 50);
	assert(!(e == f));
	assert(e != f);



	// TESTS for overlap method
	CRange x(10, 20);
	CRange y(21, 30);
	CRange z(22, 50);
	assert(x.overlap(y));
	assert(!x.overlap(z));

	//--------------------------------------------------------------------------------------------------------------------
	assert(sizeof(CRange) <= 2 * sizeof(long long));
	a = CRange(5, 10);
	a += CRange(25, 100);
	assert(toString(a) == "{<5..10>,<25..100>}");
	a += CRange(-5, 0);
	a += CRange(8, 50);
	assert(toString(a) == "{<-5..0>,<5..100>}");
	a += CRange(101, 105) + CRange(120, 150) + CRange(160, 180) + CRange(190, 210);
	assert(toString(a) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}");
	a += CRange(106, 119) + CRange(152, 158);
	assert(toString(a) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}");
	a += CRange(-3, 170);
	a += CRange(-30, 1000);
	assert(toString(a) == "{<-30..1000>}");
	b = CRange(-500, -300) + CRange(2000, 3000) + CRange(700, 1001);
	a += b;
	assert(toString(a) == "{<-500..-300>,<-30..1001>,<2000..3000>}");
	a -= CRange(-400, -400);
	assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}");
	a -= CRange(10, 20) + CRange(900, 2500) + CRange(30, 40) + CRange(10000, 20000);
	assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
	try {
		a += CRange(15, 18) + CRange(10, 0) + CRange(35, 38);
		assert("Exception not thrown" == nullptr);
	}
	catch (const std::logic_error &e) {
	}
	catch (...) {
		assert("Invalid exception thrown" == nullptr);
	}
	assert(toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
	b = a;
	assert(a == b);
	assert(!(a != b));
	b += CRange(2600, 2700);
	assert(toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
	assert(a == b);
	assert(!(a != b));
	b += CRange(15, 15);
	assert(toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}");
	assert(!(a == b));
	assert(a != b);
	assert(b.includes(15));
	assert(b.includes(2900));
	assert(b.includes(CRange(15, 15)));
	assert(b.includes(CRange(-350, -350)));
	assert(b.includes(CRange(100, 200)));
	assert(!b.includes(CRange(800, 900)));
	assert(!b.includes(CRange(-1000, -450)));
	assert(!b.includes(CRange(0, 500)));
	a += CRange(-10000, 10000) + CRange(10000000, 1000000000);
	assert(toString(a) == "{<-10000..10000>,<10000000..1000000000>}");
	b += a;
	assert(toString(b) == "{<-10000..10000>,<10000000..1000000000>}");
	b -= a;
	assert(toString(b) == "{}");
	b += CRange(0, 100) + CRange(200, 300) - CRange(150, 250) + CRange(160, 180) - CRange(170, 170);
	assert(toString(b) == "{<0..100>,<160..169>,<171..180>,<251..300>}");
	b -= CRange(10, 90) - CRange(20, 30) - CRange(40, 50) - CRange(60, 90) + CRange(70, 80);
	assert(toString(b) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}");
	return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
