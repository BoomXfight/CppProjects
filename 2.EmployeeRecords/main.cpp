#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
//======================================================================================================================
/**
 * Class CPerson that represents basic person credentials to be stored in the database
 */
struct CPerson
{
	string p_name;
	string p_surname;
	string p_email;
	unsigned int p_salary;
	//--------------------------------------------------------------------------------------------------------------------
	CPerson(const string &name, const string &surname, const string &email, unsigned int salary) : p_name(name),
																																																 p_surname(surname),
																																																 p_email(email),
																																																 p_salary(salary)
	{}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method compares p_email of 2 CPerson instances
	 * @param other
	 * @return true -> 1st email < 2nd email
	 * @return false -> otherwise
	 */
	bool isLessThanEmail(const CPerson &other) const
	{
		return (p_email.compare(other.p_email) < 0);
	}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method compares p_surname and p_name of 2 CPerson instances
	 * @param other
	 * @return true -> 1st surname < 2nd surname
	 * @return true -> 1st surname == 2nd surname && 1st name < 2nd name
	 * @return false -> otherwise
	 */
	bool isLessThanName(const CPerson &other) const
	{
		return ((p_surname.compare(other.p_surname) < 0) ||
						(p_surname == other.p_surname && p_name.compare(other.p_name) < 0));
	}
	//--------------------------------------------------------------------------------------------------------------------
};
//======================================================================================================================
/**
 * This function compares names of 2 People
 * @param st
 * @param nd
 * @return true -> st < nd; false -> otherwise
 */
bool cmp_name(const CPerson &st, const CPerson &nd)
{
	return (st.isLessThanName(nd));
}
//----------------------------------------------------------------------------------------------------------------------
/**
 * This function compares emails of 2 People
 * @param st
 * @param nd
 * @return true -> st < nd; false -> otherwise
 */
bool cmp_email(const CPerson &st, const CPerson &nd)
{
	return (st.isLessThanEmail(nd));
}
//======================================================================================================================
/**
 * Class CPersonalAgenda represents a class that stores and manipulates database of employees
 */
class CPersonalAgenda
{
public:
	//--------------------------------------------------------------------------------------------------------------------
	CPersonalAgenda(void) = default;
	//--------------------------------------------------------------------------------------------------------------------
	~CPersonalAgenda(void) = default;
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method adds a person to the database
	 * @param[in] name
	 * @param[in] surname
	 * @param[in] email
	 * @param[in] salary
	 * @return true -> success
	 * @return false -> failure == person already in the database
	 */
	bool add(const string &name, const string &surname, const string &email, unsigned int salary)
	{
		CPerson newPerson(name, surname, email, salary);
		if (searchForDuplicatesName(newPerson) || searchForDuplicatesEmail(newPerson)) // person already in the database
			return false;
		auto it_name = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), newPerson, cmp_name);
		cDatabaseName.insert(it_name, newPerson);
		auto it_email = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), newPerson, cmp_email);
		cDatabaseEmail.insert(it_email, newPerson);
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------
	// This method deletes a person form the database
	bool del(const string &name, const string &surname)
	{
		CPerson person(name, surname, "*-random-*", 0);
		if (deleteFromDatabase(person))
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	// This method deletes a person form the database
	bool del(const string &email)
	{
		CPerson person("*-random-*", "*-random-*", email, 0);
		if (deleteFromDatabase(person))
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method changes the name and the surname of a person
	 * @param email
	 * @param newName
	 * @param newSurname
	 * @return true -> success
	 * @return false -> failure == Person not found in the database
	 */
	bool changeName(const string &email, const string &newName, const string &newSurname)
	{
		if (searchForDuplicatesName(CPerson(newName, newSurname, email, 0)))
			return false; // Find whether the name and surname is unique

		CPerson person("*-random-*", "*-random-*", email, 0);
		auto it_email = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), person, cmp_email);
		if (it_email == cDatabaseEmail.end() || it_email->p_email.compare(email) != 0)
			return false; // Cannot change the person that is not in the database

		person.p_name = it_email->p_name;
		person.p_surname = it_email->p_surname;
		unsigned int old_salary = it_email->p_salary;
		auto it_name = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), person, cmp_name);
		cDatabaseName.erase(it_name); // erase old person

		CPerson newPerson(newName, newSurname, email, old_salary);
		auto it_name_new = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), newPerson, cmp_name);
		cDatabaseName.insert(it_name_new, newPerson);
		it_email->p_name = newName;
		it_email->p_surname = newSurname;
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method changes the email of a person
	 * @param name
	 * @param surname
	 * @param newEmail
	 * @return true -> success
	 * @return false -> Person not found in the database
	 */
	bool changeEmail(const string &name, const string &surname, const string &newEmail)
	{
		if (searchForDuplicatesEmail(CPerson(name, surname, newEmail, 0)))
			return false; // Find whether the email is unique

		CPerson person(name, surname, "*-random-*", 0);
		auto it_name = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), person, cmp_name);
		if (it_name == (cDatabaseName.end()) ||
				(it_name->p_name.compare(name) != 0 && it_name->p_surname.compare(surname) != 0))
			return false; // Cannot change the person that is not in the database

		person.p_email = it_name->p_email;
		unsigned int old_salary = it_name->p_salary;
		auto it_email = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), person, cmp_email);
		cDatabaseEmail.erase(it_email); // erase old person

		CPerson newPerson(name, surname, newEmail, old_salary);
		auto it_email1 = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), newPerson, cmp_email);
		cDatabaseEmail.insert(it_email1, newPerson);
		it_name->p_email = newEmail;
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------
	// This method changes the salary of a person
	bool setSalary(const string &name, const string &surname, unsigned int salary)
	{
		CPerson person(name, surname, "*-random-*", salary);
		if (setNewSalary(person, salary))
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	// This method changes the salary of a person
	bool setSalary(const string &email, unsigned int salary)
	{
		CPerson person("*-random-*", "*-random-*", email, salary);
		if (setNewSalary(person, salary))
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	// This method returns a salary of a person
	unsigned int getSalary(const string &name, const string &surname) const
	{
		CPerson person(name, surname, "*-random-*", 0);
		return getCurrentSalary(person);
	}
	//--------------------------------------------------------------------------------------------------------------------
	// This method returns a salary of a person
	unsigned int getSalary(const string &email) const
	{
		CPerson person("*-random-*", "*-random-*", email, 0);
		return getCurrentSalary(person);
	}
	//--------------------------------------------------------------------------------------------------------------------
	// This method finds a salary rank of a person compared to the others
	bool getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const
	{
		CPerson person(name, surname, "*-random-*", 0);
		if (getCurrentRank(person, rankMin, rankMax))
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	// This method finds a salary rank of a person compared to the others
	bool getRank(const string &email, int &rankMin, int &rankMax) const
	{
		CPerson person("random", "random", email, 0);
		if (getCurrentRank(person, rankMin, rankMax))
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method finds the name and surname of the first person in the alphabetically sorted database
	 * @param[out] outName
	 * @param[out] outSurname
	 * @return true -> success
	 * @return false -> failure == Database is empty
	 */
	bool getFirst(string &outName, string &outSurname) const
	{
		if (cDatabaseName.empty())
			return false;

		CPerson person = cDatabaseName[0];
		outName = person.p_name;
		outSurname = person.p_surname;
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This
	 * @param name
	 * @param surname
	 * @param outName
	 * @param outSurname
	 * @return
	 */
	bool getNext(const string &name, const string &surname, string &outName, string &outSurname) const
	{
		CPerson nowPerson(name, surname, "*-random-*", 0);
		if (!searchForDuplicatesName(nowPerson))
			return false; // person not in the database

		auto it_now = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), nowPerson, cmp_name);
		size_t next_position = it_now - cDatabaseName.begin() + 1;
		if (next_position == cDatabaseName.size()) // person at the last place of the database
			return false;

		it_now++; // next person
		outName = it_now->p_name;
		outSurname = it_now->p_surname;
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------
private:
	vector<CPerson> cDatabaseName;
	vector<CPerson> cDatabaseEmail;
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 *
	 * @param vec
	 * @param person
	 * @param cmp
	 * @return true -> duplicates found
	 * @return false -> duplicates not found
	 */
	bool searchForDuplicates(const vector<CPerson> &vec, CPerson &person,
													 const function<bool(const CPerson &a, const CPerson &b)> cmp) const
	{
		if (binary_search(vec.begin(), vec.end(), person, cmp))
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	bool searchForDuplicatesName(CPerson person) const
	{
		if (searchForDuplicates(cDatabaseName, person, cmp_name))
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	bool searchForDuplicatesEmail(CPerson person) const
	{
		if (searchForDuplicates(cDatabaseEmail, person, cmp_email))
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method deletes a person from the database
	 * @param[in] person
	 * @return true -> success
	 * @return false -> failure == Person not found in the database
	 */
	bool deleteFromDatabase(CPerson person)
	{
		auto it_email = cDatabaseEmail.begin();
		auto it_name = cDatabaseName.begin();

		if (person.p_email == "*-random-*") // Deleting by name and surname
		{
			it_name = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), person, cmp_name);
			if (it_name == cDatabaseName.end() ||
					(it_name->p_name.compare(person.p_name) != 0 && it_name->p_surname.compare(person.p_surname) != 0))
				return false; // Cannot delete a nonexistent person from the database

			person.p_email = it_name->p_email;
			it_email = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), person, cmp_email);
		} else // Deleting by email
		{
			it_email = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), person, cmp_email);
			if (it_email == cDatabaseEmail.end() || it_email->p_email.compare(person.p_email) != 0)
				return false; // Cannot delete a nonexistent person from the database

			person.p_name = it_email->p_name;
			person.p_surname = it_email->p_surname;
			it_name = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), person, cmp_name);
		}
		cDatabaseEmail.erase(it_email);
		cDatabaseName.erase(it_name);
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method assigns a new salary to the person
	 * @param[in] person
	 * @param[in] new_salary
	 * @return
	 */
	bool setNewSalary(CPerson person, unsigned int new_salary)
	{
		auto it_name = cDatabaseName.begin();
		auto it_email = cDatabaseEmail.begin();

		if (person.p_email == "*-random-*") // set by name and surname
		{
			it_name = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), person, cmp_name);
			if (it_name == cDatabaseName.end() ||
					(it_name->p_name.compare(person.p_name) != 0 && it_name->p_surname.compare(person.p_surname) != 0))
				return false; // Cannot set a salary to nonexistent person

			person.p_email = it_name->p_email;
			it_email = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), person, cmp_email);
		} else // set by email
		{
			it_email = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), person, cmp_email);
			if (it_email == cDatabaseEmail.end() || it_email->p_email.compare(person.p_email) != 0)
				return false; // Cannot set a salary to nonexistent person

			person.p_name = it_email->p_name;
			person.p_surname = it_email->p_surname;
			it_name = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), person, cmp_name);
		}

		it_name->p_salary = new_salary;
		it_email->p_salary = new_salary;
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method returns a salary of a perosn
	 * @param person
	 * @return salary
	 * @return 0 -> person not found in the database
	 */
	[[nodiscard]] unsigned int getCurrentSalary(const CPerson &person) const
	{
		auto it_salary = cDatabaseName.begin();
		if (person.p_email == "*-random-*") // Get salary by name and surname
		{
			it_salary = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), person, cmp_name);
			if (it_salary == cDatabaseName.end() ||
					(it_salary->p_name.compare(person.p_name) != 0 && it_salary->p_surname.compare(person.p_surname) != 0))
				return 0;
		} else // Get salary by email
		{
			it_salary = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), person, cmp_email);
			if (it_salary == cDatabaseEmail.end() || it_salary->p_email.compare(person.p_email) != 0)
				return 0;
		}
		return it_salary->p_salary;
	}
	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This method return a salary rank of a person compared to others.
	 * As multiple salaries might be equal the rank is relative and expressed as the range of ranks (min, max)
	 * @param p
	 * @param min
	 * @param max
	 * @return true -> success
	 * @return false -> failure = person not found in the database
	 */
	bool getCurrentRank(const CPerson &person, int &min, int &max) const
	{
		auto it_rank = cDatabaseName.begin();

		if (person.p_email == "*-random-*") // get rank by name and surname
		{
			it_rank = lower_bound(cDatabaseName.begin(), cDatabaseName.end(), person, cmp_name);
			if (it_rank == cDatabaseName.end() ||
					(it_rank->p_name.compare(person.p_name) != 0 && it_rank->p_surname.compare(person.p_surname) != 0))
				return false; // Cannot get rank of nonexistent person in the database
		} else // get rank by email
		{
			it_rank = lower_bound(cDatabaseEmail.begin(), cDatabaseEmail.end(), person, cmp_email);
			if (it_rank == cDatabaseEmail.end() || it_rank->p_email.compare(person.p_email) != 0)
				return false; // Cannot get rank of nonexistent person in the database
		}

		unsigned int salary = it_rank->p_salary;
		int same = 0, lower = 0;
		for (size_t i = 0; i < cDatabaseName.size(); i++) // Getting the rank of a salary
		{
			if (cDatabaseName[i].p_salary < salary)
				lower++;
			if (cDatabaseName[i].p_salary == salary)
				same++;
		}
		min = lower;
		max = lower + same - 1;
		return true;
	}
};
//======================================================================================================================
#ifndef __PROGTEST__
int main(void)
{
	string outName, outSurname;
	int lo, hi;
	CPersonalAgenda b1;

	/*
	assert ( b1 . add ( "Andrej", "Becka", "abecka", 1000000 ) ); //1
	assert ( b1 . add ( "Tomas", "Acka", "aatom", 500000 ) );     //2
	assert ( b1 . add ( "Aal", "Becka", "andbe", 500000 ) );      //3
	assert ( b1 . add ( "Cen", "Cecka", "cbc", 10000 ) );         //4
	assert ( ! b1 . add ( "Andrej", "Becka", "abecka", 1000000 ) ); // extra
	assert ( b1 . add ( "Brand", "Becka", "barand", 10000 ) );    //5
	assert ( b1 . add ( "Akas", "Zole", "zoz", 10000 ) );         //6
	assert ( b1 . add ( "Akator", "Acka", "akaton", 50000 ) );    //7
	*/

	assert (b1.add("John", "Smith", "john", 30000));
	assert (b1.add("John", "Miller", "johnm", 35000));
	assert (b1.add("Peter", "Smith", "peter", 23000));
	assert (!b1.add("John", "Smith", "johnyy", 10000)); // same name and surname
	assert (!b1.add("John", "Jo", "john", 10000)); // same email

	assert (b1.getFirst(outName, outSurname) && outName == "John" && outSurname == "Miller");
	assert (b1.getNext("John", "Miller", outName, outSurname) && outName == "John" && outSurname == "Smith");
	assert (b1.getNext("John", "Smith", outName, outSurname) && outName == "Peter" && outSurname == "Smith");
	assert (!b1.getNext("Peter", "Smith", outName, outSurname));
	assert (b1.setSalary("john", 32000));
	assert (b1.getSalary("john") == 32000); // 32000
	assert (b1.getSalary("John", "Smith") == 32000); //32000

	assert (b1.getRank("John", "Smith", lo, hi) && lo == 1 && hi == 1);
	assert (b1.getRank("john", lo, hi) && lo == 1 && hi == 1);
	assert (b1.getRank("peter", lo, hi) && lo == 0 && hi == 0);
	assert (b1.getRank("johnm", lo, hi) && lo == 2 && hi == 2);
	assert (b1.setSalary("John", "Smith", 35000));
	assert (b1.getSalary("John", "Smith") == 35000);
	assert (b1.getSalary("john") == 35000);
	assert (b1.getRank("John", "Smith", lo, hi) && lo == 1 && hi == 2);
	assert (b1.getRank("john", lo, hi) && lo == 1 && hi == 2);
	assert (b1.getRank("peter", lo, hi) && lo == 0 && hi == 0);
	assert (b1.getRank("johnm", lo, hi) && lo == 1 && hi == 2);
	assert (b1.changeName("peter", "James", "Bond"));
	assert (b1.getSalary("peter") == 23000);
	assert (b1.getSalary("James", "Bond") == 23000);
	assert (b1.getSalary("Peter", "Smith") == 0);
	assert (b1.getFirst(outName, outSurname) && outName == "James" && outSurname == "Bond");
	assert (b1.getNext("James", "Bond", outName, outSurname) && outName == "John" && outSurname == "Miller");
	assert (b1.getNext("John", "Miller", outName, outSurname) && outName == "John" && outSurname == "Smith");
	assert (!b1.getNext("John", "Smith", outName, outSurname));
	assert (b1.changeEmail("James", "Bond", "james"));
	assert (b1.getSalary("James", "Bond") == 23000);
	assert (b1.getSalary("james") == 23000);
	assert (b1.getSalary("peter") == 0);
	assert (b1.del("james"));
	assert (b1.getRank("john", lo, hi) && lo == 0 && hi == 1);
	assert (b1.del("John", "Miller"));
	assert (b1.getRank("john", lo, hi) && lo == 0 && hi == 0);
	assert (b1.getFirst(outName, outSurname) && outName == "John" && outSurname == "Smith");
	assert (!b1.getNext("John", "Smith", outName, outSurname));
	assert (b1.del("john"));
	assert (!b1.getFirst(outName, outSurname));
	assert (b1.add("John", "Smith", "john", 31000));
	assert (b1.add("john", "Smith", "joHn", 31000));
	assert (b1.add("John", "smith", "jOhn", 31000));
	CPersonalAgenda b2;
	assert (!b2.getFirst(outName, outSurname));
	assert (b2.add("James", "Bond", "james", 70000));
	assert (b2.add("James", "Smith", "james2", 30000));
	assert (b2.add("Peter", "Smith", "peter", 40000));
	assert (!b2.add("James", "Bond", "james3", 60000));
	assert (!b2.add("Peter", "Bond", "peter", 50000));
	assert (!b2.changeName("joe", "Joe", "Black"));
	assert (!b2.changeEmail("Joe", "Black", "joe"));
	assert (!b2.setSalary("Joe", "Black", 90000));
	assert (!b2.setSalary("joe", 90000));
	assert (b2.getSalary("Joe", "Black") == 0);
	assert (b2.getSalary("joe") == 0);
	assert (!b2.getRank("Joe", "Black", lo, hi));
	assert (!b2.getRank("joe", lo, hi));
	assert (!b2.changeName("joe", "Joe", "Black"));
	assert (!b2.changeEmail("Joe", "Black", "joe"));
	assert (!b2.del("Joe", "Black"));
	assert (!b2.del("joe"));
	assert (!b2.changeName("james2", "James", "Bond"));
	assert (!b2.changeEmail("Peter", "Smith", "james"));
	assert (!b2.changeName("peter", "Peter", "Smith"));
	assert (!b2.changeEmail("Peter", "Smith", "peter"));
	assert (b2.del("Peter", "Smith"));
	assert (!b2.changeEmail("Peter", "Smith", "peter2"));
	assert (!b2.setSalary("Peter", "Smith", 35000));
	assert (b2.getSalary("Peter", "Smith") == 0);
	assert (!b2.getRank("Peter", "Smith", lo, hi));
	assert (!b2.changeName("peter", "Peter", "Falcon"));
	assert (!b2.setSalary("peter", 37000));
	assert (b2.getSalary("peter") == 0);
	assert (!b2.getRank("peter", lo, hi));
	assert (!b2.del("Peter", "Smith"));
	assert (!b2.del("peter"));
	assert (b2.add("Peter", "Smith", "peter", 40000));
	assert (b2.getSalary("peter") == 40000);
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
