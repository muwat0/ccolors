# ccolors

Small CLI tool to extract dominant color palettes from images.

## Features

- JPEG / PNG support
- Terminal color preview
- HEX output
- JSON export
- No external dependencies

## Build

### Linux / macOS
```bash
mkdir build
cd build
cmake ..
make
```

## Usage
```bash
./ccolors image.jpg
```

## Output
- Prints color swatches + hex
- Writes palette.json

## Dependencies
- stb_image.h (included)

## License
MIT
