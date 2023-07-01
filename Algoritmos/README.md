### Trabalho string.c

As Ilhas Weblands formam um reino independente nos mares do Pacífico. Como é reino recente, a sociedade é muito influenciada pela informática. A moeda oficial é o Bit; existem notas de B$100,00, B$50,00, B$20,00, B$10,00, B$5,00 e B$2,00 e, moedas de B$1,00, B$0,50, B$0,25, B$0,10 e B$0,01.

Você foi contratado para ajudar na programação dos caixas automáticos de um grande banco nas Ilhas Weblands.

Os caixas eletrônicos das Ilhas Weblands operam com todos os tipos de notas e moedas disponíveis, mantendo um estoque de cédulas (B$100,00, B$50,00, B$20,00, B$10,00, B$5,00 e B$2,00) e moedas (B$1,00, B$0,50, B$0,25, B$0,10 e B$0,01) para cada valor.

Sua tarefa é escrever um programa que, dado o valor de Bits desejado pelo cliente, determine o número de cada uma das notas e moedas necessárias para totalizar esse valor, de modo a minimizar a quantidade de cédulas e moedas entregues. Por exemplo, se o cliente desejar retirar B$ 50,20, basta entregar uma única nota de cinqüenta Bits e duas moedas de dez CentBits. Se o cliente deseja retirar B$ 64,60, é necessário entregar uma nota de B$ 50,00, uma de B$ 10,00, duas de B$ 2,00, uma moeda de cinquenta CentBits e uma moeda de dez CentBits.

O seu contrato com o banco também prevê a programação de máquinas para preenchimento automático de cheques, ou seja, você também deve incluir no seu programa uma opção para o preenchimento de cheques. Por exemplo, se o cliente desejar preencher um cheque de B$2.103,13, você deve escrever dois mil, cento e três Bits e treze CentBits, assim como a data do preenchimento.

__O programa deve ter uma interface amigável com o cliente e, o mesmo deve digitar uma senha secreta para utilizá-lo.__

O trabalho não pode utilizar variáveis globais e loops infinitos, como por exemplo while (1). Deve-se utilizar o conceito de funções.

### Trabalho arquivo.c

Desenvolva um programa, em linguagem C, que manipule um arquivo binário com registros que contenham no mínimo cinco campos distintos. O programa deve cadastrar, alterar, excluir (exclusão lógica e física) e consultar (campo numérico e campo alfanumérico).

O programa deve ter uma interface amigável e, permitir que o usuário consulte os arquivos de uma determinada pasta, troque o nome e remova arquivos do disco, crie e altere o nome de pastas, mude um arquivo de pasta, altere data e horário do sistema entre outras funções.

O arquivo de ajuda deve ser do tipo texto e criptografado. No programa, a ajuda deve ser ativada pela tecla ```<F1>```.

### Trabalho agenda.c

Faça um programa para gerenciar uma agenda de compromissos usando uma estrutura de dados com alocação dinâmica de memória. O programa deverá conter as especificações fornecidas a seguir.

__1) Insere um compromisso na agenda__

Esta opção permite que o usuário indique se deseja inserir um novo compromisso na agenda Esta opção permite que o usuário indique se deseja inserir um novo compromisso na agenda.

Em relação aos compromissos da agenda:

* Os compromissos da agenda devem ser armazenados em uma lista encadeada e ordenada pela data/horário;  
* O usuário pode ter mais de um compromisso para uma mesma data/horário;
* Cada compromisso compreende uma data (dia, mês e ano), um horário (hora, minutos) e um texto que descreve este compromisso (de no máximo 80 caracteres);

__2) Remove compromissos da agenda de uma determinada data__

Esta opção permite que o usuário remova compromissos da agenda, uma vez que estes já tenham sido realizados. Esta opção permite que o usuário apague da agenda todos os compromissos de uma determinada data. A ideia é após terminado o dia, o usuário remove todos os compromissos daquele dia de uma vez só. O usuário apenas informa uma data (dia, mês e ano) e o sistema apaga todos os compromissos referentes aquele dia.

__3) Remove um determinado compromisso da agenda__

Esta opção permite que o usuário remova um determinado compromisso da agenda que contenham uma determinada palavra que ele forneceu. Por exemplo, o usuário irá solicitar que o sistema procure a palavra “médico” e como resposta o sistema poderia exibir compromissos como: “visita ao médico oftalmologista” e “consulta ao médico”. O usuário deve escolher qual compromisso remover, neste caso.

__4) Consulta os compromissos da agenda__

Esta opção permite que o usuário consulte os seus compromissos agendados. O usuário indica uma data (dia, mês e ano) e o sistema irá exibir todos os compromissos agendados nesta data, indicando o horário (hora e minutos) de cada um e o texto que descreve cada compromisso.

__5) Procura compromissos da agenda que contenham a palavra indicada__

Esta opção permite que o usuário procure por todos os compromissos da agenda que contenham uma determinada palavra que ele forneceu. Por exemplo, o usuário irá solicitar que o sistema procure a palavra “médico” e como resposta o sistema poderia exibir compromissos como: “visita ao médico oftalmologista” e “consulta ao médico”.

__6) Altera compromisso da agenda que contenha a palavra indicada.__

Esta opção permite que o usuário altere a data de um compromisso na agenda. A busca deve ser feita pela palavra indicada. Observe que pode ter mais de um compromisso com a palavra indicada. O usuário deve escolher qual compromisso deseja alterar.

__7) Altera compromisso da agenda de uma determinada data e horário__

Esta opção permite que o usuário altere o compromisso de uma determinada data na agenda. O usuário indica a data e o horário do compromisso que deseja alterar.

__8) Salva em disco os compromissos da agenda__

Esta opção permite que o usuário salve em arquivos do tipo texto (arquivo “agenda.txt”) todos os dados contidos na agenda de compromissos.

__9) Lê do disco os compromissos previamente salvos em disco__

Esta opção permite que o usuário recupere os dados (compromissos) que foram previamente salvos em arquivo em disco (arquivo “agenda.txt”).

__10) Termina a execução do programa.__

Termina a execução do programa.
