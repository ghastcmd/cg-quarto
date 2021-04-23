* Para mover modelos independentemente do sistema de matrizes
    * glPushMatrix()
    * A função guarda o estado atual das matrizes de transformação
    * Mover objetos do jeito que desejar
    * Poder mover e depois agir como se ainda fosse a cena básica
    * glPopMatrix()
    * Para retornar às matrizes de transformações anteriores
* Carregar a identidade de alguma transformação
    * glLoadIdentity()
    * Ele remove todas as transformações anteriores
* Definir a matrix de visualização, view matrix
    * Primeiro carrega-se o modo de matriz de modelo de vizualização
    * gluLookAt(x, y, z, xp, yp, zp, (vetor pra cima))
    * A primeira coordenada define a posição da câmera
    * A segunda câmera define a posição ao qual a câmera está olhando
* A ordem em que devem ser seguidas as transformações:
    * Translate, rotation, scale
* Quando carregar modo de matriz novo, carrega-se matriz identidade
* Para utilizar a perspectiva
    * gluPerspective(fov, aspect_ratio, min_view_distance, max_view_distance)
    * fov: campo de visão da perspectiva
    * aspect_ratio: a razão entre a altura e largura
    * min_view_distance: distância mínima de vizualização de pontos
    * max_view_distance: distância máxima de vizualização de pontos
    * Qualquer ponto que fuja do alcance não é renderizado
    * Quando utilizada, ao redimensionar a tela, os modelos se mantém
* Função reshape para quando a janela de vizualização for transformada
    * glReshapeFunc(reshape_handle)
    * reshape_handle: ponteiro para função reshape
    * A função reshape deve trocar a perspectiva com o tamanho de janela novo
    * Quando se muda o tamanho da janela, é necessário mudar o tamanho da viewport
        * glViewport(x, y, width, height)
        * Quando o viewport não é modificado, as transformações ficam bizonhas
* Função mouse handle
    * Define-se uma função no formato:
        * void mouse_handle(int button, int state, int x, int y)
    * Dentro dessa função, se é definida as ações perante comportamentos do mouse
    * button: botão do mouse
    * state: estado do mouse
    * Em geral se checa se o botão atualizado é o desejado e o estado atual dele
    * Entre os estados estão os de pressionado e solto
    * Os valores x e y representam a posição atual do mouse
* Função keyboard handle
    * Define-se uma função no formato:
        * void keyboard_handle(unsigned char key, int x, int y)
    * Geralmente só se utiliza o valor da key em um switch
* Para a movimentação pelo teclado:
    * No caso da movimentação para frente e para trás
        * Basta somar o vetor de posição da câmera com um outro vetor
        * Esse outro vetor será o vetor que indica a frente da câmera
        * E então, quando somado a câmera vai para frente e quando subtraído, para trás
    * No caso da movimentação para os lados
        * Basta conseguir um vetor que seja horizontal para os lados
        * Para conseguir tal vetor, é necessário encontrar um vetor perpendicular
        * Este vetor deve ser perpendicular ao vetor de cima e o vetor da frente da câmera
        * E então se consegue um vetor de unidade e multiplica pelo escalonamento
* Para velocidades regulares
    * Quando se trabalha com velocidades por frame, um valor constante é variável
    * Devido à alta variação dos quadros por segundo, é necessário uma mensuração de tempo
    * No glut, a medição do tempo é feita com a função
        * glutGet(GLUT_ELAPSED_TIME)
        * Retorna um valor em milisegundos desde que a função glutInit() foi chamada
* Para obter a movimentação pelo mouse
    * É necessário conseguir o offset de movimentação do mouse na tela
    * Depois de somada o offset ao pitch e ao yaw
        * Yaw: movimentação no eixo horizontal
        * Pitch: movimentação no eixo vertical
        * Roll: movimentação na rotação
    * Como queremos obter um novo vetor de direção da câmera, utilizaremos da trignometria
        * Iremos mover somente os eixos x e y
        * Para mover o eixo yaw, devemos calcular as posições relativas ao eixo z e x
        * Para mover o eixo pitch, devemos calcular as posições relativas ao eixo y e x,z
