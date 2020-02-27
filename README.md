## English

### Critical Systems

A critical RTOS is one that has a deterministic behavior, that is, the deadline can not be violated. If the ABS brake system, for example, fails or takes too long to respond, a person may get hurt. This STR class has to be ultra reliable, ie the mean time between failures has to be greater than 10 high to 9 hours. Other examples of Rigid STR are: the embedded navigation system of an aircraft, the embedded protection system for high voltage lines. In both examples, if the system fails, people may get hurt.

A preemptive system is one that is able to temporarily interrupt a task being executed by a computer system, without requiring its cooperation, and with the intention of resuming the task later. Such changes of the executed task are known as exchanges of context. Usually, it is performed by a privileged task or part of the system known as preemptive scheduler, which has the power to anticipate, interrupt, and subsequently resume other tasks in the system. This causes this type of RTOS to be classified as critical RTOS.

Starting with the Windows XP operating system, all Microsoft operating systems are now fully preemptive.

### Non-Critical Systems

A non-critical RTOS is one that also has time as the fundamental parameter, but a failure is acceptable. The system that works on a DVD player is not critical, since failure to perform a task in response and an event in a given time interval does not cause irreversible damage. Unlike critical systems, these systems typically work with a large amount of data.

Cooperative multitasking is an approach to implementing multitasking operating systems. Processor sharing is managed by the programs themselves, which voluntarily give up control to other programs at points set by programmers. Users can run two or more programs, but the program in the foreground gains control of the processor.

Microsoft's operating systems prior to Windows 95 and Windows NT were implemented with cooperative multitasking. Systems later than these and prior to Windows XP used a hybrid task manager, merging a preemptive with a non-preemptive one.

### Preemption vs Cooperation

In a preemptive RTOS there is a better treatment of the most critical problems: If there is any urgency in a co-operative system, this urgency should wait until the running function is finished so that urgency is treated. In contrast, preemptive kernels literally stop the running function and treat urgency. Another advantage of the preemptive is its stability: in a cooperative, if some function hangs, the whole system will lock. This does not happen in a preemptive.

On the other hand, the advantage of the cooperative model resides in the simplicity of the design of the operating system, being extremely easy the implementation and the maintenance of this type of nucleus. A disadvantage of the preemptive system is precisely its use: One must calculate the size of the stack memory that each task will use (something not trivial). In addition, we have a number of "amplified" computational problems when using preemptive systems, such as cases of prioritization (which occur much less frequently in cooperative systems), and are **mitigated** by the use of complex semaphores (such as the mutex, of FreeRTOS). Cooperative systems also tend to be lighter than preemptives.

Each type of RTOS addresses a range of problems. It is up to the developer to choose what fits best in their solution.

### Co-routines

Co-routine is a component of a program that generalizes subroutines to allow multiple entry points and suspend and resume execution at certain points in the code. With this, your codes will be very close to a preemptive code, with some advantages and disadvantages. An advantage that stands out is the extremely simple implementation. However, all variables declared within the function that uses co-routine must be declared statically, so that they are not lost during the context switch. This causes you to have a slightly higher memory cost.

### Semaphores

It is used to share resources, and to communicate between tasks. While a task uses a given feature it can block a semaphore; another task before using this feature checks the state of the semaphore, and if it is locked, it does not use the feature. You can expect the feature to be released, or you can perform other functions.

### Binary Switches

A binary switches has the states turned on or turned off. Switches are more free and faster than semaphores, but is less safe.

### Prerequisites

This library uses other libraries in your implementation, which must be installed in your IDE if you want to use these files in your code. These libraries are:

- [EmbeddedTools](https://github.com/DigitalEmbed/EmbeddedTools)
- [MemoryManager](https://github.com/DigitalEmbed/DataStructures)

### How to install?

To use RTOS on your system, you must:
  - Import the library *jOS.h* into your *main.c* file.
  - In the *Ports* folder of this repository there are several folders with names of microcontrollers or prototype board frameworks.
    - If there is a folder for your hardware, simply install the *System* library in your IDE.
    - If there is no folder for your hardware, you will need to edit the *System.c* file in the */jOS* directory using the *How to port?*. topic.
  - In the */jOS* folder there is the *jOS.h* file. In it, you should define:
    - The amount of switches and semaphores your system will have;
    - The amount of priorities your firmware will have;
    - The amount of priority tasks your firmware will have;
    - The amount of tasks that can be saved in the task buffer of the scheduler;
    - The maximum number of characters that will have the name of a task;
    - Software and hardware watchdog time;

### How to port?

In the */Port* directory, the *NoMicrocontroller.c* file exists. Create a copy of this file with the name of your microcontroller and, in it, edit the functions:

  - **System_restart(void)**: Restart the system.
  - **System_taskTimerConfiguration(void(vSchedulerInterruption)(void))**: Interrupt by system timer. The function that should be called during the interrupt is the vSchedulerInterruption, which is in the parameter of this function. This function is of type void, with no parameters. Finally, modify the global variable [  ]ui8TaskTickMS with the value of the interrupt time, in milliseconds.
  - **System_semaphoreTimerConfiguration(void(vfSemaphoresInterruption)(void))**: Interrupt by system timer. The function that should be called during the interrupt is the vSchedulerInterruption, which is in the parameter of this function. This function is of type void, with no parameters. Finally, modify the global variable [  ]ui8SemaphoreTickMS with the value of the interrupt time, in milliseconds. This function is mandatory for the system. You can use the same timer used in the System_taskTimerConfiguration function.
  - **System_sleepConfiguration(void)**: This is the sleep mode setting function.
  - **System_sleep(void)**: Makes the system sleep.
  - **System_enableHardwareWatchdog(void)**: This is the hardware watchdog enabler function.
  - **System_disableHardwareWatchdog(void)**: This is the hardware watchdog disabler function.
  - **System_enableInterrupts(void)**: Resumes all interrupts of the system.
  
  Edit line 3 macro by your system name:
  
    #if defined(ArduinoAVR)       - Older
    #if defined(Your_System)      - Newer
  
  In *Configs.h* file in */jOS* path, find the **Configuration: HAL** and replace the macro definition with your system macro created previously:
  
    //! Configuration: HAL
    /*!
      Set the hal system here.
      Availables:
      - NoMicrocontroller
      - ArduinoAVR
      - HALru
      - ESP12
      - Your_System
    */
    #define Your_System

### License

#### GNU Lesser General Public License v3.0

The permissions of this copyleft license are conditioned to make available the complete source code of licensed works and modifications under the same license or to GNU GPLv3. Copyright and license notices must be preserved. Employees provide an express grant of patent rights. However, further work using the licensed work through the interfaces provided by the licensed work can be distributed under different terms and without source code for the larger work.

All rights reserved.

- Commercial use;
- Modifications;
- Distribution;
- Use in patents;
- Private use;

Limitations:

- Responsibility;
- Warranty;

Conditions:

- License and copyright notice;
- Disclosure of source code;
- Changes of state;
- License compatibility (for libraries);

For more details, check the _LICENSE_ document. If you want to use a commercial product without the source code, send an email to jorge_henrique_123@hotmail.com to talk.

### How can I help you?

You can contribute to this project in several ways:

- Improving the algorithm;
- Fixing bugs;
- Improving documentation;
- Testing the algorithms and reporting errors;
- Solving problems of any kind using this library and sharing the solution to the community;

**Caution:** _The codes follow the **MISRA** pattern. Codes that do not follow this pattern **will not** enter the solution._

### Preemptive Cores

If the solution presented here does not meet your expectations, you can try a preemptive core:

  - [FreeRTOS](https://aws.amazon.com/pt/freertos/?sc_channel=PS&sc_campaign=acquisition_BR&sc_publisher=google&sc_medium=ACQ-P%7CPS-GO%7CNon-Brand%7CDesktop%7CSU%7CBrand%7CDesktop%7CSU%7CIoT%7CFreeRTOS%7CBR%7CEN%7CText&sc_content=freertos_bmm&sc_detail=%2Bfreertos&sc_category=IoT&sc_segment=293657313280&sc_matchtype=b&sc_country=BR&s_kwcid=AL!4422!3!293657313280!b!!g!!%2Bfreertos&ef_id=CjwKCAiAqt7jBRAcEiwAof2uK8RRHpqV6RHgVcB9Mede0PPaKbBIeQ4Px256YF1EoLP4J0KT_pNeKBoCKQQQAvD_BwE:G:s)
  - [mbed](https://os.mbed.com/users/mbed_official/code/mbed-rtos/) 
  - [BRTOS](https://github.com/brtos/brtos)
  - [NuttX](https://nuttx.org/)
  - [ChibiOS](http://www.chibios.org/dokuwiki/doku.php)

### Author

This system was developed by **Jorge Henrique Moreira Santana**:
  - Brazilian;
  - Electronic Technician by SENAI;
  - Electrical Engineer with emphasis in Automation and Control by the Federal University of Uberlândia;

## Português

### Sistemas Críticos

Um RTOS crítico é aquele que tem um comportamento determinístico, ou seja, o prazo para execução de uma tarefa (deadline) não pode ser violado. Se o sistema de um freio ABS, por exemplo, falhar ou demorar demais para responder, uma pessoa poderá se machucar. Essa classe de STR tem que ser ultra confiável, ou seja, o tempo médio entre falhas tem que ser maior que 10 elevado a 9 horas. Outros exemplos de STR Rígido são: o sistema embarcado de navegação de uma aeronave, o sistema embarcado de proteção de linhas de alta tensão. Em ambos os exemplos, se o sistema falhar, pessoas poderão se machucar.

Um sistema preemptivo é aquele que é capaz de interromper temporariamente uma tarefa sendo executada por um sistema computacional, sem exigir sua cooperação, e com a intenção de retomar à tarefa posteriormente. Tais mudanças da tarefa executada são conhecidas como trocas de contexto. Normalmente, é realizada por uma tarefa privilegiada ou parte do sistema conhecido como escalonador preemptivo, que tem o poder de antecipar, interromper e, posteriormente, retomar outras tarefas no sistema. Isso faz com que este tipo de RTOS seja classificado como RTOS crítico.

Á partir do sistema operacional Windows XP, todos os sistemas operacionais da Microsoft passaram a ser totalmente preemptivos.

### Sistemas Não-Críticos

Um RTOS não-crítico é aquele que também tem o tempo como parâmetro fundamental, mas uma falha é aceitável. O sistema que funciona em um leitor de DVD não é crítico, pois o não cumprimento de uma tarefa em resposta e um evento em um determinado intervalo de tempo não provoca danos irreversíveis. Ao contrário dos sistemas críticos, esses sistemas normalmente trabalham com um grande volume de dados.

A multitarefa cooperativa é uma abordagem para a implementação de sistemas operacionais multitarefa. O compartilhamento do processador é gerenciado pelos próprios programas, que cedem voluntariamente o controle para outros programas em pontos definidos pelos programadores. Os utilizadores podem executar dois ou mais programas, mas o programa que se encontra em primeiro plano ganha o controle do processador.

Os sistemas operacionais da Microsoft anteriores ao Windows 95 e ao Windows NT eram implementados com multitarefa cooperativa. Os sistemas posteriores a estes e anteriores ao Windows XP utilizavam de um gerenciador de tarefas híbrido, mesclando um sistema preemptivo com um não preemptivo.

### Preempção vs Cooperação

Num RTOS preemptivo há um melhor tratamento dos problemas mais críticos: Caso apareça alguma urgência num sistema cooperativo, essa urgência deverá esperar a função em execução terminar para que a urgência seja tratada. Em contrapartida, os núcleos preemptivos conseguem literalmente parar a função em execução e tratar a urgência. Uma outra vantagem do preemptivo é a sua estabilidade: num cooperativo, caso alguma função trave, o sistema inteiro travará. Isso não acontece num preemptivo.

Por outro lado, a vantagem do modelo cooperativo reside na simplicidade do desenho do sistema operacional, sendo extremamente mais fácil a implementação e a manutenção deste tipo de núcleo. Uma desvantagem do sistema preemptivo está justamente no seu uso: Deve-se calcular o tamanho da memória de pilha que cada tarefa irá utilizar (algo não trivial). Além disso, temos vários problemas computacionais "amplificados" ao usar sistemas preemptivos, como os casos de inversões de prioridades (que acontecem com bem menos frequência nos sistemas cooperativos), sendo **amenizados** com o uso de semáforos complexos (como o mutex, do FreeRTOS). Sistemas cooperativos também tendem a ser mais leves que os preemptivos.

Cada tipo de RTOS atende uma gama de problemas. Cabe ao desenvolvedor escolher o que melhor se adapta na sua solução.

### Co-rotinas

Co-rotina é um componente de um programa que generaliza subrotinas para permitir múltiplos pontos de entrada e a suspensão e o resumo da execução em determinados pontos do código. Com isso, seus códigos ficarão muito próximo a um código preemptivo, com algumas vantagens e desvantagens. Uma vantagem que vale ressaltar é a implementação extremamente simples. Todavia, todas as variáveis declaradas dentro da função que utiliza co-rotina devem ser declaradas de forma estática, para que as mesmas não se percam durante a troca de contexto. Isso faz com que você tenha um custo de memória um pouco mais alto.

### Semáforos

É usado para partilhar recursos, e para comunicação entre tarefas. Enquanto uma tarefa utiliza um dado recurso pode bloquear um semáforo; outra tarefa antes de utilizar esse recurso verifica o estado do semáforo, e se estiver bloqueado, não utiliza o recurso. Pode esperar que, o recurso fique liberto, ou pode executar outras funções.

### Chaves Binárias

Uma chave binária tem os estados ligado ou desligado. Embora seja menos segura, uma chave é em mais leve e mais rápida que um semáforo.

### Pré-Requisitos

Esta biblioteca utiliza outras bibliotecas em sua implementação, que devem estarem instaladas na sua IDE caso queira utilizar estes arquivos em seus códigos. Estas bilbiotecas são:

- [EmbeddedTools](https://github.com/DigitalEmbed/EmbeddedTools)
- [MemoryManager](https://github.com/DigitalEmbed/DataStructures)

### Como instalar?

Para utilizar o RTOS no seu sistema, você deve:
  - Importe a biblioteca *jOS.h* no seu arquivo *main.c*.
  - Na pasta *Ports* deste repositório existem várias pastas com nomes de microcontroladores ou frameworks de placas de prototipação. 
    - Caso exista a pasta referente ao seu hardware, simplesmente instale a biblioteca *System* na sua IDE.
    - Caso não exista a pasta referente ao seu hardware, você terá que editar o arquivo *System.c* do diretório */jOS* utilizando o tópico "**Como portar?**".
   - Na pasta */jOS* existe o arquivo *jOS.h*. Nele, você deverá definir:
    - A quantidade de semáforos que seu sistema terá;
    - A quantidade de prioridades que o seu firmware terá;
    - A quantidade de tarefas por prioridades que o seu firmware terá;
    - A quantidade de tarefas que poderão ser salvas no buffer de tarefa do escalonador;
    - A quantidade máxima de caracteres que terá o nome de uma tarefa;
    - O tempo do Watchdog por software e por hardware;

### Como portar?

No diretório *"/jOS/System"*, existe o arquivo *System.c*. Nele, você deverá editar as funções:

  - **vSystemRestart()**: Reinicia o sistema. Essa função é obrigatória para o sistema.  
  - **vSystemSleep()**: Faz o sistema dormir. Essa função é opcional para o sistema.
  - **vSystemWakeUp()**: Faz o sistema dormir. Essa função é opcional para o sistema, mas deve ser preenchida caso a função vSystemSleep() seja preenchida.
  - **vSystemTimerInterruptConfiguration(isr_t isrSchedulerInterrupt)**: Interrupção por timer do sistema. A função que deverá ser chamada durante a interrupço é a isrSchedulerInterrupt, que encontra-se no parâmetro desta função. Essa função é do tipo void, sem nenhum parâmetro. Por último, modifique a variável global ui8TickMS com o valor do tempo de interrupção, em milissegundos. Essa função é obrigatória para o sistema.  
  - **vSystemSleepConfiguration()**: É a função de configuração do modo sleep. Essa função é opcional para o sistema, mas deve ser preenchida para que as funções vSystemSleep() e vSystemWakeUp() funcionem.
  - **vSystemHardwareWatchdogConfiguration()**: É a função de configuração do watchdog de hardware. Como não é todo microcontrolador que possui um segundo watchdog, essa função é opcional para o sistema.
  - **vSystemSuspendRTOSInterrupts()**: Suspende as interrupções do timer utilizado pelo sistema. Em alguns casos, é imprencindível que um processo não seja interrompido por outras tarefas. Todavia, essa função deve ser usada com extrema cautela, sempre deixando o sistema suspenso pelo menor tempo possível. Essa função é opcional para o sistema.
  - **vSystemResumeRTOSInterrupts()**: Retoma todas as interrupções do timer utilizado pelo sistema que foram suspensas na chamada da função vSystemSuspendRTOSInterrupts(). Caso o sistema não retome, seu firmware entrará em colapso. Essa função é opcional para o sistema, mas deve ser preenchida caso a função vSystemSuspendRTOSInterrupts() seja preenchida.
  - **vSystemSuspendAllInterrupts()**: Suspende todas as interrupções do sistema. Em alguns casos, é imprencindível que um processo não seja interrompido. Todavia, essa função deve ser usada com extrema cautela, sempre deixando o sistema suspenso pelo menor tempo possível. Essa função é opcional para o sistema.
  - **vSystemResumeAllInterrupts()**: Retoma todas as interrupções do sistema que foram suspensas na chamada da função vSystemSuspendAllInterrupts(). Caso o sistema não retome, seu firmware entrará em colapso. Essa função é opcional para o sistema, mas deve ser preenchida caso a função vSystemSuspendAllInterrupts() seja preenchida.
  - **vSystemRestartTimerInit()**: Configura um temporizador para que o mesmo reinicie após um determinado tempo. Lembrando que o tempo configurado deve ser **menor** que, se configurado, o do vSystemHardwareWatchdogConfiguration(), fazendo o papel de um watchdog por software. Essa função é obrigatória para o sistema.
  - **vSystemRestartTimerStop()**: Para e reseta o temporizador de reinicialização. Essa função é obrigatória para o sistema.

### Licença

#### GNU Lesser General Public License v3.0

As permissões desta licença de copyleft estão condicionadas a disponibilizar o código fonte completo de trabalhos licenciados e modificações sob a mesma licença ou a GNU GPLv3. Direitos autorais e avisos de licença devem ser preservados. Os colaboradores fornecem uma concessão expressa de direitos de patente. No entanto, um trabalho maior usando o trabalho licenciado por meio de interfaces fornecidas pelo trabalho licenciado pode ser distribuído sob termos diferentes e sem código-fonte para o trabalho maior.

Permissões:

- Uso comercial;
- Modificações;
- Distribuição;
- Uso em patentes;
- Uso privado;

Limitações:

- Responsabilidade;
- Garantia;

Condições:

- Licença e aviso de direitos autorais;
- Divulgação do código fonte;
- Mudanças de estado;
- Compatibilidade de licenças (para bibliotecas);

Para maiores detalhes, checar o documento _LICENSE_. Caso queira utilizar em um produto comercial sem a disponibilização do código fonte, envie um e-mail para jorge_henrique_123@hotmail.com para conversarmos.

### Como posso ajudar?

Você pode contribuir para este projeto de várias maneiras:

- Melhorando o algoritmo;
- Corrigindo bugs;
- Melhorando a documentação;
- Testando os algoritmos e reportando erros;
- Resolvendo problemas de qualquer natureza utilizando essa biblioteca e compartilhando a solução para a comunidade;

**Atenção:** _Os códigos seguem o padrão **MISRA**. Códigos que não seguem esse padrão **não** entrarão na solução._

### Núcleos Preemptivos

Caso a solução apresentada aqui não atenda suas expectativas, você poderá tentar um núcleo preemptivo:

  - [FreeRTOS](https://aws.amazon.com/pt/freertos/?sc_channel=PS&sc_campaign=acquisition_BR&sc_publisher=google&sc_medium=ACQ-P%7CPS-GO%7CNon-Brand%7CDesktop%7CSU%7CBrand%7CDesktop%7CSU%7CIoT%7CFreeRTOS%7CBR%7CEN%7CText&sc_content=freertos_bmm&sc_detail=%2Bfreertos&sc_category=IoT&sc_segment=293657313280&sc_matchtype=b&sc_country=BR&s_kwcid=AL!4422!3!293657313280!b!!g!!%2Bfreertos&ef_id=CjwKCAiAqt7jBRAcEiwAof2uK8RRHpqV6RHgVcB9Mede0PPaKbBIeQ4Px256YF1EoLP4J0KT_pNeKBoCKQQQAvD_BwE:G:s)
  - [mbed](https://os.mbed.com/users/mbed_official/code/mbed-rtos/) 
  - [BRTOS](https://github.com/brtos/brtos)
  - [NuttX](https://nuttx.org/)
  - [ChibiOS](http://www.chibios.org/dokuwiki/doku.php)

### Autor

Este sistema foi desenvolvido por **Jorge Henrique Moreira Santana**:
  - Brasileiro;
  - Técnico em Eletrônica pelo SENAI;
  - Engenheiro Eletricista com ênfase em Automação e Controle pela Universidade Federal de Uberlândia;
