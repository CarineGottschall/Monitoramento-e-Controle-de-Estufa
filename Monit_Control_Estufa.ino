//Monitoramento e controle de temperatura, umidade e luminosidade de uma estufa para plantio abrigado de tomates. Para tal foi utilizado: LM35 para monitoramento de temperatura; 
//DHT11 para monitoramento de umidade e temperatura; LDR para monitoramento da luminosidade. Para o controle dessas variáveis serão utilizados dois módulos relés para acionamento
//de dois coolers e uma lâmpada.

//LER UMIDADE E TEMPERATURA
#include "DHT.h"

#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 

// Conecte pino 1 do sensor (esquerda) ao +5V
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pino 2 (dados) e ao pino 1 (VCC) do sensor
DHT dht(DHTPIN, DHTTYPE);//define o objeto DHT 11 com as atribuições das funções da biblioteca

//Porta ligada ao pino IN1 do modulo
int porta_rele1 = 7;
//Porta ligada ao pino IN2 do modulo
int porta_rele2 = 8;

//Sensor de temperatura usando o LM35
const int LM35 = A0; // Define o pino que lera a saída do LM35
float temperatura; // Variável que armazenará a temperatura medida

//Sensor de luminosidade usando o LDR
int pinoled = 10; //Pino ligado ao anodo do led  
int pinopot = 5;  //Pino ligado ao LDR  
int valorpot = 0; //Armazena valor lido do LDR, leitura analógica entre 0 e 1023  
float luminosidade = 0; //Valor de luminosidade do led 
  
void setup() {
  
Serial.begin(9600); // inicializa a comunicação serial

//LDR
pinMode(pinoled, OUTPUT); //Define o pino do led como saída  
pinMode(pinopot, INPUT);  //Define o pino do LDR como entrada 

//DHT11
 Serial.println("DHT11 test!");
 dht.begin();

 //Define pinos para o rele como saida
  pinMode(porta_rele1, OUTPUT); 
  pinMode(porta_rele2, OUTPUT);

}
  
void loop() {

  /////////////////////////////////////////////////////////////////////Monitoramento de temperatura/////////////////////////////////////////////////////////////////////
  temperatura = (float(analogRead(LM35))*5/(1023))/0.01;
  Serial.print("\nTemperatura externa: ");
  Serial.print(temperatura);
  Serial.println(" °C");
  delay(1000);

  /////////////////////////////////////////////////////////////////////Monitoramento e controle de luminosidade/////////////////////////////////////////////////////////////////////
  // Le o valor - analogico - do LDR  
  valorpot = analogRead(pinopot);  

  // Converte o valor lido do LDR
  luminosidade =  map(valorpot, 400, 1023, 0, 255); 
  Serial.print("Valor lido do LDR : ");   

  // Mostra o valor lido do LDR no monitor serial  
  Serial.print(valorpot);  
  Serial.print(" = Luminosidade : ");  

  // Mostra o valor da luminosidade no monitor serial  
  Serial.println(luminosidade); 
  
  // Acende o led com luminosidade variável
 if (valorpot <= 400){
 analogWrite(pinoled, 0); 
 }
 else {
  analogWrite(pinoled, luminosidade);
 }

  /////////////////////////////////////////////////////Monitoramento e controle de umidade e temperatura interna////////////////////////////////////////////////////////
  
  // O atraso do sensor pode chegar a 2 segundos. A leitura da temperatura e umidade pode levar 250ms.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT");
  } 
  else 
  {
    Serial.print("\nUmidade: ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print("\nTemperatura interna da estufa: ");
    Serial.print(t);
    Serial.println(" °C");
  }
  delay(1000);
  
  //Controle temperatura externa
  if (t >= 25 ){
  digitalWrite(porta_rele2, LOW);  //Liga rele 1
  }
  else{
  digitalWrite(porta_rele2, HIGH); //Desliga rele 1
  }
  delay(2000); 

  //Controle de umidade
  if (h <= 50){
  digitalWrite(porta_rele1, LOW);  //Liga rele 2
  }
  else {
  digitalWrite(porta_rele1, HIGH); //Desliga rele 2
  }
  delay(2000);
  
}
