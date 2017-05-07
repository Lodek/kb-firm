#include <Arduino.h>

void keycheck(int rowPinN, int i)
{
  digitalWrite(rowPinN, HIGH);
  
  for(int j=0; j<NUM_COLL; j++)
  {    
     if (digitalRead(col[j]) == HIGH)
    {
      state_list[selector][(i*NUM_COLL+j)]=1;
     }
     else state_list[selector][(i*NUM_COLL+j)]=0;
  }
   

  digitalWrite(rowPinN, LOW);
  return;
}

void matrixScan()
{
  for(int i=0; i<NUM_ROW; i++)
  {
    keycheck(row[i], i);
  }
  return;
}

void key_sender()
{
  //uses modvar
  //loops through matrix scan and sends it to add buffer using correct map
  
  if(map_use!=map_use_last) 
  {
   flush();
   for(int i=0; i<NUM_COLL*NUM_ROW; i++)
   {
     if(matrix_status[selector][i]==1) add_to_buffer(mapping[map_use][i]);    
   }
  }
   
   else
   {

     for(int i=0; i<NUM_COLL*NUM_ROW; i++)
       {
         if((matrix_status[selector][i]==1) && (matrix_status[!selector][i]==0))
           {
             add_to_buffer(mapping[map_use][i]);
           }

         else if((matrix_status[selector][i]==0) && (matrix_status[!selector][i]==1))
           {
             remove_from_buffer(mapping[map_use][i]);
           }
       }
  }
  write_stuff();
  return;
}

void custom_mod_handler()
{
  map_use_last=map_use;

  if(matrix_status[selector][modvar]==1 && (counter==0))
    {
      map_use=modvar_pos+1;
    }
  else
  {
    for(int i=0; i<NUM_CUSTOM_MODS; i++)
    {
      if((matrix_status[selector][(custom_mods[i])]==1) && (custom_mods[i]!=modvar))
      {
        modvar=custom_mods[i];
        modvar_pos=i;
        counter=0;
        toggle=0;
        map_use=modvar_pos+1;
        i=NUM_CUSTOM_MODS+1;
      }
    }
  
          
    if(matrix_status[selector][modvar]==0 && counter<100)
    {
      ++counter;
      map_use=0;
    }
    else if(matrix_status[selector][modvar]==1 && counter<100 && counter!=0)
    {
      toggle=1;
      counter==0;
      toggle_map=modvar_pos+1;
    }
    else if(counter>=100)
    {
      modvar=0;
    }
    else map_use=0;
  }
  if(toggle==1) map_use=toggle_map;
}
