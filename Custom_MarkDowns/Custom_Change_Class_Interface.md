# Custom: Troca de Classe via Interface (CustomClass V1)

> **Documentação Técnica Oficial - CMZone / SSeMU 97k 2K26**  
> **Sistema:** CustomClass V1  
> **Versão / Base:** UPDATE CMZ 18 (3.2.6) 11-10-26 / Kayito  
> **Status:** Concluído e Blindado em Produção (Release Win32)  

---

## Visão Geral

O **CustomClass V1** é um sistema completo e nativo de troca de classes in-game para MuOnline 97k, integrado entre o cliente (`Main.dll`) e o servidor (`GameServer`). O sistema une interface visual interativa em OpenGL com rigorosas camadas de segurança, regras de negócio e recálculo matemático de atributos.

A arquitetura do sistema é dividida em **7 módulos essenciais**:

---

### Módulo 1: Botão de Acesso "TC" & Janela Status ("C")
- **Localização e Alinhamento:** Posicionado no canto inferior direito da Janela de Status nativa (`X = 578.0f, Y = 395.0f`, tamanho `24x24`), espelhando perfeitamente a linha base do botão nativo de fechar `[X]` (`X = 475.0f, Y = 395.0f`).
- **Texturas Utilizadas:** Utiliza os botões metálicos nativos `Mix_button1.ozj` (estado normal) e `Mix_button2.ozj` (estado hover/clique), carregados a partir de `Data\Custom\Interface\CustomClass\`.
- **Atalho de Teclado:** Abertura rápida alternada através da tecla **"J"**, validada com a flag nativa `InputEnable` para impedir acionamento acidental durante a digitação no chat.

---

### Módulo 2: Janela Principal & Docking Inteligente
- **Dimensões e Fundo:** Janela renderizada em espaço virtual `640x480` com dimensões `230x270`, utilizando a textura de pedra clássica `back_middle.jpg` (`CUSTOM_CHANGE_CLASS_BG_TEXTURE`).
- **Docking Dinâmico:**
  - **Janela C Aberta (`CharacterOpened == 1`):** A janela se ancora automaticamente à esquerda da Janela de Status em `X = 210.0f` (mantendo 10px de margem).
  - **Janela C Fechada (`CharacterOpened == 0`):** A janela se centraliza matematicamente no centro da tela em `X = 205.0f`.
- **Botão Fechar:** Botão superior direito `[X]` nas coordenadas `JanelaX + 208, JanelaY + 4` com texturas `exit_01.jpg` e `exit_02.jpg`.

---

### Módulo 3: Carrossel de Classes & Moldura com Canal Alfa (97k Estrito)
- **Escopo 97k (4 Classes Evoluídas):**
  - **Blade Knight (BK)** - Código: 17
  - **Soul Master (SM)** - Código: 1
  - **Muse Elf (ME)** - Código: 33
  - **Magic Gladiator (MG)** - Código: 48
- **Moldura Central Dourada:** Arquivo `CustomClass_Frame.ozt` (96x128) decodificado em formato nativo Webzen OZT de 32 bits (cabeçalho de 22 bytes), garantindo transparência alfa real sem bordas pretas ou desvios de cor.
- **Avatares de Classe com Transparência:** Arquivos `face_bk.ozt`, `face_sm.ozt`, `face_me.ozt` e `face_mg.ozt` (80x80) renderizados sob a moldura dourada.
- **Navegação:** Setas circulares nativas `<` e `>` (`back1.jpg`, `back2.jpg`, `back3.jpg`) com alternância sonora oficial (`PlayBuffer(25, 0, 0)`).

---

### Módulo 4: Ocultação Dinâmica do Dragão Direito (`Menu04.tga`)
- **Hook Cirúrgico:** Interceptação em `0x004BD332` no loop de renderização do dragão ornamental da interface direita (`X = 532.0f`).
- **Comportamento Dinâmico:** O dragão direito é ocultado automaticamente sempre que qualquer janela do lado direito estiver aberta (Janela "C", Inventário "V", Guild "G", Party "P", Baú, Trade, Chaos Machine ou Event Time), prevenindo sobreposição visual sobre o botão "TC" e as janelas ativas.
- **Preservação do Dragão Esquerdo:** O dragão ornamental do lado esquerdo (`0x004BD300`) permanece 100% ativo e intocado em todas as circunstâncias.

---

### Módulo 5: Protocolo de Rede & Comunicação Client/Server
- **Estrutura de Pacotes Alinhada (5 Bytes):**
  - `PMSG_CUSTOM_CHANGE_CLASS_REQ` (Client -> GS | `0xF3:0xE5`): Envia a classe desejada (`TargetClass`).
  - `PMSG_CUSTOM_CHANGE_CLASS_ANS` (GS -> Client | `0xF3:0xE5`): Retorna o resultado da operação (`Result`: 0 = Sucesso, 1 = Falha).
- **Handshake Seguro:**
  - O botão **CONFIRMAR** possui trava de segurança anti-spam (`m_LastConfirmTime` com intervalo mínimo de 3 segundos).
  - A janela da interface no Client **só é fechada após o recebimento da confirmação de sucesso** (`Result == 0`) enviada pelo GameServer. Se houver falha de validação, a interface permanece aberta para correção.

---

### Módulo 6: Validações de Segurança & Regras de Negócio (GameServer)
- **Requisitos de Troca:**
  - Verificação de conta VIP ativa e saldo de moedas/zen configurados.
  - Inventário desequipado obrigatório (slots `0 a 11`), impedindo perda ou incompatibilidade de itens equipados.
- **Bloqueios de Proteção:**
  - Bloqueio imediato para personagens assassinos / PK (`PKLevel > 3`).
  - Bloqueio em mapas de eventos (**Blood Castle** e **Devil Square**) e durante **Duelos ativos** (`DuelUser != -1`).
  - Bloqueio durante transações ativas (Trade aberto, Baú aberto ou Loja Pessoal ativa).
- **Expurgo Obrigatório de Buffs:** Execução imediata de `gEffectManager.ClearAllEffect(lpObj)` ao trocar de classe, eliminando todos os buffs da classe anterior (Greater Fortitude, Mana Shield, Buffs de Elfa) e prevenindo acúmulo indevido de vantagens.

---

### Módulo 7: Transição de Personagem, Recálculo de Atributos & Comandos
- **Devolução Matemática de Atributos:**
  - Cálculo exato dos pontos distribuídos com base em `DefaultClassInfo`.
  - Estorno total dos pontos para `LevelUpPoint` sem perda de pontos ou ocorrência de atributos negativos.
- **Limpeza de Habilidades:** Varredura no inventário de skills com expurgo automático de magias incompatíveis com a nova classe (`CheckSkillRequireClass`).
- **Conclusão de Quests:** Marcação automática das quests de 2ª classe da Sebina (Level 150) para BK, SM e ME (adequado estritamente à versão 97k, sem Marlon ou Combo).
- **Comandos de Chat Suportados:** Integração completa dos comandos `/class <bk|sm|me|mg>`, `/bk`, `/sm`, `/me` e `/mg` via `CommandManager.txt`, com suporte a mensagens multilíngues (IDs 760 a 770 em Português, Inglês e Espanhol).
