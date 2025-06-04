# ESVcrypt

Simple console encoder/file decoder based on AES-256.

> This project is still under development

## ✨ Demo

<p align="center">
  <img width="700" align="center" src="https://github.com/EgeorUsl/ESVcrypt/blob/c89192e40782bab34d34847df2a6ce12329a803f/demo/gui-test.png" alt="demo-gui"/>
</p>
<p align="center">
  <img width="700" align="center" src="https://github.com/EgeorUsl/ESVcrypt/blob/f06abec893fba2047217db02434c1e6833da9cb4/demo/demo-work.gif" alt="demo-cli"/>
</p>

## 🚀 Usage

### CLI
Usage: ./ESVcrypt [options]
```bash

Options:
  -h, --help           Displays help on commandline options.
  --help-all           Displays help, including generic Qt options.
  -v, --version        Displays version information.
  -i, --input <file>   Input file
  -o, --output <file>  Output file

```

ESVcrypt encrypts files using AES-256 encryption

To simply encrypt the existing file, enter:

```sh
ESVcrypt encrypt -i input.file -o encrypted.file
```

and then enter the password you need (it is recommended at least 8 characters)

To decrypt the file, just use the "decrypt" mode:
```sh
ESVcrypt decrypt -i encrypted.file -o file.file
```

*For Linux users:
Make sure the file has a permit for execution*
```bash
chmod +x ESVcrypt
```

# Installation
---
## Release binaries

Pre-built artifacts are available to download from [latest GitHub release](https://github.com/EgeorUsl/ESVcrypt/releases).

## Development
### Requirements for build

- [qt6-base](https://www.qt.io/download-dev)
- Openssl
- Cmake
- Make
- git

### Build

```sh
git clone https://github.com/EgeorUsl/ESVcrypt
cd ESVcrypt
mkdir build
cmake ..
cmake --build .
```  

