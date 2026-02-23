// Definir los pines
const int escPin = 4;
// Motor conectado en  D4
const int pinSensor = 14;  
// Sensor conectado en D14

// Configuración del Motor
const int freq = 50;
const int resolution = 12;
const int MIN_PWM = 205;   
// Parado del motor y el armado
const int MAX_PWM = 385;   
// Máxima potencia del motor

// Configuración del sensor
volatile long pulsos = 0;
unsigned long tiempoAnterior = 0;
const int ranurasDisco = 20;
// Ranuras del disco fisico

String datoRecibido = "";
// Función para contar los pulsos (Interrupción)
void IRAM_ATTR contarPulsos() {
  pulsos++;
}

void setup() {
  Serial.begin(115200);

  // 1. Configuracion del sensor
  pinMode(pinSensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSensor), contarPulsos, RISING); 

  // 2. Configurar el motor
  ledcAttach(escPin, freq, resolution);

  // 3. Proceso de armado
  Serial.println("Armando ESC... (5 segundos)");
  ledcWrite(escPin,240); 
  delay(5000); 
  Serial.println("¡Motor listo y Sensor activo!");
}

void loop() {
if (Serial.available() > 0) {
  Serial.println("Dato recibido!");
  datoRecibido = Serial.readStringUntil('\n');
  datoRecibido.trim();
  int datoInt = datoRecibido.toInt();
  Serial.println(datoInt);
  ledcWrite(escPin,datoInt);  
}

  // CÁLCULO DE RPM (Cada 1 segundo)
  unsigned long tiempoActual = millis();
  if (tiempoActual - tiempoAnterior >= 1000) {
    
    noInterrupts(); // Pausa breve para leer la variable sin errores
    long pulsosCopia = pulsos;
    pulsos = 0;
    interrupts(); 

    tiempoAnterior = tiempoActual;
    
    // Fórmula = (pulsos / ranuras del disco) x 60 segundos
    float rpm = ((float)pulsosCopia / ranurasDisco) * 60.0;
    
    Serial.print("Pulsos: "); Serial.print(pulsosCopia);
    Serial.print(" | Velocidad: "); Serial.print(rpm);
    Serial.println(" RPM");
  }
}