const int escPin = 14;
const int pinSensor = 4;
const int freq = 60;
volatile long pulsos =0;
int tiempoAnt = 0;
const int resolution = 12; //2 ^12 partes en las que se puede dividir el periodo para analizar las pulsaciones
int velocidad = 245; //se pone así porque es la pulsación para 1ms, que es cuando está detenido (por convención)
                      //periodo 1/60=16.66 ms, y (1ms/periodo) la cantidad de divisiones en las que se analiza cada pulso (12 bits o 2^12)= 245.7 pulsos
String velocidadRecibida="";
void actPulsos(){
  pulsos++;
}

int calcRPM(int tiempoA){
  int rpm = (pulsos*60000)/(tiempoA);
  pulsos = 0;
  return rpm;
}


void setup() {
  Serial.begin(115200);
  ledcAttach(escPin,freq,resolution);
  delay(5000);
  ledcWrite(escPin, velocidad);
  

  pinMode(pinSensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinSensor), actPulsos, RISING);
  delay(5000);
}

void loop() {

  if (Serial.available()) {
    // Leer el dato hasta que se presione Enter
    velocidadRecibida = Serial.readStringUntil('\n');
    velocidadRecibida.trim(); // Eliminar espacios o saltos de línea basura
    int datoInt = velocidadRecibida.toInt();
    ledcWrite(escPin,datoInt);  
  }
  
  int time = millis();
  if(time - tiempoAnt >= 1000){
    Serial.println(pulsos);
    int tiempoA = time - tiempoAnt;
    Serial.print("RPM: ");
    Serial.println(calcRPM(tiempoA));
    tiempoAnt = time;
  }
}