# Qt CSV Viewer

Qt CSV Viewer is a lightweight, user-friendly application for viewing CSV (Comma-Separated Values) files. Built with Qt and C++, it provides a simple and efficient way to explore and analyze CSV data.

## Features

- Command-line interface for opening CSV files
- Option to use the first line as column headers
- Automatic column resizing with a minimum width
- Alternate row colors for better readability
- Sorting functionality for all columns
- Copy selection functionality (works with Google Docs and other applications)
- Read-only view to prevent accidental data modification
- Close application with Escape key

## Installation

### From Source

To build and install Qt CSV Viewer from source, follow these steps:

1. Ensure you have the following dependencies installed:
   - Qt 5
   - CMake
   - C++ compiler (e.g., GCC or Clang)

2. Clone the repository:
   ```
   git clone https://github.com/mkeyran/qt-csv-viewer.git
   cd qt-csv-viewer
   ```

3. Build the project:
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Install the application (optional):
   ```
   sudo make install
   ```

## Usage

To open a CSV file with Qt CSV Viewer, use the following command:

```
csv-viewer [OPTIONS] <file.csv>
```

Options:
- `-H, --header`: Use the first line of the CSV file as column headers

Example:
```
csv-viewer --header data.csv
```

### Keyboard Shortcuts

- `Esc`: Close the application
- `Ctrl+C` (or `Cmd+C` on macOS): Copy selected cells

### Copying Data

To copy data from the viewer:

1. Select the desired cells, rows, or columns
2. Use the keyboard shortcut (`Ctrl+C` or `Cmd+C`) or right-click and select "Copy"
3. Paste the data into your desired application:
   - In applications like Google Docs, the data will paste as a formatted table
   - In other applications, it will paste as tab-separated values

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Qt framework for providing the tools to build this application
