#include <Servo.h> 
// for right wheel
int pinR1=3;     // Define a 14 Pin
int pinR2=2;     // Define a 15 Pin

// for left wheel
int pinL1=4;    // Define a 16 Pin
int pinL2=5;    // Define a 17 Pin

int ENA = A0;

int inputPin = 8;  // Define the ultrasound signal receiving Pin
int outputPin = 9;  //Define the ultrasound signal emission Pin

int state = 4; // Control state via phone
int mode = '8'; // Auto mode

int Fspeedd = 0;      // go
int Rspeedd = 0;      // The right to
int Lspeedd = 0;      // Turn left to
int directionn = 0;   // After the former = 8 = 2 left = 4 right = 6 
Servo myservo;        // Set up the myservo
int delay_time = 250; // After the servo motor to the stability of the time

int Fgo = 8;         // go
int Rgo = 6;         // The right to
int Lgo = 4;         // Turn left to
int Bgo = 2;         // astern
void setup()
 {
  Serial.begin(9600);     // Default communication rate of the Bluetooth module
  
  pinMode(pinL1,OUTPUT); 
  pinMode(pinL2,OUTPUT); 
  pinMode(pinR1,OUTPUT); 
  pinMode(pinR2,OUTPUT); 
  pinMode(ENA, OUTPUT);
  
  pinMode(inputPin, INPUT);    // Define the ultrasound enter pin
  pinMode(outputPin, OUTPUT);  // Define the ultrasound output pin   

  myservo.attach(11);    // Define the servo motor output 10 pin(PWM)
 }

// directions
void advance(int a)     // go
    {
     digitalWrite(pinR1,HIGH);  // 16 feet for high level
     digitalWrite(pinR2,LOW);   //17 feet for low level
     digitalWrite(pinL1,HIGH);  // 14 feet for high level
     digitalWrite(pinL2,LOW);   //15 feet for high level
     delay(a * 1);     
    }
void turnR(int d)        //right
    {
     digitalWrite(pinL1,HIGH);  
     digitalWrite(pinL2,LOW);
     digitalWrite(pinR1,LOW);
     digitalWrite(pinR2,HIGH);  
     delay(d * 50);
    }
void turnL(int e)        //left
    {
     digitalWrite(pinL1,LOW);
     digitalWrite(pinL2,HIGH);   
     digitalWrite(pinR1,HIGH);   
     digitalWrite(pinR2,LOW);
     delay(e * 50);
    }    
void stopp(int f)         //stop
    {
     digitalWrite(pinL1,LOW);
     digitalWrite(pinL2,LOW);
     digitalWrite(pinR1,LOW);
     digitalWrite(pinR2,LOW);
     delay(f * 100);
    }
void back(int g)          //back
    {

     digitalWrite(pinL1,LOW);  
     digitalWrite(pinL2,HIGH);
     digitalWrite(pinR1,LOW);  
     digitalWrite(pinR2,HIGH);
     delay(g * 500);     
    }
    
void detection()        //Measuring three angles(0.90.179)
    {      
      int delay_time = 200;   // After the servo motor to the stability of the time
      ask_pin_F();            // Read in front of the distance
      
     if(Fspeedd < 10)         
      {
      stopp(1);               // Remove the output data 
      back(2);                // The back two milliseconds
      }
           
      if(Fspeedd < 25)         
      {
        stopp(1);               // Remove the output data
        ask_pin_L();            // Read the left distance
        delay(delay_time);      // Waiting for the servo motor is stable
        ask_pin_R();            // Read the right distance  
        delay(delay_time);      //  Waiting for the servo motor is stable  
        
        if(Lspeedd > Rspeedd)  
        {
         directionn = Lgo;      //Left
        }
        
        if(Lspeedd <= Rspeedd)  
        {
         directionn = Rgo;      //right
        } 
        
        if (Lspeedd < 15 && Rspeedd < 15)   
        {

         directionn = Bgo;      //Walk backwards        
        }          
      }
      else                      //If the front is not less than 25 cm (greater than)    
      {
        directionn = Fgo;        //Walk forward    
      }
     
    }    
void ask_pin_F()   // Measure the distance ahead
    {
      myservo.write(90);
      digitalWrite(outputPin, LOW);   // For low voltage 2 us ultrasonic launch
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // Let ultrasonic launch 10 us high voltage, there is at least 10 us
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // Maintaining low voltage ultrasonic launch
      float Fdistance = pulseIn(inputPin, HIGH);  // Read the time difference
      Fdistance= Fdistance/5.8/10;       // A time to distance distance
      Serial.print("F distance:");      //The output distance
      Serial.println(Fdistance);         //According to the distance
      Fspeedd = Fdistance;              
    }  
 void ask_pin_L()   // Measure the distance on the left 
    {
      myservo.write(5);
      delay(delay_time);
      digitalWrite(outputPin, LOW);   
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    
      float Ldistance = pulseIn(inputPin, HIGH);  
      Ldistance= Ldistance/5.8/10;       
      Serial.print("L distance:");       
      Serial.println(Ldistance);        
      Lspeedd = Ldistance;              
    }  
void ask_pin_R()   // Measure the distance on the right 
    {
      myservo.write(177);
      delay(delay_time);
      digitalWrite(outputPin, LOW);   
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    
      float Rdistance = pulseIn(inputPin, HIGH); 
      Rdistance= Rdistance/5.8/10;       
      Serial.print("R distance:");       
      Serial.println(Rdistance);         
      Rspeedd = Rdistance;              
    }  

void loop()
 {
    analogWrite(ENA,500);
    myservo.write(90);  /*Make the servo motor ready position Prepare the next measurement */
    
    if(Serial.available() > 0){ // Checks whether data is comming from the serial port
      state = Serial.read(); 
      Serial.println("receive signal");
      Serial.println(state);
    }
    
    if (mode == '9' ){ // Manual mode
      if (state == '0') { // forward
        Serial.println("Forward"); 
        advance(1);                 // forward
        state = 4;
      }
      else if (state == '1') { // backward
        Serial.println("Backward");
        back(1);                    //  back
        state = 4;
      } 
      else if (state == '2') { // left
        Serial.println("Left");
        turnL(6);
         state = 4;
      }
      else if (state == '3') { // right
        Serial.println("Right");
        turnR(6); 
        state = 4;
      }
       else if (state == '4') { // stop
        Serial.println("Stop");
        stopp(6); 
        state = 4;
      }

      if (state == '8'){
        mode = state;
        Serial.println("Switch to Auto");
      }
    }else if (mode == '8') { // Auto mode
       detection();        //Measuring Angle And determine which direction to go to
      
       if(directionn == 2) 
       {
         back(8);                    //  back
         turnL(2);                   //Move slightly to the left (to prevent stuck in dead end lane)
         Serial.print(" Reverse ");   
       }
       if(directionn == 6)           
       {
         back(1); 
         turnR(6);                   // right
       Serial.print(" Right ");    
       }
       if(directionn == 4)          
       {  
         back(1);      
         turnL(6);                  // left
         Serial.print(" Left ");    
       }  
       if(directionn == 8)         
       { 
        advance(1);                 // go 
        Serial.print(" Advance ");   
        Serial.print("   ");    
       }
  
       if (state == '9'){
        mode = state;
        Serial.println("Switch to Manual");
       }
      }
 }


