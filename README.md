# cg-quarto
Trabalho de computação gráfica sobre a modelagem de um quarto.

Para compilar, por padrão com o uso de 4 threads, utilize o comando `make build`.
Utilize `make compile` para compilar com um único thread.
Para compilar o arquivos e logo após executar o binário, utilize o comando `make run`.
Para compilar sem mensagens de log, utilize o comand `make silent <comando>`.

O programa funciona tanto no Windows quanto no Linux, mas no linux as bibliotecas devem ser instaladas utilizando o comando `sudo apt install freeglut3 freeglut3-dev`.

Os controles de movimento são com as teclas `wasd`, e o mouse.
Para se locomover para cima, utiliza-se a barra de espaço e para baixo, utiliza-se a tecla `b`.

Referências:

[Camera - Começando a usar](https://learnopengl.com/Getting-started/Camera)
