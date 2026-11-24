# Diretrizes do Workspace - Sistema Estelar v2

## Ambiente e Compilador
- **Sistema Operacional:** Windows
- **Compilador:** `g++` versão 15.2.0 (MSYS2 UCRT64)
- **Caminho do Compilador:** `C:/msys64/ucrt64/bin/g++.exe`
- **Depurador:** GDB (`C:/msys64/ucrt64/bin/gdb.exe`)

## Padrão e Compatibilidade de Código
- **Padrão Obrigatório:** Estritamente `-std=c++17`.
- **Portabilidade:**
  - **PROIBIDO** o uso de bibliotecas, cabeçalhos ou diretivas exclusivas do MSVC (ex.: `<stdafx.h>`, `<conio.h>`, `#pragma comment`, etc.).
  - Utilizar exclusivamente a biblioteca padrão do C++17 (`<iostream>`, `<vector>`, `<string>`, `<memory>`, `<algorithm>`, `<filesystem>`, `<random>`, etc.) e código compatível com GCC/MinGW-w64.
  - Manter conformidade com `-pedantic`.
  - Todo código gerado ou refatorado deve compilar limpo com as flags:
    `-std=c++17 -Wall -Wextra -pedantic`

## Estrutura do Projeto e Organização de Arquivos
- **Cabeçalhos (`include/`):** Espelham exatamente as subpastas de `src/`. Todos os `#include` devem partir da raiz de `include/` (ex.: `#include "fisica/fisica_estelar.hpp"`).
- **Subpastas de Domínio:**
  - `nucleo/`: Estruturas de dados fundamentais e constantes (`constantes.hpp`, `estrela`, `orbita`, `subsistema_no`).
  - `fisica/`: Cálculos astrofísicos e estabilidade (`fisica_estelar`, `estabilidade_orbital`).
  - `geracao/`: Lógica procedural e classificação (`gerador_aleatorio`, `gerador_sistema`, `classificador_wds`).
  - `interface/`: Menus e exibição de resultados (`menu_gerador`, `menu_saves`, `menus`, `terminal_utils`, `visualizador_sistema`).
- **Build:** Qualquer nova subpasta criada em `src/` deve ser adicionada manualmente em `.vscode/tasks.json`.
