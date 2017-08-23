#include <stdio.h>
#include <iostream>

using namespace std;

typedef enum
{
normal, hold, doubletap, clear, macro, dth, dead
} behavior_enum; 


typedef struct
{

	behavior_enum behavior;
	int keycode; 

} key_data;

typedef struct
{
  int important, on_buffer, state;
  
  key_data data[2];
  
} key;
void ftest(key* that);

int main()
{
	key test;
	test.important=1;
	test.on_buffer=1;
	test.state=1;
	test.data[0].keycode=1;
	test.data[0].behavior=normal;

	ftest(&test);

	cout << test.important << "\n";
	cout << test.on_buffer << "\n";
	cout << test.state << "\n";
	cout << test.data[0].keycode << "\n";
	cout << test.data[0].behavior << "\n";

	return 0;
}

void ftest(key *that)
{
	that->important=0;
	that->data[0].keycode=0;
	that->data[0].behavior=hold;
	
	return;
}
