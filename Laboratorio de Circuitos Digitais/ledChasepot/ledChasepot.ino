byte ledPin[] = {2,3,4,5,6,7,8,9,10,11};
int ledDelay, directionFirst = 1, directionLast = -1, firstLED=0, lastLED=9, potPin=A0;
unsigned long changeTime;

void setup(){
    for(int i=0; i<10; i++){
        pinMode(ledPin[i], OUTPUT);
    }
    changeTime = millis();
}

void loop(){
    ledDelay = analogRead(potPin);
    ledDelay = map(ledDelay, 0, 1023, 20, 255);
    if(millis()-changeTime > ledDelay){
        changeLed();
        changeTime = millis();
    }
}
void changeLed(){
    for(int x=0; x<10; x++){
        digitalWrite(ledPin[x], LOW);
    }
    digitalWrite(ledPin[firstLED], HIGH);
    digitalWrite(ledPin[lastLED], HIGH);
    firstLED += directionFirst;
    lastLED += directionLast;
    if(firstLED == 4) directionFirst = -1;
    else if(firstLED == 0) directionFirst = 1;
    if(lastLED == 5) directionLast = 1;
    else if(lastLED == 9) directionLast = -1;
}