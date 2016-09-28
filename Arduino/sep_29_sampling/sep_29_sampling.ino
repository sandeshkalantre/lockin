
#define PI2 6.28
#define FC 1


int SIZE = 256;
int changed = 0;
int reading = 0;
int finished = 0;
int *waveform = 0;
int *outform = 0;
int index = 0;
float xdec = exp(-PI2*FC);

int inputPin = A0;
 
void setup(){
  Serial.begin(9600);
  changed = 1;
  
  pinMode(inputPin, INPUT);
}

void loop(){
  if(Serial.available()>0){
    SIZE = Serial.parseInt();
    changed = 1;
  }
  //if the size has been changed
  if(changed){
    if (waveform != 0) {
      waveform = (int*) realloc(waveform, SIZE * sizeof(int));
    } 
    else {
      waveform = (int*) malloc(SIZE * sizeof(int));
    }
    reading = 1;
    changed = 0;
    index = 0;
  }
  if(reading){
    waveform[index++] = analogRead(inputPin);
    if(index==SIZE){
      index = 0;
      reading = 0;
      finished = 1;
    }
  }
  if(finished){
    for(int i = 0;i<SIZE;i++){
      waveform[i] = sin((PI2/SIZE)*(i+1))*waveform[i];
    }
    if (outform!= 0) {
      outform = (int*) realloc(outform, (SIZE) * sizeof(int));
    } 
    else {
      outform = (int*) malloc((SIZE) * sizeof(int));
    }
    outform[0] = (1-xdec)*waveform[0];
    for(int i = 0;i<SIZE;i++){
      outform[i] = (1-xdec)*waveform[i] + xdec*outform[i-1];
    }
    
    Serial.println("The output: ");
    for(int i = 0;i<SIZE;i++){
      Serial.print(outform[i]);
      Serial.print(" ");
    }
    Serial.println();
    finished = 0;
  }
}
