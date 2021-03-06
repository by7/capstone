#define FRONT 18
#define LEFT 17
#define RIGHT 19

#define PWM1 10
#define PWM2 11
#define IN1 5
#define IN2 6
#define IN3 7
#define IN4 8

int leftS = 940, rightS = 940;

void turn(int dir); // 0 for left +1 for right
int getCorrection(int err);

void setup() {
  // put your setup code here, to run once:
  pinMode(FRONT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);

  //speed
  pinMode(PWM1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  //motor direction
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
/*
    int leftSum = 0, rightSum = 0;
  for (int i = 0; i < 20; i++) {
    leftSum += analogRead(LEFT);
    rightSum += analogRead(RIGHT);
  }
  leftS = leftSum / 20;
  rightS = rightSum / 20;
  
  
  Serial.print(leftS);
  Serial.print(' ');
  Serial.println(rightS);
  */
  Serial.begin(9600);
}

int KI=1,KD=1,KP=1;
int dt=10;
int speed1 = 200; //100
int speed2 = 197; //95
int s1 = speed1, s2 = speed2;
int pError=0;
int integral=0;
int front, left, right;

void loop() {
  front = analogRead(FRONT);
  Serial.println(front);
  left = analogRead(LEFT);
  right = analogRead(RIGHT);
  //Serial.println(analogRead(LEFT));
  //Serial.println(analogRead(RIGHT));
  int lerr = -left + leftS;
  int rerr = right - rightS;
  int err = (lerr+rerr)/2;
  
  int correction = getCorrection(err);
  if(front>300) {
    analogWrite(PWM1, speed1-50);
    analogWrite(PWM2, s2-50);
    
  
  }if(front>650){ //800
    correction = 0;
    if(right<left) //turn rught
      turn(1);
    else
      turn(0); //turn leftsk
  }
  
  s1 = speed1-correction;
  s2 = speed2+correction*3/4;

  if(s1<0)
    s1 = 0;
  else if(s1>250) //150
    s1 = 250;
  if(s2<0)
    s2 = 0;
  else if(s2>250) //150
    s2 = 250;

  //Serial.println(err);
    //turning? make it go forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  analogWrite(PWM1, s1);
  analogWrite(PWM2, s2);

  //check if need to turn
  
  delay(dt);
}

int getCorrection(int err){
  int derivative = (pError-err)/dt;
  pError = err;
  integral += err*dt;
  
  return KP*err + KD*derivative;
}

void turn(int dir){
  //delay(200);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
  delay(200); //from 500

  if (dir == 1) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);    
  }
  else if (dir == 0) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  }
  
  analogWrite(PWM1+dir, 20); //10
  analogWrite(PWM2-dir, 200); //100
  delay(150); //450
}
