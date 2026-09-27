## Projeto Free CMZone Fixs 97K - Base SSeMU 97KOR
## Inicio: 17/08/2026
## Créditos Source: SSeMU SetecSoft Development  
## Créditos MuServer e Cliente: SSeMU SetecSoft Development
## IDE Antigravity: Organizar e Implementar as Correções.

UPDATE CMZ 09 (3.0.9) 08-09-26 / SOURCE 97K KAYITO CUSTOM 02 (UPDATE 21):
* Menu de Opcoes Avancadas In-Game (Update 21 Kayito): implementado menu interativo completo acionado na interface original de ESC do cliente. Abas incorporadas: Geral, Anti-Lag, Tela, Janela e Fontes. [Main.dll][OptionsMenu.h/.cpp][WeaponView.h/.cpp][Offset.h][Main.cpp][Client97K]
  - Mock Engine C++: implementado sistema de Mocks (CSoundMock, CGlobalTextMock, CFontMock, CHealthBarMock) para contornar limitacoes do Main.exe da 97k sem necessidade de injecao de classes inteiras originais.
  - Correcao e Insercao dos ponteiros originais faltantes (m_bAutoAttack, m_bWhisperSound, RenderLinkObject, CreateSprite, m_WindowMode).
  - Traducao nativa injetada em memoria via macro CGlobalTextMock e GetOptionText para evitar limitacao de index do arquivo Text.bmd.
* Expansao de Texturas e Modelos 3D (Zeus): realocacao de memoria grafica e remapeamento de ponteiros no Main.exe, eliminando crashes, texturas corrompidas e itens brancos ao adicionar itens custom. [Main.dll][LoadModels.h/.cpp][Client97K]

UPDATE CMZ 00 (3.0.0) / Correções CMZone 29-08-26
* Foi corrigido o crash crítico do cliente ao aprender ou usar Orbs e Scrolls de habilidades no inventário. [97KOR]
* Foi corrigida a leitura e codificação das habilidades enviadas pelo servidor ao personagem. [97KOR]
* Foi ajustada a atualização em tempo real da lista de magias aprendidas e removidas no jogo. [97KOR]
* Foi removida a exclusão incorreta de slots no inventário em caso de falha nos requisitos da habilidade. [97KOR]
* Foi corrigida a movimentação de itens com o botão direito no inventário, permitindo equipar e substituir equipamentos mesmo com os slots já ocupados. [97KOR]
* Foi corrigida a transferência automática de itens pelo botão direito para o Baú, Trade e Chaos Machine. [97KOR]
* Foi otimizado o desempenho gráfico do jogo, reduzindo o consumo indevido de CPU e eliminando a limpeza contínua de memória RAM. [97KOR]
* Foram adicionados temporizadores de alta precisão e maior prioridade de processo para eliminar micro-travamentos (stuttering). [97KOR]
* Foi corrigido o brilho e a transparência dos itens, restaurando o visual original da versão 97d (itens +11 a +13 com brilho sólido e itens +14 a +15 com corpo transparente). [97KOR]
* Foi corrigido o bug nos comandos /reset e /mreset em que o nível do personagem aumentava (+1) ao invés de voltar para o Level 1. [97KOR]
* Foi implementado o Sistema Lua de forma estável e otimizada (apenas funções da 97k), atualizado e rodando 100% sem erros e sem crashes. [GameServer][97KOR]
 
UPDATE CMZ 14 (3.1.4) 26-09-26:
* Exibicao de Resets e Tipo de Conta VIP na Janela de Status (Tecla C): implementada exibicao visual alinhada com o bloco de Point na janela de status do personagem ("C"). Exibe a contagem de Resets (Resets: %d) e o tipo de conta (Free, Vip, Vip Premium, Vip Events) com centralizacao dinamica por resolucao, tipografia em negrito (g_hFontBold), cor azul padronizada com Spare Points (Color4f: 100, 150, 255, 255), fundo translucido suave nativo (SetBackgroundTextColor = 0x80000000) e margens uniformes de 3px. Documentacao tecnica completa gerada em Custom_MarkDowns/Custom_Texto_Visual_na_Janela_de_Status_C.md. [Main.dll][PrintPlayer.h/.cpp][Client97K][Custom_MarkDowns]
* Sincronizacao de Nivel de Conta (ViewAccountLevel) entre GameServer e Client: inclusao do campo ViewAccountLevel nas estruturas binarias PMSG_CHARACTER_INFO_SEND/RECV (0xF3, 0x03) e PMSG_NEW_CHARACTER_INFO_SEND/RECV (0xF3, 0xE0), garantindo sincronizacao em tempo real do nivel VIP no login do personagem e em atualizacoes dinamicas via JoinServer. [GameServer][Main.dll][Protocol.h/.cpp][DSProtocol.cpp][JSProtocol.cpp][97KOR]
  
UPDATE CMZ 13 (3.1.3) 26-09-26 / SSeMU UPDATE 98 (2.5.7 ~ 2.5.7-1):
* Criptografia e Descriptografia de HardwareId: implementada rotina dinamica XOR (PacketArgumentEncrypt / Decrypt) para transmissao segura do HardwareId na conexao entre Client Main e GameServer, prevenindo forjamento de identidade de maquina. [GameServer][Main.dll][ConnectionManager.cpp][Protocol.cpp][97KOR]
* HardwareId Unico por Cliente: geracao de identificador unico de 44 caracteres combinando memoria fisica instalada (GlobalMemoryStatusEx) e parametros de arquitetura do processador, eliminando IDs duplicados entre clientes distintos. [Main.dll][Util.cpp][Client97K][97KOR]
* Prevencao de Congelamento de Chars Offline no Reconnect: verificacao previa de socket valido (lpObj->Socket != INVALID_SOCKET) antes de setar OBJECT_DELCMD em gObjMoveGate e gObjTeleport, evitando travamentos em contas desconectadas ou em /offattack. [GameServer][User.cpp][97KOR]
* Desconexao Limpa de Contas Reconectadas e Aviso 153: envio de notificacao 153 no JGAccountAlreadyConnectedRecv e encerramento limpo para sessoes conectadas e offline via AttackOfflineClose, solucionando o problema de jogadores presos no servidor. [GameServer][JSProtocol.cpp][Data\Message.txt][97KOR]
* Otimizacao e Ordem de Checagens no JoinServer: interrupcao antecipada de loops de contagem de conexoes por IP/HWID (AccountManager.cpp) e validacao de limites de conexoes somente apos conferir contas ja existentes, eliminando expulsoes e bloqueios indevidos. [JoinServer][AccountManager.cpp][JoinServerProtocol.cpp][97KOR]
* Otimizacao de Memoria no Cliente Main: desativacao de rotinas agressivas de purga de memoria RAM (ReduceConsumeProc) com leituras e escritas excessivas no disco, assegurando FPS estavel sem micro-engasgos. [Main.dll][Patchs.cpp][Client97K]
* Suporte a 5 Plugins no GetMainInfo: correcao do carregamento de plugins customizados (PluginName1 a PluginName5) e criacao automatica da estrutura de diretorios Data\Custom\MainInfo\. [GetMainInfo][GetMainInfo.cpp][Client97K]
* Sincronizacao da Estrutura CUSTOM_MAP_INFO: alinhamento do campo MusicPath[100] na struct entre GetMainInfo e Main.dll, eliminando a divergencia de 8.000 bytes e o erro [Error] ReadMainFile #3: Unexpected size for the main file. [Main.dll][GetMainInfo][CustomMap.h][ServerInfo.cmz][Client97K]
* Atualizacao de Versao de Titulo e Compilacao Release: macros GAMESERVER_VERSION e JOINSERVER_VERSION atualizadas para (Update 98 - Ver: 2.5.7) e flag /FS adicionada aos projetos para builds sem erro de PDB. [GameServer][JoinServer][stdafx.h][GameServer.vcxproj][JoinServer.vcxproj][Main.vcxproj][97KOR]
 
UPDATE CMZ 12 (3.1.2) 22-09-26 / SSeMU UPDATE 97 (2.5.5 / 2.5.6):
* Configuracao de Experiencia em Attack (/attack e /offattack): adicionado suporte completo as taxas CustomAttackExperienceRate_AL0~3 e CustomAttackOfflineExperienceRate_AL0~3 em GameServer\DATA\GameServerInfo - Custom.dat com aplicacao de multiplicador percentual de experiencia solo e em party. [GameServer][CustomAttack.h/.cpp][ObjectManager.cpp][GameServerInfo - Custom.dat][97KOR]
* Mensagens de Notificacao no Message.txt: suporte as mensagens 750 a 759 para alertas e notificacoes de comando de ataque e utilitarios. [GameServer][Data\Message.txt][97KOR]
* Novas Funcoes LUA e Contagem de Monstros: implementadas as funcoes de script GetItemName, GetMapName, GetMonsterName e a busca/contagem real de monstros em mapa ou retangulo de coordenadas via MonsterCount. [GameServer][LuaFunction.h/.cpp][97KOR]
* Encriptacao de Scripts LUA e Require: implementada funcao LuaRequire com suporte a leitura de arquivos criptografados com header SCRIPT_HEADER e chave XOR dinamica, com fallback transparente para scripts convencionais. [GameServer][LuaFunction.cpp][ScriptLoader.cpp][97KOR]
* Suporte a Musica Customizada em Mapas: adicionado campo MusicPath na struct CUSTOM_MAP_INFO no GetMainInfo e Main.dll, com geracao binaria em ServerInfo.sse e reproducao automatica de MP3 ao entrar em mapas custom. [Main.dll][GetMainInfo][CustomMap.h/.cpp][Map.cpp][Client97K]
* Correcao de Cores em Danos: corrigido bug de interferencia de atributos de dano duplo (0x40) e combo (0x80) que causavam alteracao incorreta nas cores visuais de dano Excellent, Critical e Ignore no cliente. [GameServer][User.h][Attack.cpp][Protocol.cpp][97KOR]

UPDATE CMZ 11 (3.1.1) 21-09-26 / SSeMU UPDATE 96 (2.5.4):
* Novo Arquivo e Sistema de Requisitos de Comandos (CommandRequirement): criado Data\CommandRequirement.txt e modulo CommandRequirement.h/.cpp adaptado para 4 classes 97k (DW, DK, FE, MG), suportando requisitos de nivel, resets, itens com contagem e consumo no inventario, mapas, horarios, dias da semana e mensagens personalizadas de falha. [GameServer][CommandRequirement.h/.cpp][CommandManager.cpp][ServerInfo.cpp][CommandRequirement.txt][97KOR]
* Nova Funcao LUA MessageGlobalSend: implementada comunicacao global inter-servidor (GDGlobalMessageSend / DGGlobalMessageRecv) via DataServer com opcode 0x2B (SDHP_GLOBAL_MESSAGE_SEND / RECV) e broadcast para todos os GameServers conectados. [GameServer][DataServer][LuaFunction.h/.cpp][DSProtocol.h/.cpp][DataServerProtocol.h/.cpp][97KOR]
* Correcao de Monstros Estaticos: corrigido bug no gObjMonsterReactionProc que mantinha PathStartEnd ativo mesmo com PathCount == 0, impedindo novas rotas de movimento. [GameServer][Monster.cpp][97KOR]
* Correcao do Sistema de Noticias (Notice): suporte a leitura de 3 e 10 colunas sem descompasso de campos no MemScript, inicializacao segura de variaveis e envio de mensagem antes do incremento de indice para nao pular a primeira noticia. [GameServer][Notice.cpp][97KOR]
* Correcao de Resolucao do Cliente Main: corrigida a largura da resolucao modo 4 de 1360 para 1366 pixels (1366x768). [Main.dll][Resolution.cpp][Client97K]

UPDATE CMZ 10 (3.1.0) 20-09-26 / SSeMU UPDATE 95 (2.5.3):
* Correcao Critica no DataServer (BadSyntax): corrigida a inversao logica em CheckTextSyntax que bloqueava o carregamento de personagens normais e travava o cliente na tela de Loading ao clicar para entrar no jogo. [DataServer][Util.cpp][BadSyntax.txt][97KOR]
* Sistema de Filtragem de Nomes Improprios (BadSyntax): implementado modulo BadSyntax.h/.cpp no DataServer carregando DataServer\BadSyntax.txt para bloquear criacao de chars, guilds e rename com termos proibidos. [DataServer][BadSyntax.h/.cpp][Util.cpp][DataServer.cpp][97KOR]
* Metodo Lua BridgeFunction_OnUserMove (ScriptCore.lua): adicionadas chamadas gScriptLoader.OnUserMove em gObjMoveGate, gObjTeleport e gObjSummonAlly para integracao com scripts de movimentacao. [GameServer][User.cpp][97KOR]
* Taxas Divididas PvM e PvP em Habilidades (SkillDamage): suporte a colunas DamageRatePvM e DamageRatePvP em SkillDamage.txt com calculo especifico em GetDamageRate por tipo de alvo (lpTarget->Type). [GameServer][SkillDamage.h/.cpp][Attack.cpp][97KOR]
* Etapa 4 do Devil Square: estagios reescalonados para 80%, 60%, 40% e 20% restantes com adicao de SetStage3 acionando StageSpawn(lpLevel, 4). [GameServer][DevilSquare.cpp][97KOR]
* Redesenho do HitManager de Monstros & MonsterGetTopHitDamageUserMaxTime: sistema temporal baseado em time(0) consistente em segundos, reinicio de dano apos expiracao (30s) e limpeza total de slots de dano em DelHitDamageUser. [GameServer][Monster.cpp][ServerInfo.h/.cpp][GameServerInfo - Common.dat][97KOR]
* Correcao de Congelamento e Crash em Titulo de Janela Lua (SetObjectWindowTitle): protecao contra desconexoes, strings nulas e formatacao com GCWindowsNameSend variadico seguro (%s) e pacote 0xF3, 0xE8. [GameServer][LuaFunction.cpp][Protocol.h/.cpp][97KOR]
* Correcao de Crash em Armadilhas e Filtro de Invisibilidade: protecao contra estouro de array (IndexCount >= MAX_VIEWPORT) e verificacao de EFFECT_INVISIBILITY em gObjTrapFindTarget e gObjMonsterFindTarget. [GameServer][Monster.cpp][97KOR]


UPDATE CMZ 09 (3.0.9) 20-09-26 / UPDATE 94 (2.5.2):
* Mensagens de Sistema (Data\Message.txt): adicionadas mensagens 740 a 744 (Guerra de Cerco e limites de mistura da Chaos Machine) com textos 100% ASCII. [GameServer][Message.txt][97KOR]
* Switch de Balanceamento Lua (GameServerInfo - Character.dat): adicionada a configuracao CalcLuaScriptSwitch para alternar entre calculos de atributos nativos em C++ (0) e calculos dinamicos em Lua (1). [GameServer][ServerInfo.h/.cpp][97KOR]
* Balanceamento Dinamico via Script Lua (CalcCharacter.lua): criado script Data\Script\Character\CalcCharacter.lua balanceado para classes DW, DK, FE e MG, controlando dano fisico/magico, velocidade de ataque, taxas de acerto, defesa e bonus de armadura +10 a +15. [GameServer][ObjectManager.cpp][CalcCharacter.lua][97KOR]
* Registro de Bindings em Lua (LuaFunction.cpp): registradas no ambiente Lua as funcoes GET_ITEM, GetObjectClass, GetObjectTotalLevel, GetObjectTotalStrength, GetObjectTotalDexterity, GetObjectTotalVitality, GetObjectTotalEnergy, GetObjectTotalLeadership e GetObjectDefense. [GameServer][LuaFunction.cpp][97KOR]
* Correcao do Buff de Invisibilidade (EFFECT_INVISIBILITY): corrigida a remocao automatica da invisibilidade ao desferir ataques corpo-a-corpo/distancia (Attack.cpp) ou executar habilidades (SkillManager.cpp) para jogadores comuns, preservando o modo invisivel para Game Masters. [GameServer][Attack.cpp][SkillManager.cpp][97KOR]


UPDATE CMZ 08 (3.0.8) 14-09-26 / UPDATE 93 (2.5.1):
* Comando /buyvip (CustomBuyVip): implementado sistema de compra de pacotes VIP via comando in-game com validacao de moedas/zen, tempo de duracao configuravel e recarga dinamica de status. [GameServer][CustomBuyVip.h/.cpp][CommandManager.cpp][97KOR]
* Comando /gift e Tabela [GiftData] (CustomGift): implementado resgate de codigos promocionais com persistencia no banco de dados via pacote 0x03 entre GameServer e DataServer. [GameServer][DataServer][CustomGift.h/.cpp][CommandManager.cpp][97KOR]
* Evento NPC Luke Helper (CustomLukeHelper): implementado evento com spawn dinamico em mapas/coordenadas configuraveis, contagem regressiva, distribuicao de buffs ou drop de itens via bag especial Luke Helper.txt. [GameServer][CustomLukeHelper.h/.cpp][NpcTalk.cpp][User.cpp][97KOR]
* Evento de Chuva de Drops (CustomEventDrop): implementado evento massivo de queda de itens e caixas especiais em zonas configuraveis por mapa e horarios com avisos globais. [GameServer][CustomEventDrop.h/.cpp][User.cpp][97KOR]
* Filtro de Barra de Vida de Monstros (CustomHealthBar): adicionado filtro de exibicao de HP Bar acima dos monstros por mapa e classe de monstro em GCNewHealthBarSend. [GameServer][CustomHealthBar.h/.cpp][Protocol.cpp][97KOR]
* Sistema de Caça de Monstros e Metas (CustomMonsterKillCount): implementado sistema de missoes de abates de monstros com persistencia em [MonsterKillCount] no DataServer (pacote 0x50), recompensas em zen/itens e mensagens personalizadas. [GameServer][DataServer][CustomMonsterKillCount.h/.cpp][Monster.cpp][97KOR]
* Presentes Iniciais para Novos Personagens (DefaultClassFreebies): concessao automatica de pacotes de itens, zen, pontos e buffs temporarios para personagens recem-criados (IsNewChar == 1) por classe. [GameServer][DataServer][DefaultClassFreebies.h/.cpp][DSProtocol.cpp][97KOR]
* CustomPick Aprimorado: suporte a filtros por AccountLevel (VIP) e novos switches de coleta seletiva (zen, joias, exc, ancient). [GameServer][CustomPick.h/.cpp][97KOR]
* Banco de Dados (UPDATE 2.5.1.sql): gerado script SQL para criacao das tabelas GiftData, MonsterKillCount e inclusao da coluna IsNewChar na tabela Character. [DataServer][MuServer97K]


UPDATE CMZ 07 (3.0.7) 13-09-26 / UPDATE 92 (2.5.0):
* Refatoracao de ItemBags: suporte a m_MaxItemDropCount < 0 (-1 para dropar todos os itens configurados, <-1 para sorteio 1..abs), correcao de inicializacao de SetOption em Convert, clamping de Option3 com m_MaxItemOption e protecao de DropZen contra drops de zen zerados. [GameServer][ItemBag.cpp][97KOR]
* Controle de Buffer de Rede: protecao em CSocketManager::DataRecv com buffers locais por thread, validacao rigorosa de tamanho minimo de cabecalho (C1/C3 >= 3, C2/C4 >= 4), limites maximos de pacote (MAX_MAIN_PACKET_SIZE) e reorganizacao com memmove unico seguro pos-laco. [GameServer][SocketManager.cpp][97KOR]
* AutoAttack (Suporte de Elfa): desacoplamento de Heal em relacao aos buffs continuos (Greater Defense / Greater Damage), ativando cura apenas quando HP <= 50% para evitar travamento do ciclo de auxilio quando o personagem esta com vida cheia. [GameServer][CustomAttack.cpp][97KOR]
* Console do GameServer (Menus Dinamicos): novo menu Drop Event (0 a 19), expansao de Invasoes (para ate 30) e rotina EditMenuLabel para atualizacao dinamica de titulos e status de menus de acordo com os arquivos de configuracao. [GameServer][Resource.h][GameServer.h/.cpp][DropEvent.h/.cpp][BonusManager.cpp][InvasionManager.h/.cpp][97KOR]
* Reconexao Offline: preservacao de credenciais de senha em RECONNECT_INFO e restauracao de conta offline apos reconexao com ResumeOffline fora da SafeZone. [GameServer][Reconnect.h/.cpp][97KOR]
* Controle de Buffer de Rede e Desbordes [ALL VERSIONS]: implementada protecao rigorosa em CSocketManager::DataRecv com buffers locais isolados por thread, validacao de tamanho minimo por tipo de cabecalho (C1/C3 >= 3, C2/C4 >= 4), truncamento seguro contra estouro de limites (MAX_MAIN_PACKET_SIZE) e reorganizacao de memoria com memmove unico seguro ao final do loop, blindando o servidor contra ataques de fragmentacao e travamentos por inundacao de pacotes. [GameServer][SocketManager.cpp][97KOR]
* Menu de EventDrop no Console do GameServer [ALL VERSIONS]: adicionados 20 menus de comando direto (gDropEvent.ForceStart de 0 a 19) e gerenciamento dinamico via rotina EditMenuLabel para monitoramento de estado e acionamento de eventos de drop personalizados em tempo real sem necessidade de reiniciar o servidor. [GameServer][GameServer.cpp][DropEvent.cpp][Resource.h][97KOR]
* Refinamento de Instancias e Reconexao Offline [ALL VERSIONS]: conclusao da etapa de estabilizacao do sistema de conexao persistente, garantindo restauracao e saida automatica da safe zone (ResumeOffline) para personagens que reconectam mantendo ataque automatico ativo. [GameServer][Reconnect.cpp][97KOR]


UPDATE CMZ 06 (3.0.6) 06-09-26 / UPDATE 91 (2.4.9):
* Sistema de Anti-Hack de Movimento e Velocidade (HackMoveSpeedCheck): reestruturada a classe para modelo singleton stateless com rastreamento de MoveTime, LastX e LastY por jogador no OBJECTSTRUCT; adicionada tolerancia de distancia extra (+2) para jogadores montados em Uniria ou Dinorant; e integrados logs de auditoria detalhados ([HackMoveCheck]) no console e arquivo de log. [GameServer][HackMoveSpeedCheck.h/.cpp][User.h/.cpp][DSProtocol.cpp][ObjectManager.cpp][97KOR]
* Sistema de Anti-Hack de Skills (HackSkillSpeedCheck): adicionado log de auditoria [HackSkillCheck] com registro de conta, nome, mapa e tempos de animacao para analise de velocidade de ataque anormal. [GameServer][HackSkillSpeedCheck.cpp][97KOR]
* Correcoes no Sistema de Drops e Fallback de Itens Excelentes: adicionado fallback seguro para o fluxo padrao de drop quando a busca por DropIndex falha em ExcItemDrop (DropIndex < 0), prevenindo monstros que morrem sem soltar itens configurados; corrigido nivel aleatorio no drop de Summon Orb de Elf Soldier (ItemIndex 12,11) com GetLargeRand()%6; e adicionada protecao contra divisao por zero no calculo de taxa de drop em ItemDrop. [GameServer][Monster.cpp][ItemDrop.cpp][97KOR]
* Spawns de Monstros em Mapas e Eventos (MonsterSetBase): adicionado suporte completo para a secao 3 (Section 3 - Spawns em area com coordenadas TX/TY e quantidade count) permitindo spawns dinamicos em eventos como Blood Castle; e adicionada validacao de mapa com gMapManager.IsValidMap no carregamento de monstros. [GameServer][MonsterSetBase.cpp][97KOR]
* Estabilidade e Prevencao de Deadlock em Monstros [ALL VERSIONS]: inserida verificacao previa obrigatoria com gMapManager.IsValidMap(map) no metodo gObjAddMonster, liberando imediatamente a secao critica (critical.unlock()) antes de retornar falha; eliminados congelamentos e deadlocks esporadicos do executavel GameServer decorrentes de tentativas de criacao de monstros em mapas nao carregados ou invalidos. [GameServer][Monster.cpp][CriticalSection.h][97KOR]
* Gestao de Recursos e Instancias [ALL VERSIONS]: reconfigurado e padronizado o target de compilacao na solucao oficial 0.97K KOR (v145 Win32), otimizando a alocacao de estruturas de monstros e usuarios e reduzindo o consumo de CPU em segundo plano. [GameServer][GameServer.vcxproj][97KOR]
* Correcao de Spawns em Mapas de Eventos [ALL VERSIONS]: implementado suporte completo para a Section 3 em MonsterSetBase.txt (spawns circulares e em area com coordenadas TX/TY), permitindo a inclusao dinamica de monstros adicionais em mapas restritos e eventos. [GameServer][MonsterSetBase.cpp][97KOR]


UPDATE CMZ 05 (3.0.5) 06-09-26 / UPDATE 90 (2.4.8):
* DataServer - Correcao na Geracao de Seriais: adicionada checagem e insercao automatica do registro GameServerInfo com Number = 0 no procedimento GDServerInfoRecv para garantir a inicializacao e geracao correta de seriais (WZ_GetItemSerial) em bancos de dados recem-criados. [DataServer][DataServerProtocol.cpp][97KOR]
* Sistema de Reconexao (Fase 2 - Devil Square & Blood Castle): implementado suporte a reconexao em eventos locais (Devil Square e Blood Castle) com rastreamento de EventFlag e EventLevel no OBJECTSTRUCT, salvamento de Account/IpAddr/HardwareId no mapa local de reconexao, e restauracao automatica de posicao e estado (ResumeLocalEvent) com validacao de tempo limite (ReconnectEventMaxTime) e vagas disponiveis. [GameServer][Reconnect.h/.cpp][User.h/.cpp][DevilSquare.cpp][BloodCastle.cpp][ServerInfo.h/.cpp][97KOR]
* Sistema de Reconexao (Opcoes Configuraveis): adicionadas chaves de controle em GameServerInfo - Common.dat (ReconnectAttackSwitch, ReconnectCommandSwitch, ReconnectEventSwitch, ReconnectEventMaxTime, ReconnectPartySwitch, ReconnectPickSwitch, ReconnectOfflineSwitch) permitindo ligar e desligar individualmente o retorno de ataque automatico, comandos ativos, eventos e party. [GameServer][ServerInfo.h/.cpp][Reconnect.cpp][97KOR]
* Seguranca de Sintaxe de Nomes (CheckNameSyntax): adicionada rotina CheckNameSyntax que rejeita caracteres invalidos e tentativas de injecao (espacos, aspas, barras, porcentagem e underline) na criacao de personagens (CGCharacterCreateRecv), criacao de guilds (CGGuildCreateRecv) e no comando /rename. [GameServer][Util.h/.cpp][Guild.cpp][Protocol.cpp][CommandManager.cpp][97KOR]
* Correcao de Posicionamento e Barreira (CGPositionRecv): adicionada verificacao de atributos de terreno bloqueado e barreiras (atributos 4 e 8) no pacote de envio de posicao pelo cliente, impedindo teleporte ou caminhada para coordenadas intransponiveis. [GameServer][Protocol.cpp][97KOR]
* Seguranca e Protecoes no Pacote 0xA7 (CGPetItemInfoRecv): implementada protecao completa e validacao de limites (INVENTORY_RANGE, WAREHOUSE_RANGE, TRADE_RANGE, CHAOS_BOX_RANGE) e tipo de pet no pacote 0xA7, alem do envio correto em GCPetItemInfoSend (0xA9), prevenindo crashes do GameServer e manipulacao indevida de memoria. [GameServer][Protocol.h/.cpp][97KOR]
* Correcoes Criticas de Crash no GameServer [ALL VERSIONS]: corrigidas as duas principais vulnerabilidades de rede que causavam a queda imediata do executavel GameServer: protecao completa e validacao de limites estritos de slots (INVENTORY_RANGE, WAREHOUSE_RANGE, TRADE_RANGE, CHAOS_BOX_RANGE) no pacote 0xA7 (CGPetItemInfoRecv) prevenindo violacoes de acesso a memoria; e verificacao de limites e barreiras de terreno (atributos 4 e 8) no pacote CGPositionRecv, bloqueando coordenadas invalidas manipuladas pelo cliente. [GameServer][Protocol.cpp][User.cpp][97KOR]
* Integridade de Dados no Reconnect [ALL VERSIONS]: corrigida a restauracao de dados do personagem ao reconectar, preservando status de ataque automatico, comandos em andamento e dados de autenticacao de contas offline sem corrupcao de atributos. [GameServer][Reconnect.cpp][User.cpp][97KOR]
* Seguranca e Verificacao de Integridade do Cliente [ALL VERSIONS]: blindagem da comunicacao com checagem de integridade e CRC de Main.dll e validacao estrita na rotina CheckNameSyntax impedindo injecoes de caracteres maliciosos. [Main.dll][Protect.cpp][GameServer][Util.cpp][97KOR]


UPDATE CMZ 04 (3.0.4) 05-09-26 / UPDATE 89 (2.4.7 & 2.4.7-1):
* Guild War & Battle Soccer: implementadas variaveis de pontuacao configuraveis em GameServerInfo - Common.dat (GuildWarSwitch, GuildWarScoreMax1, GuildWarScoreMax2, GuildWarKillScore1/2/3, GuildWarWinnerScore1/2/3) e correcao no calculo de pontuacao de vitoria em gObjGuildWarProc e gols no Battle Soccer com m_GuildWarKillScore3. [GameServer][Guild.cpp][BattleSoccer.cpp][ServerInfo.h/.cpp][97KOR]
* Comandos /reset e /mreset: corrigida subtracao de resets e niveis quando configurados como -1 (StartLevel/StartReset) com clamping de valores; restauracao correta de magias nativas (Energy Ball, Force, Fire Burst) e magias de armas equipadas via AddSkill e AddSkillWeapon ao limpar skills; e adicionado gate move para Dark Lord (Gate 17). [GameServer][CommandManager.cpp][97KOR]
* Filtro de Palavras e Nomes (Filter): reconstruido o subsistema de sanitizacao (Data\Util\Filter.txt) com std::set<std::string>, normalizacao em lowercase e mascaramento dinamico no texto original com asteriscos (*) em CheckSyntax sem descarte indevido de pacotes de chat geral ou whisper. [GameServer][Filter.h/.cpp][Protocol.cpp][DSProtocol.cpp][97KOR]
* Customizacao do Comando /post: adicionada a opcao CommandPostMessage em GameServerInfo - Command.dat permitindo customizar o indice da mensagem no Message.txt (padrao 323). [GameServer][CommandManager.cpp][ServerInfo.h/.cpp][97KOR]
* Sistema de Reconexao (Fase 1 - DataServer & ReconnectTime): implementada persistencia de reconexao no banco de dados via tabela ReconnectData (UPDATE 2.4.7-1 - ReconnectData.sql), sincronizacao bidirecional entre GameServer e DataServer via pacotes 0xC0 (insercao, delecao e envio em lote no startup do GS) e unificacao do temporizador ReconnectTime. [GameServer][DataServer][Reconnect.h/.cpp][ServerInfo.h/.cpp][ObjectManager.cpp][DSProtocol.cpp][DataServerProtocol.cpp][97KOR]
* Envio de Dados GameServer para Main [ALL VERSIONS]: alinhamento e sincronizacao da transmissao de pacotes multiplexados sob o cabecalho 0xF3 (ItemStack, ItemValue, WindowTime, HealthBar) garantindo decodificacao correta de estruturas PSWMSG_HEAD e compatibilidade perfeita entre servidor e cliente. [GameServer][Main.dll][Protocol.cpp][ItemStack.cpp][ItemValue.cpp][97KOR]
* Remocao e Unificacao de PartyReconnectTime [ALL VERSIONS]: eliminado o parametro legado e redundante PartyReconnectTime em arquivos de dados e fontes, unificando toda a temporizacao do sistema de reconexao na chave central ReconnectTime em Common.dat. [GameServer][ServerInfo.cpp/.h][Reconnect.cpp][97KOR]
* Aprimoramento do Sistema de Daily Reward [ALL VERSIONS]: adicionadas validacoes rigorosas de data e calendario (GetLocalTime com ano/mes/dia) no disparo de GDDailyRewardCheckSend e checagem de autenticidade da conta em DGDailyRewardCheckRecv, assegurando a entrega precisa de recompensas diarias por jogador. [GameServer][CustomDailyReward.cpp][97KOR]


UPDATE CMZ 03 (3.0.3) 05-09-26 / UPDATE 88 (2.4.6):
* Sistema de Recompensas Diarias: implementado sistema completo no GameServer e DataServer com configuracao em Data\Custom\CustomDailyReward.txt e persistencia em DailyRewardData (UPDATE 2.4.6 - Daily Reward.sql). [GameServer][DataServer][CustomDailyReward.h/.cpp][DSProtocol.cpp][DataServerProtocol.cpp][97KOR]
* Drops Personalizados (SpecialBag): adicionado suporte a SpecialBag na configuracao Data\Custom\CustomMonster.txt para drops personalizados por monstro e mapa via DropItemBySpecialValue, suprimindo o drop comum do monstro quando ativo. [GameServer][CustomMonster.h/.cpp][Monster.cpp][User.h/.cpp][97KOR]
* Anti-Dupe e Seguranca de Interfaces: reestruturado o fechamento concorrente de janelas (Trade, Bau, Chaos Box) e corrigido o fechamento da maquina chaos via CGNpcTalkCloseRecv (0x31), impedindo exploits de desincronizacao de itens e comite indevido. [GameServer][NpcTalk.cpp][ChaosBox.cpp][ItemManager.cpp][97KOR]
* Correcao de Distancia em NPCs: corrigida a validacao de distancia de dialogo com NPCs em CGNpcTalkRecv para comparar a distancia real entre o jogador e o NPC em vez do jogador consigo mesmo. [GameServer][NpcTalk.cpp][97KOR]
* Calculos em MonsterList: adicionada macro GET_MAX_VALUE e casting ULONGLONG na configuracao de monstros e multiplicadores de servidor, prevenindo overflows e truncamentos em vida, defesa, dano e taxas de ataque/defesa. [GameServer][MonsterManager.h/.cpp][CustomMonster.cpp][97KOR]
* Bridge LUA (ItemGive): corrigida a funcao ItemGive no script LUA com validacao previa de espaco livre no inventario e balanceamento estrito da stack de retorno com booleanos. [GameServer][LuaFunction.cpp][97KOR]
* Logs de Auditoria e Controle: incluidos novos logs no console do GameServer informando total de itens e monstros carregados, alem de deteccao de itens invalidos em Bags de eventos. [GameServer][ItemBag.cpp][ItemManager.cpp][MonsterManager.cpp][97KOR]
* Correcao do NPC 255 (Phantom Soldier / Lumen the Barmaid / Garconete de Lorencia) [97]: reparado o bug historico da versao 97k que fazia a garconete do bar de Lorencia desaparecer instantaneamente ao carregar o mapa; inserido patch de salto incondicional em 0x004689AB do Main.exe (jmp 0x0046B26B) eliminando a checagem incorreta de ponteiro de dono de pet inexistente (que executava mov dword ptr [esi+0x60], 0xFFFFFFFF); alinhada a chamada interna da rotina sub_45CCF0 em CreateMonster com passagem dos 5 argumentos corretos sem corrupcao de pilha; e adicionado o manipulador case 255 em CNpcTalk::NpcTalk. [Main.dll][Patchs.cpp][Monster.cpp][GameServer][NpcTalk.cpp][Client97K][97KOR]
* Reestruturacao Interna de Janelas e Anti-Dupe [ALL VERSIONS]: inseridas travas rigidas checando lpObj->Interface.use em NpcTrainer, NpcChaosGoblin e NpcWarehouse para impedir abertura concorrente de baus, chaos machine e treinadores enquanto outra janela de transacao estiver ativa; adicionadas checagens de gServerInfo.m_TradeSwitch e verificacao de estado de duelo (lpObj->DuelUser) em CGTradeRequestRecv e CGTradeResponseRecv para bloquear solicitacao e confirmacao de trocas durante partidas de duelo. [GameServer][Trade.cpp][NpcTalk.cpp][97KOR]
* Otimizacao do DataServer [ALL VERSIONS]: remocao de dependencias residuais e otimizacao nas rotinas de comunicacao para criacao e salvamento de novos itens no banco de dados. [DataServer][97KOR]
* Desbloqueio para 30 FPS (31.25 FPS): ajuste do limitador de frame de 40ms para 32ms, entregando +25% de fluidez sem descompasso fisico ou de animacoes. [Main.dll][Patchs.h/.cpp][Client97K]
* Reducao de Consumo de CPU: suspensao hibrida inteligente (Sleep de precisao 1ms), reduzindo o uso de CPU de 100% de um nucleo para menos de 2%. [Main.dll][Main.cpp][Patchs.cpp][Client97K]
* Monitor de FPS em Tempo Real: medicao continua e exibicao dos FPS atualizados no titulo da janela do jogo. [Main.dll][Offset.h][Patchs.cpp][Client97K]
* Configuracao no Config.ini: adicionada chave IncreaseFPSSwitch permitindo alternar entre 25 FPS e 30 FPS. [Main.dll][Config.ini][Client97K]


UPDATE CMZ 02 (3.0.2) 05-09-26 / UPDATE 87 (2.4.5):
* Foi corrigida a duplicacao de itens na Chaos Machine e NPC Trainer via validacao de Interface, transacao ativa e checagens no ItemManager. [GameServer][ChaosBox.cpp][NpcTalk.cpp][ItemManager.cpp][97KOR]
* Foi corrigida a edicao indevida de pontos e atributos do personagem adicionando validacao de montante positivo em CharacterLevelUpPointAdd e bloqueios de estado e interface em /readd (/redistribute). [GameServer][ObjectManager.cpp][CommandManager.cpp][97KOR]
* Foi otimizado o QueryManager no DataServer com correcao de operadores em loops de conversao binaria, eliminacao de leituras de buffers estaticos desnecessarios e melhoria na extracao de strings e binarios. [DataServer][QueryManager.cpp][97KOR]
* Foi implementado o sistema inteligente de serializacao sob demanda: itens dropados no chao recebem serial somente ao serem recolhidos, reduzindo drasticamente queries SQL repetitivas e overhead do banco de dados. [GameServer][DataServer][DSProtocol.cpp][ItemManager.cpp][MapItem.cpp][DataServerProtocol.cpp][97KOR]
* Foi corrigida a perda e calculo incorreto de Zen ao morrer com penalidade, evitando overflow aritmetico em 32-bit e descontos invalidos. [GameServer][User.cpp][97KOR]
* Foi corrigido o comando /rename no DataServer com passagem e ligacao correta de parametros SQL (WZ_RenameCharacter) e eliminada a dependencia depreciada de BadSyntax.txt no DataServer. [DataServer][CommandManager.cpp][DataServerProtocol.cpp][DataServer.cpp][97KOR]
* Remocao Definitiva de BadSyntax no DataServer [ALL VERSIONS]: eliminados os arquivos fisicos obsoletos (BadSyntax.h e BadSyntax.cpp), removidos includes em DataServer.cpp, DataServerProtocol.cpp e CommandManager.cpp, e limpas as configuracoes do projeto DataServer.vcxproj e filtros, consolidando toda a higienizacao de palavras e nomes proibidos de forma unificada no subsistema Filter.txt e rotinas de seguranca do GameServer. [DataServer][DataServer.cpp][DataServerProtocol.cpp][CommandManager.cpp][DataServer.vcxproj][97KOR]
* Anti-Dupe e Integridade de Transacoes [ALL VERSIONS]: aprimoradas as checagens de transacao ativa (lpObj->Transaction) e bloqueio de envio indevido de pacotes de itens com janelas de NPCs abertas na Chaos Machine e NPC Trainer. [GameServer][ChaosBox.cpp][NpcTalk.cpp][97KOR]
* Suporte e Ferramenta ItemDupeTool [ALL VERSIONS]: atualizacao dos procedimentos de varredura e exclusao de itens clonados compativeis com o formato de seriais de 32 bits da 97k. [DataServer][Tools][97KOR]


UPDATE CMZ 01 (3.0.1) 04-09-26 / UPDATE 86 (2.4.4 & 2.4.4-1):
* Foi corrigido o sistema de cliques simultaneos do mouse que causava disparos e ataques descontrolados. [Main.dll][97KOR]
* Foram implementadas as opcoes HaveSerial e HaveOption em Item.txt e GameServer: [GameServer][97KOR]
	- HaveSerial: 0 dropa o item sem gerar serial, 1 processa a serializacao pelo DataServer.
	- HaveOption: 0 impede que o item possua ou gere opcoes excelentes.
* Foi aprimorada a leitura do MemScript para suportar arquivos .TXT com codificacao UTF-8 BOM e numeros decimais com ponto flutuante. [GameServer][JoinServer][GetMainInfo][97KOR]
* Foi corrigido o erro com o Dinorant em Icarus, garantindo o deslocamento correto para a safe zone (Gate 22) tanto ao morrer quanto ao desequipar o item sem asas. [GameServer][Attack.cpp][User.cpp][97KOR]
* Foi corrigido o erro de Guild que exibia membros no servidor -1 ao trocar de servidor ou reconectar. [DataServer][GameServer][97KOR]
* Foi aprimorada a leitura e validacao de itens por LUA no inventario e mapa, prevenindo crashes por slots fora do range e corrigindo o balanceamento da pilha do interpretador. [GameServer][LuaFunction.cpp][97KOR]
* Foi corrigida a execucao de comandos por scripts LUA para evitar a queda indevida no bloco de comandos nativos em C++. [GameServer][CommandManager.cpp][97KOR]
* Foi atualizada a compatibilidade de compilacao do GameServer e DataServer para o Visual Studio 2022/2026 (Toolset v145 e bibliotecas ATL/MFC). [GameServer][DataServer][97KOR]
* Parsing de MemScript e Suporte a Numeros Decimais [ALL VERSIONS]: adicionado suporte a virgulas (,) como delimitador de tokens no loop de leitura de espacos brancos em GetToken(), viabilizando a leitura de arquivos .txt formatados com separacao por virgula; corrigida a posicao da terminacao nula da string (this->m_string[count] = 0) para antes da conversao numerica com (float)atof(), prevenindo leitura de memoria corrompida e falhas ao processar numeros decimais com ponto flutuante. [GameServer][DataServer][JoinServer][MemScript.cpp][97KOR]
* Seguranca em Comandos e Bridge LUA [ALL VERSIONS]: implementadas validacoes rigorosas de permissao e balanceamento de pilha nas chamadas de funcoes LUA via comandos, prevenindo execucao arbitraria de scripts por usuarios sem nivel de Game Master. [GameServer][LuaFunction.cpp][CommandManager.cpp][97KOR]
* Protecoes de Seguranca de Servidor [ALL VERSIONS]: correcao de duas falhas de seguranca de protocolo onde pacotes forjados com tamanhos ou estruturas invalidas provocavam a interrupcao e crash do processo do servidor. [GameServer][Protocol.cpp][97KOR]

## Titulo: Arquivos Vazados 2024 - Base SSeMU Update 85 2.4.3 97KOR
## Contato Oficial: https://www.ssemu.com.ar
## Créditos Arquivos: SetecSoft Development © SSeMU 2021
- Essa source foi postada em forums estava no Update 85 (2.4.3)
- Eu peguei essa source e continuei a partir daqui do update 85 2.4.3
- E assim demos inicio ao nosso projeto updgrade fixs e Customs 97k SSeMU

UPDATE 85 (2.4.3)
* Se repararon errores de CustomAttack relacionados a buffs o usar skills sin armas tecla F8. [ALL VERSIONS]
* Se repararon errores de ItemDrop y ItemGive en LUA que generaban crash. [ALL VERSIONS]
* Se reparo el sistema de Fruits que tenia fallos y mensajes erroneos. [97KOR][99CHS]
* Se reparo un bug que hacia caer el querymanager de los dataservers. [ALL VERSIONS]
* Se reparo el mensaje cuando no se permitia usar el pick en determinados mapas. [ALL VERSIONS]
* Se reparo el error que permitia meter items quest en personal store. [ALL VERSIONS]
* Se reparo un error en el comando rename que generaba problemas. [ALL VERSIONS]

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [97KOR][99CHS] (GuildInsertMaxUser1)
* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [99CHS] (GuildInsertMaxUser2)

CLIENTE:
* Se eliminaron opciones en "Tools\GetMainInfo\MainInfo.ini" (MaxAttackSpeed) [ALL VERSIONS]
* Se agrego nuevo archivo para configurar velocidad de ataque en "Tools\GetMainInfo\Data\SkillAttackSpeed.txt" [ALL VERSIONS]
* Se repararon problemas reportados con la ventana H de horarios. [S4][S6][S8]

UPDATE 84 (2.4.2)
* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (GuildWarAttackEnable)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (GuildWarAttackEnable)

* Se reparo un congelamiento random que se daba por un reloj duplicado. [ALL VERSIONS]
* Se reparo un error en Castle Siege que ocasionaba que ciertos miembros quedaran fuera. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]
* Se repararon los skills agregando el target inexistente default. [97KOR]
* Se repararon errores de CustomAttack relacionados a buffs o usar skills sin armas. [ALL VERSIONS]
* Se reparo el buff master en attack que generaba bucles interminables. [S4][S6][S8]
* Se reparo el buff master en offline helper que generaba bucles interminables. [S4][S6][S8]
* Se reparo el disconnect de los skills master de RF, DL y SM en Attack. [S4][S6][S8]
* Se reparo un error que no generaban algunos seed. [S4][S6]

UPDATE 83 (2.4.1 ~ 2.4.1-1):
*** ARCHIVOS MODIFICADOS ***
* Se modificó el archivo "Data\EventItemBagManager.txt", agregando los bags de Imperial Guardian. [S6ENG][S8ENG]
* Se modificó el archivo "Data\Script\System\ScriptCore.lua" para agregar el nuevo método "BridgeFunction_OnSQLAsyncResult" [ALL VERSIONS]
* Se agregaron los bags faltantes del evento Imperial Guardian en "Data\EventItemBag\Special\Imperial Guardian Stone Statue 1". [S6ENG][S8ENG]
* Se agregaron los bags faltantes del evento Imperial Guardian en "Data\EventItemBag\Special\Imperial Guardian Stone Statue 2". [S6ENG][S8ENG]
* Se agregaron los bags faltantes del evento Imperial Guardian en "Data\EventItemBag\Special\Imperial Guardian Stone Statue 3". [S6ENG][S8ENG]
* Se agregaron los bags faltantes del evento Imperial Guardian en "Data\EventItemBag\Special\Imperial Guardian Stone Statue 4". [S6ENG][S8ENG]
* Se agregaron los bags faltantes del evento Imperial Guardian en "Data\EventItemBag\Special\Imperial Guardian Stone Statue 5". [S6ENG][S8ENG]
* Se agregaron los bags faltantes del evento Imperial Guardian en "Data\EventItemBag\Special\Imperial Guardian Stone Statue 6". [S6ENG][S8ENG]
* Se agregaron mobs faltantes en "Data\Monster\Spawn\069 - Imperial Guardian 1.txt" correspondientes a Imperial Guardian. [S6ENG][S8ENG]
* Se agregaron mobs faltantes en "Data\Monster\Spawn\070 - Imperial Guardian 2.txt" correspondientes a Imperial Guardian. [S6ENG][S8ENG]
* Se agregaron mobs faltantes en "Data\Monster\Spawn\071 - Imperial Guardian 3.txt" correspondientes a Imperial Guardian. [S6ENG][S8ENG]
* Se agregaron mobs faltantes en "Data\Monster\Spawn\072 - Imperial Guardian 4.txt" correspondientes a Imperial Guardian. [S6ENG][S8ENG]

*** INFORMACIÓN GENERAL ***
* Se corrigió el problema de congelamiento por LUA. [ALL VERSIONS]
* Se corrigió el problema de que no se acumulaban los puntos del castle siege. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]
* Se corrigió el problema del mensaje 0 en los custom monsters. [ALL VERSIONS]
* Se corrigió el crash producido por CustomPick. [ALL VERSIONS]
* Se corrigió el crash producido en comandos con caracteres arabes y/o chinos. [ALL VERSIONS]
* Se corrigió el "OnShutScript" que no realizaba la funcion antes de descargarse. [ALL VERSIONS]
* Se corrigió el problema visual de las kundums. [S8ENG]
* Se modificó el bonus de party, siendo general para clases iguales y especial para clases diferentes. [ALL VERSIONS]
* Se restableció el sistema de detección de errores producidos por UTF-8 BOM. [ALL VERSIONS]
* Se agregaron los métodos SQLASync para ejecutar sentencias y obtener datos de forma asíncrona. [ALL VERSIONS]
* Se arreglo el bug que no se podia pegar a personas en duelo. [99CHS][S1KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR]

*** INFORMACIÓN SOBRE LUA ***
* Se actualizó el template con las nuevas funciones Lua en "Data\Script\Script\TemplateScript.lua" [ALL VERSIONS]
* Se actualizó la GUÍA de Lua en "Tools\Guides\Script Lua BridgeFunctions.rtf" [ALL VERSIONS]
* Se actualizó la GUÍA de Lua en "Tools\Guides\Script Lua Interface Functions.rtf" [ALL VERSIONS]

UPDATE 82 (2.4.0):
* Se reparo el crash random de los gameservers. [ALL VERSIONS]
* Se mejoro toda la seguridad del servidor interna Cliente~Servidor [ALL VERSIONS]
* Se reestructuro las conexiones cliente servidor. [ALL VERSIONS]
* Se reparo el error de clanes con mas de 9 letras ganadores del castle siege. [ALL VERSIONS]
* Se mejoro la seguridad de los mains. [ALL VERSIONS]
* Se reparo el error del Attack(F8) y su correspondiente delay. [ALL VERSIONS]
* Se reparo un error en el sistema de experiencia del Party. [ALL VERSIONS]
* Se reparo el editor con las variables. "Normal, Chat Blocked, Invisible, GameMaster y Administrador"
   - Game Master (Tiene su globito en la cabeza pero puede ser golpeado y es cazado por los mobs ademas de poder escribir usando el signo "!" en global)
   - Administrador (Tiene su globito en la cabeza, es inmune a todo tipo de ataque y ademas no es cazado por ningun mob. tambien puede user el signo "!" en global)
* Se repararon funciones del administrador y el gamemaster que no funcionaban. [ALL VERSIONS]
* Se reparo una forma de crashear el ConnectServer y/o GameServer utilizando una modificacion en un paquete. [ALL VERSIONS]
* Se reparo un crash al matarse con cierto buff en Arca Battle. [S8ENG]

* AVISO IMPORTANTE: Se debe actualizar tanto cliente (Main.dll) y servidor (Todos los ejecutables) para poder utilizar esta version y volver a generar el archivo "ServerInfo.sse" usando el nuevo GetMainInfo.

UPDATE 81 (2.3.9):
* Se reparo error de crash en mala configuracion de Bags. [ALL VERSIONS]
* Se reparo el Attack que se podia buguear 2 skills a la vez. [ALL VERSIONS]
* Se reparo el Summon de DL que podia summoner offlines. [ALL VERSIONS]
* Se reparo el error de clanes que de mas de 7 letras en castle siege. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]

* Se mejoro el sistema de gestion de conexiones. [ALL VERSIONS]
* Se reparo un error relacionado a Luke Helper que hacia fallar al mismo. [ALL VERSIONS]

* Se reorganizo gran parte del codigo. [ALL VERSIONS]

UPDATE 80 (2.3.8):
* Se reparo el error que sabiendo el nombre de la cuenta se podia desconectar a los offline. [ALL VERSIONS]
* Se reparo el error que si eras PK y superabas el dinero maximo no podias sacar tu estado PK. [ALL VERSIONS]
* Se reparo el error del Success Rate en versiones sin SD. [97KOR][99CHS][S0KOR]
* Se reparo el error del Miss en PvP. [97KOR][99CHS][S0KOR]

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackPotionRate1,CustomAttackPotionRate2)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackPotionRate1,CustomAttackPotionRate2)

* Se agregaron logs que muestran cuando derrumban puertas o rompen estatuas en Castle Siege. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]
* Se agregaron logs que muestran cuando es accesible la tower del castle siege. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]

* Se reescribio la deteccion de velocidad o HackSpeed en los ejecutables, dependiendo solamente del servidor. [ALL VERSIONS]

* Modificado el archivo "JoinServer\JoinServer.ini" se agrego opciones para limitar conexiones por IP o HardwareId. [ALL VERSIONS]

UPDATE 79 (2.3.7):
* Modificado el minimapa de servidor en tiempo real con opciones nuevas y mejores metodos de renderizado. [ALL VERSIONS]
* Reconstruido todo el Castle Siege y mejorados todos los sistemas. [ALL VERSIONS]
* Agregado el archivo "Data\Custom\CustomMonster.txt" para configurar parametros de mobs. [97KOR][99CHS]
* Modificado el archivo "Data\Event\CastleSiege.dat" nuevos valores acordes a version. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]
* Reparado el NPC Castle Guard que si lo tocabas sin clan se bugueaba. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]
* Reparado el NPC Castle Guard que no mostraba todos los clanes que van a participar en el Castle. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]

* SSeMU Tools actualizado para dar soporte a S4KOR y S6ENG.

UPDATE 78 (2.3.6):
* Arreglados problemas de reconnects aleatorios. [ALL VERSIONS]
* Arreglada la taxa de impuesto y extraccion del Sr Master. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]
* Arreglada la creacion de statuas en castle siege. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]
* Arreglada la reparación de statuas en castle siege. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]
* Arregladas las funciones de agregar o eliminar la hostilidad desde interface. [S8ENG]
* Arreglado el problemas que a veces no se tiraban items de bags o monsters. [ALL VERSIONS]
* Reparado errores del post. [S8ENG]
* Arreglado el problema de los skills master en OffHelper y OffAttack. [S6ENG][S8ENG]
* Arreglado el problema de los buffs master en OffHelper y OffAttack. [S6ENG][S8ENG]
* Arreglado un problema con Arka Battle que a veces sacaba del evento. [S8ENG]

* Recontruidas las funciones y almacenamiento de guilds. [ALL VERSIONS]
* Reformadas las funciones de patrullaje de los monstruos. [ALL VERSIONS]

UPDATE 77 (2.3.5 ~ 2.3.5-2):
* Nueva opcion agregada en "DataServer\DataServer.ini" para la serializacion de los items (0: Procedure mode, 1: Real Time mode). [ALL VERSIONS] (ItemSerialMode)

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Command.dat". [ALL VERSIONS] (CommandPostGlobal)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Command.dat". [ALL VERSIONS] (CommandPostGlobal)
* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Event.dat". [S8ENG] (WindowEventSwitch)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Event.dat". [S8ENG] (WindowEventSwitch)

* Remplazado el StartUp de xTeam por el oficial SSeMU disponible en la carpeta "StartUp\". [ALL VERSIONS]

* Modificado el archivo "Data\Message.txt" (323) se agrego %d para visualizar el ServerCode. [ALL VERSIONS]
* Modificado el archivo "ConnectServer\ServerList.dat" para agregar el GameServerArca. [S8ENG]
* Modificado el archivo "Data\MapServerInfo.txt" para agregar el GameServerArca. [S8ENG]
* Modificado el archivo "GameServerCS\DATA\GameServerInfo - Event.dat" apagando los eventos Acheron Guardian, Arca Battle, Illusion Temple Renewal y Chaos Castle Final. [S8ENG]

* Se arreglo un error critico en cuanto a seguridad que hacia borrar personajes. [97KOR][99CHS]
* Se agrego un nuevo GameServer dedicado para eventos Acheron Guardian, Arca Battle, Illusion Temple Renewal y Chaos Castle Final [S8ENG]
* Se mejoraron los sistemas de gestion de usuarios, conexiones y blacklist. [ALL VERSIONS]
* Se rediseño el sistema que paquetes en cola para evitar cuelgues y flood. [ALL VERSIONS]
* Se modifico el serializador de items. [ALL VERSIONS]
* Se reparo el Party Gap level en Auto Party. [ALL VERSIONS]
* Se reparo un bug visual al entrar en Acheron Guardian luego de jugar Arca Battle. [S8ENG]
* Se eliminaron relojes innecesarios de aplicaciones que cargaban la memoria. [ALL VERSIONS]
* Se agregaron mejoras de seguridad en SShield Protect(MHP). [ALL VERSIONS]
* Se arreglo el bug del comando guild notice que hacia crashear el gameserver. [ALL VERSIONS]
* Se arreglo el comando que hacia crashear el gameserver por gmpost. [ALL VERSIONS]

UPDATE 76 (2.3.4):
* Se reparo el auto buff en CustomAttack. [ALL VERSIONS]
* Se reparo el auto buff y auto attaque en OfflineHelper. [S6E][S8E]
* Se reparo el sistema de tiempo maximo de CustomAttack, CustomPick y OfflineHelper (Todas en segundos desde ahora). [ALL VERSIONS]
* Se reparo el bug de PersonalShop por coins que daba crash. [S6E][S8E]

* Modificado el sistema de CustomAttack, CustomPick y OfflineHelper para que sea aleatorio. [ALL VERSIONS]
* Agregado recorrido de objetos en aleatorio. [ALL VERSIONS]

* Mejorado el sistema de items periodicos. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR]
* Mejorado el sistema de efectos en clientes que lo requerian. [97K][99B][S0KOR][S2KOR][S2PHI]
* Mejoras en todos los main en cuanto a renderizado de opciones y rendimiento. [ALL VERSIONS]

UPDATE 75 (2.3.3 ~ 2.3.3-1):
* Agregado el evento Arca Battle. [S8ENG]

* Modificado el archivo "Data\Event\ArcaBattle.dat" se agregaron opciones del Arca Battle. [S8ENG]

* Nuevos mensaje en el archivo "Data\Message.txt" (731~734) [ALL VERSIONS]

* Arregladas las desconexiones en PvP, GuildWar y Mover de mapa.[97K][99B]
* Arreglado el cierre de cliente al pasear por el mapa. [97K][99B]
* Arreglado el EventEntryLevel del MG. [97K]
* Arreglado el Viewport que no se veian los niveles de items correctamente.[97K]
* Arreglado Crash de cliente aleatorio por error de tamaño de paquete.[97K][99B]
* Arreglado el bug de los colores de daños. [97K]
* Arreglado el bug de durabilidad en los arcos y ballestas. [ALL VERSIONS]
* Arreglado problema de dupeo en trade. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]
* Arreglada la lectura de ItemValue en tiempo real. [ALL VERSIONS]
* Arreglado el nivel de creacion del Magic Gladiator. [97K]
* Arreglado error de respawn. [97K][99B]
* Arreglado mover items al hacer move por chat. [ALL VERSIONS]

* Mejorada la gestion de SQL Server. [ALL VERSIONS]

UPDATE 74 (2.3.2-1):
* Arreglados los skilles Twisth Slash, Ragefull Blow, Death Stab en PvP. [97K]
* Arreglada lecturas erroneas de items al hacer reload Item. [97K][99B]
* Arreglada la caida de items en cantidades que solo se visualizaba uno. [97K]
* Arreglada la desconexion random generada por un error de paquete. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]

* Agregado soporte a SSeMU Tools 1.0.2 para version 0.97K y arreglados reportes del mismo. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]


UPDATE 73 (2.3.2):
* Nuevo archivo agregado en "Data\Event\GoldenArcher.dat" para configurar Golden Archer NPC. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG]
* Nuevo archivo para configurar mensajes del cliente "Tools\GetMainInfo\Data\CustomMessage.txt" en 3 idiomas. [ALL VERSIONS]

* Mejorado el sistema de Skills de CustomAttack y OfflineHelper para consumir menos memoria. [ALL VERSIONS]
* Mejorado el sistema de ToolTips pudiendo leer items todos los items en todas las interfaces. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR]

* Arreglado el bug visual al utilizar dos armas ancient restaba opciones. [S2KOR][S2PHI][S3KOR][S3TAI]
* Arreglado el Kriss Invisible en trade con items ancient. [99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]
* Arreglado buffs Master Tree en CustomAttack y OfflineHelper. [S6ENG][S8ENG]

* Reconstruido el NPC Golden Archer que no existia. [S4KOR]
* Agregado al cliente nuevas opciones para dar recompensas en NPC Golden Archer. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG]

* Agregados comentarios de cada Quest. (QuestObjective.txt, QuestReward.txt)[ALL VERSIONS]

UPDATE 72 (2.3.1):
* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Event.dat". [S4KOR][S6ENG] (WindowEventSwitch)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Event.dat". [S4KOR][S6ENG] (WindowEventSwitch)

* Agregada la opción "Duration" en "Data\Character\DefaultClassFreebies.txt" [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]

* Agregada la opción "Duration" en "Data\Custom\CustomGift.txt" [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]
* Agregada la opción "Duration" en "Data\Custom\CustomMarriage.txt" [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]
* Agregada la opción "Duration" en "Data\Custom\CustomMonsterKillCount.txt" [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]

* Agregada la opción "Duration" en "Data\Event\DropEvent.dat" [ALL VERSIONS]
* Agregada la opción "Duration" en "Data\Item\ItemDrop.txt" [ALL VERSIONS]

* Agregada la opción "Duration" en todos los archivos de "EventItemBag\*" [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]

* Eliminada opción "Option5" que no era requerida "Data\MapRateInfo.txt" [97K]

* Se agregaron requerimientos al uso de Attack al presionar F8. [ALL VERSIONS]
* Arreglado el problema de CustomAttack que se podia usar en simultaneo. [ALL VERSIONS]
* Mejorado el sistema de CustomAttack al usar clic derecho sobre interfaces. [ALL VERSIONS]

* Agregado nuevo sistema de ToolTip para items. en "Tools\GetMainInfo\Data\CustomTooltip.txt" [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR]

* Agregado Luke Helper a la Window Event. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]

* Se agrego ventana con horario de eventos tecla H. [S4KOR][S6ENG]

* ATENCION! Se requiere ejecutar Script SQL "Update 2.3.0 to 2.3.1.sql" incluido en DB. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR] (No se requiere para servidores nuevos)

UPDATE 71 (2.3.0):
* Se arreglo un crash por uso de un Skill. [97K]

* Modificado el archivo "Data\Event\BonusManager.dat" se agrego una opcion para el nombre del bonus. [ALL VERSIONS]
* Modificado el archivo "Data\Event\DropEvent.dat" se agrego una opcion para el nombre del evento. [ALL VERSIONS]
* Modificado el archivo "Data\Event\InvasionManager.dat" se agrego una opcion para el nombre de la invasión. [ALL VERSIONS]

* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Event.dat". [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI] (WindowEventSwitch)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Event.dat". [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI] (WindowEventSwitch)

* Se modifico con mejoras el soporte de texturas. [97K][99B][S0KOR][S2PHI][S3TAI]

* Se agrego ventana con horario de eventos tecla H. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]

* Se modifico el MainInfo.ini para agregar opciones Switch de customs. [ALL VERSIONS]

UPDATE 70 (2.2.9 ~ 2.2.9-1):
* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (AntiFloodSwitch, AntiFloodBlockTime, AntiFloodMaxCount, AntiFloodMaxTime)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (AntiFloodSwitch, AntiFloodBlockTime, AntiFloodMaxCount, AntiFloodMaxTime)

* Rediseñado el sistema de efectos Cliente ~ Servidor. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]
* Rediseñado el sistema de viewports Cliente ~ Servidor. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]
* Rediseñado el sistema de equipar/desequipar. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]

* Reparados sistemas que relentizaban el GameServer. [ALL VERSIONS]

* Reconstruidas funciones de seguridad del sistema antiflood. [ALL VERSIONS]
* Reconstruido el sistema de RightClic para equipar y desequipar. [97K][99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG]

* Reparado el efectos de los items Ancient. [ALL VERSIONS]


UPDATE 68 (2.2.8-1):
* Modificado el archivo "Data\Skill\SkillDamage.txt" se agrego una nueva seccion para controlar los efectos. [ALL VERSIONS]

* Nuevas opciones agregada en "GameServer\DATA\GameServerInfo - Skill.dat". [ALL VERSIONS] (Poison, Ice, Decay, Ice Storm, Ice Arrow, Poison Arrow Effect Settings)
* Nuevas opciones agregada en "GameServerCS\DATA\GameServerInfo - Skill.dat". [ALL VERSIONS] (Poison, Ice, Decay, Ice Storm, Ice Arrow, Poison Arrow Effect Settings)

* Agregado sistema de opciones avanzadas al cliente. [S4KOR][S6ENG]
* Agregado sistema para equipar desequipar items con el clic derecho. [S4KOR][S6ENG]

* Reparada la posicion de los botones de tamaño de fuente. [99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]
* Reparado el bug de SpawnGate y RespawnGate que no leia el valor 0 en MapManager. [ALL VERSIONS]

UPDATE 67 (2.2.8):
* Actualizado LUA a 5.3, modificados los archivos default, "ScriptMain,ScriptCore". [ALL VERSIONS]
* Incluido ScriptLua con mensaje de bienvenida, nivel NIP y administrador conectado. [ALL VERSIONS]

* Actualizadas funciones internas de los ejecutables para mejorar la gestion de paquetes. [ALL VERSIONS]

* Mejorado el rendimiento de los ejecutables DataServer y GameServer. [ALL VERSIONS]

* Agregado sistema de opciones avanzadas al cliente. [99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]
* Agregado sistema para equipar desequipar items con el clic derecho. [99B][S0KOR][S2KOR][S2PHI][S3KOR][S3TAI]

* Arreglado el bug de evolucion con MG o DL y quedaba con textura fallando. [ALL VERSIONS]

UPDATE 66 (2.2.7-1):
* Nuevo sistema de ItemValue e ItemStack en tiempo real. [ALL VERSIONS]
* Nuevo sistema de Attack agregado al cliente. Activación pulsando una tecla. [ALL VERSIONS]

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (WritePenaltyLog)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (WritePenaltyLog)

* Mejoradas funciones el CustomAttack respecto a rangos y ataques. [99B][S0K]

* Arreglada la toma de pociones estando en offline helper. [S6ENG][S8ENG]
* Arreglado el problema de los ítems periodicos que no mostraban el tiempo al moverlos ítems. [S4KOR][S6ENG][S8ENG]
* Arreglado el HIT del Chaos Castle. [ALL VERSIONS]
* Arreglado congelamiento de CustomMobs inválidos. [ALL VERSIONS]
* Arreglados los stats adicionales de items ancient [99B]
* Arreglados los textos en chino de Kanturu Event. [S2KOR][S2PHI][S3KOR][S3TAI]
* Arreglado el NPC de entrada a Land of Trial. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]
* Arreglado el reconnect al cancelar un duelo. [S4KOR]
* Arreglado el problema de establecer tasa de comision NPC y ChaosBox. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S4KOR][S6ENG][S8ENG]

* Agregados los logs de Penalidad en "GameServer\LOG\PENALTY_LOG". [ALL VERSIONS]

* Recreada la función HIT inexistente en Chaos Castle. [S0K]

* Eliminado sistema ItemStack e ItemValue del cliente, sincronizado ahora desde el servidor. [ALL VERSIONS]

* Enviado MHPClient.dll a Microsoft para excluirlo de potenciales peligros. [ALL VERSIONS]

UPDATE 65 (2.2.6):
------------------------------------------------------------------------------------------------------------------------------------
* Nuevas opciones de seguridad agregadas en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (Die Penalty Settings)
* Nuevas opciones de seguridad agregadas en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (Die Penalty Settings)
------------------------------------------------------------------------------------------------------------------------------------
ExperienceDeduceRate1 = % of experience reduced on death (Hero status)
ExperienceDeduceRate2 = % of experience reduced on death (Commoner status)
ExperienceDeduceRate3 = % of experience reduced on death (PKLevel1 status)
ExperienceDeduceRate4 = % of experience reduced on death (PKLevel2 status)
ExperienceDeduceRate5 = % of experience reduced on death (Murderer status)
DropItemRate1 = % of item drop on death (Hero status)
DropItemRate2 = % of item drop on death (Commoner status)
DropItemRate3 = % of item drop on death (PKLevel1 status)
DropItemRate4 = % of item drop on death (PKLevel2 status)
DropItemRate5 = % of item drop on death (Murderer status)
DropItemLevel = Maximum level of items that can be dropped.
DropItemPet = Pets drop, 0 disabled, 1 enabled.
DropItemWing = Wings drop, 0 disabled, 1 enabled.
DropItemExc = Excellent drop, 0 disabled, 1 to 6 excellent option count.
DropItemSet = Ancient drop, 0 disabled, 1 enabled.
MoneyDeduceRate = % loss of money at death.
------------------------------------------------------------------------------------------------------------------------------------

* Arreglado el drop de Dark Raven en PKItemDrop. [ALL VERSIONS]

* Agregado sistema de extraccion de dinero al morir. [ALL VERSIONS]
* Agregado sistema de extraccion de experiencia al morir. [ALL VERSIONS]

* Opciones removidas de [GameServer\DATA\GameServerInfo - Common.dat] [ALL VERSIONS] (PKItemDropRatePvP1,PKItemDropRatePvP2,PKItemDropRatePvP3,PKItemDropRatePvM1,PKItemDropRatePvM2,PKItemDropRatePvM3,PKItemDropMaxLevel,PKItemDropPet,PKItemDropWing,PKItemDropExc,PKItemDropSet,PKItemDropJoH,PKItemDrop380)
* Opciones removidas de [GameServerCS\DATA\GameServerInfo - Common.dat] [ALL VERSIONS] (PKItemDropRatePvP1,PKItemDropRatePvP2,PKItemDropRatePvP3,PKItemDropRatePvM1,PKItemDropRatePvM2,PKItemDropRatePvM3,PKItemDropMaxLevel,PKItemDropPet,PKItemDropWing,PKItemDropExc,PKItemDropSet,PKItemDropJoH,PKItemDrop380)

* Reparada la barra de experiencia en el cliente. [ALL VERSIONS]
* Reparada la adicion y extraccion de experiencia y dinero en las barras del cliente. [ALL VERSIONS]

UPDATE 64 (2.2.5-1):
* Arreglado el problema de party y guild al moverse. [99B]
* Arreglado problema de buff que desaparecia en versiones koreanas. [S0KOR][S2KOR][S3KOR][S4KOR]
* Arreglado problema de desconexiones por MHP. [ALL VERSIONS]

* Agregada la consola de debug. [ALL VERSIONS]

* Mejorado el sistema de conteo de pasos anti hack. [ALL VERSIONS]
* Mejorada la seguridad de MHP Client en cuanto a ByPass. [ALL VERSIONS]

UPDATE 63 (2.2.5):
* Arreglado el problema bug de PersonalShop lleno. [ALL VERSIONS]
* Arreglado el bug visual de pets en versiones bajas. [99B][S0K][S2K][S2PHI][S3KOR][S3TAI]
* Arreglado el hunting de offhelper de items superiores al slot 4. [S6][S8]
* Arreglado el problema de hunting defectuoso al usar espacio. [S6][S8]
* Arreglado sistema de efectos periodicos. [S2PHI][S3KOR][S3TAI]
* Arreglado problema de flood en ConnectServer. [ALL VERSIONS]

* Modificado el comando /attack <buff> <skill>. [ALL VERSIONS]

UPDATE 61 (2.2.4):
* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (SelfDefenseTime)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (SelfDefenseTime)

* Modificado el archivo "Data\Custom\CustomJewel.txt" se modificaron opciones para cada version. [S0KOR][S2KOR][S2PHI][S3KOR][S3TAI][S6ENG][S8ENG]

* Removidas opciones de CustomBow y CustomItemBow hasta finalizar los mismos. [S3KOR][S6ENG]

* Restructurado el sistema de CustomMonster para usar Monstruos con armas y armaduras. [ALL VERSIONS]

* Restructurado el sistema de precios de compra de items en shops. [ALL VERSIONS]
* Restructurado el sistema de precios de venta de items en shops. [ALL VERSIONS]
* Restructurado el sistema de durabilidad en items. [ALL VERSIONS]
* Restructurado el sistema de precios de reparacion de items. [ALL VERSIONS]

* Removidas opciones MinLevel,MaxLevel,MinExc y MaxExc en CustomEffect. [ALL VERSIONS]

* Reparado el Glow Excellent. [S8]

UPDATE 60 (2.2.3-2):
* Reparado problema de desconexiones aleatorias debido a las medidas de seguridad de las conexiones.

* Removidas opciones de seguridad en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (MaxConnectionTime,MaxConnectionIdleTime,MaxConnectionVerifyTime,MaxConnectionPacketCount,MaxConnectionPacketTime)
* Removidas opciones de seguridad en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (MaxConnectionTime,MaxConnectionIdleTime,MaxConnectionVerifyTime,MaxConnectionPacketCount,MaxConnectionPacketTime)

* Automatizado el sistema de conexiones. [ALL VERSIONS]

UPDATE 59 (2.2.3-1):
* Nuevas opciones agregada en "GameServer\DATA\GameServerInfo - Skill.dat". [99B] (DefenseConstA,DefenseTimeConstA)
* Nuevas opciones agregada en "GameServerCS\DATA\GameServerInfo - Skill.dat". [99B] (DefenseConstA,DefenseTimeConstA)

* Arreglado el Skill de los escudos que no reducia el daño. [99B]
* Arreglado el problema que congelaba a oponentes al hacer PvP. [S6]

* Modificado CustomAttack guarda los datos al reconectar. [ALL VERSIONS]
* Modificado CustomPick guarda los datos al reconectar. [ALL VERSIONS]
* Modificadas las bridges LUA. (OnMonsterDie, OnUserDie, OnUserRespawn). [ALL VERSIONS]

* Mejoras en la seguridad de los mains. [ALL VERSIONS]
* Mejoras en la seguridad de MHPClient.dll y MHPVerify.dll [ALL VERSIONS]

* Reformada la ventana de ConnectionManager en MHPServer. [ALL VERSIONS]

* Removidas opciones obsoletas y en desuso en "MHPServer\MHPServer.ini" [ALL VERSIONS]

UPDATE 58 (2.2.3):
* Nuevas opciones de seguridad agregadas en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (Security Settings)
* Nuevas opciones de seguridad agregadas en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (Security Settings)
* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackRepair_ALX)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackRepair_ALX)

* Arreglado el problema de no poder unirse en GuildMatching. [S8]
* Arreglado el problema de no poder subir de nivel los pentagramas con jewels. [S8]
* Arreglado el problema del Elemental Talisman eliminaba igualmente items al subir Errtel. [S8]
* Arreglado problema al hacer ping que generaba log de error. [ALL VERSIONS]

* Mejorado el sistema de limitacion por IP o HardwareId. [ALL VERSIONS]
* Mejorado el sistema de CustomAttack, reprogramado autocierre, deteccion de skills y verificaciones. [ALL VERSIONS]

* Reformada la ventana de ConnectionManager. [ALL VERSIONS]

* Mejoras en la seguridad interna de las conexiones. [ALL VERSIONS]

UPDATE 57 (2.2.2):
* Arreglados dos metodos de crasheo del servidor. [ALL VERSIONS]
* Arreglado bug de doble angel o mascotas. [99B]
* Arreglada la verificacion de creacion de nuevos clanes o personajes. [ALL VERSIONS]
* Arreglado bug de Helper 3 horas funcionando generaba crash. [S8]
* Arreglado caracteres invalidos en funciones claves. [ALL VERSIONS]
* Arreglado el crash posible por NPC QuestWorld. [ALL VERSIONS]
* Arreglado el crash al apuntar el ticket de kalima. [S8]
* Arreglado el bug de no poder crear mascotas. [99B]
* Reparada recompensa de Acheron Guardian, doble recompensa. [S8]

* Reconstruido sistema de custom monsters. [ALL VERSIONS]

* Eliminadas funciones innecesarias del gameserver. [ALL VERSIONS]
* Eliminadas funciones innecesarias de main. [ALL VERSIONS]
* Eliminado sistema de LUA cliente. [ALL VERSIONS]

* Mejorado sistema de seguridad de caracteres invalidos. [ALL VERSIONS]

UPDATE 56 (2.2.1):
* Modificado el archivo "Data\EventItemBagManager.txt" se agrego la recompensa de Illusion Temple 6 [S4][S6][S8]
* Modificado el archivo "Data\EventItemBagManager.txt" se modificaron los index special 14 ~ 17. [S8]
* Modificado el archivo "Data\EventItemBag\Special\Illusion Temple 1.txt" agregada remcompensa correspondiente. [S8]
* Modificado el archivo "Data\EventItemBag\Special\Illusion Temple 2.txt" agregada remcompensa correspondiente. [S8]
* Modificado el archivo "Data\EventItemBag\Special\Illusion Temple 3.txt" agregada remcompensa correspondiente. [S8]
* Modificado el archivo "Data\EventItemBag\Special\Illusion Temple 4.txt" agregada remcompensa correspondiente. [S8]
* Modificado el archivo "Data\EventItemBag\Special\Illusion Temple 5.txt" agregada remcompensa correspondiente. [S8]
* Modificado el archivo "Data\EventItemBag\Special\Illusion Temple 6.txt" agregada remcompensa correspondiente. [S8]
* Modificados los spawn "Data\Monster\Spawn\045 - Illusion Temple 1.txt" [S8]
* Modificados los spawn "Data\Monster\Spawn\045 - Illusion Temple 2.txt" [S8]
* Modificados los spawn "Data\Monster\Spawn\045 - Illusion Temple 3.txt" [S8]
* Modificados los spawn "Data\Monster\Spawn\045 - Illusion Temple 4.txt" [S8]
* Modificados los spawn "Data\Monster\Spawn\045 - Illusion Temple 5.txt" [S8]
* Modificados los spawn "Data\Monster\Spawn\045 - Illusion Temple 6.txt" [S8]
* Modificado el archivo "Data\MapServerInfo.txt" Agregados los mapas de Illusion Temple. [S8]
* Modificado el archivo "Data\MiniMap.txt" Modificada la locacion de Mirage. [S8]

* Agregado evento Illusion Temple Renewal. [S8]
* Agregado nuevo estado "READY" en los eventos DS,BC,CC e IT. [ALL VERSIONS]
* Agregado nuevo estado "READY" en evento Chaos Castle Final e Illusion Temple Renewal. [S8]

* Nuevos mensaje en el archivo "Data\Message.txt" (725~730) [ALL VERSIONS]

* Reformada Acheron Tower ahora aparece de diferentes elementos. [S8]
* Reformado Acheron Event ahora los mobs son del mismo elemento que la torre. [S8]

* Reparada falla del ranking en Chaos Castle Survival. [S8]

UPDATE 55 (2.2.0):
* Agregado tema oscuro a los ejecutables. [ALL VERSIONS]

* Arreglado el efecto invisible. [99B]

* Mejorado el sistema de conexiones. [ALL VERSIONS]
* Mejorado el sistema de TCP/UDP de conexiones entrantes. [ALL VERSIONS]
* Mejorado el rendimiento de la interface. [ALL VERSIONS]

* Reformada la funcionalidad del archivo GameMaster.txt usando "*" en Name aplicara a todos los personajes. [ALL VERSIONS]

UPDATE 54 (2.1.9):
* Arreglado el bug de la tienda personal que no se podia cerrar o modificar luuego de usar CustomStore. [S0][S2][S2PHI][S3][S3TAI][S6][S8]
* Arreglado el crash el usar teleport + sweel life. [99B]
* Arreglados los ataques del RageFighter en Offline mode. [S6][S8]
* Arreglado el skill Dark Side. [S6]
* Arreglado el viewport de efecto invisible. [ALL VERSIONS]
* Arreglado el bug del angel que no sumaba los 50HP. [99B]

* Eliminado archivo "Data\Skill\HelperSkill.txt" se utiliza "Data\Custom\CustomAttack.txt" en su lugar. [S6][S8]

* Reconstruido el sistema de efectos. [ALL VERSIONS]
* Reconstruido el sistema de efectos periodicos. [99B][S0][S2][S2PHI][S3][S3TAI]

* Reformada la funcionalidad del HackSkillSpeedCheck. [ALL VERSIONS]

UPDATE 53 (2.1.8-1):
* Arreglado el crash al leer scripts lua de mas de 8000 caracteres. [ALL VERSIONS]

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackPotion_ALX, CustomAttackPotionDelay_ALX)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackPotion_ALX, CustomAttackPotionDelay_ALX)

* Restructurado con nuevas opciones CustomAttack "Data\Custom\CustomAttack.txt" se agrego rate de velocidad. [ALL VERSIONS]

UPDATE 53 (2.1.8):
* Arreglado el problema de perdida de ManaShield al cambiar de mapa. [ALL VERSIONS]
* Se mejoro la lectura de los archivos. [ALL VERSIONS]
* Se mejoro la lectura de licencias desde el servidor. [ALL VERSIONS]
* Se mejoro el sistema del Custom Attack. [ALL VERSIONS]

* Nuevo archivo para configurar los daños recibidos en las mascotas en "Data\Util\PetDamageTable.txt" [ALL VERSIONS]

* Modificado el archivo "Data\Custom\CustomArena.txt" se agrego una opcion para escoger el bag de cada arena. [ALL VERSIONS]

UPDATE 52 (2.1.7):
* Se arreglo crash por sobrelectura. [ALL VERSIONS]
* Se arreglo el problema del listado del CustomPick. [ALL VERSIONS]
* Se arreglo una caida inesperada por uso al autenticar licencia. [ALL VERSIONS]
* Se arreglo el ScriptCore.lua. [ALL VERSIONS]

* Eliminadas opciones en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (CheckSkillSpeedAction,CheckSkillSpeedPenalty,ServerDebugger,ServerDebuggerName)
* Eliminadas opciones en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (CheckSkillSpeedAction,CheckSkillSpeedPenalty,ServerDebugger,ServerDebuggerName)

* Modificado el archivo "Data\Hack\HackSkillSpeedCheck.txt" se agregaron nuevos metodos de filtro. [ALL VERSIONS]

* Mejorado el sistema de ItemBag Avanzado. GradeCount aplica, durabilidad o grado de items. [ALL VERSIONS]

UPDATE 51 (2.1.6):
* Nuevos mensaje en el archivo "Data\Message.txt" (724) [ALL VERSIONS]
* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Event.dat". [99B][S0][S2][S2PHI][S3][S3TAI][S6] (ChaosCastleMoneyRate)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Event.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (ChaosCastleMoneyRate)

* Arreglado Dark Knight skills en Chaos Castle. [99B]
* Arreglado el error de nombre de mapa en party. [99B]
* Arregladas las frutas. [99B]
* Arreglado la perdida de buffs al moverse de mapa. [99B][S0][S2][S2PHI][S3][S3TAI]
* Arreglado el problema de tener que moverse para volver a comprar en shops. [99B]
* Arreglado el problema de moverse con la maquina chaos abierta. [99B]

* Agregado soporte de Bridge LUA. [ALL VERSIONS]
* Agregada la recompensa al azar en Chaos Castle. [ALL VERSIONS]
* Agregado mover items al baul, chaosmix y trade con clic derecho. [99B]
* Modificado archivos LUA. [ALL VERSIONS] (ScriptMain.lua, ScriptCore.lua y ScriptDefine.lua) (DEBE REMPLAZAR LA CARPETA SCRIPT O GENERARA CRASH)

* Eliminadas opciones en "GameServer\DATA\GameServerInfo - Common.dat". [99B] (FruitSubPointMin,FruitSubPointMax,FruitSubPointSuccessRate_ALX)
* Eliminadas opciones en "GameServerCS\DATA\GameServerInfo - Common.dat". [99B] (FruitSubPointMin,FruitSubPointMax,FruitSubPointSuccessRate_ALX)

Cliente:
* Arreglado los viewport al usar skill Twisth Slash, Inferno, Decay. [99B][S0][S2K][S2P]
* Arreglado el problema que trababa los skills en PvP. [ALL VERSIONS]

* Mejorada la lectura de LUA. [ALL VERSIONS]

UPDATE 50 (2.1.5):
* Arreglado el crash al ir a Atlans. [S2][S2P]
* Arreglado el crash al jugar Battle Soccer. [ALL VERSIONS]
* Arreglado el crash al jugar War. [ALL VERSIONS]

* Agregado al sistema de noticias el slide. [ALL VERSIONS]

* Eliminado el sistema de bridges LUA que causaba crash. [ALL VERSIONS]

UPDATE 49 (2.1.4):
* Arreglado el sistema de encriptacion de MHPServer. [ALL VERSIONS]

* Modificados sistemas de licenciamientos. [ALL VERSIONS]

UPDATE 48 (2.1.3):
* Arreglados problemas de CustomAttack en MAINS. [ALL VERSIONS]

CLIENT:
* Implementacion de LUA para desarrollo de estructuras. [ALL VERSIONS]

UPDATE 47 (2.1.2):
* Arreglado el crash al finalizar el GuildWar. [ALL VERSIONS]
* Arreglado el problema que no finalizaba el attack a usar skills. [ALL VERSIONS]

* Mejorado el sistema de attack. [ALL VERSIONS]

* Modificado el archivo "Data\Custom\CustomAttack.txt" se agrego rate de velocidad. [ALL VERSIONS]

CLIENT:
* Agregado Fog para mapas. [ALL VERSIONS]

* Arreglado un problema de la camara 3D. [S3][S3TAI]
* Arreglado un bug de ataque a larga distancia [99B]
* Arreglado el problema de % en Chaos Mix. [ALL VERSIONS]

UPDATE 46 (2.1.1):
* Arreglado el problema de las noticias en guilds. [ALL VERSIONS]
* Arreglado el modulo de LUA que causaba crashs. [ALL VERSIONS]
* Arreglado el bug de viewport al usar determinados skills. [ALL VERSIONS]
* Arreglado el problema de viewport de items al moverse. [ALL VERSIONS]
* Arreglado el mensaje erroneo en party al recoger items. [99B]

* Nuevas funciones LUA (CommandSend,SetObjectWindowTitle) [ALL VERSIONS]

CLIENT:
* Arreglado el problema de viewport de habilidad + ataque normal. [99B][S0][S2][S2PHI][S3][S3TAI]
* Arreglada la camara 3D en Valle of Loren. [S3][S3TAI]
* Arreglado el problema del ChaosMix. [ALL VERSIONS]
* Arreglado crash en ChaosMix. [S2PHI]
* Arreglada la resolucion 640x480. [ALL VERSIONS]

* Agregadas funciones de LUA de cambio de titulo de ventana. [ALL VERSIONS]
* Agregado el separador de chat. [S4][S6]

UPDATE 45 (2.1.0):
* Arreglado el error del Guild con usuarios conectados. [ALL VERSIONS]
* Arreglado el problema de los rates invalidos en creacion de pets. [ALL VERSIONS]
* Arreglado el crash por lectura de lua masivos. [ALL VERSIONS]
* Arreglado Viewport el cerrar el Guild. [ALL VERSIONS]
* Arreglado Viewport al abrir el baul. [ALL VERSIONS]
* Arreglado problema de quest diarias invalidas. [S6][S8]

* Agregado a consola debug el ingreso de opciones ancient. [ALL VERSIONS]

* Reestructurado el sistema ancient en shops y comandos. [ALL VERSIONS]

* Mejorada la lectura de scripts bridges LUA mediante clases estaticas. [ALL VERSIONS]


CLIENT:
* Arreglado el error de camara con cuadros negros. [99B]
* Arreglado el mensaje de rate y money en creacion de mascotas DL. [ALL VERSIONS]

UPDATE 44 (2.0.9):
* Arreglado el problema de viewport con items ancient. [ALL VERSIONS]
* Arreglado el problema de viewport al equipar items. [ALL VERSIONS]
* Arregladas auras de items ancient sets completos. [ALL VERSIONS]
* Arreglados atributos de items ancients. [S3][S3TAI]
* Arreglados los rates en tiempo real de Pet Trainer. [99B]
* Arreglado el viewport de los anillos de transformacion. [99B][S0]
* Arreglado el Double Damage en Ancient. [ALL VERSIONS]
* Arreglado el teleport en Stadium a Lorencia. [ALL VERSIONS]
* Arreglado el problema de las noticias de clanes que no se guardaban. [ALL VERSIONS]

* Rediseñado el sistema de guilds. [99B]
* Rediseñado el sistema de guild war. [ALL VERSIONS]
* Rediseñado el sistema de conexiones. [ALL VERSIONS]

* Mejoras al cambiar de mapa entre servidores. [ALL VERSIONS]
* Mejoras en ViewCharSet. [ALL VERSIONS]
* Mejoras al map viewer. [ALL VERSIONS]

UPDATE 43 (2.0.8):
* Arreglado el problema de Viewport al utilizar la ventana Move. [ALL VERSIONS]
* Arreglado el Battle Soccer que no se movia la pelota. [99B]
* Arreglado Skill Splash y otros que no pegaban en ciertas posiciones. [ALL VERSIONS]
* Arreglado el Skill Heal en Party de Helper Offline. [S6][S8]
* Arreglado el crash con skill Twisth Slash. [99B]

* Rediseñado el sistema de Battle Soccer. [ALL VERSIONS]
* Rediseñado todo el sistema de protocolos de versiones antiguas. [99B][S0][S2][S3]

* Mejorado el rendimiento de los ejecutables.[ALL VERSIONS]
* Mejorado el sistema de seguridad anti flood. [ALL VERSIONS]

UPDATE 42 (2.0.7):
* Arreglado el problema de las puertas del castle siege. [ALL VERSIONS]
* Arreglado el problema del skill teleport que dejaba traspasar las puertas del CS. [ALL VERSIONS]
* Arreglado el problema del skill Party Summon que dejaba traspasar las puertas del CS. [ALL VERSIONS]
* Arreglado el problema de Viewport luego de usar el skill summon party. [ALL VERSIONS]
* Arreglado la probicion de usar skills en LUA. [ALL VERSIONS]
* Arreglado un problema de custom attack que lo detenia a las 24 horas de uso. [ALL VERSIONS]

* Agregadas las bundles nuevas al comando pack y unpack. [S0][S2][S2PHI][S3][S3TAI][S4]

* Modificado el archivo "Data\Item\Item.txt" se agregaron las bundles [S0][S2][S2PHI][S3][S3TAI][S4] (seccion 12, desde el item 136 al item 143)

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (Custom Store Settings and Custom Jewel Pack Settings)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (Custom Store Settings and Custom Jewel Pack Settings)

* Mejoras en el sistema lua. [ALL VERSIONS]

CLIENTE:
* Agregadas lineas en el Text_xxx.bmd [1792~1799] [S0][S2][S2PHI][S3][S3TAI][S4]
* Modificadas lineas en el Text_xxx.bmd [1806,1807,1819] [S0][S2][S2PHI][S3][S3TAI][S4]
* Agregados las bundles en Item_xxx.bmd [S0][S2][S2PHI][S3][S3TAI][S4] (seccion 12, desde el item 136 al item 143)

UPDATE 41 (2.0.6):
* Arreglado el problema de CustomAttack que no se podia habilitar en los eventos. [ALL VERSIONS]
* Arreglado el MonsterMove que a veces no movia automaticamente los NPC. [ALL VERSIONS]
* Arreglado un Protocolo invalido que generaba problemas de conexiones. [ALL VERSIONS]
* Arreglado un problema con la funcion ItemGive Lua. [ALL VERSIONS]
* Arreglado los excesos de PCPoints y WCoin al vender en store. [S2][S2PHI][S3][S3TAI][S4]
* Arreglado efectos visuales +13 en MAINS. [S0][S2][S2PHI][S3][S3TAI][S4]

* Agregadas 3 nuevas box "Green Box,Red Box,Purple Box" en ItemBagManager. [S6][S8]
* Agregado sistema de mision de todos los NPC. [Elf Buffer, Zebina, Tercia, Gens, Zyro, Derubish] [S6][S8]

* Modificado el archivo "Data\Item\ItemStack.txt" Se agregaron 3 items quest. [S6][S8]
* Modificado el archivo "Data\Monster\MonsterMove.txt" Se restructuro el sistema de monstruos que se warpean. [ALL VERSIONS]
* Modificado el archivo "Data\Quest\Quest.txt" Se restructuro el sistema de quest. [ALL VERSIONS]
* Modificado el archivo "Data\Quest\QuestObjective.txt" Se restructuro el sistema de quest. [ALL VERSIONS]
* Modificado el archivo "Data\Quest\QuestReward.txt" Se restructuro el sistema de quest. [ALL VERSIONS]
* Modificado el archivo "Data\QuestWorld\QuestWorld.txt" Se restructuro el sistema de quest. [S6][S8]
* Modificado el archivo "Data\QuestWorld\QuestWorldObjective.txt" Se restructuro el sistema de quest. [S6][S8]
* Modificado el archivo "Data\QuestWorld\QuestWorldReward.txt" Se restructuro el sistema de quest. [S6][S8]
* Modificado el archivo "Data\EventItemBagManager.txt" se reestructuro el archivo y se agregaron 3 nuevas box. [ALL VERSIONS]

* Mejorado sistema de lectura de varios archivos. [ALL VERSIONS]
* Mejorada la carga de monstruos. [ALL VERSIONS]

*** IMPORTANTE: SE RECOMIENDA EJECUTAR LA SENTENCIA "DELETE QuestWorld" EN SQL PARA BORRAR TODAS LAS QUESTWORLD ANTERIORES, EN CASO DE NO HACERLO PUEDE GENERAR PROBLEMAS. ***

UPDATE 40 (2.0.5):
* Arreglado el problema que el comando offattack no mantenia conectados a los personajes. [ALL VERSIONS]

* Agregado al comando ClearInventory que no se borren items de eventos (BC e IT). [ALL VERSIONS]
* Agregado el filtro para usar comandos Attack, Pick, Store con la funcion MapZone. (-1: ALL Maps, 0,10: Ejemplo Lorencia e Icarus) [ALL VERSIONS]

* Modificado el mensaje en el archivo "Data\Message.txt" [ALL VERSIONS] (454) [ALL VERSIONS]
* Nuevos mensaje en el archivo "Data\Message.txt" (459,460) [ALL VERSIONS]

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackMapZone,CustomPickMapZone)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackMapZone,CustomPickMapZone)

* Modificado el archivo "Data\Custom\CustomPick.txt" [ALL VERSIONS]

* Eliminadas opciones en "GameServer\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackPickEnable_ALX,CustomAttackOfflinePickEnable_ALX,CustomPickMaxItemList)
* Eliminadas opciones en "GameServerCS\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomAttackPickEnable_ALX,CustomAttackOfflinePickEnable_ALX,CustomPickMaxItemList)

UPDATE 39 (2.0.4-1):
* Arreglado el problema de crash en noticias del guild. [ALL VERSIONS]
* Arreglado el problema stuck al portar marcotas. [ALL VERSIONS]
* Arreglado el problema de lua de algunas funciones. [ALL VERSIONS]

* Arreglado el plugin 3 de los mains. [ALL VERSIONS]

UPDATE 38 (2.0.4):
* Arreglado el problema de desconexion en Windows XP, Windows 7. [ALL VERSIONS]
* Arreglado el problema de compatibilidad de DLL's externas. [ALL VERSIONS]
* Arreglado el crash al matar el Kundum en mapas que no sean kalima. [ALL VERSIONS]

* Modificado el archivo "Data\Util\Notice.txt" Se restructuro el sistema de noticias. [ALL VERSIONS]

* Mejorado el sistema de creacion de ventanas de versiones bajas. [S0]

* Actualizado el sistema de lua a 5.3.1. [ALL VERSIONS]
* Actualizado el sistema de conexiones de Socket TCP y UDP. [ALL VERSIONS]

CLIENTE:

* Mejorado el sistema de carga de textuas. [ALL VERSIONS]
* Implementado sistema de efectos estaticos, dinamicos y especiales. [ALL VERSIONS]

* Removido el sistema de CustomEffectWing. [S6][S8]

UPDATE 37 (2.0.3):
* Nuevo archivo para configurar uso de skills en "Data\Skill\SkillEnabler.txt". [ALL VERSIONS]

* Modificado el archivo "Data\Event\EventEntryLevel.dat". [ALL VERSIONS] (Modificada la estructura)

* Arreglado el Custom Luke Helper. [ALL VERSIONS]
* Arreglado el Golden Archer, ahora resta de a 1 item. [ALL VERSIONS]

* Mejorado el sistema de filtrado de palabras. [ALL VERSIONS]
* Mejorado el sistema de hilos en multiprocesadores. [ALL VERSIONS]

* Nuevos mensaje en el archivo "Data\Message.txt" (723) [ALL VERSIONS]

CLIENTE:

* Mejorado el rendimiento del cliente. [ALL VERSIONS]
* Implementado el sistema de EventEntryLevel. [ALL VERSIONS]

UPDATE 36 (2.0.2):
* Arreglada la herramienta para generar el Item.bmd. [S0][S2][S2PHI][S3][S3TAI][S4][S4E][S6]
* Arreglada la funcion lua CreateMonster. [ALL VERSIONS]
* Arreglado el problema de perdida de ManaShield al cambiar de mapa. [S0][S2][S2PHI][S3][S3TAI]
* Arreglado el golden archer que no consumia las renas correctamente.[S0][S2][S2PHI][S3][S3TAI][S4][S4E][S6]
* Arreglado el problema que no se podia desconectar cuentas en offattack. [S0][S2][S2PHI][S3][S3TAI][S4][S4E]
* Arreglado el problema que no se podia desconectar gente desde el menu. [ALL VERSIONS]
* Arreglado el crash del chaos castle 6. [S0]
* Arreglado problema de experiencia en master level. [S6][S8]
* Arreglado Viewport al cambiar de servidor. [ALL VERSIONS]
* Arreglado el sistema de creacion de personajes por nivel ocultando innecesarios o innabilitados. [ALL VERSIONS]
* Arregladas las cartas de creacion de personajes. [S6][S8]
* Arreglados problemas de desconexiones. [ALL VERSIONS]
* Arreglado el problema del drop ancient desde ItemOptionRate. [ALL VERSIONS]

* Agregado sistema de bridges a Scripting LUA en "Script\Bridge" [ALL VERSIONS]
* Agregado activacion rapida del comando "/attack". [ALL VERSIONS]
* Agregado MonsterSkin para personalizar CustomMonsters con armaduras. [ALL VERSIONS]

* Mejorado el sistema de fuentes de los mains. [ALL VERSIONS]

* Modificado el archivo "GameServer\DATA\GameServerInfo - Common.dat" se agrego opcion "CheckLiveSecurity" para desactivar la seguridad en tiempo real. [ALL VERSIONS]
* Modificados el archivo "Data\Item\ItemOptionRate.txt" se incorporaron nuevas opciones para drop ancient(Section 5). [ALL VERSIONS]
* Modificados el archivo "Data\Item\SetItemOption.txt" se agrego a OptionIndex2. [S0][S2][S2PHI][S3][S3TAI]
* Modificados el archivo "Data\Item\SetItemOption.txt" se agrego a OptionIndex2. [S0][S2][S2PHI][S3][S3TAI]

* Nuevas funciones LUA. (MessageSend,MessageSendToAll) Ver(Script Lua Interface Functions)[ALL VERSIONS]

Cliente:
* Agregado sistema de multilenguage a los clientes [S0][S2][S2PHI][S3][S3TAI][S4]
* Fusionado el CustomMessage.txt con Text_XXX.bmd. [ALL VERSIONS]

UPDATE 35 (2.0.1):
* Nuevos comandos agregados en "Data\CommandManager.txt" (Offline) [S6][S8](21)

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [S6][S8] (WriteScriptLog,Helper Offline Settings)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [S6][S8] (WriteScriptLog,Helper Offline Settings)

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomArenaMapNumber,CustomAttack,CustomAttackOffline Settings)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Custom.dat". [ALL VERSIONS] (CustomArenaMapNumber,CustomAttack,CustomAttackOffline Settings)

* Nuevo archivo para configurar propiedades de skills de OffHelper en "Data\Skill\HelperSkill.txt". [S6][S8]

* Modificado el archivo "Data\Custom\CustomAttack.txt" se restructuro el archivo. [ALL VERSIONS]
* Modificado el archivo "Data\Custom\CustomLukeHelper.txt" Se agrego Class de mob. [ALL VERSIONS]
* Modificado el archivo "Data\MapServerInfo.txt" Agregados los mapas de Acheron Guardian. [S8]
* Modificado el archivo "Data\Util\Notice.txt" Se restructuro el sistema de noticias. [ALL VERSIONS]

* Mejorado el sistema de lectura de scripts. [ALL VERSIONS]
* Mejorado el sistema de scripts LUA. [ALL VERSIONS]

* Reconstruida la IA de los mobs que no tienen inteligencia programada. [ALL VERSIONS]
* Reconstruida la IA de los summon monsters. [ALL VERSIONS]

* Arreglada la opcion de Auto Aceptar miembros del Guild en Helper/Offhelper. [S8]
* Arreglado el MonsterSchedule. [ALL VERSIONS]
* Arreglado el sistema de creacion de personajes por cartas. [S4][S6][S8]
* Arreglado el sistema de respawn en mismos mapas donde murio. [ALL VERSIONS]
* Arreglado el problema del SD Ratio. [S2][S2PHI][S3][S3TAI][S4][S4E][S6][S8]

* Agregado Sistema de OffHelper en modo BETA 1. [S6][S8]
* Agregada tool para generar Item.bmd. [S0][S2][S2PHI][S3][S3TAI][S4][S4E][S6]

* Eliminadas opciones en "GameServer\DATA\GameServerInfo - Common.dat". [S6][S8] (HelperActiveMaxTime_AL3)
* Eliminadas opciones en "GameServerCS\DATA\GameServerInfo - Common.dat". [S6][S8] (HelperActiveMaxTime_AL3)

UPDATE 34 (2.0.0):
* Arreglado el problema de exceso de SD ratio increase que sobrepasaba el 100%. [ALL VERSIONS]
* Arreglado el problema de usar varios skills en CustomAttack. [ALL VERSIONS]
* Arreglado el problema de los randoms en ItemBag e ItemBagEx. [ALL VERSIONS]
* Arreglados los shops por coins. [ALL VERSIONS]

* Nuevos mensaje en el archivo "Data\Message.txt" (588,719~722) [ALL VERSIONS]

* Nuevas funciones LUA. (MapGetItemTable,ObjectGetCoin,PermissionCheck,PermissionInsert,PermissionRemove,QuestStateCheck) [ALL VERSIONS]
* Nuevas Bridges LUA. (OnCommandDone,OnUserItemPick,OnUserItemDrop,OnUserItemMove) [ALL VERSIONS]

* Opciones agregadas en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (CustomerLicenseId,ServerType,ServerVersion,ServerSerial,ServerEncDecKey1,ServerEncDecKey2,TeleportAttackCheck,CheckSkillSpeedAction,CheckSkillSpeedPenalty,CharacterDeleteSwitch,CharacterDeleteMaxLevel,PKLimitFree,PKLimitShop,PKLimitMove,PKLimitMoveSummon,PKLimitEventEntry,PKDeathAnnounce,PKDownPlusTimePoint,PKDownPlusKillPoint,PKDownRequirePoint1,PKDownRequirePoint2,PKDownRequirePoint3,PKDownRequirePoint4,PKItemDropRatePvP1,PKItemDropRatePvP2,PKItemDropRatePvP3,PKItemDropRatePvM1,PKItemDropRatePvM2,PKItemDropRatePvM3,PKItemDropMaxLevel,PKItemDropPetPKItemDropWing,PKItemDropExc,PKItemDropSet,PKItemDropJoH,PKItemDrop380,DuelAnnounceSwitch)
* Opciones agregadas en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS](CustomerLicenseId,ServerType,ServerVersion,ServerSerial,ServerEncDecKey1,ServerEncDecKey2,TeleportAttackCheck,CheckSkillSpeedAction,CheckSkillSpeedPenalty,CharacterDeleteSwitch,CharacterDeleteMaxLevel,PKLimitFree,PKLimitShop,PKLimitMove,PKLimitMoveSummon,PKLimitEventEntry,PKDeathAnnounce,PKDownPlusTimePoint,PKDownPlusKillPoint,PKDownRequirePoint1,PKDownRequirePoint2,PKDownRequirePoint3,PKDownRequirePoint4,PKItemDropRatePvP1,PKItemDropRatePvP2,PKItemDropRatePvP3,PKItemDropRatePvM1,PKItemDropRatePvM2,PKItemDropRatePvM3,PKItemDropMaxLevel,PKItemDropPetPKItemDropWing,PKItemDropExc,PKItemDropSet,PKItemDropJoH,PKItemDrop380,DuelAnnounceSwitch)

* Modificado el archivo "ConnectServer.ini" [ALL VERSIONS] Cambiado(CustomerName -> CustomerLicenseId)
* Modificado el archivo "DataServer.ini" [ALL VERSIONS] Cambiado(CustomerName -> CustomerLicenseId)
* Modificado el archivo "JoinServer.ini" [ALL VERSIONS] Cambiado(CustomerName -> CustomerLicenseId)
* Modificado el archivo "MHPServer.ini" [ALL VERSIONS] Cambiado(CustomerName -> CustomerLicenseId)
* Modificado el archivo "Data\MapServerInfo.txt". [ALL VERSIONS] Removido (MHPEncDecKey,ServerVersion,ServerSerial)

* Eliminado el archivo "Data\Custom\CustomMasterResetReward.txt" [ALL VERSIONS]
* Eliminado el archivo "Data\Custom\CustomMonsterKillMove.txt" [ALL VERSIONS]
* Eliminado el archivo "Data\Custom\CustomMove.txt" [ALL VERSIONS]
* Eliminado el archivo "Data\Custom\CustomNPCCollector.txt" [ALL VERSIONS]
* Eliminado el archivo "Data\Custom\CustomNPCMove.txt" [ALL VERSIONS]
* Eliminado el archivo "Data\Custom\CustomPKSafeZone.txt" [ALL VERSIONS]
* Eliminado el archivo "Data\Custom\CustomPVPZone.txt" [ALL VERSIONS]
* Eliminado el archivo "Data\Custom\CustomResetReward.txt" [ALL VERSIONS]
* Eliminado el archivo "Data\PKItemDrop.txt" [ALL VERSIONS]
* Eliminado el archivo "Data\PKManager.txt" [ALL VERSIONS]

* Eliminadas opciones en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (CustomerName,CheckHackSkillAction,LuaScriptSwitch,PKDownTime1,PKDownTime2,ShieldGaugeDisable)
* Eliminadas opciones en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (CustomerName,CheckHackSkillAction,LuaScriptSwitch,PKDownTime1,PKDownTime2,ShieldGaugeDisable)

* Eliminadas opciones en "GameServer\DATA\GameServerInfo - Skill.dat". [ALL VERSIONS] (CastleSiegeSkillForAllMaps)
* Eliminadas opciones en "GameServerCS\DATA\GameServerInfo - Skill.dat". [ALL VERSIONS] (CastleSiegeSkillForAllMaps)

UPDATE 33:
* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - ChaosMix.dat". [S8] (TrophiesMixRate1_ALX,TrophiesMixRate2_ALX,TrophiesMixRate3_ALX,TrophiesMixRate4_ALX,TrophiesMixRate5_ALX,TrophiesMixRate6_ALX,TrophiesMixRate7_ALX,TrophiesMixRate8_ALX,TrophiesMixRate9_ALX,TrophiesMixRate10_ALX)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - ChaosMix.dat". [S8] (TrophiesMixRate1_ALX,TrophiesMixRate2_ALX,TrophiesMixRate3_ALX,TrophiesMixRate4_ALX,TrophiesMixRate5_ALX,TrophiesMixRate6_ALX,TrophiesMixRate7_ALX,TrophiesMixRate8_ALX,TrophiesMixRate9_ALX,TrophiesMixRate10_ALX)

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Command.dat". [ALL VERSIONS] (CommandPKClearType,CommandPKClearMoney_ALX)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Command.dat". [ALL VERSIONS] (CommandPKClearType,CommandPKClearMoney_ALX)

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (LuaScriptSwitch,EffectOverwriteMode,PetExperienceMultiplierConstA,PetExperienceRateDivisor,MaxPetLevel)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (LuaScriptSwitch,EffectOverwriteMode,PetExperienceMultiplierConstA,PetExperienceRateDivisor,MaxPetLevel)

* Nuevos mensajes en el archivo "Data\Message.txt" [ALL VERSIONS] (713~718) (1.0.3.3-1)

* Nuevas funciones LUA. (GetObjectGuildStatus,GetObjectGuildRelationship,GetObjectGuildUnionNumber,GetObjectGuildUnionName,GetObjectOfflineFlag,InventoryGetItemTable,MonsterSummonCreate,MonsterSummonDelete) [ALL VERSIONS]  (1.0.3.3-1)

* Agregados ChaosMix Rates en tiempo real. [ALL VERSIONS]
* Agregado el comando PKClear por cantidad de muertes. [ALL VERSIONS]

* Agregado script SQL necesario en "Databases\UP 1.0.3.3 (1 - Create Table EventEntryCount).sql". [ALL VERSIONS] (1.0.3.3-1)
* Agregado script SQL necesario en "Databases\UP 1.0.3.3 (2 - Create Procedure WZ_GetEventEntryInfo).sql". [ALL VERSIONS] (1.0.3.3-1)
* Agregado script SQL necesario en "Databases\UP 1.0.3.3 (3 - Create Procedure WZ_SetEventEntryInfo).sql". [ALL VERSIONS] (1.0.3.3-1)

* Movido Acheron Guardian de "GameServer" a "GameServerCS". [S8]

* Corregido el ScheduleManager. [ALL VERSIONS]
* Corregidos los efectos de Arka War (179~184,187). [S8]
* Corregido el drop por efecto en Acheron. [S8]
* Corregido el drop ancient con efecto en Acheron. [S8]
* Corregidas las opciones JewelOfHarmonyItemRestoreMixRate_ALX. [S2][S2PHI][S3][S3TAI][S4][S4E][S6][S8]

Cliente:
* Arreglado desborde de la barra reconnect.[ALL VERSIONS]

[Text.bmd] Linea 484-1156-1167 [S2PHI]
[GetMainInfo.txt] Eliminado DisableMonsterHealthBarKeys
[GetMainInfo.txt] Eliminado DisableCamera3DKeys
[GetMainInfo.txt] Eliminado DisableMinimizeTrayKeys
[GetMainInfo.txt] Agregado KeyCodeHealthBarSwitch [S0][S2][S2PHI][S3][S3TAI][S4][S4E][S6]
[GetMainInfo.txt] Agregado KeyCodeCamera3DSwitch [ALL VERSIONS]
[GetMainInfo.txt] Agregado KeyCodeCamera3DRestore [ALL VERSIONS]
[GetMainInfo.txt] Agregado KeyCodeTrayModeSwitch [ALL VERSIONS]

UPDATE 32:
* Se reparo reconnexiones automaticas al ingresar varias personas a eventos. [ALL VERSIONS]
* Se reparo bug de caminada fantasma en Castle Siege. [ALL VERSIONS]
* Se reparo un error en el Helper que ocasionaba su parada critita. [S6]

* Modificado el archivo "Data\Hack\HackSkillSpeedCheck.txt" se agregaron nuevos metodos de filtro. [ALL VERSIONS]

* Eliminado el archivo "Data\Hack\HackChecksumCheck.txt". [ALL VERSIONS]
* Eliminadas opciones en "GameServer\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI] (PartyMaxMember,PartyGeneralExperience6,PartyGeneralExperience7,PartyGeneralExperience8,PartyGeneralExperience9,PartyGeneralExperience10,PartySpecialExperience6,PartySpecialExperience7,PartySpecialExperience8,PartySpecialExperience9,PartySpecialExperience10)
* Eliminadas opciones en "GameServerCS\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI] (PartyMaxMember,PartyGeneralExperience6,PartyGeneralExperience7,PartyGeneralExperience8,PartyGeneralExperience9,PartyGeneralExperience10,PartySpecialExperience6,PartySpecialExperience7,PartySpecialExperience8,PartySpecialExperience9,PartySpecialExperience10)

Cliente:
* Limpiado el cliente de falsos positivos.
* Limpiado el MHPClient.dll de falsos positivos.
* Se reparo falso crash. [S6][S8]
* Se elimino sistema checksum en tiempo real por falsos positivos.
* Se arreglo problema de la jaulas de stadium que desconectaban.

UPDATE 31:
* Se reestructuraron varias cosas en los ejecutables. [ALL VERSIONS]
* Se mejoro el rendimiento. [ALL VERSIONS]
* Se volvio a agregar la version BETA con LUA. [ALL VERSIONS]
* Se mejoro la ventana de conexiones de usuarios.  [ALL VERSIONS]
* Se agrego soporte para la temporada 4 episodio 6. [S4E]

Cliente:
* Se reestructuro todo el sistema del main.
* Se agrego soporte para inyeccion al main.
* Se implementaron mejoras en el OpenGL.
* Se corrigio el problema de velocidad en main.
* Se agrego correctamente el sistema CRC.

UPDATE 30:
* Arreglado el PartyExperience. [ALL VERSIONS]
* Arreglados los problemas de CustomMonster en CustomNPCCollector,CustomNPCMove y MonsterSchedule. [ALL VERSIONS]
* Arreglados efectos buffs auras ELF. [S0][S2][S2PHI][S3][S3TAI]
* Arreglado mensaje invalido post. [S8]

* Agregado UI de MapViewer en GameServers. [ALL VERSIONS]
* Agregada auto deteccion a la opcion MaxPacketPerSecond de los gameserver. [ALL VERSIONS]
* Agregada verificacion de checksum al cliente en "Data\Hack\HackChecksumCheck.txt". [ALL VERSIONS]

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI] (PartyMaxMember,PartyGeneralExperience6,PartyGeneralExperience7,PartyGeneralExperience8,PartyGeneralExperience9,PartyGeneralExperience10,PartySpecialExperience6,PartySpecialExperience7,PartySpecialExperience8,PartySpecialExperience9,PartySpecialExperience10)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI] (PartyMaxMember,PartyGeneralExperience6,PartyGeneralExperience7,PartyGeneralExperience8,PartyGeneralExperience9,PartyGeneralExperience10,PartySpecialExperience6,PartySpecialExperience7,PartySpecialExperience8,PartySpecialExperience9,PartySpecialExperience10)

Cliente:
* Arregladas las camaras 3D y extendidos sus rangos. [ALL VERSIONS]
* Arreglado crash CustomMaps. [S0]
* Extendido el party hasta 10 personas. [S0][S2][S2PHI][S3][S3TAI]

* CustomMonster y CustomNPC pueden elegirse las carpetas a usar. [ALL VERSIONS]

* Mejorado el soporte de CustomMap y agregado effectos por mapas. [ALL VERSIONS]
* Mejoradas las interface de Reconnect. [ALL VERSIONS]

UPDATE 29:
* Arreglado el reloj de Double Goer. [S6][S8]
* Arreglado el reloj de Imperial Guardian. [S6][S8]
* Arreglado el problema que no se podian crear clanes con 8 letras. [ALL VERSIONS]
* Arreglado el crash de GS en duelo al estar ShieldGaugeDisable en 1. [S6][S8]

* Mejorado el sistema de anti flood. [ALL VERSIONS]

* Agregadas nuevas UI de ConnectionList en GameServers. [ALL VERSIONS]
* Agregadas nuevas UI de DetectionList en MHPServer.

* Removido de la version oficial el soporte LUA (Solo en BETA). [ALL VERSIONS]

* Opciones eliminadas en "ConnectServer\ConnectServer.ini". [ALL VERSIONS] (MaxIpConnection,MaxPacketCount,MaxPacketDelay,TimeoutDisconnect,VerifyTimeoutDisconnect,AutomaticLockSwitch,AutomaticLockTime)
* Opciones eliminadas en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (DebugCharName,MaxPacketTime,MaxPacketCount,DSJSMaxPacketTime,DSJSMaxPacketCount,TimeoutDisconnect,VerifyTimeoutDisconnect,MaxIpConnection,MaxHidConnection)
* Opciones eliminadas en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (DebugCharName,MaxPacketTime,MaxPacketCount,DSJSMaxPacketTime,DSJSMaxPacketCount,TimeoutDisconnect,VerifyTimeoutDisconnect,MaxIpConnection,MaxHidConnection)
* Opciones eliminadas en "MHPServer\MHPServer.ini". [ALL VERSIONS] (MHPServerPort,MaxIpConnection,MaxHidConnection,MaxPacketCount,MaxPacketDelay,TimeoutDisconnect,VerifyTimeoutDisconnect)

* Nueva opcion agregada en "ConnectServer\ConnectServer.ini". [ALL VERSIONS] (MaxConnectionIdle,MaxConnectionPerIP,MaxPacketPerSecond)
* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Character.dat". [ALL VERSIONS] (DamageStuckOnPetUniria,DamageStuckOnPetDinorant,DamageStuckOnPetDarkHorse,DamageStuckOnPetFenrir)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Character.dat". [ALL VERSIONS] (DamageStuckOnPetUniria,DamageStuckOnPetDinorant,DamageStuckOnPetDarkHorse,DamageStuckOnPetFenrir)
* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (ServerDebuggerName,MaxConnectionIdle,MaxConnectionPerIP,MaxConnectionPerHID,MaxPacketPerSecond,MaxTimeConnectionVerify)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (ServerDebuggerName,MaxConnectionIdle,MaxConnectionPerIP,MaxConnectionPerHID,MaxPacketPerSecond,MaxTimeConnectionVerify)
* Nueva opcion agregada en "MHPServer\MHPServer.ini". [ALL VERSIONS] (MHPServerPortTCP,MaxConnectionIdle,MaxConnectionPerIP,MaxConnectionPerHID,MaxPacketPerSecond)

UPDATE 28:
* Arreglados los problemas de consumo excesivo de memoria en main. [S0][S2][S2PHI][S3][S3TAI]
* Arreglado el problema de HackSkillSpeedCheck que no se podian poner varios valores por rangos. [ALL VERSIONS]
* Agregados Escudos faltantes y corregido el set aura. [S4]
* Arreglados los problemas de clics en mains. [S0][S2][S2PHI][S3][S3TAI][S4][S8]
* Arreglado el problema de OZT en CustomMaps. [S0]

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (DebugCharName)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (DebugCharName)

* Modificado el archivo "Data\Item\Item.txt" se agrego el Cross Shield [S4]
* Modificado el archivo "Data\Script\ScriptMain.lua". [ALL VERSIONS]

* Finalizado el sistema de Scripts LUA. [ALL VERSIONS]
* Reducidos los consumos de CPU en todos los mains.

UPDATE 27-3:
* Arreglado el problema que no se podian hacer shops con monster custom. [ALL VERSIONS]
* Arreglado el problema que no se podian usar monster custom como CustomNPCMove,CustomNPCCollector. [ALL VERSIONS]
* Arreglado el life stone que dropeaba items y daba experiencia al destruirlo. [ALL VERSIONS]
* Arreglado el problema de los mercenarios invencibles. [ALL VERSIONS]
* Arreglado el OnlineReward que no funcionaba. [S2PHI]
* Arreglado el problema de velocidad de la summoner. [S4]

* Agregado sistema de scripts en "Data\Script\ScriptMain.lua". [ALL VERSIONS]
* Agregados los logs de Scripts en "GameServer\LOG\SCRIPT_LOG". [ALL VERSIONS]
* Agregadas referencias de Scripts en "Tools\Reference Scripts Lua.txt". [ALL VERSIONS]

* Modificado el archivo "Data\Event\BloodCastle.dat" se agrego cordenadas de las estatuas. [ALL VERSIONS]
* Modificado el archivo "Data\Monster\MonsterList.txt" Se agrego type para distinguir MOB/NPC y resistencias 5,6,7. [ALL VERSIONS]

UPDATE 27 (1-2):
* Arreglado viewport de los fireworks. [ALL VERSIONS]
* Arreglado viewport de los Heart,Silver Medal,Golden Medal. [ALL VERSIONS]
* Arreglado el problema de crash al finalizar un GuildWar. [ALL VERSIONS]
* Arreglado problema de bloqueos en el GameServer. [ALL VERSIONS]
* Arreglado problema de desconeccion de MHP Server.
* Arreglada la desaparicion de los clanes en actualizacion v 1.0.2.4 a 1.0.2.5 [ALL VERSIONS]
* Arreglado el viewport de items y mobs que aparecien en lugares "safe". [ALL VERSIONS]
* Arreglado el problema de reset. [ALL VERSIONS]
* Arreglado el problema que no se mostraba el nombre correcto en CustomPick. [ALL VERSIONS]
* Arreglado el problema de clanes al cambiar de nombre por comando. [ALL VERSIONS]
* Arreglado el error visual de ManaShield que se eliminaba al cambiar de mapa. [S0][S2][S2PHI][S3][S3TAI]
* Arreglado el problema de noticias en GameServerCS. [ALL VERSIONS]
* Arreglado el bucle de buffs interminable en comando attack. [ALL VERSIONS]
* Arreglado customattack que necesitaba todos los skiles detallados para master level. [S6][S8]
* Arreglado el battle soccer. [ALL VERSIONS]
* Arreglado el Invasion Manager. [ALL VERSIONS]
* Arreglado el problema de recarga de monstruos. [ALL VERSIONS]
* Arreglado el bloqueo de HardWareId en MHPServer.

* Mejorado el sistema DDoS en GameServer,HackServer y ConnectServer. [ALL VERSIONS]

* Nuevos mensajes en el archivo "Data\Message.txt" [ALL VERSIONS] (712)

* Agregado el LauncherMutex para escoger un nombre de mutex compatible con su launcher. [ALL VERSIONS]

* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Common.dat". [S6][S8] (LuckyMinDurabilityRepair)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Common.dat". [S6][S8] (LuckyMinDurabilityRepair)

* Opciones eliminadas en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (AutomaticLockSwitch,AutomaticLockTime)
* Opciones eliminadas en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (AutomaticLockSwitch,AutomaticLockTime)

UPDATE 26:
* Arreglado el problema de falsos chat en Guild. [ALL VERSIONS]
* Arreglado el problema que mostraba chats de otros guilds. [ALL VERSIONS]
* Arreglado el problema que mostraba a diferentes uniones chats. [ALL VERSIONS]
* Arreglado el master skill. [S4]

* Agregado los logs de items en "GameServer\LOG\ITEM_LOG". [ALL VERSIONS]
* Agregado los logs de items en "GameServerCS\LOG\ITEM_LOG". [ALL VERSIONS]

* Mejoradas Funciones nativas en gameserver. [S4]

* Reconstruido el sistema de Viewports de Guilds en Union y Rivalidad. [ALL VERSIONS]
* Reconstruidas las funciones de CannonTower. [ALL VERSIONS]
* Reconstruidas funciones del Castle Siege. [ALL VERSIONS]

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Command.dat". [ALL VERSIONS] (CommandResetClearParty_ALX,CommandMasterResetClearParty_ALX)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Command.dat". [ALL VERSIONS] (CommandResetClearParty_ALX,CommandMasterResetClearParty_ALX)

Cliente:
* Mejoras de seguridad en MHPServer, MHPClient y MHPVerify.
* Mejoras en el rendimiento de los Main.dll

UPDATE 25:
* Nuevo archivo para configurar caida de zen en "Data\Item\ItemMoneyDrop.txt". [ALL VERSIONS]

* Reconstruido el sistema Friends. [ALL VERSIONS]
* Reconstruido el sistema Guild, GuildClass, GuildManager, GuildAlliance y GuildRival. [ALL VERSIONS]

* Arreglado el RewardMessage de CustomMonster. [ALL VERSIONS]
* Arreglado el Viewport de S8 en alianza de clanes. [ALL VERSIONS]

* Agregadas mejoras de seguridad en la conexión. [ALL VERSIONS]

UPDATE 24:
* Agregados protocolos de conexiones de Illusion Temple Renewals y Arca War. [ALL VERSIONS]

* Arreglado el problema de la recompensa del Devil Square. [ALL VERSIONS]

* Reestructurado el sistema de seguridad anti DDOS. [ALL VERSIONS]
* Reestructurado el sistema de seguridad anti DDOS entre el GameServer -> JoinServer. [ALL VERSIONS]
* Reestructurado el sistema de seguridad anti DDOS entre el GameServer -> DataServer. [ALL VERSIONS]

* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (MaxPacketTime,MaxPacketCount)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Common.dat". [ALL VERSIONS] (MaxPacketTime,MaxPacketCount)

* Mejoras de seguridad MHPServer Client.
* Mejoras de seguridad MHPServer Servidor.
* Mejorado el sistema de EventItemBag. [ALL VERSIONS]

UPDATE 23:
* Arreglado el problema de crash al tirar jewels custom. [S3][S3TAI]
* Arreglado el problema que no mostraba ItemToolTip. [S0][S2][S2PHI][S3][S3TAI]
* Arreglado EventEntryLevel de Kalima. [ALL VERSIONS]
* Arreglada la recompensa del Devil Square. [ALL VERSIONS]
* Arreglados seals sin tiempo y cosas erroneas respecto a seals. [S6][S8]
* Arreglado el Viewport al recibir daño del Evil Spirit. [S0]
* Arreglado los comandos por segundos. [ALL VERSIONS]

* Modificado el archivo "Data\Event\EventEntryLevel.dat" se modifico los niveles de kalima. [ALL VERSIONS]
* Modificado el archivo "Data\Skill\EffectList.txt" se modificaron algunos items. [S6][S8]

CLIENTE:
* Arreglados los problemas de CustomToolTip.
* Mejorado el MHP Server y Cliente.
* Mejorado el flujo de datos en los mains.
* Arreglado problemas de Custom Jewels.
* Arreglado problema de seals con duracion.

UPDATE 22-1:
* Arreglado el problema de bug con el warehouse. [ALL VERSIONS]
* Arreglado el problema de zen en trade. [ALL VERSIONS]
* Arreglado el problema de Guild Alianza. [S0]
* Arreglado el bug de dup en warehouse. [ALL VERSIONS]
* Arreglado el problema del Archer y Spearman. [ALL VERSIONS]

* Agregado ItemLevel.txt para diferencias los items por nivel en "Data\Item\ItemLevel.txt" [ALL VERSIONS]
* Agregado soporte de reconocimiento de items por nivel al comando pick. [ALL VERSIONS]
* Agregado soporte de reconocimiento de items por nivel al comando pickset. [ALL VERSIONS]

* Mejorado el frujo de datos entre el DataServer y el GameServer. [ALL VERSIONS]

* Modificados mensajes en el archivo "Data\Message.txt" [ALL VERSIONS] (656,664)

UPDATE 22:
* Arreglado los problemas de Viewport en Blood Castle. [ALL VERSIONS]
* Arreglado el duplicado de items al morir con la maquina chaos. [ALL VERSIONS]
* Arreglado el trade hack al morir. [ALL VERSIONS]
* Arreglado el problema de los Shops con items Ancient. [ALL VERSIONS]
* Arreglado el empuje de mobs. [S6][S8]
* Arreglado el respawn de mobs en evento Acheron Guardian. [S8]

* Agregado Evento Acheron Guardian. [S8]
* Agregado todo el soporte de Acheron y Arca War. [S8]
* Agregados los mensajes de Arca War al Message.txt. [S8]
* Agregado soporte de NPC Lesnar para Arca War. [S8]
* Agregados los buffs de Arca War. [S8]

* Nuevos mensajes en el archivo "Data\Message.txt" [ALL VERSIONS] (23~29,596,597,702~711)

* Modificado el GameMaster.txt. [ALL VERSIONS]

* Reestructuradas a nivel codigo las funciones del protocolo de Guild Alianzas y Hostilidades. [ALL VERSIONS]
* Mejorada la carga de archivos por MemScript. [ALL VERSIONS]
* Mejorado el sistema de Sign GM desde CtrlCode en SQL Server. [ALL VERSIONS]

UPDATE 21:
* Nuevo archivo para configurar comando pick en "Data\Custom\CustomPick.txt". [S0][S2][S2PHI][S3][S3TAI][S6]

* Agregado script SQL para el comando pick en "Databases\4 - (1.0.2.1) CustomPick.sql". [S0][S2][S2PHI][S3][S3TAI][S6]

* Nuevos comandos agregados en "Data\CommandManager.txt" (Pick,PickSet,PickClear,SetBuff) [S0][S2][S2PHI][S3][S3TAI][S6] (63,64,65,129)
* Nuevos mensajes en el archivo "Data\Message.txt" [S0][S2][S2PHI][S3][S3TAI][S6] (50,395,450~458)

* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (MonsterGetTopHitDamageUserMaxTime)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (MonsterGetTopHitDamageUserMaxTime)
* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Custom.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CustomPickSwitch,CustomPickMaxItemList,CustomPickMaxTime_ALX)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Custom.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CustomPickSwitch,CustomPickMaxItemList,CustomPickMaxTime_ALX)
* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Event.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (BloodCastleNPCWithoutEntrance,DevilSquareNPCWithoutEntrance,IllusionTempleNPCWithoutEntrance)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Event.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (BloodCastleNPCWithoutEntrance,DevilSquareNPCWithoutEntrance,IllusionTempleNPCWithoutEntrance)

* Modificado el archivo "ConnectServer\ServerList.dat". [S0][S2][S2PHI][S3][S3TAI][S6]

* Arreglado el problema que mostraba horarios incorrectos en entradas BC,CC,DS e IT. [S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglado el problema del Dark Horse invisible en Blood Castle. [S2PHI]

* Agregado SSeMU Editor. [S0][S2][S2PHI][S3][S3TAI]
* Agregada opcion para mostrar en NPC los niveles necesarios para ingresar al BC,DS e IT. [S0][S2][S2PHI][S3][S3TAI][S6]
* Agregado al GetMainInfo eliminar barraSD, teclas de camara,traymode y monsterbar. [S0][S2][S2PHI][S3][S3TAI][S6]
* Agregado a GetMainInfo nivel minimo necesario para borrar a un personaje. [S0][S2][S2PHI][S3][S3TAI][S6]
* Agregado sistema de dialogo para mostrar advertencias en el juego. [S0][S2][S2PHI][S3][S3TAI][S6]

UPDATE 20-1:
* Actualizada la interface de los ejecutables. [S0][S2][S2PHI][S3][S3TAI][S6].
* Arreglado la opcion RandomOpt en Bags en Formato1. [S0][S2][S2PHI][S3][S3TAI][S6].
* Arreglado el skill invisible luego del trade. [S0][S2][S2PHI][S3][S3TAI]
* Arreglado el problema de hack de paquete 0x0D.[S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglada la estructura de los efectos. [S0][S2][S2PHI][S3][S3TAI][S6]

* Agregado MapManager. S0][S2][S2PHI][S3][S3TAI][S6]
* Agregado CustomItemBow, CustomItemGlove. [S6]

* Modificado el archivo "Data\Skill\EffectList.txt" se modificaron la estructura del archivo. [S0][S2][S2PHI][S3][S3TAI][S6]

UPDATE 20:
* Nuevo archivo para configurar la barra de vide de los mobs en "Data\Custom\CustomHealthBar.txt" [S0][S2][S2PHI][S3][S3TAI][S6].
* Nuevo archivo para configurar los daños PvM y MvP en "Data\Util\DamageTable.txt" [S0][S2][S2PHI][S3][S3TAI][S6].
* Nuevo archivo para configurar los ItemOptionRate por mapas en "Data\MapRateInfo.txt" [S0][S2][S2PHI][S3][S3TAI][S6].

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Character.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (XXReflectDamageRateToXX)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Character.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (XXReflectDamageRateToXX)

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Common.dat". [S2][S2PHI][S3][S3TAI][S6] (ShieldGaugeDisable,ExperienceRandomAditional)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [S2][S2PHI][S3][S3TAI][S6] (ShieldGaugeDisable,ExperienceRandomAditional)

* Nueva opcion agregada en "GameServer\DATA\GameServerInfo - Command.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CommandResetPointRateXX,CommandMasterResetPointRateXX)
* Nueva opcion agregada en "GameServerCS\DATA\GameServerInfo - Command.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CommandResetPointRateXX,CommandMasterResetPointRateXX)

* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Item.dat" para configurar rates de drinks. [S0][S2][S2PHI][S3][S3TAI][S6] (AleIncSpeed,AleIncSpeedTime,OliveOfLoveIncSpeed,OliveOfLoveIncSpeedTime,RemedyOfLoveIncDamage,RemedyOfLoveIncDamageTime,PotionOfBlessConstA,PotionOfBlessConstB,PotionOfBlessTimeConstA,PotionOfSoulConstA,PotionOfSoulConstB,PotionOfSoulTimeConstA)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Item.dat" para configurar rates de drinks. [S0][S2][S2PHI][S3][S3TAI][S6] (AleIncSpeed,AleIncSpeedTime,OliveOfLoveIncSpeed,OliveOfLoveIncSpeedTime,RemedyOfLoveIncDamage,RemedyOfLoveIncDamageTime,PotionOfBlessConstA,PotionOfBlessConstB,PotionOfBlessTimeConstA,PotionOfSoulConstA,PotionOfSoulConstB,PotionOfSoulTimeConstA)

* Nuevas opciones agregada en "GameServer\DATA\GameServerInfo - Skill.dat". [S2][S2PHI][S3][S3TAI][S6] (InfinityArrowSwitch_ALX)
* Nuevas opciones agregada en "GameServerCS\DATA\GameServerInfo - Skill.dat". [S2][S2PHI][S3][S3TAI][S6] (InfinityArrowSwitch_ALX)

* Arreglado el problema del Dark Horse que no daba rango adicional para atacar. [S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglado el problema del PK Assasing en personas que no poseen la actualizacion 1.0.1.9.
* Arreglado el problema de la puerta irrompible del Blood Castle 1. [S0].
* Arreglado el problema del boss indestructible con CustomMonsters en InvasionManager.[S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglado el problema de experiencia 0 en versiones con pocas experiencia. [S0][S2][S2PHI][S3][S3TAI]
* Arreglado el problema de que no andaban los rangos de niveles en itembagsex. [S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglado el error de lectura de InfinityArrowTimeConstA. [S2][S2PHI][S3][S3TAI][S6]
* Arreglado el sign en el select char de los personajes desde "Data\Util\GameMaster.txt". S0][S2][S2PHI][S3][S3TAI][S6]

* Modificada la opcion Invisibility de "Data\Util\GameMaster.txt" [S0][S2][S2PHI][S3][S3TAI][S6] (Invisibility ~ 0: Normal | 1: Invisible for all mobs | 2: Invisible for all players and mobs)

* Modificado el archivo "Data\Item\SetItemOption.txt" se modificaron la estructura del archivo. [S0][S2][S2PHI][S3][S3TAI]
* Modificado el archivo "Data\Item\SetItemType.txt" se modificaron la estructura del archivo. [S0][S2][S2PHI][S3][S3TAI]
* Modificado el archivo "Data\Quest\QuestObjective.txt" se modificaron la estructura del archivo. [S0][S2][S2PHI][S3][S3TAI][S6]
* Modificado el archivo "Data\QuestWorld\QuestWorldObjective.txt" se modificaron la estructura del archivo. [S6]
* Modificado el archivo "Data\QuestWorld\QuestWorldReward.txt" se modificaron la estructura del archivo. [S6]

* Mejorado el rendimiento de los archivos en ejecucion. [S0][S2][S2PHI][S3][S3TAI][S6]
* Se mejoro el Invasion Manager detectando el efecto en cada mapa dependiendo la cantidad de bosses. [S0][S2][S2PHI][S3][S3TAI][S6]
* Se agrego el mapa donde se mato a cada boss en Invasion Manager agregando "%s" al mensaje de BossKilledMessage. [S0][S2][S2PHI][S3][S3TAI][S6]

UPDATE 19:
* Nuevo archivo "Data\PKManager.txt" para configurar opciones de PK. [S0][S2][S2PHI][S3][S3TAI][S6]
* Nuevo archivo "Data\Item\WingOption.txt" para configurar opciones de alas. [S0][S2][S2PHI][S3][S3TAI][S6]
* Nuevas mensajes en el archivo "Data\Message.txt" [S0][S2][S2PHI][S3][S3TAI] (269, and 687 ~ 701)

* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Custom.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CustomAttackAutoPotionDelay)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Custom.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CustomAttackAutoPotionDelay)
* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Character.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (ComboDamageConstC) para calcular el % de daño del combo
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Character.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (ComboDamageConstC) para calcular el % de daño del combo

* Opciones eliminadas en "GameServer\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (PKDeathAnnounce,PKLimitBuyInShops,PKLimitEventEntry,PKLimitFree,PKLimitMove,PKLimitMoveSummon)
* Opciones eliminadas en "GameServerCS\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (PKDeathAnnounce,PKLimitBuyInShops,PKLimitEventEntry,PKLimitFree,PKLimitMove,PKLimitMoveSummon)

* Modificado el archivo "Data\CommandManager.txt" se modificaron la estructura del archivo. [S0][S2][S2PHI][S3][S3TAI][S6]
* Nuevos comandos agregados en "Data\CommandManager.txt" (Rename,DCFriend,BanChat,UnBanAcc,UnBanChar,UnBanChat,BCStart,DSStart,CCStart,ITStart) [S0][S2][S2PHI][S3][S3TAI][S6]

* Arreglado el problema del anillo Wizards Ring. [S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglado el problema de multiples renas registradas. [S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglado el problema de stackear items y quedaban en el inventario. [S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglado el problema del Horse de DL. [S3][S3TAI]
* Arreglados los problemas con las entradas CC. [S0]
* Arreglados mensajes extraños en el ejecutable GameServer de CastleSiege. [S0][S2][S2PHI][S3][S3TAI][S6]

* Agregado el tiempo entre posiones en CustomAttack. [S0][S2][S2PHI][S3][S3TAI][S6]
* Agregado script SQL para el comando rename en "Databases\1 - (1.0.1.9) WZ_RenameCharacter.sql". [S0][S2][S2PHI][S3][S3TAI][S6]
* Agregado script SQL para el comando banacc en "Databases\2 - (1.0.1.9) WZ_BanAccount.sql". [S0][S2][S2PHI][S3][S3TAI][S6]
* Agregado script SQL para el comando banchar en "Databases\3 - (1.0.1.9) WZ_BanCharacter.sql". [S0][S2][S2PHI][S3][S3TAI][S6]

UPDATE 18:

* Arreglado el problema de las Jewels en CustomStore. [S0][S2][S2PHI][S3][S3TAI]

* Agregado el comando Pack(61) Unpack(62). "Data\CommandManager.txt" [S0][S2][S3][S3TAI][S6]

* Nuevas opciones en el archivo "Data\Message.txt" [S0][S2][S2PHI][S3][S3TAI] (387 a 392)
* Nuevas opción agregada  en "Data\Util\GameMaster.txt" [S0][S2][S2PHI][S3][S3TAI][S6] (Invisibility ~ 0: Normal | 1: Invisible for all players | 2: Invisible for all players and mobs)
* Nuevas opciones agregadas en "GameServer\DATA\GameServerInfo - Custom.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CustomJewelPackSwitch,CustomJewelPackCommandJoBSyntax,CustomJewelPackCommandJoSSyntax,CustomJewelPackCommandJoLSyntax,CustomJewelPackCommandJoCSyntax,CustomJewelPackCommandJCRSyntax,CustomJewelPackCommandJoGSyntax,CustomJewelPackCommandJoHSyntax)
* Nuevas opciones agregadas en "GameServerCS\DATA\GameServerInfo - Custom.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CustomJewelPackSwitch,CustomJewelPackCommandJoBSyntax,CustomJewelPackCommandJoSSyntax,CustomJewelPackCommandJoLSyntax,CustomJewelPackCommandJoCSyntax,CustomJewelPackCommandJCRSyntax,CustomJewelPackCommandJoGSyntax,CustomJewelPackCommandJoHSyntax)
* Nuevas opción agregada en "GameServer\DATA\GameServerInfo - Skill.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CastleSiegeSkillForAllMaps)
* Nuevas opción agregada en "GameServerCS\DATA\GameServerInfo - Skill.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (CastleSiegeSkillForAllMaps)

* Soporte de usar skills castle siege en todos los mapas. [S0][S2][S2PHI][S3][S3TAI][S6]

UPDATE 17-1:
* Nuevas opciones agregada en "GameServer\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (DSJSMaxPacketTime,DSJSMaxPacketCount)
* Nuevas opciones agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (DSJSMaxPacketTime,DSJSMaxPacketCount)

* Arreglado el problema de comercio de dinero. [S0][S2]
* Arreglado el problema en CustomNPCCollector y CustomNPCMove que no funcionaba con CustomsNpcs. [S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglado el problema de los nombres incompletos en los requisitos de las quest. [S0][S2][S2PHI][S3][S3TAI][S6]
* Arreglado el problema del tiempo en las entradas del blood castle 7. [S0][S2][S2PHI][S3]
* Arreglado el problema de la High Refinery. [S3]

* Removida la healthbar de los personajes. [S0][S2][S2PHI][S3][S3TAI][S6]

MHPServer:
* Arreglado el problema de desconexiones.

UPDATE 17:
* Nuevo archivo "Data\PKItemDrop.txt" para configurar mas a fondo el PKItemDrop. [S0][S2][S2PHI][S3][S3TAI][S6]

* Nuevas opciones agregada en "GameServer\DATA\GameServerInfo - Common.dat". [S6] (HelperActiveMaxTime_ALX)
* Nuevas opciones agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [S6] (HelperActiveMaxTime_ALX)
* Nuevas opciones agregada en "GameServer\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (VerifyTimeOutDisconnect,CheckHackSkillAction,CheckSpeedHackAction,CheckLatencyHackAction,CheckAutoPotionHackAction,CheckAutoComboHackAction,CheckMoveHackAction)
* Nuevas opciones agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (VerifyTimeOutDisconnect,CheckHackSkillAction,CheckSpeedHackAction,CheckLatencyHackAction,CheckAutoPotionHackAction,CheckAutoComboHackAction,CheckMoveHackAction)
-- 0: No hacer nada | 1: Desconectar | 2: Bloquear IP, HardWareId y Cerrar Cliente.

* Modificado el archivo "Data\Item\Item.txt" se modificaron Todos los sets lucky con sus variables originales (Set 62 a Set 72). [S6]
* Modificado el formato ItemIndex de CustomWing.txt a "00,000". [S0][S2][S2PHI][S3][S3TAI][S6]

* Mejorado el rango MoneyDropRate de MapManager.txt. [S0][S2][S2PHI][S3][S3TAI][S6]

* Arreglado el problema de la masterlevel experience no mostrada. [S6]

* Agregado mensaje al alcanzar el limite máximo del SeniorMix. [S0][S2][S2PHI][S3][S3TAI][S6]
* Agregados nuevos sistemas de seguridad en ConnectServer, MHPServer y GameServer. [S0][S2][S2PHI][S3][S3TAI][S6]

* Opciones removidas de [GameServer\DATA\GameServerInfo - Common.dat] [S0][S2][S2PHI][S3][S3TAI][S6] (MainChecksum,PKItemDropMinPKLevel,PKItemDropRateOnMonsterKill_ALX,PKItemDropRateOnPlayerKill_ALX,PKItemDropItemMinLevel,PKItemDropItemMaxLevel,PKItemDropCanDropExc,PKItemDropCanDropSet,PKItemDropCanDropHarmony,PKItemDropCanDrop380,PKItemDropCanDropSocket)
* Opciones removidas de [GameServerCS\DATA\GameServerInfo - Common.dat] [S0][S2][S2PHI][S3][S3TAI][S6] (MainChecksum,PKItemDropMinPKLevel,PKItemDropRateOnMonsterKill_ALX,PKItemDropRateOnPlayerKill_ALX,PKItemDropItemMinLevel,PKItemDropItemMaxLevel,PKItemDropCanDropExc,PKItemDropCanDropSet,PKItemDropCanDropHarmony,PKItemDropCanDrop380,PKItemDropCanDropSocket)

ConnectServer:
* Nuevas opciones agregada en "ConnectServer\ConnectServer.ini". [S0][S2][S2PHI][S3][S3TAI][S6] (TimeoutDisconnect,VerifyTimeoutDisconnect)

MHPServer:
* Nuevas opciones agregada en "MHPServer\MHPServer.ini". [S0][S2][S2PHI][S3][S3TAI][S6] (TimeoutDisconnect,VerifyTimeoutDisconnect)

Clientes:
* Arreglado el problema crasheo en el cliente al mover un ítem a la maquina chaos. [S0][S2][S2PHI][S3][S3TAI]
* Agregado el TrayMode Tecla F12. [S2][S2PHI][S3][S3TAI][S6]
* Agregada la opciones de cambio de fuente en el cliente.  [S0][S2][S2PHI][S3][S3TAI][S6]

UPDATE 16-1:
* Nuevas opciones agregada en "GameServer\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (ServerDebugger)
* Nuevas opciones agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [S0][S2][S2PHI][S3][S3TAI][S6] (ServerDebugger)

* Agregado soporte para Season 2 Episode 1 Philipine. [S2PHI]
* Agregada consola debug para SkillHack, MoveHack, Ítems inválidos y Monsters inválidos. [BETA] [S0][S2][S2PHI][S3][S3TAI][S6]
* Agregado se muestra la experiencia siempre aunque se suba de nivel. [S0][S2][S2PHI][S3][S3TAI][S6]

* Arreglado el problema de los ítems 380 que no aplicaba el daño. [S2][S3][S3TAI][S6]
* Arreglado el problema del viewport en s2 al recibir veneno. [S2]

Cliente:
* Agregado Right Clic Ítem Move en Main. [S0][S2][S3][S3TAI]

* Arreglado el problema de las jewels Lower Refining Stone y Higher Refining Stone. [S2][S3][S3TAI]
* Arreglado el problema de la Jewel of Creation que tildaba el cliente. [S0]
* Arreglado el problema de la puerta del Castle Siegue. [S6]
* Arreglado el problema del Death Stab que no mostraba el efecto. [S0]

UPDATE 16:
* Arreglado el problema de PKItemDrop que en ciertas ocasiones dropeba ítem con 0 probabilidad. [S0][S2][S3][S3TAI][S6]
* Arreglado el crash del GMPost al usar %s
* Arreglado el CashShop no restaba puntos al comprar.[S3TAI]
* Arreglado el problema que no se borraban las entradas a eventos con durabilidad (BC, CC, DS y IT)
* Arreglado el problema del castle siege de procedimientos erróneos.
* Arreglado el problema de fechas incorrectas en MUCASTLE_DATA. [S0][S2][S3][S3TAI][S6]
* Arreglado el problema de CustomMonster en NPC con barra de vida. [S0][S2][S3][S3TAI][S6]

* Agregado soporte para CustomJewel en "Data\Custom\CustomJewel.txt" [S0][S2][S3][S3TAI]
* Agregada la opción gate en "Data\Custom\CustomMove.txt" [S0][S2][S3][S3TAI][S6]
* Agregada la opción gate en "Data\Custom\CustomNPCMove.txt" [S0][S2][S3][S3TAI][S6]
* Agregada la recompensa de custom arena en "Data\EventItemBag\Custom Arena.txt". [S0][S2]
* Agregado el comando MakeSet(120). "Data\CommandManager.txt" [S0][S2][S3][S3TAI][S6]
* Agregado soporte para recarga automática de flechas durante el attack y offattack. [S0][S2][S2PHI][S3][S3TAI][S6]
* Agregada nueva opción para no descontar flechas. [S0]

* Nuevas opciones agregada en "GameServer\DATA\GameServerInfo - Skill.dat". [S0] (InfinityArrowSwitch_ALX)
* Nuevas opciones agregada en "GameServerCS\DATA\GameServerInfo - Skill.dat". [S0](InfinityArrowSwitch_ALX)

* Modificado el archivo "Data\Custom\CustomJewel.txt" se modificaron opciones para cada version. [S0][S2][S3][S3TAI][S6]
* Modificado el archivo "Data\Event\InvasionManager.dat" se borro la invasión 5 correspondiente a un mob invalido. [S3TAI]
* Modificado el archivo "Data\Monster\MonsterList.txt" se borro el mobs 413 no correspondiente a esta versión. [S3TAI]
* Modificado el archivo "Data\EventItemBagManager.txt" para soportar recompensa CustomArena. [S0][S2]
* Modificado el archivo "GameServer\DATA\GameServerInfo - Character.dat" agregada la linea "CustomArenaDamageRate". [S0][S2]

Cliente:
* Agregado soporte para Custom Jewel en Main. [S0][S2][S3][S3TAI]
* Agregado soporte para Custom Tool Tip en Main.  [S0][S2][S3][S3TAI]
* Agregado soporte para ItemStack e ItemValue en Main.  [S0][S2][S3][S3TAI][S6]
* Agregado AttackAnimationValue en GetMainInfo.  [S0][S2][S3][S3TAI][S6]
* Agregado soporte multilenguage en CustomMessage de GetMainInfo.  [S0][S2][S3][S3TAI]

UPDATE 15:
* Arreglado el crash o interface invisible en resoluciones HD en Kanturu. [S2][S3][S3TAI]
* Arreglado el CustomMap en main.dll. [S6]
* Arreglado el contador de gameserver en ConnectServer al recargar. [S0][S2][S3][S3TAI][S6]

UPDATE 14-1:
* Agregados los logs al comprar en CashShop. [S3TAI]
* Agregado el comando "!" para gamemasters. [S0][S2][S3][S3TAI][S6]

* Arreglada la creación de Mobs de InvasionManager en SafeZone. [S0][S2][S3][S3TAI][S6]
* Arreglado el Skill Invisible. [S0][S2][S3][S3TAI][S6]
* Arreglado el error de dupeo por maquina chaos. [S0][S2][S3][S3TAI][S6]
* Arreglados los daños erróneos en main. [S0][S2][S3][S3TAI]

* Mejorado el Skill invisible en GameMasters. [S0][S2][S3][S3TAI][S6]

* Agregada la reducción de consumo de CPU en main. [S0][S2][S3][S3TAI]
* Agregado sistema de Minimap en main. [S0][S2][S3][S3TAI]

UPDATE 14:
* Nuevas opciones agregada en "GameServer\DATA\GameServerInfo - Common.dat". [S0][S2][S3][S3TAI][S6] (ExperienceMultiplierConstA,ExperienceMultiplierConstB,MaxLevel,DuelMaxTime)
* Nuevas opciones agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [S0][S2][S3][S3TAI][S6] (ExperienceMultiplierConstA,ExperienceMultiplierConstB,MaxLevel,DuelMaxTime)

* Nuevo archivo "Data\Custom\CustomArena.txt". [S0]
* Nuevo archivo "Data\Terrain\Terrain41.att" [S0][S2]

* Modificado el archivo "Data\MapManager.txt". [S0][S2] (Agregados el mapa index 40)

* Modificado el archivo "Data\Event\EventEntryLevel.dat". [S0][S2][S3][S3TAI][S6] (Cambiado los MaxLevel 400 por *)
* Modificado el archivo "Data\Move\Gate.txt". [S0][S2][S3][S3TAI][S6] (Cambiado los MaxLevel 400 por *)
* Modificado el archivo "Data\Move\Move.txt". [S0][S2][S3][S3TAI][S6] (Cambiado los MaxLevel 400 por *)
* Modificado el archivo "Data\Move\MoveSummon.txt". [S0][S2][S3][S3TAI][S6] (Cambiado los MaxLevel 400 por *)
* Modificado el archivo "Data\Quest\Quest.txt". [S0][S2][S3][S3TAI][S6] (Cambiado los MaxLevel 400 por *)
* Modificado el archivo "Data\QuestWorld\QuestWorld.txt". [S6] (Cambiado los MaxLevel 400 por *)

* Agregado el mapa 40 a main. [S0][S2]
* Agregado sistema de reconnexion en main. [S0][S2]

* Arreglado el Ctrl Fix en Main. [S0]
* Arreglada la Jewel of Elevation. [S6]
* Arreglado el Party Bug al morir. [S0][S2][S3][S3TAI][S6]
* Arreglado el problema de reparar items con trade. [S0][S2][S3][S3TAI][S6]
* Arreglado el problema de zen en trade. [S0][S2][S3][S3TAI][S6]
* Arreglado el CustomMove, CustomNPCMove y otros moves en safezone. [S0][S2][S3][S3TAI][S6]

UPDATE 13:
* Arreglado el problema al coronar Castle Siege. [S0][S2]
* Arreglado el error WZ_CW_InfoLoad y WZ_CW_SaveLoad. [S2]
* Arreglado el sistema de terrenos. [S0][S2][S3][S3TAI][S6]
* Arreglado el disconnect del MG con Evil Spirit. [S3 TAI]
* Arreglado el problema del dragón derecho usando interface downgrade en Kanturu Event,Crywolf y Castle Siege. [S6]
* Arreglado el problema que no se mostraba precio en tiendas coins. [S3][S3TAI]

* Agregado el menú de desconexión y baneo de cuentas y personajes en gameserver. [S0][S2][S3][S3TAI][S6]

* Eliminados todos los terrain, sincronizados los nuevos. [SE DEBE ELIMINAR TERRAINS ANTERIORES] [S0][S2][S3][S3TAI][S6]
* Eliminado el sistema de launcher del connectserver. [S0][S2][S3][S3TAI][S6]

* Modificado el archivo "Data\Hack\HackPacketCheck.txt". [S3TAI]
* Modificado el archivo "Data\Item\Item.txt". [S3TAI]
* Modificado el archivo "Data\Event\CastleSiege.dat" con sus valores reales. [S0][S2][S3][S3TAI]

UPDATE 12:
* Implementado el sistema de reconnect.[S3][S3TAI]
* Implementada las opciones de forzar el inicio de las invasión. [S0][S2][S3][S3TAI][S6]

* Nuevo archivo "Data\EventItemBag\Chaos Card Mix 1.txt" [S3TAI]
* Nuevo archivo "Data\CashShop.txt" [S3TAI]

* Nueva opción agregada en "GameServer\DATA\GameServerInfo - Common.dat". [S2][S3][S3TAI] (SetItemAcceptHarmonySwitch,AutomaticLockSwitch)
* Nueva opción agregada en "GameServerCS\DATA\GameServerInfo - Common.dat". [S2][S3][S3TAI] (SetItemAcceptHarmonySwitch,AutomaticLockSwitch)
* Modificado el archivo con ítems nuevos "Data\Item\Item.txt" [S3TAI]
* Nuevas opciones en el archivo "ConnectServer\ConnectServer.ini" [S0][S2][S3][S3TAI][S6]

* Arreglado la posibilidad de sacar el Harmony en ítems ancient desde el NPC. [S2][S3][S3TAI]
* Arreglado el problema del Seal of Movement que no dejaba warpear gratis. [S3TAI][S6]
* Arreglado el problema de los MISS en PVP. [S0]
* Arreglado el problema de la healthbar que mostraba las traps invisibles. [S0][S2][S3][S3TAI][S6]
* Arreglado el problema del GateKeeper.[S6]
* Arreglado el problema de la alianza desconectaba. [S2]
* Arreglado los mensajes erróneos en Guild Alianza y Hostilidad. [S0][S2][S3][S3TAI][S6]
* Arreglado el error del menú escape con trade. [S2][S3][S3TAI]

* Mejorado el autobloqueo en ConnectServer, MHPServer y GameServers. [S0][S2][S3][S3TAI][S6]
* Mejorado el sistema de WindowMode incluido minimizador (Tecla F12). [S0]
* Mejorado el sistema de efectos. [S0][S2][S3][S3TAI]
* Mejorado el activador/desactivador de MonsterBar (Tecla F8). [S0][S2][S3][S3TAI][S6]

UPDATE 11:
* Nuevas opciones en el archivo "Data\CommandManager.txt" [S0][S2][S3][S6]
* Nuevas opciones en el archivo "Data\Message.txt" [S0][S2][S3][S6]
* Nuevas opciones en el archivo "GameServer\DATA\GameServerInfo - Common.dat" [S0][S2][S3][S6]
* Nuevas opciones en el archivo "GameServerCS\DATA\GameServerInfo - Common.dat" [S0][S2][S3][S6]

* Nuevos comandos agregados /banacc y /banchar. [S0][S2][S3][S6]
* Nuevos comandos agregados /buyvip configuracion en "Data\Custom\CustomBuyVip.txt". [S0][S2][S3][S6]

* Agregado sistema para deshabilitar caída de ítems quest en party y conteo de monstruos. [S0][S2][S3][S6]
* Agregado sistema ItemLoot para evitar que otros personajes tomen ítems de otros personajes. Config en "Data\Item\ItemLoot.txt" [S0][S2][S3][S6]

* Arreglado el sistema de bonusmanager al OnlineReward.txt. [S0][S2][S3][S6]
* Arreglado el problema del party con un solo miembro en PartyReconnect 0. [S0][S2][S3][S6]
* Arreglado el problema de los ítems quest que podían ser agarrado por cualquier personaje.  [S0][S2][S3][S6]

UPDATE 10:
* Nuevo archivo agregado en "Data\Util\OnlineReward.txt" [S3][S6]
* Agregada la posibilidad de respawnear donde se muere o al reloguear desde MapManager.
* Agregadas opciones el comando /make 380 y JoH
* Arreglado el problema de la invasionmanager que no respawnean.
* Arreglado el problema de los ítems SET en los shops.
* Arreglado el problema de Kanturu Hands.
* Arreglado el problema de la vida de los mobs, puertas y estatuas en castle siege.

UPDATE 9:
* Arreglado el problema de la opción de matarse entre miembros de la misma party.
* Arreglado el problema al apagar el cashshop no funcionaban las tiendas por coins.

* Nuevas opciones en el archivo "Data\MapManager.txt" [S0][S2][S3][S6]

* Agregado el healthbar al main s0.
* Agregados los puntos en tiempo real en s2.

UPDATE 8:
* Nuevo formato en el archivo "Data\Item\ExcellentOptionRate.txt" [S0][S3][S6]
* Nuevo formato en el archivo "Data\Item\ItemOption.txt" [S0][S3][S6]

* Arreglo en problemas varios en MAIN [S3][S6]

UPDATE 7:
* Nuevas opciones en el archivo "GameServer\DATA\GameServerInfo - Common.dat" [S3][S6]
* Nuevas opciones en el archivo "GameServerCS\DATA\GameServerInfo - Common.dat" [S3][S6]

* Agregado el Golden Archer en "Data\Monster\Spawn\002 - Devias.txt" [S3]
* Agregado el Golden Archer en "Data\Monster\Spawn\003 - Noria.txt" [S3]
* Agregado el empuje en el combo al morir el oponente. [S3][S6]
* Agregado el empuje en el combo al morir los mobs. [S3][S6]
* Agregada la opción a PKItemDrop de ítems 380. [S3][S6]

* Arreglada las opciones CommandMasterResetKeepX [S3][S6]
* Arreglado el problema de CustomGift en DataServer y GameServer. [S3][S6]
* Arreglado el problema de mensajes confusos al dropear cajas. [S3]
* Arreglado el mensaje erróneo en el castle siege. [S3][S6]
* Arreglado el master level del dark horse que no aplicaba defensa. [S6]
* Arreglado el drop de ítems en duelo. [S3]

* Arreglos en "Data\Character\DefaultClassFreebies.txt" [S3]
* Arreglos en "Data\Custom\CustomJewel.txt" [S3]
* Arreglos en "Data\EventItemBag\Game Master Box.txt" [S3]
* Arreglos en "Data\EventItemBag\Silver Medal.txt" [S3]
* Arreglos en "Data\EventItemBag\Gold Medal.txt" [S3]
* Arreglos en "Data\EventItemBag\Leo the Helper.txt" [S3]

* Agregada nueva tabla en base de datos (EventGoldenArcher,GiftData). [S3][S6]
* Procedimientos modificados en base de datos (WZ_DeleteCharacter,WZ_RenameCharacter). [S3][S6]
* Eliminada la tabla (GiftSystem). [S3][S6]

* Cambio de color en la UI de las aplicaciones.

UPDATE 6:
* Agregado el NPC Mirage en "Data\Monster\Spawn\000 - Lorencia.txt" [S3]
* Arreglados el sistema de skill de Illusion Temple. [S3][S6]
* Arreglado Skill Nova [S3]
* Arreglado problemas de texturas. [S3]
* Arreglado ítems brillantes raros. [S3]

* Removido CustomJewel. [S3]

UPDATE 5:
* Agregado sistema de ConnectMember en "Data\ConnectMember.txt" [S3][S6]
* Agregado sistema de ComboSkill en "Data\Skill\ComboSkill.txt" [S3][S6]
* Agregado soporte al ConnectServer para hacer auto update. [S3][S6]

* Nuevas opciones en el archivo "GameServer\DATA\GameServerInfo - Character.dat" [S3][S6] 
* Nuevas opciones en el archivo "GameServerCS\DATA\GameServerInfo - Character.dat" [S3][S6]
* Nuevas opciones en el archivo "GameServer\DATA\GameServerInfo - Common.dat" [S3][S6]
* Nuevas opciones en el archivo "GameServerCS\DATA\GameServerInfo - Common.dat" [S3][S6]

* Arreglado el problema de conteo de ítem en QuestWorld. [S6]

UPDATE 4:
* Agregado sistema de uso de skills en zona safe "Data\Skill\SkillUseArea.txt"
* Agregado sistema Monster Move en "Data\Monster\MonsterMove.txt"
* Agregados los nuevos sistemas de recompensa a QuestWorld.

* Sincronizados los terrenos con los terrenos del cliente.

* Arreglado el problema del comando /edit
* Arreglado el drop de la estatua en blood castle.
* Arregladas las misiones de Wandering Merchant Zyro.
* Arreglado el sistema de Quest filtrado por mapnumber.
* Arreglado el sistema de QuestWorld filtrado por mapnumber.

* Removidos los bots (buffer y trader)
* Removido el sistema de "Data\Monster\MarlonSpawn.txt"

UPDATE 3:
* Agregado el sistema de bloqueo automático.

* Arreglado el problema de desconexión al hacer Reload Common.
* Arreglado el problema de mobs creados en zonas safe.
* Arreglado el problema de los PCPoints/Coins por tiempo.

* Nuevas opciones en el archivo "GameServer\DATA\GameServerInfo - Event.dat"
* Nuevas opciones en el archivo "GameServer\DATA\GameServerInfo - Common.dat"

* Modificado el comando makemob Modo1: /makemob <mob> Modo2: /makemob <-mob> <map> <y> <x>

* Mejorado el comando /setcoin
* Mejorado el comando /edit

UPDATE 2:
* Agregada la opción de recuperación de vida del Kundum al fallar Crywolf.
* Agregado el sistema de venta en shops por coins.

* Agregado el archivo "Data\Custom\CustomMasterResetReward.txt"

* Arreglado el problema del SeniorMix en Season3.
* Arreglado el problema de carga de terrenos inválidos.
* Arreglado el problema de carga de mobs en mapas inválidos.
* Arreglado el problema de múltiples shops con diferente archivo.
* Arreglado el problema del offhelper de reparación automática.
* Arreglado el problema del offhelper del heal constante.

* Eliminado el archivo "Data\Custom\CustomRespawn.txt"
* Eliminado el archivo "Data\Custom\CustomSpawn.txt"

* Nuevas opciones en el archivo "Data\MapManager.txt"
* Nuevas mensajes en el archivo "Data\Message.txt"
* Nuevas opciones en el archivo "Data\ShopManager.txt"
* Nuevas opciones en el archivo "GameServer\DATA\GameServerInfo - Event.dat"
* Nuevas opciones en el archivo "GameServer\DATA\GameServerInfo - Common.dat"

* Removido el GameServerCS.
* Removidas cosas innecesarias de "ConnectServer\ServerList.dat"
* Removidas cosas innecesarias de "Data\MapServerInfo.txt"