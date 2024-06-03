//CONFIGURACION TIEMPOS (Segundos)
// La bomba activa apenas se recibe señal del combistato
int activacion_Valvulas_Aceite = 15;
int activacion_Compresores     = 25;  
int tiempo_Compresor_A1        = 20; // Tiempo de funcionamiento Compresor A1
int tiempo_Compresor_A2        = 20; // Tiempo de funcionamiento Compresor A2
int tiempo_Arranque_Estrella   =  6; // Tiempo de Arranque estrella de los Compresores

const int combistato     = 2;   // Señal rele combistato (entrada)
const int bomba_Aceite   = 10;  // Señal Bomba de aceite (salida)
const int valvula_Aceite = 9;   // Señal Valvula de aceite (salida)
const int compresor_A1   = 8;   // Señal estrella compresor A1 (Salida)
const int compresor_A1_T = 7;   // Señal triangulo compresor A1 (Salida)
const int compresor_A2   = 6;   // Señal estrella  Compresor A2 (Salida)
const int compresor_A2_T = 5;   // Señal triangulo Compresor A2 (Salida)
const int forzadores_1   = 4;   // Señal primer par de forzadores
const int forzadores_2   = 3;   // Señal segundo par de forzadores

int estado_Combistato = LOW;          // Variable para rastrear el estado del combistato
unsigned long ciclo_forzador;

unsigned long contador = 0;           //Contador tiempo de funcionamiento de Todo el sistema
int tiempoSistema = 0;
int compresores = 0;                  //Contador tiempo de funcionamiento de los compresores
int tiempoCompresores = 0;

//Reinicio tiempo sistema
int reinicioSistema(int tiempoSistema){
  contador = tiempoSistema;
  return contador;
  }

//Reinicio tiempo compresores
int reinicioCompresores(int tiempoCompresores){
  compresores = tiempoCompresores;
  return compresores;
  }

int forzadores(){
 if(ciclo_forzador % 2 == 0){
    digitalWrite(forzadores_1, LOW);
    digitalWrite(forzadores_2, HIGH);
 }else{
    digitalWrite(forzadores_1, HIGH);
    digitalWrite(forzadores_2, LOW);
 }  
}

void setup() {
  pinMode(combistato, INPUT);
  pinMode(bomba_Aceite, OUTPUT);
  pinMode(valvula_Aceite, OUTPUT);
  pinMode(compresor_A1, OUTPUT);
  pinMode(compresor_A1_T, OUTPUT);
  pinMode(compresor_A2_T, OUTPUT);
  pinMode(compresor_A2, OUTPUT);
  pinMode(forzadores_1, OUTPUT);
  pinMode(forzadores_2, OUTPUT);
 
  Serial.begin(9600); 
  
    digitalWrite(bomba_Aceite, HIGH);
    digitalWrite(valvula_Aceite, HIGH);
    digitalWrite(compresor_A1, HIGH);
    digitalWrite(compresor_A1_T, HIGH);
    digitalWrite(compresor_A2, HIGH);
    digitalWrite(compresor_A2_T, HIGH);
    digitalWrite(forzadores_1, HIGH);
    digitalWrite(forzadores_2, HIGH);
        
}

void loop() {
  
  int lectura_Combistato = digitalRead(combistato); 
  if (lectura_Combistato == HIGH && estado_Combistato == LOW) { 
      
      delay(1000);
      contador ++; 
      
      forzadores();
      
      if(lectura_Combistato == HIGH && contador >= 1){
          digitalWrite(bomba_Aceite, LOW);
    
      Serial.println(contador);
      
          if (lectura_Combistato == HIGH && contador >= activacion_Valvulas_Aceite) {             
              digitalWrite(valvula_Aceite, LOW);
          }

          if (contador  >= activacion_Compresores){
              
              compresores ++;
              digitalWrite(bomba_Aceite, HIGH); // Apaga la Bomba de aceite
              
              if(compresores <= tiempo_Arranque_Estrella){
                      digitalWrite(compresor_A1, LOW);
                      digitalWrite(compresor_A1_T, HIGH);
                      digitalWrite(compresor_A2, HIGH);
                      digitalWrite(compresor_A2_T, HIGH);
                      Serial.print(compresores);
                      Serial.println(" Arranque Estrella Compresor A1 ");
                      }
              if(compresores > tiempo_Arranque_Estrella && compresores < tiempo_Compresor_A1){
                      delay(300);
                      digitalWrite(compresor_A1, HIGH);
                      digitalWrite(compresor_A1_T, LOW);
                      digitalWrite(compresor_A2, HIGH);
                      digitalWrite(compresor_A2_T, HIGH);
                      Serial.print(compresores);
                      Serial.println(" Compresor A1 Activo");
                      }

              if(compresores > tiempo_Compresor_A1 && compresores <= (tiempo_Compresor_A1 + tiempo_Arranque_Estrella)){
                      delay(300);
                      digitalWrite(compresor_A1, HIGH);
                      digitalWrite(compresor_A1_T, HIGH);
                      digitalWrite(compresor_A2, LOW);
                      digitalWrite(compresor_A2_T, HIGH);
                      Serial.print(compresores);
                      Serial.println(" Arranque Estrella Compresor A2");
                  } 

              if(compresores > (tiempo_Compresor_A1 + tiempo_Arranque_Estrella) && compresores <= (tiempo_Compresor_A2 + tiempo_Compresor_A1)){
                      delay(300);
                      digitalWrite(compresor_A1, HIGH);
                      digitalWrite(compresor_A1_T, HIGH);
                      digitalWrite(compresor_A2, HIGH);
                      digitalWrite(compresor_A2_T, LOW);
                      Serial.print(compresores);
                      Serial.println(" Compresor A2 Activo");
                      }

              if(compresores == (tiempo_Compresor_A2 + tiempo_Compresor_A1)){
                  reinicioCompresores(0); 
                  ciclo_forzador ++;
              }  
          }
      }       
  } else{
      delay(1000);
      digitalWrite(bomba_Aceite, HIGH);
    digitalWrite(valvula_Aceite, HIGH);
    digitalWrite(compresor_A1, HIGH);
    digitalWrite(compresor_A1_T, HIGH);
    digitalWrite(compresor_A2, HIGH);
    digitalWrite(compresor_A2_T, HIGH);
    digitalWrite(forzadores_1, HIGH);
    digitalWrite(forzadores_2, HIGH);
      reinicioSistema(0);
      reinicioCompresores(0);
      estado_Combistato = LOW;
      ciclo_forzador = 0;    
  }
}