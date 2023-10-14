# S-talk: Chat Application

## Table of Contents

- [S-talk: Chat Application](#s-talk-chat-application)
  - [Table of Contents](#table-of-contents)
  - [Description](#description)
  - [Features](#features)
  - [Usage](#usage)
  - [Installation](#installation)

## Description

This project is a command-line chat/messaging application which allows communication between two hosts on different networks. 

## Features

The program is a multithreaded application that uses POSIX Threads and Linux's mutexes.

## Usage

```bash
./s-talk [DESIRED_PORTNUM] [FOREIGN_HOST_IP] [FOREIGN_HOST_PORT] 
```

## Installation

This project does not have any external dependencies. Make file is provided to build the project. 

```bash
# Installation instructions
git git@github.com:abay-kulamkadyr/S-talk.git 
# In 'src' directory: run
make
```