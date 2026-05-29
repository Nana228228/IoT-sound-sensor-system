# 📊 Dashboard do Grafana - Monitor de Contaminação Acústica

Para replicar o painel visual deste projeto, você pode importar diretamente o arquivo JSON gerado pelo Grafana.

## Como importar este painel para o seu Grafana:
1. Abra o seu Grafana Cloud ou Local.
2. No menu esquerdo, vá em **Dashboards** e clique em **New** -> **Import**.
3. Faça o upload do arquivo `painel_grafana_exportado.json` que está nesta pasta.
4. Selecione o seu banco de dados InfluxDB na lista de conexões (Data Sources).
5. Clique em **Import** e o painel estará pronto com as configurações de Eixo Duplo (Dual Axis) e o Limite Crítico de 55 dB da OMS.
