#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>
#include <Servo.h>

#define green A0
#define red 10
const byte Rows= 4;
const byte Cols= 4; 

const int buzzer = A1;

byte rowPins[Rows] = {9,8,7,6};
byte colPins[Cols]= {5,4,3,2}; 
char keypressed;

String password = "1230";
String tempPassword="";  
int doublecheck;   
boolean storedPassword = true;
boolean changedPassword = false;
int i = 1;

char keymap[Rows][Cols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};


Servo servo;
LiquidCrystal lcd(12, 11, A5,A4,A3,A2);
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, Rows, Cols);

void setup()
{
  servo.attach(13);
  servo.write(0);
  lcd.begin(16, 2);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
}


void loop()
{
 unlockTheDoor();
}

void unlockTheDoor(){
  lcd.setCursor(0,0);
  lcd.print("  You want to");
  lcd.setCursor(0,1);
  lcd.print(" open the door?  ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Let's see if you");
  lcd.setCursor(0,1);
  lcd.print(" know the pass ");
  delay(3000);
  lcd.clear();
  lockAgain: //goto label
  tempPassword="";
  lcd.clear();
  i=6;
  noTone(buzzer);
  servo.write(0);
  while(true){
    digitalWrite(green, LOW);
    digitalWrite(red, HIGH);
    lcd.setCursor(0,0);
    lcd.print("Open the door  ");
    lcd.setCursor(0,1);
    lcd.print("PASS|");
    keypressed = myKeypad.getKey();   
    if (keypressed != NO_KEY){   
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ){
        tempPassword += keypressed;
        lcd.setCursor(i,1);
        lcd.print("*");      
        i++;
        tone(buzzer,800,200); 
      }
      else if (keypressed == 'A'){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("CHANGE PASS!!!");
        lcd.setCursor(0,1);
        lcd.print("----------------");
        delay(2500);
        changePassword();
        goto lockAgain;
      }
      else if (keypressed=='B'){
        break;
      }
      else if (keypressed == '*'){  
        if (password==tempPassword){
          lcd.clear();      
          lcd.setCursor(0,0);
          lcd.print("Correct password!");
          lcd.setCursor(0,1);
          lcd.print("You have 5 sec");
          digitalWrite(red, LOW);
          digitalWrite(green, HIGH);
          tone(buzzer,100);    
          servo.write(55);
          delay(5000);
          goto lockAgain;
        }
        else{
          tempPassword="";
          tone(buzzer,500,200);
          delay(300);
          tone(buzzer,500,200);
          delay(300);
          tone(buzzer,500,200);
          delay(300);
          goto lockAgain;
        }
      }
    }
  }
}

void changePassword(){
  retry: //label for goto
  tempPassword="";
  lcd.clear();
  i=2;
  while(!changedPassword){        
    keypressed = myKeypad.getKey();  
    lcd.setCursor(0,0);
    lcd.print("CURRENT PASSWORD");
    lcd.setCursor(0,1);
    lcd.print("->");
    if (keypressed != NO_KEY){
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ){
        tempPassword += keypressed;
        lcd.setCursor(i,1);
        lcd.print("*");
        i++;
        tone(buzzer,800,200);       
      }
      else if (keypressed=='B'){
        break;
      }
      else if (keypressed == '*'){
        i=1;
        if (password==tempPassword){
          storedPassword=false;
          tone(buzzer,500,200);
          newPassword();          
          break;
        }
        else{             
          tempPassword="";
          tone(buzzer,500,200);
          delay(300);
          tone(buzzer,500,200);
          delay(300);
          goto retry;
        }
      }
    }
  }
}
String firstpass;
//Setup new password
void newPassword(){
  tempPassword="";
  changedPassword=false;
  lcd.clear();
  i=1;
  while(!storedPassword){
    keypressed = myKeypad.getKey();   
    if (doublecheck==0){
      lcd.setCursor(0,0);
      lcd.print("SET NEW PASSWORD");
      lcd.setCursor(0,1);
      lcd.print(">");
    }
    else{
      lcd.setCursor(0,0);
      lcd.print("One more time...");
      lcd.setCursor(0,1);
      lcd.print(">");
    }
    if (keypressed != NO_KEY){
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ){
        tempPassword += keypressed;
        lcd.setCursor(i,1);
        lcd.print("*");
        i++;
          tone(buzzer,800,200);
      }
      else if (keypressed=='#'){
        break;
      }
      else if (keypressed == '*'){
        if (doublecheck == 0){
          
            firstpass=tempPassword;
            doublecheck=1;
            newPassword();
           
          
        }
        if (doublecheck==1){
          doublecheck=0;
          if (firstpass==tempPassword){
            i=1;
            firstpass="";
            password = tempPassword; 
            tempPassword="";
            lcd.setCursor(0,0);
            lcd.print("PASSWORD CHANGED");
            lcd.setCursor(0,1);
            lcd.print("----------------");
              storedPassword=true;
              tone(buzzer,500,400);
              delay(2000);
              lcd.clear();
              break;
          }
          else{
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("DOES NOT MATCH!");
            lcd.setCursor(0,1);
            lcd.print("----------------");
            delay(2500);
            firstpass="";
            newPassword();
          }
        } 
      } 
    }
  }
}
