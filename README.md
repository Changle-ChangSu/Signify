# Signify
Signify aims to create an interactive computer app that teaches users the basics of sign language through an intuitive interface, 
real-time gesture recognition, enjoyable form of game, and personalized progress tracking. 
The app is designed to bridge communication gaps by enabling users to learn and practice sign language quickly and effectively. 
The primary user cases include browsing sign language information, engaging in practice and interactive games, and monitoring learning progress.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![License](https://img.shields.io/badge/license-MIT-blue)

## Features
- Real-time gesture detection with YOLO model
- Cross-language support with TCP-based communication
- Interactive GUI with Qt
- Creative Gamification Features

## Installation
### Downloading the Already-Built Executable Files
You can download the source code and already-built executable files directly from Hugging Face: [![Hugging Face Model](https://img.shields.io/badge/Hugging%20Face-Signify-blue)](https://huggingface.co/Koalephant/Signify)     
Here in GitHub, only source code and pre-trained YOLO v8 model are provided.

### Recommended Prerequisites (Our Environment)
- Python 3.12.5
- pip
- OpenCV, Ultralytics YOLO, and other dependencies
- Qt 6.5.3 MinGW_64 QMake
- Linus or Windows System (MacOS has a different protocol in opening the camera?)

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/Changle-ChangSu/Signify.git
   ```
2. Install dependencies for python file:
   ```bash
   conda activate your-env
   conda install opencv
   pip install ultralytics
   ```
   
## Usage
1. Build and run the C++ codes in Qt. If nothing goes wrong, you will see the following interface. Click the green "Developer Mode" button,
and the C++ program will start hearing on port 12345 on your computer.
![image](https://github.com/user-attachments/assets/f1bf112f-ad4c-436e-a167-208183e7a138)

2. Open and run the model.py file in your complier such as VS Code. This will activate your camera and run the YOLO model,
and the detected sign language will be sent through port 12345 to C++ program.

   
