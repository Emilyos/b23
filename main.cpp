#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>
#include "LCRS_BalancedTree.h"

#define MAX_KEY UINT_MAX
#define MIN_KEY 0

#define SEED 33356
#define INITIAL_INSERTED_NUM 91
#define OPERATIONS_NUM 200
#define DS_MAXIMUM_SIZE (INITIAL_INSERTED_NUM+OPERATIONS_NUM)
#define VALUES_LENGTH 10
#define KEYS_RANGE (DS_MAXIMUM_SIZE*10)


#define NUM_OF_OPERATIONS 6
#define INSERT 1
#define DELETE 2
#define SEARCH 3
#define RANK 4
#define SELECT 5
#define PRINTVALUES 6

using namespace std;

class MyKey :public Key
{
public:
	MyKey(unsigned k) : key(k) {}
	MyKey(MyKey const &mk) :key(mk.key) {}
	~MyKey() {}
	MyKey* clone() const
	{
		return new MyKey(*this);
	}
	unsigned get_key() { return key; }
	void set_key(unsigned k) { key = k; }
	bool operator<(const Key &rhs) const { return key < ((MyKey&)rhs).key; }
	void print() const { cout << key; }
private:
	unsigned key;
};


class MyValue : public Value
{
public:
	MyValue(string val) : value(val) {}
	~MyValue() {}
	MyValue(MyValue const &v) { value = v.value; }
	MyValue* clone() const
	{
		return new MyValue(*this);
	}
	string get_value() { return value; }
	void set_value(string val) { value = val; }
	void print() const { cout << value<<","; }
private:
	string value;
};

unsigned generate_random_unsigned(const unsigned max_ind)
/*generates a random unsigned int in range [0,max_ind-1]*/
{
	return rand() % max_ind;
}

void generate_unique_keys(unsigned keys_array[], unsigned keys_num)
/*generates keys_num unique keys in the range [1,KEYS_RANGE]. 
the keys are stored in keys_array of size keys_num*/
{
	unsigned modulo_array[KEYS_RANGE];
	for (unsigned i = 0;i < KEYS_RANGE;i++)
	{
		modulo_array[i] = 0;
	}
	unsigned unique_keys_counter = 0;
	unsigned key = 0;
	while (unique_keys_counter < keys_num)
	{
		key = generate_random_unsigned(KEYS_RANGE);
		if (modulo_array[key] == 0)
		{
			modulo_array[key] = 1;
			keys_array[unique_keys_counter] = key + 1;
			unique_keys_counter = unique_keys_counter + 1;
		}
	}
}
string generate_random_string(const unsigned len)
/*returns a random string of length len*/
{
	const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	string s;
	for (int i = 0; i < len; i++) {
		s = s + alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	return s;
}


void exe_rank(LCRS_BalancedTree* T, unsigned keys_array[], unsigned currently_stored_num, MyKey* my_key)
{
	unsigned index = generate_random_unsigned(currently_stored_num);
	my_key->set_key(keys_array[index]);
	unsigned rank = T->Rank(my_key);
	cout << "Rank for key: ";
	my_key->print();
	cout << ", resulted in: ";
	cout << rank << "\n";
}

void exe_select(LCRS_BalancedTree* T, unsigned currently_stored_num)
{
	const MyKey* key = NULL;
	unsigned index = generate_random_unsigned(currently_stored_num) + 1;
	key = (const MyKey*)T->Select(index);
	cout << "Select for index: " << index;
	cout << ", resulted in key: ";
	if (key != NULL)
	{
		key->print();
	}
	else
	{
		cout << "NULL";
	}
	cout << "\n";
}

void exe_search(LCRS_BalancedTree* T, unsigned keys_array[], unsigned currently_stored_num, MyKey* my_key)
{
	const MyValue* val = NULL;
	unsigned index = generate_random_unsigned(currently_stored_num);
	my_key->set_key(keys_array[index]);
	val = (const MyValue*)T->Search(my_key);
	cout << "Search for key: ";
	my_key->print();
	cout << ", resulted in value: ";
	if (val != NULL)
	{
		val->print();
	}
	else
	{
		cout << "NULL";
	}
	cout << "\n";
}

void print_print_values(LCRS_BalancedTree* T, MyKey* my_key1, MyKey* my_key2)
{
	cout << "PrintValues for interval [";
	my_key1->print();
	cout << ",";
	my_key2->print();
	cout << "]";
	cout << ", resulted in values: (";
	T->Print_Values(my_key1, my_key2);
	cout << ")";
	cout << "\n";
}

void exe_print_values(LCRS_BalancedTree* T,unsigned keys_array[], MyKey* my_key1, MyKey* my_key2)
{
	unsigned random_bit = generate_random_unsigned(2);
	if (random_bit)
	{
		my_key1->set_key(generate_random_unsigned(KEYS_RANGE));
		my_key2->set_key(my_key1->get_key() + generate_random_unsigned(KEYS_RANGE));
		while (my_key1->get_key() > my_key2->get_key())
		{
			my_key1->set_key(generate_random_unsigned(KEYS_RANGE));
			my_key2->set_key(my_key1->get_key() + generate_random_unsigned(KEYS_RANGE));
		}
	}
	else
	{
		my_key1->set_key(keys_array[generate_random_unsigned(DS_MAXIMUM_SIZE)]);
		my_key2->set_key(keys_array[generate_random_unsigned(DS_MAXIMUM_SIZE)]);
		while (my_key1->get_key() > my_key2->get_key())
		{
			my_key1->set_key(keys_array[generate_random_unsigned(DS_MAXIMUM_SIZE)]);
			my_key2->set_key(keys_array[generate_random_unsigned(DS_MAXIMUM_SIZE)]);
		}
	}
	print_print_values(T, my_key1, my_key2);
}


int main()
{
	srand(SEED);

	MyKey* max_key = new MyKey(MAX_KEY);
	MyKey* min_key = new MyKey(MIN_KEY);
	LCRS_BalancedTree* T = new LCRS_BalancedTree(min_key, max_key);

	unsigned small_keys_array[] = { 1, 4, 7, 9, 5 ,10,8};
	string small_values_array[] = { "1","4","7","9","5","10","8" };

	MyKey* my_key1 = new MyKey(1);
	MyKey* my_key2 = new MyKey(1);
	MyValue* my_value = new MyValue("");
	unsigned currently_stored_num = 0;

	for (unsigned i = 0;i < 7;i++)
	{
		my_key1->set_key(small_keys_array[i]);
		my_value->set_value(small_values_array[i]);
		T->Insert(my_key1, my_value);
	}
	currently_stored_num = 7;

	delete my_key1;
	delete my_value;
	my_key1 = new MyKey(1);
	my_value = new MyValue("");

	exe_search(T, small_keys_array, currently_stored_num, my_key1);
	exe_rank(T, small_keys_array, currently_stored_num, my_key1);
	exe_select(T, currently_stored_num);
	my_key1->set_key(4);
	my_key2->set_key(3);
	print_print_values(T, my_key1, my_key2);
	my_key1->set_key(1);
	my_key2->set_key(9);
	print_print_values(T, my_key1, my_key2);
	my_key1->set_key(3);
	my_key2->set_key(9);
	print_print_values(T, my_key1, my_key2);
	my_key1->set_key(3);
	my_key2->set_key(4);
	print_print_values(T, my_key1, my_key2);
	my_key1->set_key(4);
	my_key2->set_key(6);
	print_print_values(T, my_key1, my_key2);


	for (unsigned i = 7;i > 0;i--)
	{
		my_key1->set_key(small_keys_array[i-1]);
		T->Delete(my_key1);
	}

	T->Delete(my_key1);
	exe_search(T, small_keys_array, currently_stored_num, my_key1);
	exe_rank(T, small_keys_array, currently_stored_num, my_key1);
	exe_select(T, currently_stored_num);
	my_key1->set_key(1);
	my_key2->set_key(7);

	unsigned keys_array[DS_MAXIMUM_SIZE];
	generate_unique_keys(keys_array, DS_MAXIMUM_SIZE);
	currently_stored_num = 0;

	for (unsigned i=0;i < INITIAL_INSERTED_NUM;i++)
	{
		my_key1->set_key(keys_array[i]);
		my_value->set_value(generate_random_string(VALUES_LENGTH));
		T->Insert(my_key1,my_value);
	}

	currently_stored_num = INITIAL_INSERTED_NUM;
	my_key1->set_key(keys_array[DS_MAXIMUM_SIZE - 1]);
	T->Rank(my_key1);
	T->Search(my_key1);
	T->Select(DS_MAXIMUM_SIZE - 1);
	
	unsigned operation = 0;

	for (unsigned i = 0;i < OPERATIONS_NUM;i++)
	{
		operation = generate_random_unsigned(NUM_OF_OPERATIONS)+1;
		switch (operation)
		{
		case INSERT:
			my_key1->set_key(keys_array[currently_stored_num]);
			my_value->set_value(generate_random_string(VALUES_LENGTH));
			T->Insert(my_key1, my_value);
			currently_stored_num = currently_stored_num + 1;
			break;
		case DELETE:
			my_key1->set_key(keys_array[generate_random_unsigned(currently_stored_num)]);
			T->Delete(my_key1);
			break;
		case SEARCH:
			exe_search(T, keys_array, currently_stored_num, my_key1);
			break;
		case RANK:
			exe_rank(T, keys_array, currently_stored_num, my_key1);
			break;
		case SELECT:
			exe_select(T, currently_stored_num);
			break;
		case PRINTVALUES:
			exe_print_values(T, keys_array, my_key1, my_key2);
			break;
		}
	}
	
	delete T;
	T = new LCRS_BalancedTree(min_key, max_key);
	for (unsigned i = 0;i < currently_stored_num;i++)
	{
		my_key1->set_key(keys_array[i]);
		my_value->set_value(generate_random_string(VALUES_LENGTH));
		T->Insert(my_key1,my_value);
	}
	delete T;
	delete max_key;
	delete min_key;
	delete my_key1;
	delete my_key2;
	delete my_value;
}