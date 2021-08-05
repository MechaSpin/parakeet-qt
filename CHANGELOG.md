# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0] - 2021-08-02
### Added
- Added support for the Parakeet ProE sensor
- Added support for a resample filter as part of the Parakeet ProE sensor

### Modified
- Bump to using parakeet-sdk v3.0.0

## [1.0.3] - 2021-07-12
### Modified
- Modified CMake to find the latest version of parakeet-sdk which can be found
- Updated copyright action to call the action from inside the parakeet-devtools repo
- Bump to using parakeet-sdk v2.0.0

## [1.0.2] - 2021-06-21
### Modified
- Properly label units of measure on variables
- A GitHub action which will test Parakeet-Qt's build status on a PR into main
- A GitHub action which creates a release, along with distributables, on push to main

## [1.0.1] - 2021-06-09
### Modified
- Changed documentation to more clearly state the need for the UART to USB Driver.

## [1.0.0] - 2021-06-07
### Added
- Visualization of data through use of the parakeet-sdk library
- Ability to modify sensor settings and baud rate
