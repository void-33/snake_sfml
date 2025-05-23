# Snake Game (C++ & SFML)

A simple snake game built using C++ and the [SFML](https://www.sfml-dev.org/) multimedia library.

---

## 🔧 Requirements

- A C++17-compatible compiler (e.g., `g++`)
- SFML **version 2.6.1**
- `make` utility (or compile manually)

---

## 🚀 Installation & Building

This project uses a `Makefile` for simplified compilation.

### 🛠️ Installing SFML

You can install the SFML 2.6.1 library using tools such as **MSYS2**, or manually from the **official SFML website**:

#### ✅ Option 1: Using MSYS2 (UCRT64 environment)

```bash
pacman -S mingw-w64-ucrt-x86_64-sfml
```
⚠️ This works for SFML 2.6.1 and assumes you're using the UCRT64 terminal. Adjust the package or environment based on your setup.

---

#### 📦 Option 2: Manually from Official Website
- Go to SFML Downloads.
- Download the SFML 2.6.1 version . Also make sure your compiler version exactly matches as included in the docs itself.(e.g., GCC 13.1.0 for SMFL 2.6.1).
- Extract the archive.
- Copy the following folders into your project directory:
  - `bin/` (contains required .dll files)
  - `include/` (for headers)
  - `lib/` (for linking)
- The default Makefile should work fine. Optionally, you may need update it as well.
   
---
 
### ⚙️ Building the Project

Once SFML is set up:

```bash
make
```
This will compile the game into the bin/ directory.
Then run:
```bash
./bin/app.exe
```
---

### ⚠️ Important Notes
- Your compiler version must match exactly with the version SFML was built for.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Example: If using SFML built for GCC 13.1.0, then g++ must also be 13.1.0.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Version mismatches can cause runtime or linking errors.

- This project currently works with SFML 2.6.1.

- It may not work with newer versions of SFML due to potential breaking changes.

- Always refer to the official SFML documentation for the latest and most accurate guidance:

📚 https://www.sfml-dev.org/documentation/2.6.1/

---

### 🎮 Gameplay
Classic snake mechanics:

- Move with arrow keys

- Eat food to grow

- Avoid crashing into yourself or the walls

### Enjoy the game! 🐍
