# Conversas entre Hardware e Software
## Introdução

Visão geral do projeto
	As GPUs (Graphics Processing Units), ou Unidades de Processamento Gráfico, desempenham um papel cada vez mais crucial no cenário da computação moderna. Originalmente concebidas para lidar com a complexidade do processamento gráfico em jogos e aplicações visuais, essas unidades evoluíram para se tornarem poderosas ferramentas de processamento paralelo em uma ampla gama de domínios, desde aprendizado de máquina até simulações científicas.
Nesse contexto, o aluno de engenharia da computação, Gabriel B. Alves, concebeu e desenvolveu um projeto para ser utilizado em FPGAs. Neste projeto, ele criou uma GPU para facilitar o processo de criação de jogos, recebendo instruções e renderizando as imagens em um monitor VGA.
Em consonância com isso, foi proposto o desenvolvimento de um módulo Kernel em linguagem C para a placa de estudos DE1-SoC. O objetivo é criar um módulo kernel no sistema operacional Linux, localizado na parte HPS (Hard Processor System) da placa, para interagir com uma GPU (Graphics Processing Unit) localizada na parte FPGA (Field-Programmable Gate Array). Essa GPU será responsável por mover e controlar elementos em um monitor VGA (Video Graphics Array) com resolução de 640x480 pixels. Além disso, será necessário desenvolver uma biblioteca para facilitar a comunicação entre o usuário e o módulo kernel, proporcionando uma solução viável para a interação entre o código C escrito pelo usuário e o módulo kernel que controlará a GPU.


Objetivos principais
- Compreender o funcionamento da GPU.
- Compreender a comunicação entre o HPS e o FPGA.
- Desenvolver um módulo kernel que se comunique com a GPU.
- Entender a comunicação entre o User Space e o Kernel Space.
- Implementar a capacidade do módulo kernel de receber informações do User Space.
- Criar uma biblioteca em C para simplificar a interação do usuário com o sistema.

## Descrição dos equipamentos e softwares utilizados:

* Entendimento sobre a HPS e FPGA
A principal função do HPS é fornecer o processamento de software e gerenciar o fluxo de dados entre os componentes de hardware e software em um sistema embarcado. Já o FPGA é uma matriz de blocos lógicos configuráveis e uma rede de interconexão que permite que os usuários configurem a funcionalidade do chip para atender às suas necessidades específicas.

* GPU
Unidade de processamento gráfico, também conhecida como GPU, é um componente eletrônico projetado para acelerar tarefas relacionadas à computação gráfica e ao processamento de imagens em uma ampla gama de dispositivos, incluindo placas de vídeo, placas-mãe, smartphones e computadores pessoais (PCs).
A capacidade da GPU de realizar cálculos matemáticos complexos de forma rápida e eficiente reduz significativamente o tempo necessário para que um computador execute uma variedade de programas.
	
* Monitor CRT
O monitor utilizado no projeto foi o DELL M782p, um modelo CRT. Esse tipo de monitor utiliza um tubo de raios catódicos (CRT) para exibir imagens. O DELL M782p possui uma tela de visualização de 17 polegadas e uma resolução máxima de 1280x1024 pixels. Ele oferece uma interface VGA para conexão com o computador ou placa de desenvolvimento. Os monitores CRT são conhecidos por sua reprodução de cores vibrantes e tempos de resposta rápidos, tornando-os uma escolha adequada para projetos que exigem interação em tempo real, como jogos e simulações.

* VScode
O Visual Studio Code, comumente referido como VSCode, é um ambiente de desenvolvimento amplamente utilizado. Desenvolvido pela Microsoft, é um editor de código gratuito e de código aberto, dotado de recursos como realce de sintaxe, conclusão de código e depuração integrada. Suporta diversas linguagens de programação e oferece extensões para personalização.

* Bibliotecas do Kernel
- linux/kernel.h
O arquivo linux/kernel.h é um cabeçalho (header) do kernel do Linux que contém definições e declarações usadas em várias partes do código do kernel. Este arquivo é parte do sistema de build do kernel e é incluído por outros arquivos de código-fonte para acessar essas definições e funções.

- linux/module.h
A biblioteca linux/module.h é fundamental para o desenvolvimento de módulos do kernel no Linux. A principal função dele é definir Metadados do Módulo.

- linux/init.h
O cabeçalho linux/init.h no kernel do Linux contém macros e funções relacionadas à inicialização e finalização do kernel e de módulos do kernel. Este arquivo é fundamental para garantir a inicialização e a finalização do módulo kernel.
https://elixir.bootlin.com/linux/v4.18/source/include/linux/init.h

- asm/io.h
O asm/io.h é um arquivo de cabeçalho (header file) que faz parte do kernel e é utilizado para fornecer funções e macros que permitem o acesso direto a portas de entrada/saída (I/O) em hardware, facilitando a comunicação direta com dispositivos de hardware a partir do código em C.

- linux/uaccess.h
O linux/uaccess.h é um arquivo de cabeçalho no kernel do Linux que contém macros e funções para realizar a cópia segura de dados entre o espaço de usuário e o espaço do kernel. Essas funções tem como finalidade garantir que esses acessos sejam seguros e não corrompa a memória do sistema.

- linux/fs.h
O linux/fs.h é um cabeçalho de arquivo do sistema operacional Linux que contém declarações e definições relacionadas ao sistema de arquivos. Ele faz parte do kernel do Linux. Esse arquivo inclui estruturas, macros e funções necessárias para manipulação e interação com sistemas de arquivos no nível do kernel.

- linux/cdev.h
O linux/cdev.h é um cabeçalho de arquivo do kernel do Linux que fornece as definições e declarações necessárias para a implementação e manipulação de dispositivos de caracteres (character devices). Dispositivos de caractere permitem acesso sequencial aos dados, como terminais, portas seriais, e muitos outros dispositivos de hardware que transferem dados byte a byte.


## Metodologia

  Para realizar o projeto, foi necessário o entendimento acerca de algumas peculiaridades do funcionamento do kernel do linux, em conjunto com a construção de um módulo externo para o kernel, e compreensão do funcionamento da unidade de processamento gráfica utilizada. 
Para o desenvolver do projeto foi necessário fazer algumas subdivisões a fim de atingir a produção final do que foi proposto, são elas:
* 1. Comunicação entre HPS e FPGA
Para fazer a comunicação entre HPS e FPGA é utilizado uma série de pontes AXI (Advanced Extensible Interface) para conectá-los. Existem três pontes principais para comunicação:
1. FPGA-to-HPS bridge (f2h)
2. HPS-to-FPGA bridge (h2f)
3. Lightweight HPS-to-FPGA bridge (lwh2f)
Os programas que são executados no HPS no sistema operacional Linux têm a capacidade de acessar os periféricos de hardware implementados no FPGA. Isso é possível através da ponte HPS-to-FPGA ou da ponte Lightweight HPS-to-FPGA, ambas mapeadas para regiões no espaço de memória ARM. Quando um componente do lado do FPGA é conectado a uma dessas pontes, os registradores de memória mapeados desse componente tornam-se acessíveis para leitura e gravação pelo processador ARM na região de memória da ponte.
No projeto em questão, optou-se pela Lightweight HPS-to-FPGA bridge (lwh2f). Esta ponte é projetada especificamente para comunicação do HPS para a FPGA, sendo limitada a 32 bits, mas otimizada para minimizar a latência. Sua função principal é transferir registros de controle e status do HPS para a FPGA. Na figura abaixo pode-se ver como é organizado a comunicação entre essas duas partes:

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

Esta figura fornece um esboço detalhado de como o HPS, FPGA e seus associados periféricos se comunicam através de uma série de pontes e barramentos AXI.
O HPS suporta a comunicação com o FPGA e periféricos através da interconexão L3, que está conectada ao controlador SDRAM HPS (DDR3).
Abaixo pode-se ver uma figura representativa do L3:

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

A interconexão do sistema fornece acesso a um espaço de endereço de 4 GB, os espaços de endereço são divididos em uma ou mais regiões contíguas não sobrepostas.
As regiões da janela fornecem acesso a outros espaços de endereço. As setas pretas indicam qual espaço de endereço é acessado por uma região de janela (as setas apontam para o espaço de endereço acessado).
Nesse sentido, o mapeamento permite que a CPU visualize e acesse o espaço de endereçamento do FPGA, onde os componentes estão localizados. Dessa forma, é possível ler e gravar informações conforme necessário, controlando o hardware por meio do software.
O aplicativo em C utiliza APIs do módulo kernel do Linux para enviar dados de escrita ou receber dados de leitura de endereços de memória específicos. Cada componente possui um endereço base, que é utilizado para acessar, controlar e transferir dados de e para os componentes no FPGA por meio do aplicativo em C.
A tabela a seguir mostra os endereços de memória das pontes: 

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

* 2. Mapeamento de memória física
	Quando os programas estão sendo executados no Linux, o acesso à memória física não é feito de maneira direta. Isso ocorre porque o Linux utiliza um sistema de memória virtual, onde os endereços de memória usados pelos programas são mapeados para endereços de memória física pelo sistema operacional.
Para acessar endereços de memória física de um programa em execução no Linux, é necessário chamar a função do kernel Linux ioremap_nocache. Essa função, que significa mapa de memória, mapeia um arquivo para a memória virtual, permitindo que os programas acessem dados por meio desse arquivo.
Um arquivo de dispositivo importante para esse propósito é o /dev/mem, que representa a memória física do sistema do computador. Ao acessar esse arquivo em algum deslocamento, é equivalente a acessar a memória física no endereço correspondente. Usando a função mmap para mapear o arquivo /dev/mem na memória virtual, é possível mapear endereços físicos para endereços virtuais, permitindo que os programas acessem a memória física de forma controlada e segura.
* 3. Arquitetura do Processador Gráfico

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

Analisando cada elemento presente na imagem acima é possível compreender o fluxo que se dá dentro do processador gráfico, podendo ser descrito, dentre seus elementos principais, da seguinte forma:
A unidade de controle inicia o processo do fluxo de dados, responsável por controlar o fluxo de instruções e coordenar a operação dos outros componentes, ela recebe o opcode para saber com qual instrução está lidando e gera sinais de controle como memory_wr_sp, reset_vga, e a indicação para a instrução passada, representado por “new inst”, este módulo vai funcionar como uma máquina de estados. Na sequência, o decodificador de instrução decodifica a instrução recebida para determinar quais operações realizar e relacioná-las aos multiplexadores, ao banco de registradores e às memórias. O banco de registradores tem a função de armazenar dados temporários durante a execução de instruções, recebendo register_wr para escrita em registradores, o dado que vai ser escrito e em qual registrador, e fornece R0 como saída, além de se conectar ao módulo de desenho e outros componentes. O módulo de desenho é responsável por desenhar os elementos gráficos na tela, recebendo os bits vindos do banco de registradores, o dado de saída da memória de background e os pixels correspondentes ao desenho. A memória de background armazena os dados de fundo (background) da tela, conectando-se ao módulo de desenho pela saída out_data. A memória de sprites armazena os dados dos sprites (imagens) na tela, similar à memória de background, conectando-se ao gerador RGB que em seguida envia os sinais para o monitor. O co-processador realiza operações auxiliares que suportam a unidade de controle principal, recebendo sinais de diversas partes da estrutura e sendo responsável por gerar os polígonos no monitor. O controlador VGA controla o sinal VGA para a tela, recebendo sinais de controle como reset_vga, e gerando sinais para o monitor (monitor_signals) e coordenadas dos pixels (pixel_x, pixel_y). O gerador RGB converte os dados gráficos para sinais RGB que são enviados ao monitor, recebendo dados de diversos módulos e enviando sinais RGB para o monitor. Por fim, os multiplexadores (Mux) selecionam entre várias entradas e encaminham para a saída com base nos sinais de controle, recebendo vários sinais de entrada, como data, address, etc., e fornecendo as saídas apropriadas aos componentes.

* 4. Acesso a GPU
O acesso à GPU é feito através da Lightweight HPS-to-FPGA Bridge, que possui o endereço base 0xFF200000. Para enviar informações sobre o que será exibido, é necessário mapear a memória física para a memória virtual(LW_Virtual). Esse mapeamento permite somar o endereço base da memória com os deslocamentos específicos dos dados, obtendo o endereço correto para realizar as operações necessárias na GPU.
Assim, para enviar os dados do opcode e endereçamento do Banco de Registrador e Memórias, a LW_Virtual será somada ao endereço do dataA, que é 0x80. Da mesma forma, ela será somada ao endereço do dataB, que é 0x70, para enviar os dados a serem armazenados e/ou atualizados. Além disso, o valor de Start, que é 0xc0, também será somado para permitir a escrita dos dados presentes nos registros dataA e dataB para a GPU.
Por utilizar a ponte Lightweight, todas essas instruções devem possuir 32 bits, mesmo que nem todos os bits sejam úteis na representação da instrução.
Abaixo haverá figuras representando os dados que a GPU recebe e o que eles fazem: 

Essas figuras representam a instrução de Escrita no Banco de Registradores (WBR), responsáveis por configurar os registradores que armazenam as informações dos sprites e a cor base do background, respectivamente.

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

Em ambas, o valor de "dataA" é idêntico. O campo "opcode" é composto por 4 bits e determina qual instrução será executada pelo processador gráfico, nesse campo o seu valor deve ser 0000. O campo “registrador” em questão possui 5 bits e é empregado para especificar onde os parâmetros de impressão serão armazenados.
Na primeira figura, o campo "dataB" representa as configurações do sprite. Ele inclui o campo "offset" com 9 bits, os quais são utilizados para selecionar o sprite na memória. Além disso, existem os campos "coordenada X" e "coordenada Y" para definir a posição dos sprites na tela, e o campo "sp" para determinar se o sprite é visível ou não. Na segunda figura, os campos “R”, “G” e “B” representam, respectivamente, os bits das cores vermelha, verde e azul, cada um contendo 3 bits, para configurar a cor base do background do sprite. Abaixo, está a representação de todos os sprites disponíveis que podem ser usados e que estão armazenados na memória.

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

Essa figura representa a instrução de Escrita na Memória de Sprites (WSM) responsável por armazenar ou modificar o conteúdo presente na Memória de Sprites. 

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

O campo “dataA” possui 4 bits e deve ser configurado como 0001 para esta instrução. O endereço de memória possui 14 bits e é utilizado para especificar qual local da memória será alterado. Já o campo “dataB” possui os campos “R”, “G” e “B”, representando, respectivamente, os bits das cores vermelha, verde e azul, cada um contendo 3 bits, para definir os novos componentes RGB para o local desejado.

A instrução de  Escrita na Memória de Background (WBM) é responsável por armazenar ou modificar o conteúdo presente na Memória de Background. Sua função é configurar valores RGB para o preenchimento de áreas do background. Os campos de sua estrutura assemelham-se aos da instrução WSM, com as únicas discrepâncias sendo no campo de endereço de memória, o qual possui um tamanho de 12 bits e o valor do opcode é definido como 0010.
Essa figura representa a instrução para a Definição de um Polígono (DP) responsável por definir os dados referentes a um polígono que deve ser renderizado.

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

Nesta instrução, o campo “dataA” possui 4 bits e deve ser configurado como 0011. O endereço de memória possui 4 bits e é utilizado para determinar o local de armazenamento em memória da instrução, permitindo o gerenciamento da sobreposição dos polígonos. Já o campo “dataB” possui os campos “R”, “G” e “B”, representando, respectivamente, os bits das cores vermelha, verde e azul, cada um contendo 3 bits, para definir as cores. Também possui o campo “forma” com 1 bit, que se possui valor igual a 0 será um quadrado e se possui o valor 1 será um triângulo. Outro campo é o “tamanho”, com 4 bits, que representam o tamanho do polígono, desde o desativado até 160 x 160 pixels. Os campos "ref_point_X" e "ref_point_Y" são empregados para estabelecer as coordenadas do ponto de referência do polígono.


* 5. Construção do módulo kernel
O kernel: 
O kernel é o núcleo de qualquer sistema operacional, essencial para o funcionamento de dispositivos eletrônicos, trabalhando no gerenciamento da comunicação entre hardware e software. Além disso, o kernel gerencia periféricos, necessitando de configurações específicas para cada dispositivo. Sua função é organizar, controlar e processar dados, desde a inicialização do sistema até a tradução das solicitações dos usuários em ações, assegurando que todas as partes do sistema operacional funcionem harmoniosamente. Sem o kernel, a operação eficaz de qualquer sistema seria impossível, tornando-o um componente indispensável.

Módulo kernel: Arquitetura do módulo kernel
Um módulo externo de kernel é um trecho de código que pode ser adicionado ao kernel conforme necessário, sem que seja necessário recompilar o kernel ou reiniciar o computador. Os módulos de kernel comumente usados referem-se  a drivers de dispositivo, que permitem o acesso direto ao computador e ao hardware periférico. A intenção de criar um módulo externo do kernel passa pela lógica de não ser necessário construir um sistema monolítico, que desempenharia a única função determinada pelo módulo kernel. Ao invés disso, é possível executar vários módulos associados ao mesmo kernel. Isso significa que diante da necessidade apresentada os módulos de interesse podem ser carregados, com as funcionalidades específicas e executando uma tarefa independentemente da existência de outros módulos, deste modo, ainda é possível economizar recursos e melhorar a eficiência. 
Para o presente projeto foi desenvolvido um módulo para permitir que o sistema operacional consiga realizar o processamento paralelo da GPU. Este tipo de módulo atua como uma ponte entre o sistema operacional e a GPU, gerenciando a comunicação e a execução de tarefas na GPU. Possuindo a responsabilidade de alocar memória para a GPU, transferir dados entre a CPU e a GPU, e invocar operações de processamento gráfico, renderizando elementos na tela, por exemplo. Além disso, o módulo lida com a sincronização e a coordenação das operações, garantindo que os dados processados na GPU sejam corretamente associados ao fluxo de trabalho do sistema.
Durante o desenvolvimento de um módulo externo do kernel, como foi o necessário para o projeto, é necessário observar 4 tópicos principais que compõem a arquitetura do kernel, são eles:
Espaço do kernel: Refere-se a parte central do sistema operacional, que é responsável por gerenciar os recursos do sistema, como a memória, além de ser responsável por fornecer interfaces que possibilitam o espaço do usuário interagir com o kernel, através de chamadas de sistema.
Espaço do usuário: Refere-se ao ambiente onde os programas voltados ao usuário são executados. Um ponto importante a ser observado é que, a partir do espaço do usuário, não é possível acessar diretamente todos os recursos do sistema, especialmente os de hardware. Para isso, é necessário fazer solicitações ao kernel. No projeto, essas solicitações foram essenciais para se comunicar com a GPU conectada à placa de desenvolvimento DE1-SoC. Uma biblioteca foi utilizada para abstrair as funcionalidades disponíveis ao usuário, permitindo identificar o que é necessário e, consequentemente, solicitar ao kernel o acesso adequado.
Seção de controle de processos: O kernel também é responsável pelo gerenciamento dos processos que podem estar associados ao sistema, garantindo um controle de como as execuções são feitas e gerindo os recursos que podem ser associados a cada processo em funcionamento. No contexto do projeto que envolve a comunicação com a GPU conectada à placa de desenvolvimento DE1-SoC, a criação de um módulo externo do kernel vai estar ligada a esse gerenciamento de processos, pois, na inicialização, o módulo deve ser carregado no kernel, registrar suas funcionalidades e alocar os recursos necessários para permitir a comunicação com a GPU. Na finalização, o módulo deve liberar todos os recursos alocados pelo kernel, garantindo que não haja vazamentos de memória ou recursos que permaneçam ocupados indevidamente.
Seção de controle de memória: O kernel pode ainda ser responsável por associar corretamente espaço de memória aos processos que solicitarem, garantindo que exista de forma coerente a atribuição dos espaços de memória para cada processo. É fundamental existir o controle da memória para que se tenha segurança e garantir a existência do isolamento entre os espaços de acesso, que são cruciais para evitar que um processo possa interferir ou acessar a memória de outro processo, prevenindo possíveis falhas e vulnerabilidades. No projeto, esse isolamento garantiu que a comunicação com a GPU fosse feita de maneira segura e eficiente, protegendo tanto os dados quanto os recursos do sistema.
Fluxo de dados dentro do módulo: Dentro do módulo kernel existem várias etapas que estão relacionadas a como a comunicação entre o espaço do usuário e do kernel estão ligadas. A princípio tem-se a inicialização do módulo kernel, uma vez que o módulo foi carregado no sistema torna-se possível realizar as operações de entrada e saída que são de interesse, para o projeto desenvolvido utiliza-se a operação de escrita através do módulo, com o intuito das instruções, que devem ser passadas para a GPU sejam realizadas de modo eficaz. A lógica de como os elementos são divididos dentro do sistema operacional pode ser visualizada na imagem abaixo:

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

No projeto atual, o módulo de kernel desenvolvido para o processamento paralelo da GPU utiliza um "device char" driver para gerenciar as operações de entrada e saída, assegurando que as instruções e os dados sejam transmitidos de maneira eficiente e segura entre a CPU e a GPU. A relação entre um módulo de kernel e um "device char" driver é essencial para a comunicação direta e eficiente com o hardware. 
Conceituando um “device char” é um componente fundamental no sistema operacional Linux, sendo utilizado para interagir com o hardware que processa dados em pequenas quantidades. Estes dispositivos são representados como arquivos no sistema de arquivos, permitindo acesso através de chamadas de sistemas convencionais como leitura e escrita. Esses dispositivos são fundamentais para a comunicação direta com o hardware e utilizam da sincronização para garantir que os dados sejam lidos e escritos de forma ordenada e exclusiva, evitando conflitos de acesso simultâneos
Esses drivers são carregados como módulos de kernel, proporcionando uma interface que facilita a leitura e escrita de dados entre o sistema operacional e o hardware, garantindo acesso ordenado e exclusivo, e evitando conflitos.

* 6. Construção da biblioteca
Para tornar o uso do módulo kernel mais acessível aos usuários, desenvolvemos uma biblioteca em C que simplifica o entendimento e a interação com o módulo kernel. Essa abstração facilita o envio de instruções do espaço do usuário para o espaço do kernel, tornando o processo mais intuitivo para usuários comuns.

As funções disponibilizadas pela biblioteca são as seguintes:

1) `setBackground`: Esta função permite a alteração do plano de fundo da tela, recebendo as cores RGB que serão exibidas.
2) `setPixelOnSpriteMemory`: Ela tem como objetivo definir a cor de um pixel na memória de sprites, fornecendo as cores RGB e o endereço de memória onde o pixel deve ser definido.
3) `editBlockOnBackgroundMemory`: Esta função edita a cor de um bloco na memória de fundo, recebendo o identificador do bloco a ser editado e a estrutura que contém as componentes de cor (R, G, B).
4) `setSpriteOnScreen`: Posiciona um sprite na tela, recebendo o endereço, a variação, a posição relativa (rel_x, rel_y) e a visibilidade.
5) `setPolygon`: Desenha um polígono na tela, especificando a posição relativa (rel_x, rel_y), o endereço, o tamanho, a cor (R, G, B) e a forma.
6) `eraseBackground`: Apaga o plano de fundo, definindo sua cor como preto.

* 7. Comunicação entre o espaço do usuário e do kernel
A comunicação entre o espaço do usuário e o espaço do kernel é realizada por meio de chamadas de sistema, que são mecanismos que permitem que os programas de usuário solicitem serviços do kernel. Quando um programa de usuário precisa executar uma operação que requer acesso a recursos do sistema, ele faz uma chamada de sistema para o kernel, que executa a operação em seu nome. Essas chamadas de sistema são a maneira pela qual os programas de usuário podem interagir com o kernel e acessar recursos privilegiados.
O diretório /dev/ contém arquivos especiais chamados arquivos de dispositivo, que representam dispositivos de hardware e interfaces do kernel. Esses arquivos permitem que os programas de usuário interajam com o hardware e outros recursos do sistema por meio de operações de leitura e escrita. Quando um programa abre um arquivo de dispositivo, na verdade está abrindo uma interface para o driver do dispositivo no kernel, que gerencia a comunicação com o hardware correspondente.
Portanto, as funções da biblioteca destinadas à comunicação com a GPU precisam transferir dados do espaço de usuário para o espaço de kernel através desse diretório. Para isso, é necessário abrir o arquivo /dev/gpu123 no modo de leitura e escrita, já que esse é o diretório utilizado para essa comunicação entre essas duas partes. Dessa forma, as funções podem escrever os dados necessários no arquivo usando a função write.() e ler os dados do módulo kernel usando a função read(), mantendo assim uma comunicação eficiente e segura.

* 8. Descrição de instalação e execução.

Para compilar e gerenciar o módulo kernel desenvolvido, foi utilizado um makefile como meio de facilitação dos comandos e ações necessárias para executar o projeto na placa de desenvolvimento. Através do makefile é possível: Compilar e instalar um módulo do kernel (BOTAR LINK PARA kernelgpudriver.ko), compilar um programa de usuário (BOTAR LINK PARA gputest), limpar os arquivos de compilação gerados para ambos.
Para testar e executar o projeto é necessário seguir os seguintes passos, já relacionados a uma descrição do arquivo (BOTAR LINK PARA makefile):

Compilação do Módulo do Kernel:
Acesse através do terminal o diretório que contém o Makefile.
Execute o comando “make all” para compilar o módulo do kernel. Esse comando utiliza o Makefile para compilar “kernelgpudriver.c” no contexto do kernel atual do sistema, gerando o módulo “kernelgpudriver.ko”. Além disso, o comando em seguida instala o módulo “kernelgpudriver” no kernel da placa, a partir do comando “sudo insmod kernelgpudriver.ko”, que exige permissões de super usuários do sistema linux para ser executado. Logo, essa instalação irá exigir que se insira a senha do usuário para ser concluída. 
Configuração do Dispositivo:
Se houver algum arquivo de dispositivo previamente associado (/dev/gpu123), ele será removido com o comando rm -f /dev/gpu123, que força a remoção do dispositivo, para garantir uma configuração limpa.
Sabendo que já não se tem nenhum dispositivo no sistema é necessário criar um novo arquivo de dispositivo com o comando,  “sudo mknod /dev/gpu123 c <major_number> 0”. Substituindo <major_number> pelo número maior atribuído ao dispositivo pelo sistema. Esse número pode ser obtido a partir do comando do sistema “cat /proc/devices” (insira esse comando e busque pelo major number associado ao driver gpu123), ou via dmesg após carregar o módulo.
Compilação do Programa de Teste:
Execute make program para compilar o programa de teste. Este comando compila gputest.c, lib/instructions.c, e lib/gpulib.c em seus respectivos arquivos objeto (gputest.o, instructions.o, gpulib.o), e então os liga para criar o executável gputest.
Execução do Programa de Teste:
Com o módulo do kernel carregado, o dispositivo configurado e o programa de teste compilado, é possível executar o programa de teste simplesmente executando no terminal ./gputest. 
Adicional: 
Após a execução dos testes, é possível realizar a limpeza dos arquivos de compilação para limpar os arquivos de compilação gerados executando “make clean”. Isso remove todos os arquivos objeto e binários, assim como os arquivos temporários do módulo do kernel.


##Resultados e conclusão

O projeto alcançou todos os objetivos estabelecidos, possuindo um módulo kernel e uma biblioteca, como interface de facilitação da passagem de dados do espaço do usuário para o kernel. O entendimento de como ocorre todo o processo, desde a utilização de uma função na biblioteca até a chegada da solicitação de execução de uma instrução a GPU, foi crucial para o bem desenvolver do trabalho.
O código em C implementado para exemplificar o uso de todos os entes apresentados, pode ser visualizado na imagem abaixo, que é uma representação da tela do jogo “Space invaders”.

<p align="center"><strong> </strong></p>
<p align="center">
</p>
<p align="center"><strong>Fonte: 
</strong></p>

Possuindo o uso das instruções de: 
setSpriteOnScreen(): Usada para adicionar sprites na tela. Adiciona fileiras de inimigos (aliens) na parte superior da tela, a nave na parte inferior da tela e o tiro que sai da nave.

setPixelOnSpriteMemory(): Edita pixels na memória de sprites, utilizada para definir a cor dos pixels, como no exemplo onde uma estrela é desenhada na tela, os pixels de cor branca são adicionados para representar estrelas no céu.

editBlockOnBackgroundMemory(): Edita blocos na memória de fundo, foi usada para criar um fundo estrelado na tela.

setPolygon(): Adiciona polígonos na tela, foi utilizada para adicionar triângulos verdes na tela, os quatro triângulos são desenhados em posições específicas para representar obstáculos.

##Referências


## Autores

- Fábio Santos Miranda
- Ícaro José Batista de Oliveira
- Nalbert Santos Araujo
- Valmir Alves Nogueira Filho
