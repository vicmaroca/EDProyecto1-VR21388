//-----------LIBRERÍAS--------------
#include <stdint.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//----------------VARIABLES--------------------------
uint8_t prendido = 0;
uint8_t bajo = 0;
uint8_t mitad = 0;
uint8_t rapido = 0;
uint8_t temperatura = 1;
uint8_t intensidad = 1;

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //


//----------------ESTADOS------------------------------
typedef enum estados {off, on, lento, medio, alto, frio, tibio, caliente} estados;
estados estadoActual = off;
estados estadoFuturo = off;

//---------------FSM1 : Encendido/Apagado---------------
const uint8_t botonE = 7; //Botón Encendido
const uint8_t botonA = 6; //Botón Apagado

const uint8_t ledPinO = 12; //LED On/Off

//----------------FSM2 : Velocidad------------------
const uint8_t botonS = 4; //Botón Subir Nivel
const uint8_t botonB = 2; //Botón Bajar Nivel

const uint8_t ledV1 = 10; //LED 1
const uint8_t ledV2 = A0;  //LED 2
const uint8_t ledV3 = A1;  //LED 3

//----------------FSM3 : Temperatura----------------
const uint8_t botonC = 9; //Botón Caliente
const uint8_t botonF = 8; //Botón Frío

const uint8_t ledF1 = 11; //LED Frio 1
const uint8_t ledF2 = 11; //LED Frio 2
const uint8_t ledF3 = 11; //LED Frio 3

const uint8_t ledT1 = 5; //LED Tibio 1
const uint8_t ledT2 = 5; //LED Tibio 2

const uint8_t ledC1 = 3; //LED Caliente 1
const uint8_t ledC2 = 3; //LED Caliente 2
const uint8_t ledC3 = 3; //LED Caliente 3

//----------------RELOJ--------------
const long interval = 100;  // interval at which to blink (milliseconds)
unsigned long previousMillis = 0;


void setup() {
//---------------FSM1 : Encendido/Apagado---------------
  pinMode(botonE, INPUT);
  pinMode(botonA, INPUT);

  pinMode(ledPinO, OUTPUT);
//----------------FSM2 : Velocidad----------------------
  pinMode(botonS, INPUT);
  pinMode(botonB, INPUT);
  
  pinMode(ledV1, OUTPUT);
  pinMode(ledV2, OUTPUT);
  pinMode(ledV3, OUTPUT);
//----------------FSM3 : Temperatura----------------
  pinMode(botonC, INPUT);
  pinMode(botonF, INPUT);
  
  pinMode(ledF1, OUTPUT);
  pinMode(ledF2, OUTPUT);
  pinMode(ledF3, OUTPUT);

  pinMode(ledT1, OUTPUT);
  pinMode(ledT2, OUTPUT);

  pinMode(ledC1, OUTPUT);
  pinMode(ledC2, OUTPUT);
  pinMode(ledC3, OUTPUT);

//--------------------LCD------------------------
// Inicializar el LCD
  lcd.init();
  //Encender la luz de fondo.
  lcd.backlight();

}

void loop() {
//----------------------lectura de las entradas--------------------
  bool estadoE = digitalRead(botonE);
  bool estadoA = digitalRead(botonA);

  bool estadoS = digitalRead(botonS);
  bool estadoB = digitalRead(botonB);

  bool estadoC = digitalRead(botonC);
  bool estadoF = digitalRead(botonF);

//----------------------transiciones de estado----------------------
  switch (estadoActual){
    case off: 

      if(estadoE == HIGH )
        estadoFuturo = on;
      else
        estadoFuturo = off;
      break;

    case on:

      if(estadoA == HIGH )
        estadoFuturo = off;
      else
        estadoFuturo = on;

        estadoFuturo = lento; 

      break;

    case lento: 

      if(estadoS == HIGH ){
        estadoFuturo = medio;  
        intensidad = 2;
      }else if (estadoB == HIGH){
        estadoFuturo = lento;
        intensidad = 1;
      }

        estadoActual = lento;
        
        if (temperatura == 1){
            estadoFuturo = frio;
        }else if(temperatura == 2){
            estadoFuturo = tibio;
        }else if(temperatura == 3){
            estadoFuturo = caliente;
        }

        if(estadoA == HIGH )
        estadoFuturo = off;

      break;
      

    case medio: 

      if(estadoS == HIGH ){
        estadoFuturo = alto;
        intensidad = 3;
      }else if (estadoB == HIGH){
        estadoFuturo = lento;
        intensidad = 1;
      }

      estadoActual = medio;
        
        if (temperatura == 1){
            estadoFuturo = frio;
        }else if(temperatura == 2){
            estadoFuturo = tibio;
        }else if(temperatura == 3){
            estadoFuturo = caliente;
        }

      if(estadoA == HIGH )
        estadoFuturo = off;

      break;

    case alto: 

      if(estadoS == HIGH ){
        estadoFuturo = alto;
        intensidad = 3;
      }else if (estadoB == HIGH){
        estadoFuturo = medio;
        intensidad = 2;
      }

      estadoActual = alto;
        
      if (temperatura == 1){
          estadoFuturo = frio;
      }else if(temperatura == 2){
          estadoFuturo = tibio;
      }else if(temperatura == 3){
          estadoFuturo = caliente;
      }

      if(estadoA == HIGH )
        estadoFuturo = off;

      break;
      
    case frio: 

        if(estadoC == HIGH){
          estadoFuturo = tibio;  
          temperatura = 2;
        }else if (estadoF == HIGH){
          estadoFuturo = frio;
          temperatura = 1;
        }

        if(intensidad == 1){
          estadoFuturo = lento; 
        }else if (intensidad == 2){
          estadoFuturo = medio;
        }else if (intensidad == 3){
          estadoFuturo = alto;
        }

        if(estadoA == HIGH )
        estadoFuturo = off;

      break;

    case tibio: 

      if(estadoC == HIGH){
        estadoFuturo = caliente;
        temperatura = 3;
      }else if (estadoF == HIGH){
        estadoFuturo = frio;
        temperatura = 1;
      }

      if(intensidad == 1){
          estadoFuturo = lento; 
        }else if (intensidad == 2){
          estadoFuturo = medio;
        }else if (intensidad == 3){
          estadoFuturo = alto;
        }

      if(estadoA == HIGH )
        estadoFuturo = off;
        
      break;
      
    case caliente: 

       if(estadoC == HIGH){
        estadoFuturo = caliente;
        temperatura = 3;
       }else if (estadoF == HIGH){
        estadoFuturo = tibio;
        temperatura = 2;
       }

      if(intensidad == 1){
          estadoFuturo = lento; 
      }else if (intensidad == 2){
          estadoFuturo = medio;
      }else if (intensidad == 3){
          estadoFuturo = alto;
      }

      if(estadoA == HIGH )
        estadoFuturo = off;

      break;
      
  }

  //------------------------SALIDAS-----------------------------
  switch (estadoActual){
    case off: // LED apagado
      digitalWrite(ledPinO, LOW);
      digitalWrite(ledV1, LOW);
      digitalWrite(ledV2, LOW);
      digitalWrite(ledV3, LOW);

      analogWrite(ledF1, 0);
      analogWrite(ledF2, 0);
      analogWrite(ledF3, 0);

      analogWrite(ledT1, 0);
      analogWrite(ledT2, 0);

      analogWrite(ledC1, 0);
      analogWrite(ledC2, 0);
      analogWrite(ledC3, 0);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("OFF");
      lcd.setCursor(4,0);
      lcd.print("Velocidad: 0");
      lcd.setCursor(0,1);
      lcd.print("Temperatura: OFF");
      break;
    case on: // LED encendido
      digitalWrite(ledPinO, HIGH);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ON");
      lcd.setCursor(4,0);
      lcd.print("Velocidad: ");
      lcd.print(intensidad);
      lcd.setCursor(0,1);
      lcd.print("Temp: ");
      lcd.print(temperatura);
      break;

    case lento: // LED encendido
      digitalWrite(ledV1, HIGH);
      digitalWrite(ledV2, LOW);
      digitalWrite(ledV3, LOW);
      
      break;
    case medio: // LED encendido
      digitalWrite(ledV1, LOW);
      digitalWrite(ledV2, HIGH);
      digitalWrite(ledV3, LOW);
      break;
    case alto: // LED encendido
      digitalWrite(ledV1, LOW);
      digitalWrite(ledV2, LOW);
      digitalWrite(ledV3, HIGH);
      break;
    case frio: // LED encendido
      if(intensidad == 1){
      analogWrite(ledF1, 30);
      analogWrite(ledF2, 30);
      analogWrite(ledF3, 30);
      }else if (intensidad == 2){
      analogWrite(ledF1, 120);
      analogWrite(ledF2, 120);
      analogWrite(ledF3, 120);
      }else if (intensidad == 3){
      analogWrite(ledF1, 255);
      analogWrite(ledF2, 255);
      analogWrite(ledF3, 255);
      }

      analogWrite(ledT1, 0);
      analogWrite(ledT2, 0);

      analogWrite(ledC1, 0);
      analogWrite(ledC2, 0);
      analogWrite(ledC3, 0);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ON");
      lcd.setCursor(4,0);
      lcd.print("Velocidad: ");
      lcd.print(intensidad);
      lcd.setCursor(0,1);
      lcd.print("Temp: Frio");
      break;
    case tibio: // LED encendido
      analogWrite(ledF1, 0);
      analogWrite(ledF2, 0);
      analogWrite(ledF3, 0);

      if(intensidad == 1){
      analogWrite(ledT1, 30);
      analogWrite(ledT2, 30);
      }else if (intensidad == 2){
      analogWrite(ledT1, 120);
      analogWrite(ledT2, 120);
      }else if (intensidad == 3){
      analogWrite(ledT1, 255);
      analogWrite(ledT2, 255);
      }

      analogWrite(ledC1, 0);
      analogWrite(ledC2, 0);
      analogWrite(ledC3, 0);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ON");
      lcd.setCursor(4,0);
      lcd.print("Velocidad: ");
      lcd.print(intensidad);
      lcd.setCursor(0,1);
      lcd.print("Temp: Tibio");
      break;
    case caliente: // LED encendido
      analogWrite(ledF1, 0);
      analogWrite(ledF2, 0);
      analogWrite(ledF3, 0);

      analogWrite(ledT1, 0);
      analogWrite(ledT2, 0);

      if(intensidad == 1){
      analogWrite(ledC1, 30);
      analogWrite(ledC2, 30);
      analogWrite(ledC3, 30);
      }else if (intensidad == 2){
      analogWrite(ledC1, 120);
      analogWrite(ledC2, 120);
      analogWrite(ledC3, 120);
      }else if (intensidad == 3){
      analogWrite(ledC1, 255);
      analogWrite(ledC2, 255);
      analogWrite(ledC3, 255);
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ON");
      lcd.setCursor(4,0);
      lcd.print("Velocidad: ");
      lcd.print(intensidad);
      lcd.setCursor(0,1);
      lcd.print("Temp: Caliente");
      break;
    
  }
//----------------Actualización de estado---------------------
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    estadoActual = estadoFuturo;
    previousMillis = currentMillis;
  }
  delay(1);
}
