// ========================================================
// CONSULTAS FLUX PARA O GRAFANA (INFLUXDB V2)
// Projeto ODS 11 - Monitoramento de Contaminação Acústica
// ========================================================

// 🟢 QUERY A: Nível de Ruído (Eixo Esquerdo - Decibéis Reais)
// Esta consulta puxa os dados do ESP32 calibrados para o ambiente físico.
from(bucket: "NOME_DO_SEU_BUCKET") // Substitua pelo nome do seu bucket
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r._measurement == "sensor_ruido")
  |> filter(fn: (r) => r._field == "decibelios_reales")
  |> yield(name: "Ruido_Acustico_dB")

// 🟡 QUERY B: Tráfego e Congestionamento (Eixo Direito - Porcentagem)
// Esta consulta puxa os dados consumidos da API de mapas (TomTom).
from(bucket: "NOME_DO_SEU_BUCKET") // Substitua pelo nome do seu bucket
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
  |> filter(fn: (r) => r._measurement == "sensor_ruido")
  |> filter(fn: (r) => r._field == "congestio_api")
  |> yield(name: "Congestionamento_Transito")
