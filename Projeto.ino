const int rele = 13; // Porta para o cooler
const int sensorTemp = A0; // Pino do sensor de temperatura
float tempTarget = 25.0; // Temperatura-alvo inicial em graus Celsius

bool programaAtivo = false; // Estado inicial do programa

void setup() {
  Serial.begin(9600);
  pinMode(rele, OUTPUT);
  digitalWrite(rele, HIGH); // Cooler inicialmente desligado (assumindo que HIGH desliga)
}

void loop() {
  // Verifica se há dados disponíveis na porta serial
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n'); // Lê a linha enviada
    if (comando == "L") {
      programaAtivo = true;
    } else if (comando == "D") {
      programaAtivo = false;
      digitalWrite(rele, HIGH); // Desliga o cooler
    } else {
      // Ajusta a temperatura-alvo com o valor recebido
      tempTarget = comando.toFloat();
    }
  }

  if (programaAtivo) {
    int sensorValor = analogRead(sensorTemp);
    float voltage = sensorValor * (5.0 / 1023.0);
    float temperatureC = voltage * 100.0; // Conversão do valor lido para temperatura

    Serial.print("T"); // Prefixo para identificação no Qt
    Serial.println(temperatureC, 2); // Envia a temperatura atual

    ajusteIntensidade(temperatureC);
  }

  delay(1000); // Delay para estabilização e leitura
}

void ajusteIntensidade(float temperatura) {
  float tempDiferenca = temperatura - tempTarget;

  // Determina o estado do cooler e o brilho do LED baseado na diferença de temperatura
  if (tempDiferenca > 0.5) {
    digitalWrite(rele, LOW); // Liga o cooler se acima da temperatura-alvo
  } else if (tempDiferenca < -0.5) {
    digitalWrite(rele, HIGH); // Desliga o cooler se abaixo da temperatura-alvo
  } else {
    // Temperatura dentro da faixa de tolerância
    digitalWrite(rele, HIGH); // Desliga o cooler
  }
}
