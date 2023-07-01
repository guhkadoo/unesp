float RGB1[2], RGB2[2], INC[2];
int red, green;
int redPin = 11, greenPin = 10;

void setup(){
    randomSeed(analogRead(A0));

    for(int i=0; i<2; i++) RGB1[i]=0;
    for(int i=0; i<2; i++) RGB2[i]=random(125);
}

void loop(){
    randomSeed(analogRead(A0));
    for(int x=0; x<2; x++){
        INC[x] = (RGB1[x]-RGB2[x])/200;
    }
    for(int x=0; x<200; x++){
        red = int(RGB1[0]);
        green = int(RGB1[1]);

        analogWrite(redPin, red);
        analogWrite(greenPin, green);
        delay(50);

        RGB1[0] -= INC[0];
        RGB1[1] -= INC[1];
    }
    for(int x=0; x<2; x++){
        RGB2[x] = random(356)-100;
        RGB2[x] = constrain(RGB2[x], 0, 255);
        delay(200);
    }
}