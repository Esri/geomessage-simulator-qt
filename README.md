# vehicle-commander-qt

The Vehicle Commander Qt demonstrates developer samples for building and testing in-vehicle military applications with the ArcGIS Runtime Qt SDK.  The Vehicle Commander Qt projects contains source code for creating sample in-vehicle, touchscreen, and support applications.

![Image of Vehicle Commander]( ScreenShot.jpg "vehicle-commander-qt")

## Features

* Displays high-performance touchscreen maps, including provisioned basemaps and operational data
* Displays a GPS location (simulated)
* Displays military symbology and grids

## Sections

* [Requirements](#requirements)
* [Instructions](#instructions)
* [Resources](#resources)
* [Issues](#issues)
* [Contributing](#contributing)
* [Licensing](#licensing)

## Requirements

* ArcGIS Runtime SDK for Qt 10.2 or later
    * Hardware and software requirements are the same as those for ArcGIS Runtime SDK for Qt.  See the [Runtime SDK documentation for more information](https://developers.arcgis.com/en/qt/).
* The [Qt Software Developer Kit](http://qt.digia.com/) with Qt Creator
    * See the ArcGIS Runtime SDK documentation for specific versions supported.
    * Currently:
        * Qt 5.1 for Windows
        * Qt 5.1 or Qt 4.7.4-4.8.5 for Linux

## Instructions

### General Help

* [New to Github? Get started here.](http://htmlpreview.github.com/?https://github.com/Esri/esri.github.com/blob/master/help/esri-getting-to-know-github.html)

### Getting Started with the Vehicle Commander (Qt)
* Check your development environment is properly configured for the ArcGIS Runtime SDK for Qt; some items of note:
    * Ensure the configuration file, ex. `esri_runtime_qt_10_2.prf` has been installed
        * Copied from` {ARCGISRUNTIMESDKQT}\sdk\ideintegration\esri_runtime_qt_10_2.prf to {QTDIR}\mkspecs\features\esri_runtime_qt_10_2.prf` 
    * (Linux only) Initialize runtime. From a Command Prompt> `> . {RuntimeSDKHome}/init_sdk_qt64.sh`
* Open, build, and run one of the sample projects in Qt Creator
    * Note: if you are not using the latest version of the Runtime SDK, you may need to edit the .pro file for each project
    * Change the line `CONFIG += esri_runtime_qt_10_2_2` to match the version of your Runtime SDK.
* These projects include:
    * ArcGISQtMessageSimulator - sends simulated network update messages
    * ArcGISQtSearchAPIApp - a stand-alone symbol search sample
    * MapTestApp - a simple map demonstration application
    * VehicleCommander - a more advanced in-vehicle, touchscreen application
        * To automate the copy of the deployment files, a manual step is required to the Qt Creator Project. To do this in Qt Creator: Projects | VehicleCommander - Build Settings tab | Build Steps click on Add Build Step | Choose Make from the Add menu | Add "install" as Make argument. See the [VehicleCommander.pro](source/VehicleCommander/VehicleCommander.pro) for details on the install step. Note: this must been done for both the Debug and Release Builds.
        * IMPORTANT/WORKAROUND: in order to debug the VehicleCommander project you will need to disable QML debugging (in Projects | Run | Debugger Settings). For more information see [Issue #5](https://github.com/Esri/vehicle-commander-qt/issues/5)

### Services

* The default application uses ArcGIS Online services to display basemaps. See the source code for more details.

## Resources

* Learn more about Esri's [ArcGIS for Defense maps and apps](http://resources.arcgis.com/en/communities/defense-and-intelligence/).
* This application uses [Esri's ArcGIS Runtime SDK for Qt](https://developers.arcgis.com/en/qt/);
see the site for concepts, samples, and references for using the API to create mapping applications.

## Issues

Find a bug or want to request a new feature?  Please let us know by submitting an issue.

## Contributing

Esri welcomes contributions from anyone and everyone. Please see our [guidelines for contributing](https://github.com/esri/contributing).

## Licensing

Copyright 2012-2014 Esri

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

A copy of the license is available in the repository's
[license.txt](license.txt) file.

[](Esri Tags: ArcGIS Defense and Intelligence Situational Awareness ArcGIS Runtime Qt Military ArcGISSolutions)
[](Esri Language: C++)
