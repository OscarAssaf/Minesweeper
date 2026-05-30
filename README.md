# Minesweeper

A classic Minesweeper game built with **C++17** and **QT 6 Widgets** for the UI. 


## Difficulty levels

The game comes with sepearte difficulty levels and a custom one where you can customize the board size and the amount of mines.



| Difficulty   | Rows | Cols | Mines |
|--------------|------|------|-------|
| Easy         | 9    | 9    | 10    |
| Medium       | 16   | 16   | 40    |
| Hard         | 16   | 30   | 99    |
| Custom       | 5–32 | 5–48 |  Limited to chosen rows and column size|



## Building & Running
 
### 1. Clone the repository


### 1. Clone the repository
 
```bash
git clone https://github.com/OscarAssaf/Minesweeper
cd Minesweeper
```

### 2. Configure with CMake
 
```bash
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt6
```
 
> Replace `/path/to/Qt6` with your actual Qt installation path, e.g.:
> - **Linux/macOS:** `~/Qt/6.x.x/gcc_64` or `~/Qt/6.x.x/macos`
> - **Windows:** `C:/Qt/6.x.x/msvc2022_64`


### 3. Build
 
```bash
cmake --build build
```
 
### 4. Run
 
```bash
./build/MinesweeperQt        # Linux / macOS
build\Debug\MinesweeperQt.exe  # Windows
```

### Using Qt Creator
 
1. Open `CMakeLists.txt` in **Qt Creator** via *File → Open File or Project…*
2. Select your Qt kit and click **Configure Project**
3. Press **Run** (▶)
---
 

## How to Play
 
| Action | Input |
|--------|-------|
| Reveal a cell | Left-click |
| Place / remove flag | Right-click |
| New game (same settings) | Click 🙂 button |
| Change difficulty | Dropdown or *Game* menu |
| Custom board | *Game → New (Custom…)* |
 
- Reveal all non-mine cells to **win** 😎
- Click a mine and it's **game over** 💥

 
