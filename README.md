
# ⚡synd3 — lightweight process manager for Linux (CLI + ncurses)

**synd3** is a lightweight and fast CLI tool for process management. It provides a simple ncurses-based interface to monitor and terminate processes with just a few keystrokes.




## 📸Screenshots

![App Screenshot](https://github.com/oxonomiAK/synd3/blob/main/assets/images/Screenshot2026.png)


## 🚀Installation

### From source

1. Install dependencies (skip if you already have C/C++, Makefile and libncurses):

```bash  
sudo apt install build-essential libncurses-dev
```
2. Clone and build the project:

```bash
git clone https://github.com/oxonomiAK/synd3.git
cd synd3
make
```
3. Run:

```bash
./synd3
```

### From release (recommended)

1. Download the latest binary from [Releases](https://github.com/oxonomiAK/synd3/releases) (pre-release).
2. Install dependencies (skip if you already have `libncurses`):

```bash
sudo apt install libncurses-dev
```
3. Make it executable and run:

```bash
chmod +x synd3
./synd3
```

4. (Optional) Install globally:

```bash
sudo mv synd3 /usr/local/bin/
```
Now you can run it from anywhere:

```bash
synd3
```
    
## 📚Usage

If you installed from source or downloaded the binary (tested on Ubuntu and WSL):

```bash
./synd3
```
If you installed globally (moved to `/usr/local/bin`):

```bash
synd3
```

### Controls
- **Arrow keys** — navigate between processes
- **F1** — open help screen
- **F9** — kill selected process
- **F10** or **Ctrl+C** — exit safely

## 👨‍💻Authors

- **Kirils Artjuhovs** — [@oxonomiAK](https://github.com/oxonomiAK)
- **Sergejs Tishins** — [@Arl1ve](https://github.com/Arl1ve)


## 📄License

This project is licensed under the MIT License. See the [LICENSE](https://github.com/oxonomiAK/synd3/blob/main/LICENSE) file for details.

