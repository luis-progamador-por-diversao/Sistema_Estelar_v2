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

