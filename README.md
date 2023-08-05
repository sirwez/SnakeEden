# Game.h
## Estruturas

1. **Color**: Esta estrutura é usada para representar uma cor em RGBA. Ela contém quatro variáveis de ponto flutuante, representando o vermelho (r), o verde (g), o azul (b) e o alfa (a). O alfa representa a transparência da cor.

2. **Vector2**: Esta estrutura é usada para representar uma posição 2D ou um vetor. Ela contém duas variáveis de ponto flutuante, representando x e y, e uma variável inteira, representando a movimentação.

3. **Obstacle**: Esta estrutura representa um obstáculo no jogo. Ela contém uma variável de posição que é um objeto da estrutura Vector2.

## Enumerações

1. **GameState**: Esta enumeração representa os diferentes estados do jogo. Os estados possíveis são `MENU` e `JOGANDO`.

## Classes

1. **Enemy**: Esta classe representa um inimigo no jogo. Ela tem uma posição (um objeto Vector2), e uma variável de velocidade. Existem dois métodos nesta classe: `moveRandomly` e `detectPlayer`. 
# Game.cpp
Aqui está uma descrição das funções e métodos implementados:

1. **getSnakeDirection**: Retorna a direção atual da cobra.

2. **Enemy::moveRandomly**: Move o inimigo aleatoriamente dentro da arena, a menos que o novo local esteja fora da arena ou dentro de um obstáculo.

3. **Enemy::detectPlayer**: Move o inimigo em direção ao jogador se o jogador estiver dentro de uma certa distância.

4. **drawCube**: Desenha um cubo em uma posição e tamanho especificados e com uma cor especificada.

5. **Score**: Desenha o placar na tela.

6. **drawMenu**: Desenha o menu do jogo, incluindo o título do jogo e as instruções de como começar o jogo. Também exibe o MVP.

7. **compareScores**: Uma função de comparação usada para ordenar as pontuações. Retorna `true` se a primeira pontuação for maior que a segunda.

8. **writeScore**: Escreve a pontuação do jogador em um arquivo chamado "highscores.txt".

9. **getHighScores**: Lê o arquivo de pontuações e retorna um vetor dos cinco principais jogadores e suas pontuações com base nas maiores pontuações.

10. **drawHighScores**: Desenha as pontuações mais altas na tela inicial, o MVP de todas as pontuações.

11. **Ghost**: Desenha um fantasma, representando um inimigo.

12. **Arena**: Desenha a arena do jogo, incluindo as paredes e o chão. Também desenha obstáculos e inimigos.

13. **Snake**: Desenha a cobra na tela. A cabeça e o corpo da cobra são diferenciados por cores.**

14. **setSnakeInput**: Define a direção do movimento da cobra com base na entrada do usuário.**

15. **handleFood**: Lida com a comida no jogo. Se a comida não estiver presente, ela será gerada após um certo atraso. Se a comida estiver presente, verifica se a cobra colidiu com a comida.

16. **incrementSnake**: Incrementa o tamanho da cobra, até um máximo definido pela constante `MAXLENGTH`.

17. **increaseDifficulty**: Aumenta a dificuldade do jogo aumentando a velocidade da cobra a cada 1 ponto.

18. **detectFoodColision**: Verifica se a cobra colidiu com a comida. Se sim, incrementa a pontuação do jogador, aumenta o tamanho da cobra, e marca a comida como não presente para que mais comida possa ser gerada.

19. **getRandomPosInArena**: Retorna uma posição aleatória que não está dentro de um obstáculo, útil para posicionar comida ou outros objetos.**

20. **isPositionInObstacle**: Verifica se uma posição está dentro de um obstáculo.

21. **gameOver**: Exibe uma mensagem de fim de jogo, joga um som de fim de jogo, exibe a pontuação do jogador, pede ao jogador para inserir seu nome, grava a pontuação do jogador e exibe as maiores pontuações.

22. **calcCamRotation**: Ajusta a rotação da câmera para dar uma sensação dinâmica ao jogo.**

23. **checkOutOfBounds**: Verifica se a cobra saiu dos limites da arena ou colidiu com obstáculos ou inimigos.**

24. **checkTailCollision**: Verifica se a cabeça da cobra colidiu com sua cauda, o que resultaria em um fim de jogo.

25. **moveSnake**: Move a cobra na direção atual, atualiza a posição da cobra e verifica se ela saiu da arena.**

### Variáveis

1. **Vector2 Dir**: Esta é a direção atual na qual a cobra está se movendo.

2. **Vector2 SnakePos[MAXLENGTH] = {Vector2(0, 0, 1)}**: Este é um array que armazena a posição de cada segmento da cobra. Ele é inicializado com a cobra na posição (0, 0) e com uma movimentação de 1.

3. **bool AlreadyUpdate**: Esta variável parece ser usada para verificar se algo (possivelmente a posição da cobra) já foi atualizado.
  
9. **int SnakeSize = 1**: Este é o tamanho inicial da cobra. A cobra cresce à medida que come a comida.

10. **float cam_x** e **float cam_z**: Estas são as coordenadas da câmera no eixo x e z, respectivamente.

11. **float z_offset = 0.01f** e **float x_offset = 0.01f**: Estes são os deslocamentos da câmera no eixo x e z, respectivamente.

12. **int Points**: Esta é a pontuação atual do jogador.

13. **std::vector<Obstacle> obstacles**: Este é um vetor que armazena todos os obstáculos na arena.

14. **std::vector<Enemy> enemies**: Este é um vetor que armazena todos os inimigos na arena.

15. **bool FoodSpawned = false**: Esta é uma variável booleana que indica se a comida está presente na arena.

16. **Vector2 FoodPos**: Esta é a posição atual da comida na arena.

17. **float FoodSpawnDelay**: Este é o atraso antes que a comida seja gerada novamente depois de ser comida pela cobra.

18. **Vector2 getSnakeDirection() { return Dir; }**: Esta função retorna a direção atual da cobra.

# Main.cpp

### Funções

1. **init()**: Esta função inicializa as configurações de renderização do OpenGL, definindo a cor de fundo para branco, habilitando o teste de profundidade e a iluminação, e configurando o modelo de iluminação. Além disso, a função também inicia a música de fundo do jogo.

2. **checkOverlap(float x, float y, std::vector<Obstacle> &obstacles)**: Esta função verifica se um inimigo está se sobrepondo a um obstáculo. Ela faz isso percorrendo todos os obstáculos e verificando se as coordenadas (x, y) fornecidas coincidem com as de qualquer obstáculo.

3. **main(int argc, char** argv)**: Esta é a função principal do programa, onde o jogo é inicializado e executado. A função começa inicializando o GLUT e criando uma janela. Em seguida, exibe o menu do jogo e lida com a entrada do usuário para selecionar as opções do menu. Os obstáculos e inimigos são então gerados aleatoriamente na arena. Finalmente, a função inicia o loop principal do GLUT, que continuará executando até que o jogo seja encerrado.

4. **timer_callback(int)**: Esta função é chamada a cada intervalo de tempo definido pela variável `FPS`. A função marca a janela para ser redesenhada, o que fará com que a função de callback de exibição seja chamada, e reinicia o timer para que a função seja chamada novamente após o mesmo intervalo.

5. **display_callback()**: Esta função é responsável por renderizar o jogo na tela. Ela limpa o buffer de cor e profundidade, define a matriz de modelo e vista e, dependendo do estado do jogo, desenha a arena, a cobra e a pontuação, ou desenha o menu do jogo.

6. **reshape_callback(int w, int h)**: Esta função é chamada sempre que a janela do jogo é redimensionada. Ela ajusta a viewport e a matriz de projeção para corresponder ao novo tamanho da janela, garantindo que o jogo seja renderizado corretamente, independentemente do tamanho da janela.

7. **keyboard (unsigned char key, int x, int y)**: Esta função é um callback para eventos de teclado. Ela define o comportamento do jogo quando certas teclas são pressionadas. Por exemplo, as teclas 'w', 'a', 's' e 'd' são usadas para controlar a direção da cobra, a tecla ESC é usada para sair do jogo, e as teclas 'i', 'k', 'j', 'l' e 'c' são usadas para controlar a câmera.

### Variáveis

1. **int FPS = 0**: FPS (Frames Por Segundo) determina a taxa de atualização do jogo. Uma taxa de FPS mais alta significa que o jogo se atualizará mais rapidamente, tornando o movimento mais suave. No entanto, um FPS mais alto também pode ser mais exigente para o hardware.

2. **int move = 0**: Esta variável é usada para armazenar a direção do movimento atual da cobra. É provável que diferentes números representem diferentes direções de movimento (por exemplo, 1 pode significar para cima, 2 pode significar para baixo, etc.).

3. **std::string gameDifficulty = "N00B"**: Essa variável é usada para armazenar o nível de dificuldade do jogo escolhido pelo usuário. Isso pode afetar vários aspectos do jogo, como a velocidade da cobra, o número de obstáculos, etc.

4. **bool cameraFollowSnake = false**: Esta variável controla se a câmera deve seguir a cobra ou não. Se for verdadeiro, a câmera se move com a cobra, caso contrário, a câmera permanece fixa.
