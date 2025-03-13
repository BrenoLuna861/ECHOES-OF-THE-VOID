# Echoes of the Void

## Descrição

"Echoes of the Void" é um jogo simples desenvolvido em C utilizando a biblioteca OpenGL e GLUT. O objetivo do jogo é sobreviver o máximo de tempo possível enquanto desvia de monstros que se movem em direção ao jogador. O jogador pode se mover em quatro direções (cima, baixo, esquerda, direita) e deve evitar colisões com os monstros. O jogo termina quando o jogador colide com um monstro.

## Funcionalidades

- **Movimento do Jogador**: O jogador pode se mover usando as teclas 'W', 'A', 'S', 'D'.
- **Monstros**: Os monstros se movem em direção ao jogador e aumentam de velocidade a cada 15 segundos de sobrevivência.
- **Colisões**: O jogo verifica colisões entre o jogador e os monstros, bem como entre o jogador e os pontos no mapa.
- **Menu e Reinício**: O jogo começa com um menu inicial e pode ser reiniciado pressionando a tecla 'R' após o término do jogo.

## Controles

- **W**: Move o jogador para cima.
- **A**: Move o jogador para a esquerda.
- **S**: Move o jogador para baixo.
- **D**: Move o jogador para a direita.
- **R**: Reinicia o jogo após o término.
- **Enter**: Sai do jogo.

## Como Compilar e Executar

1. **Instale as dependências necessárias**: Certifique-se de ter o OpenGL e GLUT instalados no seu sistema.

2. **Compile o código**:
   ```bash
   gcc -o echoes_of_the_void main.c -lGL -lGLU -lglut -lm
   ```

3. **Execute o jogo**:
   ```bash
   ./echoes_of_the_void
   ```

## Estrutura do Código

- **Estruturas de Dados**: `Point`, `Player`, `Monster` são usados para representar os elementos do jogo.
- **Funções Principais**:
  - `checkCollisionWithPoints`: Verifica colisões entre o jogador e os pontos.
  - `checkCollisionWithMonsters`: Verifica colisões entre o jogador e os monstros.
  - `moveMonsters`: Atualiza a posição dos monstros.
  - `updatePlayerPosition`: Atualiza a posição do jogador com base nas entradas do teclado.
  - `display`: Renderiza o jogo na tela.
  - `drawGameOverScreen` e `drawMenu`: Desenham a tela de game over e o menu inicial, respectivamente.

## Notas

- O jogo é renderizado em uma janela de 800x800 pixels.
- O tempo de sobrevivência é exibido no canto superior esquerdo da tela.
- O jogo termina quando o jogador colide com um monstro, e uma mensagem de game over é exibida.
