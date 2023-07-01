byte ledPin[] = {2,3,4,5,6,7,8,9,10,11};
int ledDelay(65), direction = 1, currentLED=0;
unsigned long changeTime;

void setup(){
    for(int i=0; i<10; i++){
        pinMode(ledPin[i], OUTPUT);
    }
    changeTime = millis();
}

void loop(){
    if(millis()-changeTime > ledDelay){
        changeLed();
        changeTime = millis();
    }
}
void changeLed(){
    for(int x=0; x<10; x++){
        digitalWrite(ledPin[x], LOW);
    }
    digitalWrite(ledPin[currentLED], HIGH);
    currentLED += direction;
    if(currentLED == 9) direction = -1;
    if(currentLED == 0) direction = 1;
}