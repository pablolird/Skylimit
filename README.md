<p align="center">
  <!-- Logo image: centered SKYLIMIT logo/title card, e.g. the game title over a space/sky background -->
  <!-- <img src="assets/logo.png" alt="SKYLIMIT Logo" width="400"/> -->
</p>

  ![GitHub Created At](https://img.shields.io/github/created-at/pablolird/Skylimit)
  ![GitHub contributors](https://img.shields.io/github/contributors/pablolird/Skylimit)

---

![C++ Badge](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=fff&style=for-the-badge)
![SFML Badge](https://img.shields.io/badge/SFML-8CC445?logo=sfml&logoColor=fff&style=for-the-badge)
![Windows Badge](https://img.shields.io/badge/Windows-0078D4?logo=windows&logoColor=fff&style=for-the-badge)
![macOS Badge](https://img.shields.io/badge/macOS-000000?logo=apple&logoColor=fff&style=for-the-badge)

# 🚀 SKYLIMIT — An Endless Jumper

**SKYLIMIT** is an endless vertical platformer built from scratch in **C++ with SFML**, using a fully custom **Entity-Component System (ECS)** architecture. Jump from platform to platform, climb as high as you can, and beat your high score — all while the world dynamically shifts around you with parallax backgrounds and increasing difficulty.

---

<!-- Gameplay GIF/video: the player jumping between platforms, camera following upward, score increasing -->
<!-- Replace with: <img src="assets/gameplay.gif" alt="Gameplay" width="700"/> -->

---

## 🌟 Features

- **Endless Procedural Generation**: Platforms spawn dynamically above the player. Spacing increases the higher you go, making the game progressively harder.
- **Two Platform Types**:
  - **Grass tiles** — Static platforms, safe to land on.
  - **Log tiles** — Moving platforms that bounce back and forth horizontally.
  - Both types have **hazard variants** (spikes) that kill the player on contact.
- **8 Scrolling Parallax Backgrounds**: The world visually evolves every 5000 points with smooth background crossfade transitions.
- **Persistent High Score**: Your best run is saved to disk and shown in-game with a visual marker line.
- **Character Shop**: Unlock and equip 6 unique skins — Default, Ghost, Golden, Twilight, Emerald, and Venoso.
- **Settings**: Adjust volume, toggle music on/off, and switch between Arrow Keys and WASD controls.
- **Pause & Replay**: Pause at any time or instantly replay after a run ends.
- **Sound & Music**: Distinct background music for menu and gameplay, plus sound effects for jumping, navigation, and selection.
- **Frame-Rate Independent Physics**: Game speed is normalized via delta time, ensuring identical gameplay at any monitor refresh rate (60Hz, 120Hz, etc.).

---

## 🎮 Controls

| Action       | Key (Default) | Key (WASD) |
| ------------ | ------------- | ---------- |
| Move Left    | ← Arrow       | A          |
| Move Right   | → Arrow       | D          |
| Pause        | P             | P          |
| Replay       | R             | R          |
| Quit to Menu | ESC           | ESC        |

> Controls can be switched between **Arrow Keys** and **WASD** in the Settings menu.

---

## 🏪 Skin Shop

<!-- Shop screen image: grid of 6 character previews with their names and prices -->
<!-- Replace with: <img src="assets/shop.png" alt="Skin Shop" width="700"/> -->

| Skin     | Price |
| -------- | ----- |
| Default  | Free  |
| Ghost    | $10   |
| Golden   | $20   |
| Twilight | $30   |
| Emerald  | $40   |
| Venoso   | $69   |

---

## 🛠️ Architecture

SKYLIMIT is built around a clean **Entity-Component System (ECS)** inspired by game engine design patterns.

```
Endless Jumper - OOP/
│
├── Main.cpp                   # Entry point
├── GameEngine.{h,cpp}         # Core game loop, window, scene & asset management
│
├── Scene.{h,cpp}              # Abstract base scene
├── Scene_Menu.{h,cpp}         # Main menu scene
├── Scene_Play.{h,cpp}         # Gameplay scene (physics, generation, collision)
├── Scene_Shop.{h,cpp}         # Skin shop scene
├── Scene_Settings.{h,cpp}     # Settings scene
│
├── EntityManager.{h,cpp}      # Manages all active entities
├── Entity.{h,cpp}             # Base entity class
├── Components.h               # All ECS components (CTransform, CInput, CGravity, ...)
│
├── Physics.{h,cpp}            # Overlap / collision detection utilities
├── Animation.{h,cpp}          # Sprite animation system
├── Assets.{h,cpp}             # Loads and stores textures, fonts, sounds, music
├── Background.{h,cpp}         # Parallax background layer management
├── Vec2.{h,cpp}               # Custom 2D vector math
├── Action.{h,cpp}             # Input action abstraction
│
├── assets.txt                 # Asset manifest (textures, animations, sounds, music)
└── config.txt                 # Persistent save data (coins, high score, settings)
```

### Components

| Component      | Purpose                                       |
| -------------- | --------------------------------------------- |
| `CTransform`   | Position, previous position, velocity         |
| `CInput`       | Player input state (left, right, up, canJump) |
| `CBoundingBox` | Axis-aligned hitbox size and half-size        |
| `CAnimation`   | Current animation and repeat flag             |
| `CGravity`     | Per-entity gravity value                      |
| `CState`       | Player state string (idle, jump, fall, run)   |
| `CKill`        | Marks a platform as lethal (spike variant)    |
| `CMove`        | Marks a platform as horizontally moving       |

---

## 🚀 Setup and Build

### Prerequisites

- **C++17** compiler (MSVC, GCC, or Clang)
- **SFML 2.x** — [sfml-dev.org](https://www.sfml-dev.org/)

### Windows (Visual Studio)

1. Clone the repository:

   ```bash
   git clone https://github.com/pablolird/Skylimit.git
   cd Skylimit
   ```

2. Open `Endless Jumper - OOP.vcxproj` in Visual Studio.

3. Set up SFML include and library paths in the project properties to point to your SFML installation.

4. Build and run (`F5`).

### macOS / Linux (CMake or manual)

1. Install SFML via your package manager:

   ```bash
   # macOS
   brew install sfml

   # Ubuntu/Debian
   sudo apt install libsfml-dev
   ```

2. Compile all `.cpp` files and link against SFML:

   ```bash
   g++ -std=c++17 *.cpp -o Skylimit -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
   ```

3. Run:
   ```bash
   ./Skylimit
   ```

> Make sure the `Assets/` folder is in the same directory as the executable.

---

## 📸 Screenshots

<!-- Main menu screenshot: title "SKYLIMIT" over a parallax background with Play/Shop/Settings options -->
<!-- Replace with: <img src="assets/menu.png" alt="Main Menu" width="700"/> -->

<!-- Gameplay screenshot: player mid-jump between platforms with score displayed and parallax background -->
<!-- Replace with: <img src="assets/gameplay.png" alt="Gameplay" width="700"/> -->

<!-- Background transition GIF: the crossfade between two parallax world themes -->
<!-- Replace with: <img src="assets/transition.gif" alt="Background Transition" width="700"/> -->

---

Enjoy the climb!
