#include <HIDKeyboard.h>


void matrixScan(); //handles matrix scan
void keyCheck(int, int); //checks each key for keypress
void outstream(); //receives charstream and handles keybord.presskey


HIDKeyboard keyboard;

uint8_t outarr[8]= {0};

int selector=1;
int row[2] = {8,9};
int col[3] = {10,11,12};

int matrix[6] = {0x4,0x5,0x6,0x7,0x8,0x9};
int state_list[2][6] = {0};


void setup() 
{
  keyboard.begin();
  
  for(int i=0; i<2; i++) //sets row pins as OUT
  {
    pinMode(row[i], OUTPUT);
    digitalWrite(row[i], LOW);
  }

  for(int i=0; i<3; i++) //sets colls pins as IN
  {
    pinMode(col[i], INPUT);
  }

    
}

void loop() 
{
  selector=!selector;
  matrixScan();
  outstream();
  
  
}

void keycheck(int rowPinN, int i)
{
  digitalWrite(rowPinN, HIGH);
  
  for(int j=0; j<3; j++)
  {    
     if (digitalRead(col[j]) == HIGH)
    {
      state_list[selector][(i*3+j)]=1;
     }
     else state_list[selector][(i*3+j)]=0;
  }
   

  digitalWrite(rowPinN, LOW);
  return;
}

void matrixScan()
{
  for(int i=0; i<2; i++)
  {
    keycheck(row[i], i);
  }
  return;
}

void outstream()
{
  
  for(int i=0; i<6; i++)
  {
    if((state_list[selector][i]==1) && (state_list[!selector][i]==0))
    {
        for(int k=2;k<8;k++)
        { 
            if(outarr[k]==0) 
            {
              outarr[k]=matrix[i];
              k=9;
            }
        }      
    }
    
    if((state_list[selector][i]==0) && (state_list[!selector][i]==1))
    {
      for(int k=2;k<8;k++)
      { 
            if(outarr[k]==matrix[i]) 
            {
              outarr[k]=0;
              k=9;
            }
        }   
      }
    }
  Serial.write(outarr,8);
  return;

}

