# Modelo de quarto em computação gráfica

## Trabalho de computação gráfica sobre a modelagem de um quarto.

Para compilar, por padrão com o uso de 4 threads, utilize o comando `make build`.
Utilize `make compile` para compilar com um único thread.
Para compilar o arquivos e logo após executar o binário, utilize o comando `make run`.
Para compilar sem mensagens de log, utilize o comand `make silent <comando>`.

O programa funciona tanto no Windows quanto no Linux, mas no linux as bibliotecas devem ser instaladas utilizando o comando `sudo apt install freeglut3 freeglut3-dev`.

Os controles de movimento são com as teclas `wasd`, e o mouse.

Para movimentação vertical, utilize `barra de espaço` para ir para cima e `b` para ir para baixo.

Para abrir e fechar as portas e a janela, pressione a tecla `p` para abrir, `o` para fechar, para trocar entre os objetos que podem abrir ou fechar, utilize a tecla `l`.
    No total são 3 objetos que podem ser abertos/fechados, sendo eles duas portas e uma janela.

Para ligar ou desligar a luminária, utilize a tecla `k`.

## Requisitos da AB1

 - [x] Poder navegar pelo quarto utilizando o mouse e teclado
 - [x] A modelagem e apresentação do quarto, cama, mesa, ventilador e quadro
 - [x] Além de mais 5 objetos à sua escolha
 - [x] Uma porta deverá poder abrir e fechar com o teclado
 - [x] Uma janela deverá abrir e fechar como teclado

## Requisitos da AB2

 - [x] Modelar e apresentar um quadro de Vincent Van Gogh
 - [x] Ventilador de mesa com hélice girando no sentido horário em velocidade baixa
 - [x] Luminária spot em cima da mesa
 - [x] Iluminação especular e difusa na modelagem
 - [x] Aplicar mapeamento de textura em 5 objetos

## Referências:

* [Referência do monitor da disciplina](https://github.com/valeriojr/COMP269/blob/master/opengl.c)
* [Básicos do opengl](https://github.com/valeriojr/monitoria-cg/blob/master/material/book.pdf)
* [Camera - Getting started](https://learnopengl.com/Getting-started/Camera)
* [Viewing obj file format](https://en.wikipedia.org/wiki/Wavefront_.obj_file)
* [Applying MSAA](https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing)
* [Textures in OpenGL](https://www.youtube.com/watch?v=n4k7ANAFsIQ)