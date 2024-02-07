# Arduino Flappy Birds

This project is an implementation of the popular game Flappy Birds for Arduino. The game is displayed on a 16x2 LCD screen and controlled using a single button.

## Project Structure

```
.vscode/
	arduino.json
	c_cpp_properties.json
	settings.json
arduinoFlappyBirds.ino
```

- [`arduinoFlappyBirds.ino`](command:_github.copilot.openRelativePath?%5B%22arduinoFlappyBirds.ino%22%5D "arduinoFlappyBirds.ino"): This is the main Arduino sketch file for the game. It includes the game logic, collision detection, and display code.
- [`.vscode/`](command:_github.copilot.openRelativePath?%5B%22.vscode%2F%22%5D ".vscode/"): This directory contains configuration files for Visual Studio Code.
  - `arduino.json`: This file contains configuration settings for the Arduino extension in Visual Studio Code.
  - [`c_cpp_properties.json`](command:_github.copilot.openRelativePath?%5B%22.vscode%2Fc_cpp_properties.json%22%5D ".vscode\c_cpp_properties.json"): This file contains configuration settings for the C/C++ extension in Visual Studio Code.
  - [`settings.json`](command:_github.copilot.openRelativePath?%5B%22.vscode%2Fsettings.json%22%5D ".vscode\settings.json"): This file contains general configuration settings for Visual Studio Code.

## How to Run

1. Open the [`arduinoFlappyBirds.ino`](command:_github.copilot.openRelativePath?%5B%22arduinoFlappyBirds.ino%22%5D "arduinoFlappyBirds.ino") file in the Arduino IDE or Visual Studio Code with the Arduino extension installed.
2. Connect your Arduino board to your computer.
3. Select the correct board and port in the Arduino IDE or Visual Studio Code.
4. Upload the sketch to your Arduino board.

## Controls

- Press the button to make the bird jump.
- Avoid the pipes and collect coins to increase your score.

## License

This project is open source and available under the [MIT License](LICENSE).
