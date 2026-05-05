#ifndef KEY_PROCESSOR_HPP
#define KEY_PROCESSOR_HPP

enum class ProcessingMode {
    Original = 0,
    Invert,
    Gray,
    Blur,
    Canny,
    Sobel,
    Binary,
    Cartoon,
    TextOverlay,
    SaveFrame
};

class KeyProcessor {
public:
    KeyProcessor();
    bool handleKey(int key);
    ProcessingMode getMode() const;
    bool shouldQuit() const;
    bool shouldSave() const;
    void clearSaveFlag();
    void changeBlurSize(int delta);
    int getBlurSize() const;

private:
    ProcessingMode mode;
    bool quit;
    bool saveFlag;
    int blurSize;
};

#endif
