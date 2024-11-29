# Solar System Simulation 🌌

A visually captivating **solar system simulation** developed in C++ using OpenGL and GLUT. This project showcases dynamic planetary motion, collision detection, orbital eccentricity, morphing effects, and an interactive camera for an immersive experience.

## 🚀 Features

1. **Realistic Planetary Motion**  
   - Simulates orbits with adjustable eccentricity.  
   - Includes rotation and tilt for added realism.

2. **Collision Detection**  
   - Detects and pauses the simulation upon planet collisions.

3. **Morphing Animation**  
   - Planets can transform their shape interactively.

4. **Trail Rendering**  
   - Each planet leaves a colorful trail marking its orbital path.

5. **Interactive Camera**  
   - Rotate, zoom, and pan to explore the simulation dynamically.

6. **Pause and Resume**  
   - Control the simulation flow with simple keyboard inputs.

## 🛠️ Installation

1. **Prerequisites**  
   - OpenGL and GLUT libraries installed on your system.
   - A compatible C++ compiler (e.g., GCC).

2. **Build Instructions**  
   - Clone this repository:
     ```bash
     git clone https://github.com/your-username/solar-system-simulation.git
     cd solar-system-simulation
     ```
   - Compile the program:
     ```bash
     g++ -o SolarSystem main.cpp -lGL -lGLU -lglut
     ```
   - Run the executable:
     ```bash
     ./SolarSystem
     ```

## 🖥️ Controls

### Keyboard:
| Key       | Action                                       |
|-----------|---------------------------------------------|
| `e`       | Increase orbit eccentricity for a planet    |
| `c`       | Reset orbit eccentricity                   |
| `m`       | Start morphing animation for all planets    |
| `p`       | Pause the simulation                       |
| `r`       | Resume the simulation                      |
| `Esc`     | Exit the simulation                        |

### Mouse:
- **Drag**: Rotate the camera.
- **Scroll Up**: Zoom in.
- **Scroll Down**: Zoom out.

## 🌍 Planet Details

| Planet   | Radius | Distance from Sun | Color (R,G,B) |
|----------|--------|-------------------|---------------|
| Gray     | 0.2    | 2.0               | 0.5, 0.5, 0.5 |
| Orange   | 0.3    | 3.0               | 0.9, 0.6, 0.1 |
| Blue     | 0.4    | 4.0               | 0.2, 0.4, 1.0 |
| Red      | 0.35   | 5.0               | 1.0, 0.2, 0.2 |
| Sun      | 1.5    | 0.0               | 1.0, 1.0, 0.0 |

## 🧑‍💻 Code Overview

1. **Main Components**  
   - **`Planet`**: Represents a planet with properties like radius, distance, orbit speed, and more.  
   - **`OrbitTrail`**: Manages trail rendering for each planet.  
   - **Camera Controls**: Enables rotation, zoom, and pan.

2. **Collision Detection**  
   - Checks for overlapping planetary boundaries and halts simulation.

3. **Rendering Pipeline**  
   - Utilizes OpenGL for lighting, shading, and dynamic transformations.


Enjoy exploring the cosmos! 🌌
