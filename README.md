# Controle de LEDs e Botões com Interrupções no RP2040

# Introdução

### Este projeto implementa um sistema de controle de uma matriz 5x5 de LEDs WS2812 e um LED RGB utilizando a placa de desenvolvimento BitDogLab com o microcontrolador RP2040. O controle é realizado através de interrupções nos botões, com tratamento de debouncing via software.

# Uso

Conecte a placa de desenvolvimento BitDogLab ao computador via USB.

Compile e carregue o código na placa utilizando a ferramenta apropriada, como o SDK do Raspberry Pi Pico. Caso utilize o Wokwi, abra o projeto na plataforma ou diretamente no VSCode com a extensão Wokwi após a compilação do projeto.

O comportamento programado será executado conforme a interação com os botões.

# Funcionamento

As seguintes funcionalidades são implementadas:

1. O LED vermelho do LED RGB pisca continuamente 5 vezes por segundo.
2. O botão A (GPIO 5) incrementa o número exibido na matriz de LEDs a cada pressão.
3. O botão B (GPIO 6) decrementa o número exibido na matriz de LEDs a cada pressão.
4. A matriz 5x5 de LEDs WS2812 (conectada à GPIO 7) exibe números de 0 a 9 em um formato fixo ou criativo, mas claramente identificável.

# Link para o vídeo de demonstração

https://drive.google.com/drive/folders/1lsMfmo8AEE4ACxMUmJ7l_WuXw7q5oCHg?usp=sharing

# Responsável pelo projeto

Este projeto foi desenvolvido individualmente, abrangendo desde a concepção até a implementação e testes. O vídeo de demonstração inclui a apresentação pessoal e a explicação das funcionalidades implementadas.

Todos os aspectos do código, incluindo o tratamento de interrupções, debouncing, e controle dos LEDs WS2812 e RGB, foram desenvolvidos de forma a demonstrar o domínio sobre o uso de microcontroladores e componentes eletrônicos.

