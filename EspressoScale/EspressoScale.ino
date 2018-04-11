
#include "LCDdriver.h" ;
 
LCD mylcd(0x3E);    //Create LCD object 
int j;
//float flow;
int a=0;
float i;
float sum;
float avg;
int b=1;
char empty[10] = "    ";
float offset=0;
int data; 
int minute=0, second=0, milli=0; 
char showtime[10]; 
char showweight[10]; 
char showrate[10]; 
int weightx10; 
float weight; 
int dweight; 
int ratex10; 

unsigned long millisthen = 0; 
unsigned long current_time;  
int val; 
int input2 = A5; 
int valrecord =0; 
 
void setup() 
{ 
  offset=0;
  mylcd.init(); 
  Serial.begin(9600);
  mylcd.clear(); 
  mylcd.backLight(500); 
  mylcd.writeString("Time Weight Flo"); 
  mylcd.writeString("w"); 
//  mylcd.down(); 
  mylcd.blinkOff(); 
  mylcd.cursorOff();   
  pinMode(input2, INPUT); 
  pinMode(5,INPUT_PULLUP);
} 
 
void loop () { 
   
while(b==1){ 
  //When button pushed, reset time to zero  
  if (digitalRead(5)==0){
  current_time=0;
  minute=0;
  millisthen = millis();
  a=1;
  }
 
  current_time = (millis()-millisthen); 
  second = current_time/1000; 

  if (second==60) 
  { 
   minute++; 
   current_time=0; 
   millisthen = millis();  
  } 
  if (second < 10) 
  { 
   sprintf(showtime, "%d:0%d", minute, second);  
  } 
  else 
  { 
   sprintf(showtime,"%d:%d", minute, second);  
  } 
  if (minute == 10) 
  { 
   minute = 0;  
  } 
  mylcd.lineFeed(); 
  mylcd.writeString(showtime); 
  delay(0); 
  
  //Calculating Weight
  val= (analogRead(input2))-valrecord-60; 
  weight =(((val/4.7)-118.9)*5.82893)-offset;
  
  //When button is pushed, tare the weight
  if (digitalRead(5)==0)
  {
  offset = weight;  
  }
  Serial.println(offset);
  
  //averaging weight values
  for(j=0;j<200;j++){
    i= weight;
    sum+=i;
    delay(1);
  }
  avg = sum/200;
  sum=0;
  weight=avg;
  
  //Convering weight from float to string
  weightx10 = weight * 10; 
  if (weight <10) 
  { 
    sprintf(showweight,"00%d.%d",abs(weightx10/10),abs(weightx10%10)); 
  } 
  else if (weight ==0) 
  { 
   sprintf(showweight,"000.0");  
  } 
  else if (weight>=10, weight<100) 
  { 
   sprintf(showweight,"0%d.%d", abs(weightx10/10), abs(weightx10%10)); 
  } 
  else 
  { 
    sprintf(showweight,"%d.%d", abs(weightx10/10),abs(weightx10%10)); 
  } 
  //Printing weight to LCD
  mylcd.lineFeed(); 
  mylcd.writeString(5, showweight); 
  
  //Flow rate calculation
  ratex10 = weight/(minute*60+second); 
  sprintf(showrate,"%d.%d", abs(ratex10/10),abs(ratex10%10)); 
  if (ratex10<0.1) 
  { 
   sprintf(showrate, "0.00");  
  } 
  mylcd.lineFeed(); 
  mylcd.writeString(12, showrate); //print flowrate
  
  //Stop when time reaches 30seconds
  if (second>29){
  delay(2000);
  break;}
}

//Show Grind instructions and await reset button to restart program again 
mylcd.home();

if (weight>34){
  mylcd.writeString("Finer Grind    "); 
}
else if (weight<26){
  mylcd.writeString("Courser Grind  "); 
}
else
{
  mylcd.writeString("Good Grind     "); 
}

mylcd.writeString(" "); 
mylcd.lineFeed();
mylcd.writeString("Wght:");
mylcd.writeString(12, empty);
b=2; //ensure the while loop (measuring weight and counting time) doesnt run again
}

