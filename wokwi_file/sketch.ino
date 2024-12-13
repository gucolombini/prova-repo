#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 41 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int pinoBotao = 16;  // Pino do botão
bool estadoBotao = 0;
bool ultimoEstadoBotao = 0; // Váriavel que diz o último estado do botão

const int pinoLdr = 4;  // Pino do LDR
int luzMinima=600;
int contagem = 0; // conta quantas vezes botao foi pressionado

unsigned long ultimaPiscada = 0; // Usado para o intervalo de 1 seg no loop
unsigned long ultimaPressionada = 0; // Ultima vez que o botao foi pressionado

void setup() {
  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(pinoBotao, INPUT); // Inicializa o pino do botão como input
  pinMode(pinoLdr, INPUT); // Inicializa o pino do LDR como input

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() == WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int ldrstatus=analogRead(pinoLdr); // Detecta luminosidade
  if(ldrstatus>=luzMinima){
//    Serial.print("Escuro, piscar LED amarelo!");
    Serial.println(ldrstatus);
    escuro();
   }else{
//    Serial.print("Claro, ligar sequência!");
    Serial.println(ldrstatus);
    claro();
  }
  if(digitalRead(pinoBotao)) {
    if (millis() - ultimaPressionada < 100)
    Serial.println("pressionado");
    botaoPressionado();
  }
}

void escuro() { // Pisca luz amarela a cada 1 seg
  if (millis() - ultimaPiscada >= 1000){
    digitalWrite(led_verde, LOW);
    digitalWrite(led_vermelho, LOW);
    if(digitalRead(led_amarelo)) {
      digitalWrite(led_amarelo, LOW);
    } else {
      digitalWrite(led_amarelo, HIGH);
    }
    ultimaPiscada = millis();
  }
}

void claro() {
  if (digitalRead(led_vermelho)) {
    if (millis() - ultimaPiscada >= 5000) {
      digitalWrite(led_vermelho, LOW);
      digitalWrite(led_amarelo, LOW);
      digitalWrite(led_verde, HIGH);
      ultimaPiscada = millis();
    }
  } else if (digitalRead(led_verde)) {
    if (millis() - ultimaPiscada >= 3000) {
      digitalWrite(led_vermelho, LOW);
      digitalWrite(led_amarelo, HIGH);
      digitalWrite(led_verde, LOW);
      ultimaPiscada = millis();
    }
  } else {
    if (millis() - ultimaPiscada >= 2000) {
      digitalWrite(led_vermelho, HIGH);
      digitalWrite(led_amarelo, LOW);
      digitalWrite(led_verde, LOW);
      ultimaPiscada = millis();
    }
  }
}

void botaoPressionado() {
//  if (millis() - ultimaPressionada < 1500) {
//    contagem = contagem + 1;
//  } else {
//    contagem = 0;
//  }
//  if (contagem == 2) {
//    enviarRequisicao();
//    contagem = 0;
//  }
  ultimaPressionada = millis();
  if (digitalRead(led_vermelho)){
    delay(1000);
    digitalWrite(led_vermelho, LOW);
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_verde, HIGH);
    ultimaPiscada = millis();
  }
}

void enviarRequisicao() {
  Serial.println("requisição enviada");
}
