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