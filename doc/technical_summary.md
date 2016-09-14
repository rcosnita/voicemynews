<!-- MarkdownTOC -->

- Summary
- Technical summary
    - JS Layer
    - C++ code
    - UI Layer
    - Distribution
- Revisions history

<!-- /MarkdownTOC -->


# Summary

This document provides the technical summary for implementing VoiceMyNews so that most of the code is cross platform. The goal of the design is to provide common code for all platforms and also to allow UI to be added for each specific platform. Like this we can obtain a unique feeling for each individual platform and we can benefit from the latest APIs available.

# Technical summary

![Platform components](images/platform_components.png?raw=true)

In the above mentioned diagram we estimate the following distribution of code:

* 90% is **JS Layer** component which works cross platform.
* 5% is **C++ code** component must be provided on each individual platform.
* 5% is **UI Layer** component must be provided on each individual platform.

## JS Layer

JS Layer expects to have some objects available into code which can be easily used to glue JS with UI Layer and allow them to communicate. Besides this, in the JS Layer the following technologies will be used:

* webpack.
* commonjs modules.
* jasmine (for unit tests).
* nodejs / npm for running the unit tests.
* nodejs / npm for installing third party dependencies.

Whenever possible we prefer to reuse existing code in JS rather than implementing our own. We know upfront that functions like require must be extracted from node and binded to our **JS Engine**.

### JS Engine

JS Engine is a generic name for the actual engine we are going to use on each platform. After the initial research we have the following options:

| **OS** | **JS Engine Name** | **Comments / Issues** |
|--------|--------------------|-----------------------|
| **Win10 desktop**| Chakra | - |
| **Win10 Xbox**| Chakra | - |
| **Win10 mobile**| Chakra | - |
| **Win10 surface**| Chakra | - |
| **Android**| V8 | - |
| **Chromebook OS** | V8 | - |
| **MacOS** | JavaScriptCore | - |
| **iOS** | JavaScriptCore | - |

## C++ code

C++ code is somehow peculiar to each platform. This is because most platform will provide individual non portable apis for:

* networking.
* I/O.

We are going to rely on the following frameworks / namespaces:

* gtest / gmock (for unit tests).

The aim into this layer is to have as much of the code as possible portable.

### JS Engine compilation

On some platform where the JS engine is not directly distributed with the OS there will be a cost involved in compiling the engine and linking the platform with it.

Usually, this is not such a big deal and the overall size of the platform will still be small.

## UI Layer

UI Layer is completely platform specific and will be written from scratch for each platform. We are going to provide a standardized programming model for **C++ Dispatcher** and **UI Dispatcher** so that it is extremely easy to reuse it cross platform.

## Distribution

Each OS specific deliverable must pass store certification. There is no intention to use any other distribution channels except official stores.

# Revisions history

| **Version** | **Date** | **Author** | **Description** |
|-------------|----------|------------|-----------------|
| 1.0 | 25.05.2016 | Radu Viorel Cosnita | This revision contains the initial technical specification for **VoiceMyNews** platform. |
| 1.1 | 15.09.2016 | Radu Viorel Cosnita | I updated the js engine we are going to use on Apple devices and improved the deployment diagram. |