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
int speed1 = 95;
int speed2 = 95;
int pError=0;
int integral=0;

void loop() {
  // put your main code here, to run repeatedly:
  int lerr = -analogRead(LEFT) + leftS;
  int rerr = analogRead(RIGHT) - rightS;
  int err = (lerr+rerr)/2;
  
  int derivative = (pError-err)/dt;
  pError = err;
  integral += err*dt;
  
  Serial.println(analogRead(FRONT));
  int correction = KP*err + KD*derivative;

  

  int s1 = speed1-correction;
  int s2 = speed2+correction*3/4;

  if(s1<0)
    s1 = 0;
  else if(s1>150)
    s1 = 150;
  if(s2<0)
    s2 = 0;
  else if(s2>150)
    s2 = 150;

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
