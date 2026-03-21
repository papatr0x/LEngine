# LEngine

LEngine is a basic 2D game engine built on top of **SDL3**. This project was created as a personal learning exercise to become familiar with the SDL3 library and engine development.

## Goal
The primary objective of LEngine is to provide a simple, educational implementation of a game engine. It was originally designed to get familiar with SDL3, but I believe it can be helpful for those who are starting in game development and want to understand how an engine works "under the hood" without getting lost in the complex source code of professional, production-grade engines.

## Features
LEngine implements several core systems commonly found in game engines:

- **Component-Based Architecture**: Uses an `Object-Component` system to manage game entities and their behaviors.
- **2D Rendering System**: 
  - Texture management via `TextureManager`.
  - Sprite rendering with support for pivots, offsets, and rotation.
  - Multi-layer rendering support.
  - Screen-space vs. World-space rendering (for UI vs Game objects).
  - ShapeComponents allows drawing shapes like rectangles, circles, and lines.
- **Input System**: Comprehensive support for:
  - Keyboard (Key states and events).
  - Mouse (Position, delta, and buttons).
  - Gamepad (Buttons and analog axes).
  - A PlayerController class that handles player input and movement related to a player in the scene.
- **Collision System**:
  - AABB (Axis-Aligned Bounding Box) collision detection.
  - Broad-phase optimization (Sort and Prune).
  - Collision events: `onEnter`, `onStay`, and `onExit`.
- **Animation System**: Frame-based sprite animations via `AnimatorComponent`.
- **Audio System**: Support for both background Music and Sound Effects (SFX) using `SDL_mixer`.
- **Text Rendering**: Support for TrueType fonts (TTF) via `TextComponent`.
- **Scene Management**: 
  - `SceneOrchestrator` for managing multiple scenes.
  - Support for scene transitions and intermediate "loading" scenes.
- **Fixed Timestep Update**: Supports fixed-interval updates (`updatePhysics`) to ensure deterministic behavior, facilitating integration with physics engines like **Jolt**.
- **Camera System**: Simple camera support for world navigation.
- **Scripting**: Logic can be attached to objects using `ScriptComponent`.

## Demo1 — Feature Showcase

A sandbox application that exercises most engine systems together:

- **Scene flow**: Splash screen → Main Menu → Game → back, with an intermediate loading scene between menu and game.
- **Player**: Moves with WASD or a gamepad. Left-click teleports the player to the cursor position. ESC triggers a scene transition back to the menu.
- **Camera modes**: Press `P` to cycle between three modes — free (no follow), follow player, follow enemy.
- **Enemy AI**: A bounce AI that reflects off screen edges and deflects off the player using a reflection formula. Plays a sound effect on each bounce.
- **Collision**: Box collider on the player, circle collider on the enemy. `onEnter`/`onExit` callbacks logged to the console.
- **Animation**: The enemy cycles through a sprite-strip swim animation.
- **Audio**: Background music (OGG) with volume control, sound effect (MP3) on collision.
- **HUD**: Screen-space text overlay showing controls.

## Demo2 — Pong

A complete single-player Pong game (human vs. AI) that demonstrates how to build a self-contained game on top of the engine:

- **Gameplay**: First player to reach 10 points wins. The ball resets to center after each point with a 1-second delay before the next serve.
- **Player paddle**: Controlled with `Up`/`Down` arrow keys. Paddle movement influences the ball's exit angle on hit — moving the paddle while striking transfers a fraction of its velocity to the ball, allowing the player to add spin and curve shots past the AI.
- **AI paddle**: Tracks the ball's vertical position at a limited speed, making it beatable with well-timed shots.
- **Ball physics**: Exit angle on a paddle hit depends on the impact point relative to the paddle center (center = flat return, edge = steep angle up to ±60°). Wall bounces off top and bottom edges.
- **Scoring**: Score displayed at the top of the screen. Reaching 10 points transitions to a Game Over screen showing the winner and final score.
- **Scene flow**: Main Menu → Pong → Game Over → Main Menu.

## Platform Support
LEngine has been tested on:
- **macOS** (AppleClang)
- **Windows** (Visual Studio 2022 Professional)

## Requirements
- SDL3
- SDL3_mixer
- SDL3_ttf
- CMake 4.1+
- A C++23 compatible compiler

