## Descricao das tarefas

#### Tarefa 1 - Criando páginas Web

#### Tarefa 2 - Usando HTML e CSS    
https://www.frontendmentor.io/challenges/results-summary-component-CE_K6s0maV

#### Tarefa 3 - Criando animações  
##### CSS Animation:  
a) Crie uma animação em que um elemento de texto balança de um lado para o outro. A animação deve durar 2 segundos e a velocidade deve ser suave e fluida. Use a propriedade "animation-timing-function" para definir o tempo da animação. Salve como animate-a.html e animate-a.css.

b) Crie uma animação em que uma imagem gira continuamente em 360 graus. A animação deve durar 3 segundos e a velocidade deve ser consistente ao longo da animação. Use a propriedade "animation-iteration-count" para definir que a animação deve ser infinita. Salve como animate-b.html e animate-b.css.

##### CSS Transform:  
a) Crie um elemento de imagem que aumenta gradualmente de tamanho quando o usuário passa o mouse sobre ele. A animação deve durar 1 segundo e a escala deve aumentar suavemente de 1x para 1.5x.  Salve como transform-a.html e transform-a.css.

b) Crie um elemento de imagem que se move para cima e para baixo em um movimento de onda. A animação deve durar 3 segundos e a velocidade deve ser suave e fluida. Use a propriedade "animation-timing-function" para criar um efeito de "elasticidade". Salve como transform-b.html e transform-b.css.

#### Tarefa 4 - Usando javascript  
1) Em uma página HTML mostre um retangulo (pode ser uma DIV) com tamanho 400 x 300 pixels e borda de 2 px.  Ao lado desse retangulo apresente três slide bars. Cada um deles deve variar entre 0 e 255. O primeiro representará valores para o R, o segundo para o G e o terceiro para o B, ou seja, RBG.  Cada vez que os valores dos sliders forem alterados,  a cor de fundo do retangulo deve mudar para a combinação RBG mostrada nos slide bars.  Nomeie os arquivos como SlideRGB.html, SlideRGB.css e SlideRGB.js  
```HTML
<div class="slideRGB">  
  <input type="range" min="0" max="255" value="50" class="slider" id="R">  
  <input type="range" min="0" max="255" value="50" class="slider" id="G">  
  <input type="range" min="0" max="255" value="50" class="slider" id="B">  
</div>  
```
2) Estamos no período de entrega da declaração de Imposto de Renda (IR). O cálculo do IR é baseado em uma tabela de descontos. Pesquise e entenda como esse cálculo deve ser realizado considerando o salário anual recebido por uma pessoa.  O programa deve receber o valor do salário anual e calcular quando de imposto a pessoa irá pagar ou se será isenta. Cria página HTML com CSS e Javascript para realizar o cálculo.  Nomeie os arquivos como CalcIR.html, CalcIR.css e CalcIR.js  
3) Agora que você conhece o CSS Grid, monte uma calculadora das operações básicas (soma, subtração, divisão e multiplicação). O grid pode ser utilizado para representar as teclas da calculadora (números, operações e outras funções com o apaga tudo). Você consegue criar uma página HTML, sem usar forms, para montar a calculadora? Calculadora.html, Calculadora.css, Calculadora.js.  

#### Tarefa 5 - Formulário  
1) O curso de BCC da UNESP de Bauru começou em agosto de 1984. Considerando que a cada ano entram 30 alunos, já passaram por aqui mais de 1.100 alunos. Gostaríamos de ter um cadastro de nossos ex-alunos com as seguintes informações: nome completo, apelido, data de nascimento, ano que entrou no BCC, ano que se formou, telefone,  email e texto sobre onde atua no momento (trabalho).  Você deve criar esse formulário com eventos que facilitam a digitação com onfocus e onblur, bem com alguns consistência de digitação para evitar que dados fiquem em branco.  No envio do formulário, programe a geração de um texto no formato JSON (use stringify) com os dados do formulário. Obs: Não é necessário gravar o arquivo, nem mesmo enviar para um servidor (isso será visto nas próximas aulas). Apenas gera uma variável texto.  

#### Tarefa 6 - Usando Set e Map   
1) Crie um Set  de itens de uma bancada de um lab de eletrônico, como: capacitor, resistor, protoboard, fonte de alimentação, multimetro, ponta de prova, fios, microcontrolador, transistor, diodo  e outros itens que você comumente usa na prática da aula de Circuitos Digitais. A ideia é simples. Dada a lista de itens de bancada, você quer escolher quais irá utilizar na sua aula. Para isso apresente essa lista em uma LISTBOX a esquerda da tela. No lado direito, crie outra LISTBOX vazia.  Entre as duas LISTBOX, coloque dois botões, um em cima e outro embaixo. O botão superior tem o texto >> e o botão inferior tem o texto <<.  Quando o botão >> receber o click do mouse você deve passar o item selecionado na LISTBOX da esquerda para a LISTBOX da direita, apagar o item da LISTBOX da esquerda.  A operação contrária deve (botão <<) também deve ser implementada. Insira, também, um botão para LIMPAR a LISTBOX da direita, bem como um contador de itens, isto é, mostre quantos itens estão na LISTBOX da esquerda e da direita. Gere o HTML, CSS e o Javascript da solução e poste no Moodle com o nome items.html, items.css e items.js.  
2) A disciplina "Lab of Lazy Programming" conta com 30 alunos e o professor deseja montar equipes com 3 ou 4 alunos cada. Faça uma página Web para ajudar o professor. Essa página deve ter uma LISTBOX a esquerda com o nome dos alunos, uma LISTBOX no meio da página onde é possível incluir os nomes das equipes e uma área onde aparece o nome da equipe e seus componentes.  As operações possíveis são: associar um aluno a uma equipe, cadastrar ou remover uma equipe.  Utilize a estrutura de dados Map do Javascript para armazenar as equipes e seus membros. Poste no Moodle como LLP.html, LLP.css e LLP.js  

#### Tarefa 7 - Usando Web APIs    
O Github é uma ferramenta muito utilizada por desenvolvedores e conta com um API . O objetivo é explorar esta API e demonstrar como fazer uma chamada, recuperar dados e apresentar em uma página HTML.  

#### Tarefa 8 - Node + Javascript  
Muitas aplicações exigem que um usuário faça o seu registro antes de utilizar as funcionalidades do site.  O objetivo aqui é criar um servidor que vai receber uma requisição para realizar um cadastro contendo nome, email e senha.  Não precisa fazer uma página HTML, basta para as informações via URL para o servidor.  Crie o arquivo o server.js que irá conter o código do servidor, isto é, receber tal requisição e armazenar os dados em um arquivos JSON (registro.json).  Novas requisições devem ser adicionadas adicionadas como um objeto no arquivo JSON, mantendo uma estrutura de array de usuários.  Você pode optar por criar um servidor javascript puro ou usar o express.js.   O endpoint poderia ser algo com /registro.  Não se esqueça de criar uma pasta de projeto onde os códigos devem ser copiados.  

#### Tarefa 9 - Usando banco de dados  
Crie uma página HTML com CSS e javascript para realizar consulta, inclusão e exclusão de registro na tabela USUARIOS do banco de dados PESSOAL. Você deve criar o banco de dados no seu computador para realizar os testes.  Você pode utilizar o CURL ou Postman (não esqueça de instalar o Postman Agent no seu Windows para poder realizar consultas no seu LOCALHOST).  Poste o html, css, javascript do lado do servidor, bem como o index.js modificado.

#### Tarefa 10 - Usando Sequelize  
Utilizando como base os códigos apresentados na aula, crie um arquivo HTML, Javascript e CSS que pegue os dados do usuário (nome, email, curso) e grave na tabela usuarios no banco de dados. Utilize o Express para criar um endpoint e o SEQUELIZE para incluir o registro no banco de dados.  Não esqueça de criar o projeto node.js e instalar o express, sequelize e o mysql2.
