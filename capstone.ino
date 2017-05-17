#define FRONT 18
#define LEFT 17
#define RIGHT 19


#define PWM1 10
#define PWM2 11
#define IN1 5
#define IN2 6
#define IN3 7
#define IN4 8

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
  
  Serial.begin(9600);
}

int KI=1,KD=1,KP=1;
int dt=10;
int speed1 = 70;
int speed2 = 100;

void loop() {
  // put your main code here, to run repeatedly:
  int lerr = analogRead(LEFT);//-780;
  lerr /= 5;
  int rerr = analogRead(RIGHT);
  rerr /= 3;
  int err = (lerr+rerr)/2;
  int correction = KP*err;
  Serial.print(lerr);
  Serial.print(' ');
  Serial.println(rerr);
//  Serial.println(correction);

  //turning? make it go forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(PWM1, speed1-correction);
  analogWrite(PWM2, speed2+correction);

  //check if need to turn
  
  delay(dt);
}
