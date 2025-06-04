# PGCCHIB - Processamento Gráfico (EM ANDAMENTO)

Este repositório é baseado na estrutura do projeto PGCCHIB - Processamento Gráfico, fornecido pela disciplina de Processamento Gráfico. 

![image](https://github.com/user-attachments/assets/b27f999e-ff81-4394-9637-3ade365d57ed)

## 🛠️ Como compilar e rodar o projeto (PowerShell - Windows)
![image](https://github.com/user-attachments/assets/c75dd07f-e1f1-47de-a9da-1b1cd1c46849)

- Deletar a pasta build (limpar cache/configurações antigas)
- Criar a pasta build
- Entrar na pasta build
- Rodar o CMake apontando o toolchain do vcpkg, com a arquitetura (ex: -A x64)
- Buildar o projeto (cmake --build .)
- Rodar o executável gerado, que normalmente fica em build\Debug\

## Exemplo Manual
- mkdir build
- cd build
- cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/Users/raian/OneDrive/Desktop/vcpkg/scripts/buildsystems/vcpkg.cmake" -A x64
- cmake --build . 
- .\build\Debug\MultiTexturedSprites.exe

## ⚠️ **IMPORTANTE: Baixar a GLAD Manualmente**
Para que o projeto funcione corretamente, é necessário **baixar a GLAD manualmente** utilizando o **GLAD Generator**.

### 🔗 **Acesse o web service do GLAD**:
👉 [GLAD Generator](https://glad.dav1d.de/)

### ⚙️ **Configuração necessária:**
- **API:** OpenGL  
- **Version:** 3.3+ (ou superior compatível com sua máquina)  
- **Profile:** Core  
- **Language:** C/C++  

### 📥 **Baixe e extraia os arquivos:**
Após a geração, extraia os arquivos baixados e coloque-os nos diretórios correspondentes:
- Copie **glad.h** para `include/glad/`
- Copie **khrplatform.h** para `include/glad/KHR/`
- Copie **glad.c** para `common/`

🚨 **Sem esses arquivos, a compilação falhará!** É necessário colocá-los nos diretórios corretos conforme a orientação acima.

---



