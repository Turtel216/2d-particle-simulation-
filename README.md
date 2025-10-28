# 2D Particle Simulation (C++ & SFML)

> **Note:** This project is a work in progress and still under active development.

A high-performance **2D Particle Simulation** built with **C++** and **SFML**, focused on **optimization and real-time rendering**.

---

## Features

* **Real-time 2D particle physics simulation**
* **Highly optimized particle update loop** using efficient data structures and minimal cache misses
* **Smooth rendering** with SFML (Simple and Fast Multimedia Library)
* **Modular C++ design** for easy extension and customization
* **CMake build system** for cross-platform development

---

## Controls

| Key     | Action                      |
| ------- | --------------------------- |
| ⬆️ / ⬇️ | Toggle or change gravity    |
| ESC     | Close the simulation window |

---

##  Build Instructions

### Prerequisites

* **C++17 or later**
* **CMake ≥ 3.16**
* **SFML ≥ 2.5**

### Building the Project

```bash
# Clone the repository
git clone https://gitlab.com/papakonstantinou/2d-particle-simulation.git
cd 2d-particle-simulation 

# Create build directory
mkdir build && cd build

# Generate and build
cmake ..
cmake --build .
```

###  Run the Simulation

After building, run:

```bash
./sim
```

---

## Optimization Highlights

* **Efficient memory layout** for particles (minimized cache misses)
* **Loop unrolling and SIMD-friendly updates** (where supported)
* **Frame rate independent physics step**
<!--* **Reduced draw calls** by batching particle vertices-->

---

## Preview: TODO

Example:

![Particle Simulation Demo](assets/demo.gif)

---

## Author

**Your Name**
[Gitlab](https://gitlab.com/papakonstantinou) • [LinkedIn](https://www.linkedin.com/in/dimitrios-papakonstantinou-44a7672b3/)

---

## License

This project is licensed under the **MIT License** — see [LICENSE](LICENSE) for details.
