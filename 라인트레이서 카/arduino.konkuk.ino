#include<Servo.h>
#include"pitches.h" // 음의 주파수를 저장한 pitches.h 헤더 파일을 포함시킴

Servo servoLeft; //왼쪽 서보모터를 제어하기 위한 변수
Servo servoRight;//오른쪽 서보모터를 제어하기 위한 변수

int IR_outputL=0; //Left sensor 를 0번 핀에 연결
int IR_outputM=1; //Middle sensor를 1번핀에 연결
int IR_outputR=2; //Right sensor를 2번핀에 연결

int LED_R=3; //빨간색 LED를 3번 핀에 연결한다
int LED_G=4; //초록색 LED를 4번핀에 연결한다.
int LED_Y=5; //노란색 LED를 5번핀에 연결한다.
int speakerPin=9; //피에조 스피커를 9번핀에 연결한다.


int check=100; //check변수 초깃값을 100으로 설정(센서가 어떤값을 읽었는지 알기 위해 check
변수 선언)
int pre_check; //이전에 check가 어떤값을 읽었는지 알기 위해 pre_check 변수 선언
int musicCount=0; //BWB 상태가 검출이 될때마다 1씩 더한다, musicCount=2가 되면 음악을
재생할 조건이 된다.
int threshold=860; //센서가 흰색과 검은색을 판단하는 기준. 센서가 읽는 값이 860 이상이면
검은색, 그보다 작으면 흰색으로 인식한다.


unsigned long current_time=0;// 현재 시간을 저장할 변수
unsigned long new_BBB_time; //BBB에 들어온 시간을 저장할 변수
unsigned long pre_BBB_time=-1000; //이전에 BBB에 들어왔었던 시간을 저장 할 변수
unsigned long BBB_enter_time; //BBB에 들어온 시간을 저장할 변수
char notes[]={'c','d','e','f','g','C'}; //연주해야할 악보에 포함된 음의 종류
double durations[] =
{2,2,0.33,0.33,0.33,2,1,0.33,0.33,0.33,2,1,0.33,0.33,0.33,3,2,2,0.33,0.33,0.33,2,1,0.33,0.33,0.33,2,1,0.33,
0.33,0.33,3}; //악보에 있는 음의 박자를 순서대로 나타낸 배열
int pitches[]={NOTE_C5,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_G5,NOTE_C6}; //c,d,e,f,g,C에 해당하는
음의 주파수값을 나타낸 배열
char melodies[]={"cgfedCgfedCgfefdcgfedCgfedCgfefd"}; //연주할 악보의 계이름을 순서대로 나
타낸 배열



int noteCount=sizeof(notes)/sizeof(notes[0]); //notes배열의 원소 개수
int melodyCount=sizeof(melodies)/sizeof(melodies[0]);//연주할 음의 개수
int duration=0;
int index=0;
int playMusic=0;
int changeCount=0; //BBB에서 상태가 바뀌었는지 안 바뀌었는지 확인하기 위해 설정한 변수
int changeCheck; // BBB에서 상태가 바뀌었는지 안 바뀌었는지 확인하기 위해 설정한 변수


int stopCount=0; //정지미션에서 짧은시간동안 BBB가 세 번 검출되면 정지해야 하는데 이때 몇
번 검출 되었는지 count 해주는 변수

int pre_Left_check=0; //왼쪽 센서가 이전에 어떤 값을 읽었는지 저장하는 변수
int pre_Right_check=0; //오른쪽 센서가 이전에 어떤 값을 읽었는지 저장하는 변수


unsigned long play_time; //음악 연주시 play_time을 저장하는 변수
unsigned long play_start_time=-1000; //음악 연주시 한 음을 연주 시작한 시간을 저장한 변수


void goForward(int duration); //앞으로 가는 함수
void goLeftsmooth(int duration); //왼쪽으로 부드럽게 가는 함수
void goRightsmooth(int duration); //오른쪽으로 부드럽게 가는 함수
void goLeft(int duration); //좌회전
void goRight(int duration); //우회전
void motorStop(int duration); //정지
void spinRight(int duration); //오른쪽으로 큰 각도로 돌 때 실행하는 함수
void spinLeft(int duration);// 왼쪽으로 큰 각도로 돌 때 실행하는 함수
void setup() //한번만 실행하는 부분
{
 //Serial.begin(9600); //1초에 9600비트씩 PC와 통신
  pinMode(IR_outputL,INPUT); //0번핀에 연결된 IR센서를 INPUT으로 설정(센서가 값을 읽어오
기 떄문에)
  pinMode(IR_outputM,INPUT); //1번핀에 연결된 IR센서를 INPUT으로 설정(센서가 값을 읽어
오기 떄문에)
  pinMode(IR_outputR,INPUT); //1번핀에 연결된 IR센서를 INPUT으로 설정(센서가 값을 읽어
오기 떄문에)
  pinMode(LED_R,OUTPUT); //3번 핀을 빨간색 LED로 설정
  pinMode(LED_G,OUTPUT); //4번 핀을 초록색 LED로 설정
  pinMode(LED_Y,OUTPUT); //5번 핀을 노란색 LED로 설정


  servoLeft.attach(12); //12번 핀에 왼쪽 서보모터를 연결한다.
  servoRight.attach(13); //13번 핀에 오른쪽 서보모터를 연결한다.


  servoLeft.writeMicroseconds(1500); //모터 정지
  servoRight.writeMicroseconds(1500); //모터 정지

//시작미션 시작
  tone(speakerPin,NOTE_G6,500); // G6음을 0.5초간 발생시킨다
  digitalWrite(LED_Y,HIGH); //노란색 LED를 켠다.
  delay(500); //0.5초간 음이 재생되는 동시에 LED가 켜진다.
  digitalWrite(LED_Y,LOW); //노란색 LED를 끈다.
  digitalWrite(LED_R,HIGH); //빨간색 LED를 켠다.
  delay(500); //0.5초간 기다린다.
  tone(speakerPin,NOTE_G6,500); //피에조 스피커를 통해 G6음을 0.5초간 출력한다.
  digitalWrite(LED_R,LOW); //빨간색 LED를 끈다.
  delay(1000); //피에조 스피커가 출력되는 0.5초와 이후 아무것도 실행하지 않는 0.5초를 기다
린다.
  digitalWrite(LED_Y,HIGH);//노란색 LED를 켠다.
  digitalWrite(LED_R,HIGH); //빨간색 LED를 켠다.
  tone(speakerPin,NOTE_C7,1000); //C7음을 1초간 출력시킨다.
  servoLeft.writeMicroseconds(1600); //왼쪽 서보모터를 반시계방향으로 회전시킨다.
  servoRight.writeMicroseconds(1400); //오른쪽 시계 모터를 시계방향으로 회전시킨다.
//라인트레이서 전진
  delay(1000); //1초간 기다린다.(LED를 1초간 켜야 하기 떄문에)
  digitalWrite(LED_Y,LOW); //노란색 LED를 끈다.
  digitalWrite(LED_R,LOW); //빨간색 LED를 끈다.
//시작미션 종료
}

void loop() //계속 반복
{
  int IR_value_L=analogRead(IR_outputL); //왼쪽 센서가 읽은 값을 IR_value_L변수에 저장한다.
  int IR_value_M=analogRead(IR_outputM); //중간 센서가 읽은 값을 IR_value_M변수에 저장한
다.
  int IR_value_R=analogRead(IR_outputR); //오른쪽 센서가 읽은 값을 IR_value_R변수에 저장한
다.
 
 current_time=millis(); //루프를 시작한 시간을 읽는다(현재시간)
 digitalWrite(LED_R,LOW); //빨간색 LED를 끈다.
 digitalWrite(LED_G,LOW); //초록색 LED를 끈다.
 digitalWrite(LED_Y,LOW); //노란색 LED를 끈다.
/* 모터가 왼쪽으로 가는 함수 안에 초록색 LED를 켜고, 오른쪽으로 가는 함수 안에 빨간색
LED를 켜도록 했다. LED가 계속 켜져 있는 것을 막기 위해 매 루프를 돌 때 마다 LED가 꺼지
도록 했다. */



 //1. Left:White, Mid:Black, Right:White
 if(IR_value_L<threshold && IR_value_M>=threshold && IR_value_R<threshold)
 {
   goForward(1); //WBW경우 앞으로 전진하는 함수 delay값을 1로 하여 센서 값을 좀 더 자
주 읽도록 했다.
    check=1; //WBW인 경우 check 변수에 1을 저장하고 if문을 빠져나온다.
 }
 //2. Left:White, Mid:Black, Right:Black
 else if(IR_value_L<threshold && IR_value_M>=threshold && IR_value_R>=threshold)
 {
   goRightsmooth(50); //WBB인 경우 오른쪽으로 부드럽게 가는 함수로 delay값을 50으로 하
여 조금 덜 자주 값을 읽어오도록 했다.
 check=2; // if문을 빠져나가기 전에 check변수에 2를 저장하고 나간다.
 pre_Left_check=0; //이전에 왼쪽 센서에 기록된 값은 0
 pre_Right_check=1;//이전에 오른쪽 센서에 기록된 값은 1을 저장한다.
 }
 //3. Left:Black, Mid:Black, Right:White
 else if(IR_value_L>=threshold && IR_value_M>=threshold && IR_value_R<threshold)
 {
   goLeftsmooth(50); //오른쪽으로 부드럽게 가는 함수 delay값을 50으로 해서 값을 덜 자주
읽어오도록 했다.
 check=3; //if문을 빠져나가기 전에 check 변수에 3을 저장하고 나간다.
 pre_Left_check=1; //이전에 왼쪽센서에서 읽은 값 1
 pre_Right_check=0; //이전에 오른쪽 센서에서 읽은 값 0
 }
//4. Left:Black, Mid:Black, Right:Black 
/* BBB구간에서 3가지 미션이 있다.
첫번째: 교차로를 그냥 통과할 것
두번째: 일시정지구간
세번째: 정지미션 */
 else if(IR_value_L>=threshold && IR_value_M>=threshold && IR_value_R>=threshold)
 { 
   goForward(1); //교차로를 그냥 통과해야하므로 BBB를 만나도 직진을 해야한다.
 if(check!=4) //이전에 check변수가 BBB일때의 check변수가 아니라면 if 구문 실행
 {
 BBB_enter_time=millis(); //BBB에 새로 들어온 시간을 BBB_enter_time에 저장한다.
 new_BBB_time=millis();//BBB에 새로 들어온 시간을 new_BBB_time에 저장한다.
 changeCheck=changeCount; //BBB 구간에 들어왔을 때 changeCount값을 changeCheck에
저장한다. 만약 changeCount값이 바뀌게 되면 changeCheck와의 비교로 BBB에서 다른 상태로
의 변화가 있었던 것을 파악할 수 있다.
 /* BBB_enter_time 과 new_BBB_time중 하나만 써도 상관없으나,
미션마다 변수를 구분해서 잘 알아보기 위해 다른 변수를 사용해서 구분했다.
BBB_enter_time: 1초 멈춤 구간미션에 사용, 
new_BBB_time: 종료미션에 사용*/
 }
 else //이전에 읽어온 check 값이 4였다면 두꺼운 트랙안에 들어와 있는 상태라는 것
 {
   goForward(1); //정지하기 전 까지 직진해야하므로 직진함수를 사용.
 }
 if((current_time-BBB_enter_time>=250) && (current_time-BBB_enter_time<=600) && 
(changeCheck+1==changeCount)) //BBB구간에 들어오고 250ms이상 600ms 이하의 시간동안
상태가 변하지 않았다면 if구문을 실행한다.
 {
   servoLeft.writeMicroseconds(1500); //왼쪽 모터 정지
   servoRight.writeMicroseconds(1500); //오른쪽 모터 정지
   delay(1000); //1초간 멈춤
 }
/* BBB 두꺼운 트랙 주행미션: 센서에 BBB상태가 바뀌지않고 250ms정도 있었다면 두꺼운
트랙을 약 5.4cm전진 했다고 볼 수 있다. 그 조건을 나타내기 위해 changeCount라는 변수를
사용해서 두꺼운 트랙임을 인식하게 했다. changeCount변수가 바뀌지 않으면 BBB상태에서
변화가 없었다는 의미이고 이는 BBB트랙임을 의미한다. 하지만 changeCount변수값이
바뀌었다면 두꺼운 BBB트랙이 아니라는 의미이고, 이 조건문을 실행하지 않는다. */
 //종료구간 코드
/* 짧은 시간동안 BBB구간이 3번 검출되어야 한다.
BWB-BBB-BWB-BBB-BWB-BBB 순으로 검출되었을 때
마지막 BBB를 검출 하는 즉시 정지해야 한다*/
 {if(new_BBB_time-pre_BBB_time>=60 && new_BBB_time-pre_BBB_time<=600 && 
changeCheck+1!=changeCount) //짧은시간동에 BBB구간이 검출되면 실행되는 코드
 {
 stopCount++; // 처음 BBB통과 후 BWB로 바뀌었다가 짧은 시간 내에 다시 BBB로 바
뀌면 stopCount 값을 1 증가시킨다.
 if(stopCount==2) //BBB구간을 3번 만나게 되면 stopCount==2이므로 정지조건을 충족
하게 되어 if문을 실행한다.
 { //정지미션
 servoLeft.detach(); //왼쪽 바퀴 분리
 servoRight.detach(); //오른쪽 바퀴 분리
 tone(speakerPin,NOTE_C7,1000); // C7음 1초간 출력
 digitalWrite(LED_Y,HIGH);
 digitalWrite(LED_R,HIGH);
 delay(500); // 노란색, 빨간색 LED 0.5초간 켜기
 digitalWrite(LED_Y,LOW);
 digitalWrite(LED_R,LOW);
 delay(500); //노란색, 빨간색 LED 0.5초간 끄기
 digitalWrite(LED_Y,HIGH);
 digitalWrite(LED_R,HIGH);
 delay(500); // 노란색, 빨간색 LED 0.5초간 켜기
 digitalWrite(LED_Y,LOW);
 digitalWrite(LED_R,LOW);
 delay(500); //노란색, 빨간색 LED를 0.5초간 끈다.
 tone(speakerPin,NOTE_C7,1000); //C7음을 1초간 재생
 digitalWrite(LED_Y,HIGH);
 digitalWrite(LED_R,HIGH);
 delay(500); //노란색, 빨간색 LED를 0.5초간 켠다
 digitalWrite(LED_Y,LOW);
 digitalWrite(LED_R,LOW);
 digitalWrite(LED_G,LOW);
 delay(100000);//모든 LED를 끄고, 다른 기능을 실행하지 않기 위해 delay 함수로 긴
시간동안 기다리도록 했다.
 }
 }
 else if(changeCheck+1!=changeCount) // 위의 if문 안의 조건 시간을 만족시키지 않으면
stopCount값을 0으로 초기화 시킨다
 {
 stopCount=0;
 }
 }
 check=4; // check값을 4로 설정(이전의 check값과 비교하기 위해 미리 check값을 4로 바꾼
다)
 if(pre_check!=check) //BBB if 구문 안으로 들어 오기 전 check값이 지금 check값인 4가 아
니 라면
 {
 changeCount++; //changeCount값을 1 증가시킨다.(변화를 감지한다)
 }// 변화 감지
 pre_BBB_time=new_BBB_time; //BBB if구문을 빠져나가기 전 pre_BBB_time에 현재
pre_BBB_time을 저장한다.
 } 
 //5. Left:Black, Mid:White, Right:White
 else if(IR_value_L>=threshold && IR_value_M<threshold && IR_value_R<threshold)
 { //BWW구간인 경우
 goLeft(1); //좌회전한다
 check=5; //if구문을 빠져나가기 전 check에 5를 저장한다.
 pre_Left_check=1; //이전에 왼쪽 센서에 저장된 값 1
 pre_Right_check=0; // 이전에 오른쪽 센서에 저장된 값 0
 }
 //6. Left:White, Mid:White, Right:Black
 else if(IR_value_L<threshold && IR_value_M<threshold && IR_value_R>=threshold)
 {
 goRight(1); // 우회전 함수
 pre_Left_check=0; //이전에 왼쪽 센서에 저장 된 값 0
 pre_Right_check=1; //이전에 오른쪽 센서에 저장 된 값 1
 check=6; //if 구문을 빠져나오기 전 check변수에 6을 저장한다.
 }
//7. Left:White, Mid:White, Right:White
/* 라인트레이서가 급커브를 만나면 WWW를 인식하게 된다.
이전에 왼쪽센서에 읽은 값이 1이면 좌회전을 하고,
오른쪽 센서에 읽은 값이 1이면 우회전을 한다. */
 else if(IR_value_L<threshold && IR_value_M<threshold && IR_value_R<threshold)
 { 
 if(pre_Right_check==1 && pre_Left_check==0) // 6: WWB , 2: WBB
 { //이전에 오른쪽에 1이 검출되었을 때는 오른쪽으로 급커브가 있다는 말이므로 오른쪽으
로 회전을 시켜준다.
 int a=0, b=0,c=0;
 while(a!=1) // a:1 , b=0 ,c=0 일때 까지 돌아야 하니까 a!=0 
 { 
 IR_value_L=analogRead(IR_outputL);
 IR_value_M=analogRead(IR_outputM);
 IR_value_R=analogRead(IR_outputR);
 if(IR_value_L>=threshold)
 {a=1;}
 else
 {a=0;} //왼쪽 센서에 인식된 값이 threshold값 보다 크면 a=1 작으면 a=0
 
 if(IR_value_M>=threshold)
 {b=1;}
 else
 {b=0;} //가운데 센서에 인식된 값이 threshold값 보다 크면 b=1 작으면 b=0
 
 if(IR_value_R>=threshold)
 {c=1;}
 else
 {c=0;} //오른쪽 센서에 인식된 값이 threshold값 보다 크면 c=1 작으면 c=0
 spinRight(1);
 
 }
 //a=1이 검출될 때까지 spinRight함수를 실행하는 while구문 실행 
 }
 else if(pre_Left_check==1 && pre_Right_check==0)//5: BWW 3: BBW
 {
 int a=0, b=0,c=0;
 while(c!=1)
 { 
 IR_value_L=analogRead(IR_outputL);
 IR_value_M=analogRead(IR_outputM);
 IR_value_R=analogRead(IR_outputR);
 if(IR_value_L>=threshold)
 {a=1;}
 else
 {a=0;} //왼쪽 센서에 인식된 값이 threshold값 보다 크면 a=1 작으면 a=0
 
 if(IR_value_M>=threshold)
 {b=1;}
 else
 {b=0;} //가운데 센서에 인식된 값이 threshold값 보다 크면 b=1 작으면 b=0
 
 if(IR_value_R>=threshold)
 {c=1;}
 else
 {c=0;} //오른쪽 센서에 인식된 값이 threshold값 보다 크면 c=1 작으면 c=0
 spinLeft(1);
 } //오른쪽 센서에 (c=1)이 인식 될 때까지 while 구문을 통해 spinRight함수를 실행한다
 }
 check=7; //check변수에 7을 저장하고 if 구문을 빠져나온다.
 }
//8. Left:Black, Mid:White, Right:Black 
/* 음악 재생 미션이 있는 BWB 구문
BWB가 2번 검출되면 playMusic변수에 1을 대입해서
음악 실행 블록을 실행하도록 코드를 작성했다.*/
 else if(IR_value_L>=threshold && IR_value_M<threshold && IR_value_R>=threshold)
 {
 goForward(10); BWB구문에 들어왔을 때 직진해야함.
 if(check!=8) //이전의 check가 8이 아니라면(이전에 BWB상태가 아니었다면)
 {
 musicCount++; //musicCount변수에 1을 더한다.
 if(musicCount==2) //WBW-BWB-WBW-BWB 이렇게 두 번째로 상태가 BWB로 상태가 바
뀌는 순간
 {
 playMusic=1; //playMusic 변수를 1로 만들어서 음악 재생 블록을 실행시킨다.
 }
 }
 check=8; //check에 8을 저장하고 if구문을 빠져나온다.
 }
if(playMusic==1) // BWB if 문에서 playMusic==1 을 만족시키는 순간 음악을 재생한다.
 {
 play_time = current_time - play_start_time; //현재시간에서 음 재생을 시작한 시간을
play_time변수에 저장한다.
 if((play_time>=1.3*duration) && (index<melodyCount)) //play_time 에 저장된 변수가
duration*1.3 의 값보다 크고, index가 melodyCount값보다 작으면 다음 음을 재생한다.
 {
 play_start_time=millis(); //새로운 음을 재생 시작한 시간을 play_start_time에 millis()함수
를 통해 저장한다.
 for(int i=0;i<noteCount;i++) //재생할 음을 찾는 과정
 {
 if(melodies[index]==notes[i]) //index첨자에 해당하는 음이 notes[]배열에서 몇번째 인
지 찾고, 그 음의 주파수 값을 pitches[]배열에서 찾아 tone 함수로 음을 재생한다.
 {
 duration=250*durations[index]; //1박의 기준을 250ms로 해서 재생해야할 음의 박자
를 durations[]배열에서 읽어와 음을 재생할 시간을 duration변수에 저장한다.
 tone(speakerPin,pitches[i],duration); //tone 함수로 재생해야할 음을 duration시간동
안 재생한다.
 }
 }
 index++; //1개 음의 재생이 끝나면 index를 1 증가시켜 다음 음을 증가시키도록 한다.
 if(index==melodyCount) //melodyCount의 값이 index의 값과 같으면
 {
 playMusic=0; //playMusic변수의 값을 0으로 설정해 다시 음악이 연주되지 않도록 한
다. 만약 음악을 무한반복 해야한다면 playMusic=0대신 index=0을 써준다.
 }
 }
 }
 pre_check=check; //loop구문이 끝나기 전에 현재 check값을 pre_check에 저장한다.
 
}
void goForward(int duration) //직진 함수
{
 servoLeft.writeMicroseconds(1680);
 servoRight.writeMicroseconds(1300);
delay(duration);
// 두 모터의 속도차를 고려해서 조정한 값, 기록단축을 위해 최대속도로 전진하도록 했음
}
void goLeftsmooth(int duration)
{
 servoLeft.writeMicroseconds(1600); //왼쪽 바퀴는 오른쪽보다 조금 더 느리게 설정
 servoRight.writeMicroseconds(1300);// 오른쪽 바퀴는 시계방향으로 최대속도
 digitalWrite(LED_G,HIGH); //왼쪽으로 꺾는 경우 초록색 LED를 켜도록 설정
 delay(duration);
}
void goRightsmooth(int duration)
{
 servoLeft.writeMicroseconds(1700); //왼쪽 바퀴는 반시계 방향으로 최대속도
 servoRight.writeMicroseconds(1400); //오른쪽 바퀴는 왼쪽보다 조금 더 느리게 설정
 digitalWrite(LED_R,HIGH); //오른쪽으로 꺾을 경우 빨간색 LED를 켜도록 설정
 delay(duration);
}
void goLeft(int duration)
{ //왼쪽으로 많이 꺾어야 하는 경우
 servoLeft.writeMicroseconds(1500); //왼쪽 바퀴를 정지시키고
 servoRight.writeMicroseconds(1300); //오른쪽 바퀴를 시계방향으로 최대속도로 회전
 digitalWrite(LED_G,HIGH); //왼쪽으로 꺾을 경우 초록색 LED 켠다
 delay(duration);
}
void goRight(int duration)
{//오른쪽으로 많이 꺾어야 하는 경우
 servoLeft.writeMicroseconds(1700); //왼쪽 바퀴를 반시계방향으로 최대속도로 회전시킨다.
 servoRight.writeMicroseconds(1500); //오른쪽 바퀴 정지
 digitalWrite(LED_R,HIGH); //오른쪽으로 꺾을 경우 빨간색 LED를 켠다.
 delay(duration);
}
void motorStop(int duration)
{//라인트레이서 정지 함수
 servoLeft.writeMicroseconds(1500); //왼쪽 모터 정지
 servoRight.writeMicroseconds(1500); //오른쪽 모터 정지
 delay(duration);
}
void spinRight(int duration)
{ //오른쪽으로 급커브를 만났을 때(Z구간) 오른쪽으로 제자리 회전 하는 함수
 servoLeft.writeMicroseconds(1700); //반시계 방향으로 최대속도 회전
 servoRight.writeMicroseconds(1700); //반시계 방향으로 최대속도 회전
 digitalWrite(LED_R,HIGH); // 오른쪽으로 꺾을 경우 빨간색 LED를 켠다.
 delay(duration);
}
void spinLeft(int duration)
{ // 왼쪽 방향 급커브를 만났을 때 (Z구간) 왼쪽 방향으로 제자리 회전
 servoLeft.writeMicroseconds(1300); //시계방향으로 최대속도 회전
 servoRight.writeMicroseconds(1300); //시계방향으로 최대속도 회전
 digitalWrite(LED_G,HIGH); // 왼쪽으로 꺾을 경우 초록색 LED 켠다
 delay(duration);
}