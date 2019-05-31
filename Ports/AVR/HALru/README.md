## English

### HALru Framework Users

It is possible to use this port together with the HALru framework. The micro controllers tested were:

- ATMega328p
- ATMega2560

The system will work with the Timer 2 (8-bits) of the micro controller. The advantage of working with this timer is that it can wake the microcontroller when it is in the **SLEEP_MODE_PWR_SAVE** mode, thus ensuring greater power savings.

### Timebase

Every 10 ms will interrupt the system for the management of tasks. This time is fully configurable by changing the ui8TickMS variable.

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

### Author

This system was developed by **Jorge Henrique Moreira Santana**:
  - Brazilian;
  - Electronic Technician by SENAI;
  - Electrical Engineer with emphasis in Automation and Control by the Federal University of Uberlândia;

## Português

### Usuários da Framework HALru

É possvel utilizar este port juntamente com a framework HALru e seus drivers. Os micro controladores testados foram:

- ATMega328p
- ATMega2560

O sistema trabalhará com o Timer 2 (8-bits) do microcontrolador. A vantagem de se trabalhar com este temporizador é que ele consegue despertar o microcontrolador quando ele estiver no modo **SLEEP_MODE_PWR_SAVE**, garantindo assim uma maior economia de energia.

### Base de tempo

A cada 10 ms acontecerá a interrupção do sistema para o gerenciamento das tarefas. Esse tempo é totalmente configurável alterando a variável ui8TickMS.

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

### Autor

Este sistema foi desenvolvido por **Jorge Henrique Moreira Santana**:
  - Brasileiro;
  - Técnico em Eletrônica pelo SENAI;
  - Engenheiro Eletricista com ênfase em Automação e Controle pela Universidade Federal de Uberlândia;
