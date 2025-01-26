#  Geração de animações em uma matriz de LEDs 5x5

Este projeto implementa a funcionalidade de controle de uma matriz de LEDs coloridos 5x5 (WS2812) utilizando o **Raspberry Pi Pico W** e um teclado matricial 4x4 para geração de animações. O código é desenvolvido em **C** para sistemas embarcados e integra feedback visual e sonoro por meio de animações e um buzzer passivo.

## Estrutura do Projeto

- **diagram.json**: Arquivo que contém a estrutura da simulação do WOKWI.
- **wokwi.toml**: Arquivo para definir o caminho do .elf e .uf2 usado para a simulação.
- **CMakeLists.txt**: Definição dos processos de compilação e requisitos necessários.
- **pico_sdk_import.cmake**: Arquivo com as configurações de importação do Pico SDK.

## Funcionalidades

- **Teclas "0 a 9"**: Acionam animações específicas.  
- **Tecla "A"**: Desliga todos os LEDs.  
- **Tecla "B"**: Liga todos os LEDs na cor azul com 100% de luminosidade.  
- **Tecla "C"**: Liga todos os LEDs na cor vermelha com 80% de luminosidade.  
- **Tecla "D"**: Liga todos os LEDs na cor verde com 50% de luminosidade.  
- **Tecla "#"**: Liga todos os LEDs na cor branca com 20% de luminosidade.  
- **Tecla "*"**: Habilita o modo de gravação via software (reboot do Raspberry Pi Pico W).  

## Requisitos

- **Hardware**:
  - Computador pessoal
  - Raspberry Pi Pico W (RP2040)
  - Teclado Matricial 4x4
  - Matriz de LEDs Coloridos 5x5 WS2812
  - Buzzer Passivo Eletromagnético MLT-8530 SMD 3.6V
  - Cabo micro-USB para USB-A

- **Software**:
  - Visual Studio Code
  - Pico SDK
  - Compilador ARM GCC
  - Wokwi integrado ao VS Code

## Configuração Inicial
1. Clone o repositório para sua máquina local:
  ```bash
  git clone <URL do Repositório>
  ```
2. Abra o projeto no VS Code.
3. Abra o arquivo diagram.json no Wokwi para visualizar a simulação.
 
## Equipe
- **Hiago Moura**
- **Ikaro Silva**
- **Yasmin Cruz**

## Vídeo demonstrativo 
[Assista aqui](Url_do_vídeo) 
