byte ledPin[] = {2,3,4,5,6,7,8,9,10,11}, ledStart = 9, control=-1, ledEnd = 0;
unsigned long setupTime;

void setup(){
    for(int i=0; i<10; i++) pinMode(ledPin[i], OUTPUT);
    setupTime = millis();
}
void loop(){
    if(millis()-setupTime>300){
        bounce();
        setupTime = millis();
    }
}
void bounce(){
    for(int i=0; i<10; i++) digitalWrite(ledPin[i], LOW);
    digitalWrite(ledPin[ledStart], HIGH);
    ledStart += control;
    if(ledStart==ledEnd && ledEnd!=9) control = 1;
    if(ledStart==9 && ledEnd!=9){
        control = -1;
        ledEnd += 1;
    }
    if(ledEnd==9) control = 0;
}