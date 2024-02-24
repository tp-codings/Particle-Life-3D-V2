# Particle Life 3D

The goal of this project is to develop a particle simulation within a three-dimensional, walkable environment. Particles interact based on attraction and repulsion with adjustable interaction factors, moving accordingly in space. 
The primary focus is on creating an application that simulates and visualizes this particle behavior. Through a GUI, users can interactively adjust various settings such as attraction factors, particle size, attraction radius, etc., influencing particle behavior in real-time. 
Crucially, the application should smoothly render thousands of objects simultaneously. Additionally, light effects will be integrated into the simulation. To achieve optimal performance, C++ has been chosen as the programming language. 
The project aims to deepen understanding of the OpenGL graphics library, rather than aiming for a physically realistic simulation.
## Features
- freely movable camera in 3D space
- particle simulation based on [Tom Mohrs YouTube video](https://www.youtube.com/watch?v=p4YirERTVF0&t=201s) but in 3D space
- many setting options like shading options, sky boxes, distance of interaction, slow motion, attraction values, ...
- a lot of fun to play around

## Project Overview

| Personal Rating            | **Year of Development**  | **Languages**                        | **Tools**                                 | **Type of Application** |
|------------------------|--------------------------|--------------------------------------|-------------------------------------------|--------------------------|
| ⭐️⭐️⭐️⭐️⭐️ (5/5)    | 2023          | C++ | OpenGL, GLFW, ImGUI, Freetype | Simulation               |

### Additional Notes

This project leverages the latest technologies to create an immersive simulation experience. It's actively maintained and open to collaboration. Feel free to contribute!


# Getting Started
## Prerequisites and basic information
- This application is tested in windows 10/11
- This repo does not provide an executable -> you have to compile your own (I used Visual Studio)
- I don't guarantee that every corner case has been taken into account. The project is part of my learning process for C++ and OpenGL
- All dependencies are included in this repo, so it is almost plug and play :)

## Installation
1. Clone Repository
`git clone https://github.com/tp-codings/Particle_Life_3D_V2.git`
2. Build Executable (e.g. Visual Studio)

# 3. User Manual
## 3.1 Camera Control in Space
- **W:** Forward (camera direction)
- **A:** Left
- **S:** Backward
- **D:** Right
- **Space:** Up
- **Shift:** Down
- **Mouse:** Look around
- **Scroll wheel:** Zoom
- **Arrow keys:** Control the sun on the sphere around the border box (only in dirLightShading mode)

## 3.2 GUI Usage
- **F:** Open/Close GUI
- **TimeFactor:** Slow Motion Factor (useful for chaotic particle behavior)
- **Distance:** Radius for particle interaction
- **Scale:** Particle size
- **Boxsize:** Size of the space where particles can move (if Border is active)
- **Random:** Set random interaction factors
- **Start/Stop:** Start/Stop the simulation
- **RandomPos:** Distribute particles randomly in the space (within the Border Box)
- **Show Border:** Draw the edges of the cube space (Border Box)
- **Borders/No Borders:** Specify whether particles are confined to the Border Box space
- **Postprocessing:** Effects generated with postprocessing kernel
- **DirLightShading:** Directional light source (sun) illuminates the scene (Phong Shading)
- **RandomColors:** Assign a random color to each particle
- **Slider:** Set individual interaction factors
- **DirLight:** Set the color of the incoming directional light

## 3.3 Keyboard Shortcuts
- **R:** Random interaction factors
- **L:** Toggle Shading Mode
- **P:** Random Position of particles
- **B:** Toggle Border
- **Enter:** Start/Stop simulation
- **0:** No postprocessing
- **1:** Blur
- **2:** Edge Detection
- **3:** Inversion
- **4:** Grayscale

# Simulation Concept
In this simulation, there are 5 types of particles differentiated only by their color and interaction factors with other particle types. The particles come in red, green, blue, yellow, and white. They can interact with themselves or influence other types of particles. Initially, all particles are randomly distributed within the space, confined by the size of the Border Box. When the simulation is started via the GUI or keyboard, the positions and velocity vectors of the particles are calculated and updated.

The calculation of forces acting on individual particles is based on Newton's law of gravitation. However, the gravitational constant is not a constant here; it is determined by the interaction factors, and the force is inversely proportional to the distance between particles, not the square of the distance. Additionally, each particle has a mass of 1, making the force equivalent to the derivative of velocity.

Each particle influences every other particle within its action radius. Therefore, the resulting force on each particle is the sum of all influencing forces:

\[ \vec{F}_{\text{result}_i} = a_i = \text{distance}_{\text{max}} \cdot \sum_{j}^{N} f\left(\frac{\text{distance}}{\text{distance}_{\text{max}}}, \text{attraction}\right) \cdot \frac{\vec{d}}{\text{distance}} \]

Here, \(\text{distance}\) is the distance between two particles, \(\text{distance}_{\text{max}}\) is the action radius, and \(\vec{d}\) describes the direction to the second particle. The force function \(f(d, a)\) is from Tom Mohr, another creator of a Particle Life Simulation, and prevents points from collapsing into a singularity. It is defined as:

\[ f(d,a) = \begin{cases} \frac{d}{\beta} - 1, & d < \beta \\ a \cdot \left(1 - \frac{|2d - 1 - \beta|}{1 - \beta}\right), & \beta < d < 1 \\ 0, & 1 < d \end{cases} \]

The velocity of a particle at a specific time is then calculated as:

\[ \vec{v}_i(t) = \vec{v}_i(t - \Delta t) \cdot \text{friction} \cdot \vec{F}_{\text{result}_i} \cdot \Delta t \cdot \text{timeFactor} \]

Here, \(\Delta t\) is the time between each frame, \(\text{friction}\) is the friction (a factor between 0 for total friction and 1 for no friction), \(\text{amount}\) is the number of particles of a type, and \(\text{timeFactor}\) is the time factor enabling slow motion.

# 7. Potential Improvements
## 7.1 Particle Shadow Casting
An opportunity for improvement in the simulation could involve simulating the casting of shadows between individual particles. Currently, all objects behave as completely transparent to light. However, shadow calculations, especially with numerous objects, can be computationally expensive and noticeably impact performance. Directional light, as the sole light source, may pose challenges as objects farther behind could be almost completely obscured by the sheer mass of objects in front.

## 7.2 Optimization with Octrees and Barnes-Hut Algorithm
In addition to Instancing, which primarily enhances efficiency by minimizing CPU-GPU communication, there are other optimization possibilities. Octrees, for instance, are data structures that hierarchically divide space into segments, providing an efficient way to search for other particles in the nearby environment. This allows limiting particle interaction to the immediate surroundings, such as the action radius. The Barnes-Hut Algorithm treats distant particles as cumulative mass points, approximating the force vector on the observed particle. Instead of individually calculating interactions between all particles, groups of particles are considered, minimizing calculation complexity.

Combining both methods could significantly accelerate the simulation on the CPU side by substantially reducing the total number of calculations.

## 7.3 Optimization with GPU Computing
While multithreading allows parallelization of processes, such as nested for-loops, a CPU doesn't have numerous cores designed for parallel calculations. GPUs, with their architecture designed for parallelization, could significantly enhance performance. The bottleneck in my simulation is the `updateInteraction()` function, which calculates particle interactions. Currently, the outer loops linking different particle types are parallelized by the CPU using multithreading. However, within particle types, all particles of two types are compared, requiring a much larger number of calculations. These nested for-loops could be effectively parallelized by the GPU using Compute Shaders, vastly improving simulation performance, potentially enabling real-time simulation of hundreds of thousands of particles.

## 7.4 Clean Code
The architecture of the simulation is likely in need of improvement. Currently, the Engine class encompasses a lot of functionality. Some of this could be outsourced into additional classes for better code organization, such as a Window Manager class, a Buffer Handler, a Renderer, a Helper class, or an Input Handler. Additionally, there might be concepts to make some functions more efficient or straightforward.

## 7.5 Glow/Bloom Effect
Implementing a Glow/Bloom effect can add a radiant shimmer around light sources, making the scene more dynamic. This involves extracting bright areas, applying a blur effect, and then reapplying it to the scene. This could make particles appear to glow.

## 7.6 Dynamic Particle Count Adjustment
Adding interactivity could include dynamically adjusting the particle count during runtime. For example, a mouse click could introduce another chunk of particles at the cursor position, or sliders could manually add or remove particles from individual types.

## 7.7 Interactively Changeable Types
It would be beneficial to be able to add more particle types in addition to the existing 5, creating even more unique structures.

## 7.8 Camera as an Actor
Allowing the camera to actively influence the scene, perhaps by exerting a repulsive force on all particles in the vicinity, could help disperse existing structures.

## 7.9 Mass
Introducing the concept of mass could mean that objects exert more or less influence on others based on their mass. Particles with larger masses could, for instance, be displayed as larger.

## 7.10 Export Scene as Mesh
Exporting a scene at a specific point in time as a mesh, such as an .obj file, could be intriguing for use in other software.

## 7.11 The Fourth Dimension
Moving beyond 2D and 3D, exploring the fourth dimension could be the next logical step. Mathematically, force calculation wouldn't be more complex than adding another component. A slider could then be used to navigate along the fourth spatial axis.

## Contact Information

Feel free to reach out to me for any inquiries or collaboration opportunities:

- **Email:** [tobipaul50@gmail.com](mailto:tobipaul50@gmail.com)
- **LinkedIn:** [Tobias Paul](https://www.linkedin.com/in/tobias-paul-657513276/)
- **GitHub:** [@tp-codings](https://github.com/tp-codings)

Looking forward to connecting with you! 😊

