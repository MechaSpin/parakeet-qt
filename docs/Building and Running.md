# Parakeet Viewer Building and Running

## Windows & Linux - Qt Creator
#### 1. Install the parakeet-sdk following the [Building and Installing](https://github.com/MechaSpin/parakeet-sdk/blob/main/docs/Building%20and%Installing.md) instructions found in the parakeet-sdk docs folder
#### 2. Clone parakeet-qt, the root directory will be refered to as {PARAKEET_QT_ROOT}

```
git clone https://github.com/MechaSpin/parakeet-qt
```

#### 3. Install [Qt Creator](https://www.qt.io/product/development-tools)
#### 4. Building with Qt Creator
- Open Qt Creator
- File -> Open project -> {PARAKEET_QT_ROOT}/CMakeLists.txt
- Click the correct project configuration, followed by Configure Project
- Build -> Build all Projects

#### 5. Running using Qt Creator
- Click the run button (green triangle) in the bottom left

## Windows - Visual Studio
#### 1. Install the parakeet-sdk following the [Building and Installing](https://github.com/MechaSpin/parakeet-sdk/blob/main/docs/Building%20and%Installing.md) instructions
#### 2. Clone parakeet-qt, the root directory will be refered to as {PARAKEET_QT_ROOT}

```
git clone https://github.com/MechaSpin/parakeet-qt
```

#### 3. Install [Qt5](https://www.qt.io/download)
#### 4. Create a folder inside {PARAKEET_QT_ROOT} called "build"
#### 5. Open CMake GUI
- Click browse source
- Navigate to {PARAKEET_QT_ROOT}
- Click browse build
- Select {PARAKEET_QT_ROOT}/build
- Click configure
- Select your version of Visual Studio from the combobox
- If on a 64-bit machine, set the generator to x64
- Click Finish
- Specify the location of Qt5_DIR in the CMake cache to the directory containing the Qt5Config.cmake file from your Qt5 installation (ie: C:\Qt\Qt5.12.10\5.12.10\msvc2017_64\lib\cmake\Qt5)
- Click Configure
- Once configuration completes, click Generate
- Once generation completes, open Visual Studio as administrator (required for installing to Program Files)

#### 6. Building with Visual Studio
- Open parakeet-qt.sln which was created by CMake in {PARAKEET_QT_ROOT}/build
- In the solution explorer, select the parakeet-qt project
- Right click the project, and select build

#### 7a. Running using Visual Studio
- Ensure that you have installed the [UART to USB Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
- Locate your Qt bin folder
- In the solution explorer, right click on the SimpleExample project, and select properties
- Under the debugging tab, in the Environment section, add the following, and replace MY_PATH_TO_Qt_BIN with the path found earlier

```
PATH=%PATH%;MY_PATH_TO_Qt_BIN
```

- Click OK
- In the solution explorer, right click on the parakeet-qt project, and select Set as Startup Project
- Click the run button

#### 7b. Running using Command Prompt
- Ensure that you have installed the [UART to USB Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
- Attempting to run the application without the correct Qt Dependencies in the same folder, will result in DLL missing issues
- If building as a debug build: 
	- Move the following DLLs into the {PARAKEET_QT_ROOT}/build/Debug folder:
	- Qt5Cored.dll
	- Qt5Guid.dll
	- Qt5SerialPortd.dll
	- Qt5Widgetsd.dll
- If building as a release build: 
	- Move the following DLLs into the {PARAKEET_QT_ROOT}/build/Release folder:
	- Qt5Core.dll
	- Qt5Gui.dll
	- Qt5SerialPort.dll
	- Qt5Widgets.dll
- Run the app by executing parakeet-qt.exe

## Linux - Terminal

#### 1. Clone parakeet-qt, the root directory will be refered to as {PARAKEET_QT_ROOT}

```
git clone https://github.com/MechaSpin/parakeet-qt
```

#### 2. Install the parakeet-sdk following the [Building and Installing](https://github.com/MechaSpin/parakeet-sdk/blob/main/docs/Building%20and%Installing.md) instructions
#### 3. Install [Qt5](https://www.qt.io/download)
#### 4. Building using Terminal
- In a terminal, navigate to {PARAKEET_QT_ROOT} and execute

```
mkdir build
cd build
cmake ..
make
```

#### 5. Running using Terminal
- In a terminal, navigate to {PARAKEET_QT_ROOT}/build and execute

```
./parakeet-qt
```