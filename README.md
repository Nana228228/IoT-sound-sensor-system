# 🏙️ Monitoramento IoT: Contaminação Acústica e Tráfego Urbano

## 📺 Apresentação e Demonstração
Clique no link abaixo para assistir ao pitch do projeto e a demonstração da infraestrutura em tempo real operando com os dados do sensor:

▶️ **[Assistir à Demonstração no YouTube](https://www.youtube.com/watch?v=1LrNW-aGjaU)**

---

## 🎯 Descrição do Projeto
Este projeto consiste em um ecossistema de Internet das Coisas (IoT) desenvolvido para monitorar a correlação entre o ruído urbano e o fluxo de trânsito em vias de grande movimento. 

Alinhado ao **ODS 11 da ONU (Cidades e Comunidades Sustentáveis)**, o sistema estabelece um limite de saúde acústica de 55 dB (recomendação da OMS) e avalia o conforto acústico em um ambiente residencial com janelas fechadas. O objetivo é provar empiricamente que, embora a infraestrutura residencial mitigue o ruído padrão, o tráfego pesado e veículos irregulares rompem a barreira de saúde recomendada.

---

## 🏗️ Arquitetura do Sistema
<img width="912" height="571" alt="Image" src="https://github.com/user-attachments/assets/bf2f0451-1fd2-4357-8050-4fc592a05c3f" />

A solução foi construída utilizando uma arquitetura distribuída em nuvem:

* **Camada Física (Edge):** Microcontrolador ESP32 integrado a um sensor de som (SparkFun) captando envelopes analógicos e interrupções digitais.
* **Comunicação:** Protocolo MQTT (Broker público HiveMQ) via Wi-Fi, utilizando formato JSON.
* **Middleware (Orquestração):** Plataforma Low Code Node-RED para cruzamento de dados, calibração acústica (algoritmo polinomial) e chamadas de API externas.
* **API Externa:** TomTom Traffic API para extração síncrona do índice de retenção veicular (Cálculo da Velocidade Livre vs. Velocidade Atual).
* **Data Lake / Nuvem:** InfluxDB Cloud Serverless (AWS) operando como banco de dados de séries temporais (TSDB).
* **Visualização (Analytics):** Dashboard no Grafana Cloud com eixo dual e limiar (Threshold) de alerta da OMS aos 55 dB.

---

## 💻 Código Fonte e Componentes

Abaixo estão as descrições dos códigos disponibilizados nas pastas deste repositório:

### 1. Dispositivo Físico (ESP32)
O código fonte em C++ desenvolvido na **Arduino IDE** encontra-se na pasta `/esp32_hardware`. 
Ele é responsável por:
* Estabelecer conexão Wi-Fi e MQTT.
* Ler as portas GPIO 34 (Analógica) e 18 (Digital).
* Empacotar os dados brutos e publicá-los em um tópico do broker.

### 2. Plataforma Low Code (Node-RED)
O código fonte estrutural (formato `.json`) encontra-se na pasta `/node_red_middleware`. 
Ele pode ser importado diretamente para qualquer instância do Node-RED para recriar o ecossistema. Contém dois fluxos paralelos que se integram via Memória Global (RAM):
1.  **Fluxo de Tráfego:** Consome a API da TomTom a cada 5 minutos e calcula a porcentagem de congestionamento (0% a 100%).
2.  **Fluxo de Calibração:** Recebe o payload MQTT do hardware, aplica a matriz de calibração residencial (onde silêncio = 40 dB), anexa o congestionamento da RAM e dispara o pacote para a nuvem da AWS.

---

## 🚀 Como Utilizar (Deploy)

1. Clone este repositório: `git clone https://github.com/SEU_USUARIO/SEU_REPOSITORIO.git`
2. Abra o arquivo `.ino` na Arduino IDE, insira suas credenciais de Wi-Fi e faça o upload para o ESP32.
3. Abra a interface do Node-RED, vá em **Menu > Importar** e selecione o arquivo `fluxo_iot_completo.json`.
4. Insira a sua API Key da TomTom no nó HTTP Request do Node-RED.
5. Configure suas credenciais e tokens do InfluxDB no nó correspondente.
6. Pressione **Deploy**.

---

*Desenvolvido para fins acadêmicos e de pesquisa urbana. 2026.*
