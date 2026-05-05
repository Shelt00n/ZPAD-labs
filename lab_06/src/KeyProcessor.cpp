#include "KeyProcessor.hpp"
#include <algorithm>

KeyProcessor::KeyProcessor()
    : mode(ProcessingMode::Original), quit(false), saveFlag(false), blurSize(7) {}

bool KeyProcessor::handleKey(int key) {
    if (key == -1) return false;

    switch (key) {
        case 27: // ESC
        case 'q':
        case 'Q':
            quit = true;
            break;
        case '0': mode = ProcessingMode::Original; break;
        case '1': mode = ProcessingMode::Invert; break;
        case '2': mode = ProcessingMode::Gray; break;
        case '3': mode = ProcessingMode::Blur; break;
        case '4': mode = ProcessingMode::Canny; break;
        case '5': mode = ProcessingMode::Sobel; break;
        case '6': mode = ProcessingMode::Binary; break;
        case '7': mode = ProcessingMode::Cartoon; break;
        case '8': mode = ProcessingMode::TextOverlay; break;
        case 's':
        case 'S':
            saveFlag = true;
            break;
        case '+':
        case '=':
            changeBlurSize(2);
            break;
        case '-':
        case '_':
            changeBlurSize(-2);
            break;
        default:
            break;
    }
    return true;
}

ProcessingMode KeyProcessor::getMode() const { return mode; }
bool KeyProcessor::shouldQuit() const { return quit; }
bool KeyProcessor::shouldSave() const { return saveFlag; }
void KeyProcessor::clearSaveFlag() { saveFlag = false; }

void KeyProcessor::changeBlurSize(int delta) {
    blurSize = std::max(1, blurSize + delta);
    if (blurSize % 2 == 0) blurSize++;
}

int KeyProcessor::getBlurSize() const { return blurSize; }
