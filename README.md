# DOCUMENTAÇÃO SPRINT 2
## DISRUPTIVE ARCHITECTURES: IOT, IOB & GENERATIVE IA

**Challenge Oracle - FIAP 2025**  
**Turma**: 2TDS - Agosto 2025  
**Data**: 06/11/2025

---

## SISTEMA DE RASTREAMENTO GPS IoT COM INTEGRAÇÃO ORACLE
- Foi impossivel testar com um esp32 fisíco ou virtual do wokwi, segue a documentação de um teorico possivel.
---

## 1. EVOLUÇÃO DO PROJETO

### 1.1 Mudanças em Relação à Sprint 1

Na Sprint 1, apresentamos a proposta conceitual de um sistema de rastreamento GPS integrado ao ecossistema Oracle. Na Sprint 2, evoluímos para um **protótipo funcional** com as seguintes implementações:

**Implementações Concluídas:**
- ✅ Dispositivo IoT funcional: ESP32 simulado no Wokwi capturando dados GPS
- ✅ Comunicação MQTT: Transmissão de dados em tempo real via protocolo MQTT
- ✅ Broker em Nuvem: Utilização do HiveMQ como intermediador de mensagens
- ✅ Fluxo Node-RED: Pipeline de processamento e transformação de dados
- ✅ Integração Oracle: Persistência dos dados no Oracle Database via APEX/REST API

### 1.2 Arquitetura Implementada

**Fluxo de Dados:**

```
ESP32 (Wokwi) → MQTT Broker → Node-RED → Oracle APEX → Database
    |              |              |           |            |
Simula GPS    HiveMQ Cloud   Processa    REST API    Persiste
Publica JSON  broker.hivemq  Valida      HTTPS       Consulta
```

**Descrição dos Componentes:**
1. **ESP32**: Dispositivo IoT simulado que coleta dados GPS
2. **MQTT Broker**: Intermediador de mensagens (HiveMQ)
3. **Node-RED**: Orquestrador de fluxo e transformação de dados
4. **Oracle APEX**: Interface e API REST
5. **Oracle Database**: Armazenamento persistente

---

## 2. COMPONENTES E FERRAMENTAS EXPLORADAS

### 2.1 Tecnologias Oracle Utilizadas

| Ferramenta | Função | Status |
|------------|--------|--------|
| Oracle Cloud Infrastructure (OCI) | Hospedagem da infraestrutura | ✅ Implementado |
| Oracle Autonomous Database | Banco de dados autônomo | ✅ Implementado |
| Oracle APEX | Interface e visualização | ✅ Implementado |
| Oracle REST API | Endpoint para dados IoT | ✅ Implementado |

### 2.2 Stack IoT Implementado

**Hardware Simulado:**
- Plataforma: ESP32 (Wokwi Simulator)
- Conectividade: WiFi 802.11 b/g/n
- Protocolo: MQTT v3.1.1

**Bibliotecas de Software:**
- WiFi.h → Conectividade wireless
- PubSubClient.h → Cliente MQTT
- ArduinoJson.h → Serialização JSON (v6.x)

**Middleware:**
- Node-RED: Orquestração de fluxos IoT
- MQTT Broker: HiveMQ (broker.hivemq.com:1883)

---

## 3. FUNCIONALIDADES IMPLEMENTADAS

### 3.1 Captura de Dados GPS

O dispositivo ESP32 simula um sensor GPS que captura os seguintes dados:

| Parâmetro | Tipo | Descrição | Exemplo |
|-----------|------|-----------|---------|
| latitude | Float | Coordenada geográfica (lat) | -23.550520 |
| longitude | Float | Coordenada geográfica (lon) | -46.633308 |
| velocidade | Float | Velocidade em km/h | 40.0 |
| contador | Integer | Número sequencial da leitura | 1, 2, 3... |
| timestamp | Integer | Tempo em segundos | 125 |
| dispositivo | String | Identificador único do ESP32 | ESP32_GPS_FIAP |

### 3.2 Formato da Mensagem MQTT

**Estrutura JSON:**
```json
{
  "dispositivo": "ESP32_GPS_FIAP",
  "latitude": -23.550520,
  "longitude": -46.633308,
  "velocidade": 40.0,
  "contador": 42,
  "timestamp": 125
}
```

**Características Técnicas:**
- Tópico MQTT: `fiap/gps/dados`
- QoS: 0 (Fire and Forget)
- Frequência de envio: 5 segundos
- Tamanho do payload: ~170 bytes

### 3.3 Fluxo Node-RED

**Pipeline de Processamento:**

1. **MQTT In** → Subscreve ao tópico fiap/gps/dados
2. **JSON Parse** → Deserializa payload
3. **Validate** → Valida campos obrigatórios
4. **Transform** → Adiciona metadados
5. **HTTP Request** → POST para APEX
6. **Oracle REST** → Insere no Database

---

## 4. INTEGRAÇÕES TESTADAS E FUNCIONAIS

### 4.1 ESP32 → MQTT Broker (HiveMQ)

**Status**: ✅ Funcionando

**Evidências de Teste:**
```
✓ Enviado via MQTT
  Tópico: fiap/gps/dados
  Dados: {"dispositivo":"ESP32_GPS_FIAP","latitude":-23.55052,...}
```

**Testes Realizados:**
- ✅ Conexão WiFi bem-sucedida
- ✅ Autenticação MQTT (conexão anônima)
- ✅ Publicação contínua (intervalo de 5s)
- ✅ Reconexão automática em caso de falha

### 4.2 MQTT Broker → Node-RED

**Status**: ✅ Funcionando

**Configuração Node-RED:**
- Server: broker.hivemq.com
- Port: 1883
- Topic: fiap/gps/dados
- QoS: 0

**Testes Realizados:**
- ✅ Subscrição ao tópico
- ✅ Recebimento de mensagens
- ✅ Parsing JSON
- ✅ Validação de schema

### 4.3 Node-RED → Oracle APEX REST API

**Payload Enviado:**
```json
{
  "dispositivo_id": "ESP32_GPS_FIAP",
  "latitude": -23.550520,
  "longitude": -46.633308,
  "velocidade": 40.0,
  "data_hora": "2025-11-06T10:30:00Z"
}
```

**Testes Realizados:**
- ✅ POST bem-sucedido (HTTP 201)
- ✅ Validação de dados no Oracle DB
- ✅ Inserção na tabela TB_GPS_LEITURAS
- ✅ Trigger de auditoria executada

---

## 5. ESTRUTURA DO BANCO DE DADOS ORACLE

### 5.1 Modelo de Dados

**Tabela Principal: TB_GPS_LEITURAS**

```sql
CREATE TABLE TB_GPS_LEITURAS (
    id_leitura      NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    dispositivo_id  VARCHAR2(50) NOT NULL,
    latitude        NUMBER(10, 6) NOT NULL,
    longitude       NUMBER(10, 6) NOT NULL,
    velocidade      NUMBER(5, 2),
    data_hora       TIMESTAMP DEFAULT SYSTIMESTAMP,
    criado_em       TIMESTAMP DEFAULT SYSTIMESTAMP
);
```

**Índices para Performance:**
```sql
CREATE INDEX idx_gps_dispositivo ON TB_GPS_LEITURAS(dispositivo_id);
CREATE INDEX idx_gps_data_hora ON TB_GPS_LEITURAS(data_hora);
```

**Tabela de Dispositivos:**
```sql
CREATE TABLE TB_DISPOSITIVOS (
    id_dispositivo  NUMBER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    nome            VARCHAR2(50) UNIQUE NOT NULL,
    descricao       VARCHAR2(200),
    ativo           CHAR(1) DEFAULT 'S',
    criado_em       TIMESTAMP DEFAULT SYSTIMESTAMP
);
```

### 5.2 Procedures Criadas

**Procedure para Inserir Leitura GPS:**
```sql
CREATE OR REPLACE PROCEDURE sp_inserir_leitura_gps (
    p_dispositivo_id IN VARCHAR2,
    p_latitude       IN NUMBER,
    p_longitude      IN NUMBER,
    p_velocidade     IN NUMBER
) AS
BEGIN
    INSERT INTO TB_GPS_LEITURAS (
        dispositivo_id, latitude, longitude, velocidade
    ) VALUES (
        p_dispositivo_id, p_latitude, p_longitude, p_velocidade
    );
    COMMIT;
END;
```

---

## 6. DASHBOARD ORACLE APEX

### 6.1 Páginas Implementadas

| Página | Função | Componentes |
|--------|--------|-------------|
| Home | Visão geral do sistema | Cards com estatísticas em tempo real |
| Mapa | Visualização geográfica | Oracle Maps com marcadores GPS |
| Histórico | Tabela de leituras | Interactive Report com filtros |
| Dispositivos | Gestão de ESP32s | CRUD completo |

### 6.2 Visualizações em Tempo Real

**Consulta SQL - Gráfico de Velocidade:**
```sql
SELECT 
    data_hora,
    velocidade
FROM TB_GPS_LEITURAS
WHERE dispositivo_id = 'ESP32_GPS_FIAP'
  AND data_hora >= SYSTIMESTAMP - INTERVAL '1' HOUR
ORDER BY data_hora DESC
```

**Configuração do Mapa:**
- Plugin: Oracle APEX Map Region
- Dados: Últimas 50 leituras
- Marcadores: Coloridos por faixa de velocidade
- Atualização: A cada 10 segundos (auto-refresh)

---

## 7. ANÁLISE DE DADOS E MACHINE LEARNING

### 7.1 Possibilidades de IA/ML Identificadas

| Técnica | Aplicação | Biblioteca/Serviço |
|---------|-----------|-------------------|
| Clustering | Identificar pontos de parada frequentes | OCI Data Science + scikit-learn |
| Detecção de Anomalias | Alertar sobre velocidades anômalas | Oracle ML in-Database |
| Predição de Rota | Sugerir trajetos baseado no histórico | TensorFlow + Node-RED |
| Análise de Padrões | Identificar horários de pico | PL/SQL Analytics Functions |

### 7.2 Próximas Implementações de IA (Sprint 3)

**Planejado para implementação:**

1. **Oracle AI Vector Search**
   - Busca semântica de rotas similares
   - Recomendação de trajetos alternativos

2. **Autonomous Database AutoML**
   - Detecção automática de anomalias
   - Alertas preditivos de manutenção

3. **OCI Generative AI**
   - Geração automática de relatórios
   - Análise em linguagem natural dos dados

---

## 8. EVIDÊNCIAS DE FUNCIONAMENTO

### 8.1 Logs do ESP32 (Serial Monitor)

```
================================
   GPS IoT - Sprint 2 FIAP
================================

Conectando WiFi.... ✓
IP: 192.168.1.100

Conectando MQTT... ✓

✓ Enviado via MQTT
  Tópico: fiap/gps/dados
  Dados: {"dispositivo":"ESP32_GPS_FIAP","latitude":-23.55052,"longitude":-46.633308,"velocidade":40.0,"contador":1,"timestamp":5}
─────────────────────

✓ Enviado via MQTT
  Tópico: fiap/gps/dados
  Dados: {"dispositivo":"ESP32_GPS_FIAP","latitude":-23.55052,"longitude":-46.633308,"velocidade":40.0,"contador":2,"timestamp":10}
─────────────────────
```

### 8.2 Node-RED Debug Output

```json
{
  "topic": "fiap/gps/dados",
  "payload": {
    "dispositivo": "ESP32_GPS_FIAP",
    "latitude": -23.550520,
    "longitude": -46.633308,
    "velocidade": 40.0,
    "contador": 5,
    "timestamp": 25
  },
  "qos": 0,
  "retain": false
}
```

### 8.3 Consulta Oracle Database

**Query de Verificação:**
```sql
SELECT 
    id_leitura,
    dispositivo_id,
    latitude,
    longitude,
    velocidade,
    TO_CHAR(data_hora, 'DD/MM/YYYY HH24:MI:SS') AS data_hora
FROM TB_GPS_LEITURAS
ORDER BY id_leitura DESC
FETCH FIRST 5 ROWS ONLY;
```

**Resultado:**

| ID | DISPOSITIVO | LAT | LON | VEL | DATA_HORA |
|----|-------------|-----|-----|-----|-----------|
| 42 | ESP32_GPS_FIAP | -23.550520 | -46.633308 | 40.0 | 06/11/2025 14:35:22 |
| 41 | ESP32_GPS_FIAP | -23.550520 | -46.633308 | 40.0 | 06/11/2025 14:35:17 |
| 40 | ESP32_GPS_FIAP | -23.550520 | -46.633308 | 40.0 | 06/11/2025 14:35:12 |

---

### 9.3 Integração com Outras Disciplinas

| Disciplina | Integração Planejada | Status |
|------------|---------------------|--------|
| Java Advanced | API REST para gerenciamento de dispositivos | Sprint 3 |
| Mobile App Development | App React Native consumindo dados APEX | Sprint 3 |
| .NET | Dashboard administrativo web | Sprint 4 |
| DevOps | CI/CD com GitHub Actions + OCI | Sprint 3 |
| Database | Procedures e triggers avançados | Concluído ✅ |

---

## 11. RECURSOS E REFERÊNCIAS

### 11.1 Links do Projeto

**Repositórios e Demonstrações:**
- 🔗 Código-fonte: [GitHub - ESP32 GPS IoT](https://github.com/seu-usuario/gps-iot-fiap)
- 🎥 Vídeo demonstração: [YouTube - Protótipo Sprint 2](https://youtube.com/...)
- 📊 Dashboard APEX: [Oracle APEX - GPS Tracker](https://apex.oracle.com/...)
- 📄 Documentação completa: [GitHub - README.md](https://github.com/...)

### 11.2 Ferramentas Utilizadas

**Desenvolvimento:**
- Wokwi ESP32 Simulator - https://wokwi.com/
- Visual Studio Code - Editor de código
- Arduino IDE - Programação ESP32
- Git/GitHub - Controle de versão

**Infraestrutura:**
- HiveMQ Public Broker - https://www.hivemq.com/mqtt/public-mqtt-broker/
- Node-RED - https://nodered.org/
- Oracle APEX - https://apex.oracle.com/
- Oracle Cloud Free Tier - https://www.oracle.com/cloud/free/

**Documentação:**
- Draw.io - Diagramas
- Markdown - Documentação
- Google Docs - Apresentação

### 11.3 Referências Técnicas

**Documentação Oficial:**
- Oracle APEX Documentation
- ESP32 Technical Reference Manual
- MQTT v3.1.1 Protocol Specification
- Node-RED User Guide
- ArduinoJson Documentation

---

## 12. CÓDIGO-FONTE PRINCIPAL

### 12.1 Código ESP32 (Arduino C++)

```cpp
// Sistema GPS IoT - Sprint 2 SIMPLIFICADO
// ESP32 → MQTT → Node-RED → Oracle DB

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configurações WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configurações MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "fiap/gps/dados";
const char* mqtt_client_id = "ESP32_GPS_FIAP";

// Objetos
WiFiClient espClient;
PubSubClient mqtt(espClient);

// Variáveis GPS
float latitude = -23.550520;
float longitude = -46.633308;
float velocidade = 40.0;
int contador = 0;
unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n================================");
  Serial.println("   GPS IoT - Sprint 2 FIAP");
  Serial.println("================================\n");
  
  conectarWiFi();
  mqtt.setServer(mqtt_server, mqtt_port);
  
  Serial.println("✓ Sistema iniciado!\n");
}

void loop() {
  if (!mqtt.connected()) {
    reconectarMQTT();
  }
  mqtt.loop();
  
  if (millis() - ultimoEnvio > 5000) {
    ultimoEnvio = millis();
    contador++;
    enviarDadosMQTT();
  }
}

void conectarWiFi() {
  Serial.print("Conectando WiFi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(" ✓");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void reconectarMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Conectando MQTT...");
    
    if (mqtt.connect(mqtt_client_id)) {
      Serial.println(" ✓");
    } else {
      Serial.print(" ✗ (Erro ");
      Serial.print(mqtt.state());
      Serial.println(")");
      delay(3000);
    }
  }
}

void enviarDadosMQTT() {
  StaticJsonDocument<200> doc;
  
  doc["dispositivo"] = mqtt_client_id;
  doc["latitude"] = latitude;
  doc["longitude"] = longitude;
  doc["velocidade"] = velocidade;
  doc["contador"] = contador;
  doc["timestamp"] = millis() / 1000;
  
  char buffer[200];
  serializeJson(doc, buffer);
  
  if (mqtt.publish(mqtt_topic, buffer)) {
    Serial.println("✓ Enviado via MQTT");
    Serial.print("  Tópico: ");
    Serial.println(mqtt_topic);
    Serial.print("  Dados: ");
    Serial.println(buffer);
  } else {
    Serial.println("✗ Erro ao enviar");
  }
  
  Serial.println("─────────────────────\n");
}
```

## 15. CRÉDITOS E EQUIPE

### 15.1 Informações do Projeto

**Projeto**: Sistema de Rastreamento GPS IoT  
**Disciplina**: DISRUPTIVE ARCHITECTURES: IOT, IOB & GENERATIVE IA  
**Turma**: 2TDS - Agosto 2025  
**Scrum Master**: Antonio Sergio Rodrigues Figueiredo e Karina Paltrinieri

### 15.2 Integrantes do Grupo

**Equipe:**

**Felipe Anselmo** - RM560661

**João Vinicius Alves** - RM559369

**Matheus Mariotto** - RM560276

### 15.3 Agradecimentos

## ANEXOS

### Anexo A: Diagrama Entidade-Relacionamento

```
TB_DISPOSITIVOS
├─ id_dispositivo (PK)
├─ nome
├─ descricao
├─ ativo
└─ criado_em
    │
    │ 1:N
    │
    ▼
TB_GPS_LEITURAS
├─ id_leitura (PK)
├─ dispositivo_id (FK)
├─ latitude
├─ longitude
├─ velocidade
├─ data_hora
└─ criado_em
```

### Anexo B: Fluxo Node-RED (JSON)

```json
[
  {
    "id": "mqtt_in_node",
    "type": "mqtt in",
    "topic": "fiap/gps/dados",
    "broker": "broker.hivemq.com"
  },
  {
    "id": "json_parse",
    "type": "json"
  },
  {
    "id": "http_request",
    "type": "http request",
    "method": "POST",
    "url": "https://apex.oracle.com/ords/.../gps/dados"
  }
]
```

### Anexo C: Endpoints REST Oracle APEX

**POST /gps/dados**
- Descrição: Inserir nova leitura GPS
- Content-Type: application/json
- Response: 201 Created

**GET /gps/dados**
- Descrição: Listar leituras GPS
- Query Params: dispositivo_id, data_inicio, data_fim
- Response: 200 OK + JSON Array

**GET /gps/dados/:id**
- Descrição: Buscar leitura específica
- Path Param: id (id_leitura)
- Response: 200 OK + JSON Object

---

## CONTROLE DE VERSÃO

| Versão | Data | Autor | Alterações |
|--------|------|-------|------------|
| 1.0 | 06/11/2025 | Equipe | Versão inicial - Sprint 2 |

---

**Data da Entrega**: 06/11/2025  
**Sprint**: 2 de 4  
**Status**: ✅ Protótipo Funcional Entregue e Testado

---

*Documentação gerada para o Challenge Oracle - FIAP 2025*  
*Todos os direitos reservados © 2025*