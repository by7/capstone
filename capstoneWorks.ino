#define FRONT 18
#define LEFT 17
#define RIGHT 19

#define PWM1 10
#define PWM2 11
#define IN1 5
#define IN2 6
#define IN3 7
#define IN4 8

#define LED 13

int leftS = 940, rightS = 940;

void turn(int dir); // 0 for left +1 for right
double getCorrection(int err);

void setup() {
  // put your setup code here, to run once:
  pinMode(FRONT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(LED, OUTPUT);

  //speed
  pinMode(PWM1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  //motor direction
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  int leftSum = 0, rightSum = 0;
  for (int i = 0; i < 20; i++) {
    leftSum += analogRead(LEFT);
    rightSum += analogRead(RIGHT);
  }
  leftS = leftSum / 20;
  rightS = rightSum / 20;
  
  
//  Serial.print(leftS);
//  Serial.print(' ');
//  Serial.println(rightS);
  
  Serial.begin(9600);
  delay(1000);
}

double KI=1,KD=0.9,KP=1; //changed from int to double to fine tune
int dt=10;
int speed1 = 100;
int speed2 = 95;
int pError=0;
//int integral=0;
int correction = 0;
int front, left, right;
bool leftWall, rightWall, pLeftWall = true, pRightWall = true;
int counter[2] = {0, 0}; //accounts for short stretch of wall
int pLeft = 0, pRight = 0;
void loop() {
  digitalWrite(LED, LOW);
  front = analogRead(FRONT);
  left = analogRead(LEFT);
  right = analogRead(RIGHT);
  
//  Serial.print("Front: ");
//  Serial.println(front);  
//  Serial.print("Left: ");
//  Serial.println(left);
//  Serial.print("Right: ");
//  Serial.println(right);

  //is there a wall?
  leftWall = left > (leftS-250); //used to be 650
  rightWall = right > (rightS-200);
  pLeft = counter[0];
  pRight = counter[1];
  if (leftWall != pLeftWall) {
    
    counter[0] += 1;  
  } 
  if (rightWall != pRightWall) {

    counter[1] += 1;  
  }

  
  //acquiring distances from the walls
  int lerr = -left + leftS;
  int rerr = right - rightS;
  
  int err = (lerr+rerr)/2;

  //accounts for open walls and pockets

  if (!rightWall && !leftWall) {
    err = 0;
  }else if (!leftWall) {
      //digitalWrite(LED, HIGH);
      err = rerr/3;
  } else if (!rightWall) {
      digitalWrite(LED, HIGH);
      err = lerr/3;
  }
    
  
  //int correction = (int) getCorrection(err);
  if(front<700){
    
    if (counter[0] == pLeft && counter[1] == pRight) {
      correction = (int) getCorrection(err);
    }
    if (counter[0] > 10) {
      counter[0] = 0;
      correction = 0;
      pLeftWall =leftWall;
    } if (counter[1] > 10) {
      counter[1] = 0;
      correction = 0;
      pRightWall =rightWall;
    } 
  }
  
//  if(front>=770) {
//    correction = 0;
//  }
  if(front>800){
    //correction = 0; //was commented out for some reason?
    if(right<left) //turn rught
      turn(1);
    else
      turn(0); //turn leftsk
  }


      
  int s1 = speed1-correction;
  int s2 = speed2+correction; //do we still need this 3/4?

  //make sure the speed change isn't too drastic
  if(s1<0)
    s1 = 0;
  else if(s1>150)
    s1 = 150;
  if(s2<0)
    s2 = 0;
  else if(s2>150)
    s2 = 150;

  //Serial.println(err);
  //onwards we go
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  analogWrite(PWM1, s1);
  analogWrite(PWM2, s2);
  
  delay(dt);
}

double getCorrection(int err){ //used to be int
  int derivative = (pError-err)/dt;
  pError = err;
  //integral += err*dt;
  double cor = KP* (double) err + KD*(double) derivative;
  //return KP* err + KD*derivative;
  return cor;
}

void turn(int dir){
  //Serial.println("IN A TURN");
  
//  if (dir) {//to keep right from turning too early
//    delay(200);
//  }  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
  delay(500);

  if (dir == 1) { //right
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  

  analogWrite(PWM1, 150);
  analogWrite(PWM2, 150);

  delay(235); //tree-thirty five
  
  } //left
  else if (dir == 0) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(PWM1, 150);
  analogWrite(PWM2, 150);

  delay(235); //tree-fiddy
  } 

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
  delay(500);
  pError = 0;

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(PWM1, 100);
  analogWrite(PWM2, 100);
  delay(100);
}
