// defining the pins
const int trigPin1 = 3;//left distance sensor
const int echoPin1 = 5;
const int trigPin2 = 6;//mid distance sensor
const int echoPin2 = 9;
const int trigPin3 = 10;//Right distance sensor
const int echoPin3 = 12;
const int in1 = 13;//control pin of left motor
const int in2 = 8;//control pin of left motor
const int in3 = 4;//control pin of right motor
const int in4 = 7;//control pin of right motor
const int speed1 = 11;// controls the motor speed
int irpin =2;//IR sensor data pin
//defining constant
const int mindis = 20;//min distance to change the direction of the bot

// defining variables
int distanceleft;//left distance
int distancefront;//front distance
int distanceright;//right distance
int a=0;
void setup() {

  pinMode(trigPin1, OUTPUT);// Sets the trigPin as an Output
  pinMode(trigPin2, OUTPUT);// Sets the trigPin as an Output
  pinMode(trigPin3, OUTPUT);// Sets the trigPin as an Output
  pinMode(echoPin1, INPUT);// Sets the echoPin as an Input
  pinMode(echoPin2, INPUT);// Sets the echoPin as an Input
  pinMode(echoPin3, INPUT);// Sets the echoPin as an Input
  pinMode(irpin, INPUT);// Sets the IR pin as an Input
  pinMode(in4, OUTPUT);// Sets the in4(of motor driver) as an Output
  pinMode(in3, OUTPUT);// Sets the in3(of motor driver) as an Output
  pinMode(in2, OUTPUT);// Sets the in2(of motor driver) as an Output
  pinMode(in1, OUTPUT);// Sets the in1(of motor driver) as an Output
  pinMode(A0, OUTPUT);//common anode of the leds
  pinMode(A1, OUTPUT);//red led
  pinMode(A2, OUTPUT);//blue led
  pinMode(A3, OUTPUT);//green led
  pinMode(speed1, OUTPUT);//Sets the speed control pin as an Output
  Serial.begin(9600);//starts the serial monitor

  analogWrite(speed1,80);//sets the speed of the motor(31.25% of the full speed)

}

int distance(int trig,int echo)//takes the trig and echo pin as arguments , it returns the distance of the observed by the the sensor
{
      int distance;//stores the distance of the sensor
      long duration;//stores the duration of the pulse recieved

      //creating the pulse      
      digitalWrite(trig, LOW);
      delayMicroseconds(2);
      digitalWrite(trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig, LOW);

      duration = pulseIn(echo, HIGH);//storing the duration of the pulse recieved
      distance = duration * 0.034 / 2;//calculating the distance 

      return distance;//returning the distance
}

void green()//turns on the green led
{
  digitalWrite(A3,LOW);
  digitalWrite(A2,HIGH);
  digitalWrite(A1,HIGH);
  digitalWrite(A0,HIGH);
}

void blue()//turns on the blue led
{
  digitalWrite(A3,HIGH);
  digitalWrite(A2,LOW);
  digitalWrite(A1,HIGH);
  digitalWrite(A0,HIGH);
}

void red()//turns on the red led
{
  digitalWrite(A3,HIGH);
  digitalWrite(A2,HIGH);
  digitalWrite(A1,LOW);
  digitalWrite(A0,HIGH);
}


void off()//turns off the led
{
   digitalWrite(A3,HIGH);
  digitalWrite(A2,HIGH);
  digitalWrite(A1,HIGH);
  digitalWrite(A0,HIGH);
}

void blink_b()//blink the blue led
{
  blue();
  delay(70);
  off();
  
}


void blink_r()//blink the red led
{
  red();
  delay(70);
  off();
  
}

void blink_g()//blink the green led
{
  green();
  delay(70);
  off();
  
}


void stop(void)//stops the bot
{
    //setting all pins to low
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in4,LOW);
}

void gofront(void)//forward movement of the bot
{
    digitalWrite(in1,HIGH);//moving the left motor in forward direction
    digitalWrite(in2,LOW);
    digitalWrite(in3,HIGH);//moving the right motor in forward direction
    digitalWrite(in4,LOW);
}

void goleft(void)//left turn of the bot 
{
    digitalWrite(in1,LOW);//moving left motor backward 
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);//moving right motor forward
    digitalWrite(in4,LOW);
}

void goright(void)//right turn of the bot
{
    digitalWrite(in1,HIGH);//moving left motor forward 
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);//moving right motor backward
    digitalWrite(in4,HIGH);
}

void goback(void)
{
    digitalWrite(in1,LOW);//moving left motor backward
    digitalWrite(in2,HIGH);
    digitalWrite(in3,LOW);//moving right motor backward
    digitalWrite(in4,HIGH);
}


void loop() {

//reading the distances
distanceleft = distance(trigPin1,echoPin1);
distancefront = distance(trigPin2,echoPin2);
distanceright = distance(trigPin3,echoPin3);

//printing all the distances
Serial.print("Distance1: ");
Serial.println(distanceleft);

Serial.print("Distance2: ");
Serial.println(distancefront);

Serial.print("Distance3: ");
Serial.println(distanceright);



int s = digitalRead(irpin);//reading the state of the IR SENSOR



  if(s==LOW)//floor is there
  {
    if(distancefront>=mindis)//nothing in front
    {
        if((distanceleft<8))//too close from the left then turn right
        goright();
        else if((distanceright<8))//too close from the right then turn left
        goleft();
        else //else go front
        gofront();

    }
    else//there is an object in the front
    {
        if((distanceleft<mindis)&&(distanceright>=mindis))//object is on left but not in the right 
            goright();

        else if((distanceleft>=mindis)&&(distanceright<mindis))//object is on right but not in the left
            goleft();

        else if((distanceleft>=mindis)&&(distanceright>=mindis))//object is only in front
             {
                if(distanceright>distanceleft)//more distance in right then go right
                  goright();
                else 
                  goleft();              
            }

        else if(((distanceleft<mindis)&&(distanceright<mindis)))//object in left, right and front then go back
        {   
            goback();
        }
    }
  }
  else//floor is not there
  {
    goback();//moves the bot backwards
    delay(1000);
  
      if(distanceright>distanceleft)//then moving left or right depending upon the situation
        goright();
      else 
        goleft(); 
        delay(400);//delay is for completely turn the bot rin the desire Direction


  }

delay(10);
}